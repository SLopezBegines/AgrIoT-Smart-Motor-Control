# Firmware

## Overview

| Folder | MCU | Role |
|--------|-----|------|
| `gateway/` | ESP32 TTGO LoRa v2 | WiFi bridge — receives Blynk commands, forwards via LoRa; receives node telemetry, pushes to Blynk cloud |
| `node/` | Arduino Nano (ATmega328P) + RFM95W | Engine control — drives 8 relays, reads 5 sensors, sends telemetry every 60 s |

## Required Libraries

Install via Arduino IDE → Library Manager:

### Gateway (ESP32)
| Library | Version | Purpose |
|---------|---------|---------|
| `Blynk` | ≥ 1.1.0 | Cloud IoT platform |
| `WiFiManager` (tzapu) | ≥ 2.0.0 | WiFi auto-configuration portal |
| `AsyncElegantOTA` | ≥ 2.2.0 | Over-the-air firmware updates |
| `ArduinoJson` | ≥ 6.21.0 | JSON serialisation / deserialisation |
| `LoRa` (Sandeep Mistry) | ≥ 0.8.0 | SX1278 LoRa driver |

Built-in (ESP32 core): `WiFi`, `ESPmDNS`, `WiFiUdp`, `ArduinoOTA`, `SPI`

### Node (Arduino Nano)
| Library | Version | Purpose |
|---------|---------|---------|
| `ArduinoJson` | ≥ 6.21.0 | JSON serialisation / deserialisation |
| `SimpleTimer` | ≥ 1.0.0 | Non-blocking timer callbacks |
| `LoRa` (Sandeep Mistry) | ≥ 0.8.0 | RFM95W LoRa driver |

Built-in (AVR core): `SPI`, `EEPROM`

## Board Settings (Arduino IDE)

**Gateway:**
- Board: `ESP32 Dev Module`
- Upload speed: `921600`
- Partition scheme: `Default 4MB with spiffs`
- Flash frequency: `80MHz`

**Node:**
- Board: `Arduino Nano`
- Processor: `ATmega328P (Old Bootloader)` — use this if upload fails
- Upload speed: `57600`

## Quick Start

```bash
# 1. Clone and configure credentials
cp config/config.h.example \
   firmware/gateway/Diesel_Motor_Lora_Gateway_NoOLED/config.h
# Edit config.h with your Blynk token and WiFi credentials

# 2. Open in Arduino IDE
#    Gateway: firmware/gateway/Diesel_Motor_Lora_Gateway_NoOLED/
#    Node:    firmware/node/Diesel_Motor_Lora_Node/

# 3. Select board, port, upload

# 4. Open Serial Monitor at 115200 baud to verify
```

See `docs/deployment-guide.md` for the full step-by-step procedure.
