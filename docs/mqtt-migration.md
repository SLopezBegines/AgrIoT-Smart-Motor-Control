# MQTT Migration Guide

## Why Migrate from Blynk?

The current deployment uses Blynk IoT (legacy energy model):
- ~2,400 energy credits used, limiting widget additions
- Cloud dependency — if Blynk servers are down, remote control is lost
- Limited historical data retention
- Blynk legacy API may be deprecated

**Target stack:** Mosquitto MQTT broker + Node-RED + InfluxDB + Grafana

## Architecture After Migration

```
Smartphone / Browser
        │ HTTPS / WebSocket
        ▼
Node-RED (flow editor + dashboard)
        │ MQTT
        ▼
Mosquitto broker (self-hosted, VPS or Raspberry Pi)
        │ MQTT over WiFi
        ▼
Gateway ESP32
        │ LoRa 868 MHz
        ▼
Arduino Node
```

## Topic Structure

```
agriot/engine1/command/relay1     # publish 1 to start
agriot/engine1/command/relay2     # publish 1 to stop
agriot/engine1/command/rpm        # publish 0-10 slider
agriot/engine1/telemetry          # node publishes JSON array
agriot/engine1/status/voltage     # gateway re-publishes individual values
agriot/engine1/status/pressure
agriot/engine1/status/rpm
agriot/engine1/status/fuel
```

## Gateway Changes Required

Replace Blynk library with PubSubClient:

```cpp
#include <PubSubClient.h>

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

// In setup():
mqtt.setServer("your-broker-ip", 1883);
mqtt.setCallback(onMqttMessage);

// Replace BLYNK_WRITE handlers with:
void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  // parse topic and payload, forward via LoRa
}

// Replace Blynk.virtualWrite with:
mqtt.publish("agriot/engine1/status/voltage", String(Voltage).c_str());
```

## Node Changes Required

No changes — the Arduino node only speaks LoRa. All MQTT logic stays on the gateway.

## Mosquitto Setup (Ubuntu/Debian)

```bash
sudo apt install mosquitto mosquitto-clients
sudo systemctl enable mosquitto

# Test
mosquitto_sub -t agriot/# -v &
mosquitto_pub -t agriot/engine1/command/relay1 -m 1
```

## Node-RED Dashboard

1. Install Node-RED: `npm install -g node-red`
2. Add `node-red-dashboard` palette
3. Create flows:
   - MQTT in → function (parse JSON) → dashboard gauge/LED nodes
   - Dashboard button → MQTT out (topic: `agriot/engine1/command/relay1`)

## InfluxDB + Grafana (Historical Data)

```
Node-RED → InfluxDB write node → InfluxDB database
Grafana → InfluxDB datasource → time-series panels
```

Retention policy: 1 year for sensor data (small dataset — ~10 values/minute).
