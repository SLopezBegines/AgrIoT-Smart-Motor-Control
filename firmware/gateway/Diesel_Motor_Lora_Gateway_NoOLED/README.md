# Gateway Firmware — ESP32 TTGO LoRa v2

## Role

Acts as the WiFi-to-LoRa bridge:
- Receives commands from Blynk cloud (virtual pins V1–V8, V24) via WiFi
- Forwards each command to the Arduino node as a LoRa JSON packet
- Receives telemetry from the node every 60 s (JSON array, 9 values)
- Pushes telemetry to Blynk virtual displays and LED widgets every 1 s

## Hardware

| Component | Notes |
|-----------|-------|
| ESP32 TTGO LoRa v2 | Built-in SX1278, 868 MHz, integrated WiFi |
| External LoRa antenna | SMA connector, 868 MHz, 3–5 dBi |

## LoRa Pin Mapping (TTGO LoRa v2)

Defined in `LoRa.h` — pins are fixed by the TTGO board layout:

| Signal | GPIO |
|--------|------|
| SCK | 5 |
| MISO | 19 |
| MOSI | 27 |
| NSS (CS) | 18 |
| RST | 14 |
| DIO0 | 26 |

## File Descriptions

| File | Purpose |
|------|---------|
| `Diesel_Motor_Lora_Gateway_NoOLED.ino` | Main sketch — setup/loop, Blynk timer |
| `BlynkFunctions.h` | All BLYNK_WRITE handlers + `onReceive()` + `sendServer()` |
| `LoRa.h` | LoRa initialisation (frequency, TX power, sync word) |
| `WiFiManager.h` | WiFiManager captive portal setup |
| `OTA.h` | ArduinoOTA + AsyncElegantOTA initialisation |
| `mDNS.h` | mDNS hostname registration (`motorgateway.local`) |

## Configuration

Copy `config/config.h.example` → this folder → rename to `config.h`.
Fill in `BLYNK_AUTH_TOKEN`, `BLYNK_TEMPLATE_ID`, and OTA credentials.

## Blynk Virtual Pin Mapping

| Virtual Pin | Direction | Data | Widget |
|-------------|-----------|------|--------|
| V1 | Cloud → Node | Start engine (1 = start pulse) | Button |
| V2 | Cloud → Node | Stop engine (1 = stop sequence) | Button |
| V3–V8 | Cloud → Node | Relay 3–8 state | Button |
| V11 | Cloud → Node | Arduino soft-reset | Button |
| V13 | Node → Cloud | Battery voltage (V) | Value display |
| V14 | Node → Cloud | Alternator voltage (V) | Value display |
| V15 | Node → Cloud | Oil pressure (bar) | Gauge |
| V17 | Node → Cloud | Engine OFF indicator | LED |
| V18 | Node → Cloud | Engine ON indicator | LED |
| V19 | Node → Cloud | Electrovalve state | LED |
| V20 | Node → Cloud | Fuel level | LED |
| V23 | Node → Cloud | Engine RPM | Value display |
| V24 | Both | RPM target slider (0–10) | Slider |
