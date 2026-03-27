# Deployment Guide

## Prerequisites

- Arduino IDE 2.x (or 1.8.x)
- ESP32 board support installed (`https://dl.espressif.com/dl/package_esp32_index.json`)
- All libraries listed in `firmware/README.md` installed
- Blynk account with a configured template (see `blynk-setup.md`)
- Multimeter, USB-serial adapter, laptop

---

## Step 1 — Configure credentials

```bash
cp config/config.h.example \
   firmware/gateway/Diesel_Motor_Lora_Gateway_NoOLED/config.h
```

Edit `config.h`:
- `BLYNK_TEMPLATE_ID` — from Blynk console → Template → Info
- `BLYNK_AUTH_TOKEN` — from Blynk console → Device → Device Info
- `OTA_PASSWORD` — set a strong password for OTA updates

Leave `WIFI_SSID` and `WIFI_PASSWORD` blank to use WiFiManager (recommended for
first deployment — a captive portal will appear on first boot).

---

## Step 2 — Flash the gateway (ESP32)

1. Open `firmware/gateway/Diesel_Motor_Lora_Gateway_NoOLED/` in Arduino IDE
2. Board: **ESP32 Dev Module**
3. Upload speed: **921600**
4. Select the correct COM port
5. Upload

**First boot:** The ESP32 creates a WiFi access point named `AutoConnectAP`.
Connect to it from your phone, navigate to `192.168.4.1`, enter your WiFi
credentials. The device saves them to flash and reboots.

**Verify:** Open Serial Monitor (115200 baud). You should see:
```
Booting
LoRa init succeeded.
WiFi connected
IP: 192.168.x.x
Blynk connected
```

---

## Step 3 — Flash the node (Arduino Nano)

1. Open `firmware/node/Diesel_Motor_Lora_Node/` in Arduino IDE
2. Board: **Arduino Nano**
3. Processor: **ATmega328P (Old Bootloader)**
4. Upload speed: **57600**
5. Select COM port
6. Upload

**Verify:** Open Serial Monitor (115200 baud). You should see:
```
LoRa init succeeded.
Sending packet: ...
```

If upload fails, try switching to "ATmega328P" (without Old Bootloader).

---

## Step 4 — Bench test (no engine connected)

With both devices powered and Serial Monitor open on the gateway:

1. Tap **Start** in the Blynk app → gateway should log `Sending packet: {"V1":1}`
2. Node should log `Received from: 0xFF ... V1: 1`
3. After 60 s, node sends telemetry → gateway logs decoded values
4. Check RSSI (should be > −110 dBm at bench distance) and SNR > 0

---

## Step 5 — PCB assembly and wiring

Refer to `hardware/pcb/PCB_Motor.pdf` for the schematic.

**Power connections (node PCB):**
- 12 V+ from engine battery → PCB VIN (via 1N5819 and fuse)
- GND from battery → PCB GND
- Verify 5 V and 3.3 V rails with multimeter before connecting other components

**Relay wiring:**
- Each relay COM terminal → one side of the load circuit
- NO (normally open) terminal → other side
- Engine start circuit to Relay 1 COM/NO
- Engine stop circuit to Relay 2 COM/NO
- (See firmware node README for full relay assignments)

**Sensor wiring:**
- Voltage divider (R1=22kΩ, R2=4.7kΩ) output → A7 (battery), A6 (alternator)
- Pressure transducer signal → A5, supply → 5V, GND → GND
- Fuel float switch → D4 (INPUT_PULLUP — LOW = fuel present)
- Hall effect sensor → D3 (signal), 5V, GND

---

## Step 6 — Field installation

1. Mount PCB in IP65 enclosure with cable glands
2. Mount gateway enclosure within WiFi range of farmhouse router
3. Attach 868 MHz antennas to both gateway and node
4. Power up node first, then gateway
5. Verify telemetry appears in Blynk app within 60 s
6. Test start/stop sequence with engine **disconnected** from relays first
7. Reconnect relay outputs to engine circuits and test one relay at a time

---

## Step 7 — OTA updates (gateway only)

After initial deployment, update gateway firmware over WiFi:

1. Build new firmware in Arduino IDE (Sketch → Export Compiled Binary)
2. Navigate to `http://motorgateway.local/update` (or gateway IP)
3. Log in with OTA credentials from `config.h`
4. Upload the `.bin` file

The node (Arduino Nano) requires physical USB access for firmware updates.

---

## Troubleshooting

See `docs/troubleshooting.md` for common issues.
