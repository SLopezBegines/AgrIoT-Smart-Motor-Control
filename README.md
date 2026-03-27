# AgrIoT — Smart Motor Control

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-ESP32%20%7C%20Arduino%20Nano-blue)](firmware/)
[![LoRa](https://img.shields.io/badge/LoRa-868%20MHz%20%7C%2020%20dBm-green)](docs/communication-protocol.md)
[![Uptime](https://img.shields.io/badge/Uptime-99.2%25%20%286%20months%29-brightgreen)](docs/architecture.md)
[![Cost](https://img.shields.io/badge/Cost-%3C€150-orange)](hardware/bom/BOM.md)

> **Legacy system modernisation:** Adding remote monitoring and control to a
> 1972 diesel irrigation engine using LoRa wireless — without replacing a
> single original component.

---

## Overview

A two-node LoRa network enables remote start/stop and real-time telemetry of
a 50-year-old Diter-Zafra diesel engine from 2+ km away. A custom PCB integrates
an Arduino Nano, RFM95W LoRa module, 8 relay channels, and four sensor inputs.
An ESP32 gateway bridges LoRa to WiFi, pushing telemetry to the Blynk cloud.

**Key results — 6 months production deployment:**

| Metric | Value |
|--------|-------|
| System uptime | 99.2% |
| Wireless range (real-world) | 2.3 km |
| Site visits reduced | 7×/week → 1×/week (86% reduction) |
| Low-pressure events caught | 3 (each would have damaged the engine) |
| Total hardware cost | < €150 |
| Engine modifications | 0 |

---

## System Architecture

```
Blynk Mobile App
        │ WiFi / HTTPS
        ▼
ESP32 TTGO Gateway (0xFF)
        │ LoRa 868 MHz · 20 dBm · 2.3 km · SF7
        ▼
Arduino Nano Node (0xBB)
        │ 5V relay wiring and 12V relay
        ▼
1972 Diter-Zafra Diesel Engine
```

- **Gateway → Node:** Individual JSON commands per Blynk virtual pin (`{"V1":1}`)
- **Node → Gateway:** 9-value JSON telemetry array every 60 s + on state change
- **State persistence:** EEPROM stores start/electrovalve state across power cycles
- **Safety sequences:** Start (500 ms key pulse) → Stop (550 ms) → 5 s delay → Cooldown (3 s)

See [`docs/architecture.md`](docs/architecture.md) for the full system diagram and state machine.

---

## Hardware

### Control Node (custom PCB)

| Component | Part | Function |
|-----------|------|----------|
| Arduino Nano | ATmega328P | Main MCU |
| RFM95W | SX1276, 868 MHz | LoRa radio |
| 8× relay (SPDT 10A) | — | Start, stop, cooldown, oil pump, throttle, brake, valve, aux |
| LM2596 | Buck, 12V→5V | Power regulation |
| LM1117T-3.3 | LDO, 5V→3.3V | LoRa module supply |
| Voltage dividers | 22kΩ / 4.7kΩ | Battery + alternator monitoring |
| Pressure transducer | 0–5V analog | Oil pressure |
| Hall effect sensor | A3144 | Engine RPM (interrupt on D3) |
| Fuel float switch | Digital | Fuel level warning |

### Gateway Node

| Component | Notes |
|-----------|-------|
| ESP32 TTGO LoRa v2 | Built-in SX1278, WiFi, 868 MHz antenna |

See [`hardware/bom/BOM.md`](hardware/bom/BOM.md) for the complete parts list with LCSC and supplier part numbers.

---

## Software Setup

### Prerequisites

- Arduino IDE 2.x
- ESP32 board support package
- Libraries: `Blynk`, `WiFiManager` (tzapu), `AsyncElegantOTA`, `ArduinoJson ≥6.21`, `LoRa` (Sandeep Mistry), `SimpleTimer`

See [`firmware/README.md`](firmware/README.md) for exact versions and install instructions.

### Quick Start

```bash
# Clone
git clone https://github.com/SLopezBegines/AgrIoT-Smart-Motor-Control.git
cd AgrIoT-Smart-Motor-Control

# Configure credentials (Blynk token, WiFi, OTA password)
cp config/config.h.example \
   firmware/gateway/Diesel_Motor_Lora_Gateway_NoOLED/config.h
# Edit config.h

# Flash gateway (ESP32 TTGO):
#   Open firmware/gateway/Diesel_Motor_Lora_Gateway_NoOLED/ in Arduino IDE
#   Board: ESP32 Dev Module → Upload

# Flash node (Arduino Nano):
#   Open firmware/node/Diesel_Motor_Lora_Node/ in Arduino IDE
#   Board: Arduino Nano (Old Bootloader) → Upload

# Verify: open Serial Monitor at 115200 baud on both devices
```

See [`docs/deployment-guide.md`](docs/deployment-guide.md) for the full procedure including bench testing, PCB assembly, and field installation.

---

## Documentation

| Document | Description |
|----------|-------------|
| [`docs/architecture.md`](docs/architecture.md) | System block diagram, data flow, state machines, power architecture |
| [`docs/communication-protocol.md`](docs/communication-protocol.md) | LoRa physical layer, packet format, JSON payload specification |
| [`docs/deployment-guide.md`](docs/deployment-guide.md) | Step-by-step: configure, flash, bench test, field install, OTA |
| [`docs/blynk-setup.md`](docs/blynk-setup.md) | Blynk template, virtual pin mapping, widget configuration |
| [`docs/sensor-calibration.md`](docs/sensor-calibration.md) | Calibration formulas and procedures for all sensors |
| [`docs/troubleshooting.md`](docs/troubleshooting.md) | LoRa, ESP32, Arduino, PCB/power common issues |
| [`docs/mqtt-migration.md`](docs/mqtt-migration.md) | Migration path from Blynk to self-hosted MQTT + Node-RED |
| [`hardware/bom/BOM.md`](hardware/bom/BOM.md) | Full bill of materials with part numbers and suppliers |

---

## Repository Structure

```
AgrIoT-Smart-Motor-Control/
├── firmware/
│   ├── gateway/          # ESP32 TTGO LoRa — WiFi bridge + Blynk
│   └── node/             # Arduino Nano + RFM95W — engine control
├── hardware/
│   ├── pcb/              # Schematic PDF + gerber files
│   └── bom/              # Bill of materials
├── docs/                 # Architecture, protocol, deployment, calibration
├── config/               # config.h.example + Blynk virtual pin spreadsheet
├── tools/                # Serial monitor, range test, sensor test sketches
├── tests/                # Unit tests (JSON parsing, calibration formulas)
├── examples/             # Standalone sender/receiver examples
└── photos/               # System, PCB, installation, dashboard photos
```

---

## Skills Demonstrated

**Embedded Systems:** C/C++, Arduino, ESP32, real-time control, interrupt-driven sensing
**Wireless:** LoRa 868 MHz, addressed packet routing, bidirectional JSON telemetry
**Hardware Design:** PCB layout, power regulation, sensor conditioning, relay driving
**IoT Platforms:** Blynk cloud, OTA updates, WiFiManager, mDNS
**Reliability:** EEPROM state persistence, timed safety sequences, guard conditions
**Legacy Integration:** Non-invasive retrofit, no engine modifications required

---

## Future Work

- [ ] Migrate from Blynk to self-hosted MQTT + Node-RED + Grafana
- [ ] Hardware watchdog timer on Arduino Nano
- [ ] LoRa confirmed delivery (ACK + retry) for start/stop commands
- [ ] Solar panel + LiPo battery backup for gateway node
- [ ] Multi-node support (second engine on same LoRa network)

---

## Acknowledgments

- LoRa communication based on [LoRaDuplexCallback](https://github.com/sandeepmistry/arduino-LoRa) by Sandeep Mistry
- Blynk + LoRa integration inspired by [vinayyn on Hackster](https://www.hackster.io/vinayyn/lora-blynk-app-interfacing-1e1851)
- Flow meter code originally from [pkarun](https://github.com/pkarun) and [Luis Llamas](https://www.luisllamas.es/)

---

## License

MIT — see [LICENSE](LICENSE)

## Author

**Santiago López Begines**
[slopezbegines.github.io](https://slopezbegines.github.io) · [LinkedIn](https://linkedin.com/in/santibegines) · [ORCID 0000-0001-8809-8919](https://orcid.org/0000-0001-8809-8919)
