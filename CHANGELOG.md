# Changelog

All notable changes to this project are documented here.

## [1.0.0] — 2026-03-27

### Added
- ESP32 TTGO gateway firmware with Blynk cloud integration and OTA updates
- Arduino Nano node firmware with 8-relay motor control
- Bidirectional LoRa communication (868 MHz, 20 dBm, sync word 0xF3)
- Sensor integration: dual voltage, oil pressure, RPM (Hall effect), fuel level
- EEPROM state persistence (start/stop survives power cycle)
- Timed start/stop sequences with automatic cooldown (Relay 2 → 5 s → Relay 3 for 3 s)
- RPM closed-loop speed control via linear actuator (throttle/brake relays)
- Electrovalve (fertiliser) control with EEPROM execution tracking
- Custom PCB design (2-layer, JLCPCB)
- Complete documentation suite

### Deployed
- Production deployment on 1972 Barreiros diesel irrigation engine
- 6 months operational: 99.2% uptime, 2.3 km real-world LoRa range

## [Planned] — Future releases

### To Add
- MQTT broker support (Mosquitto) to replace Blynk cloud dependency
- Hardware watchdog timer on Arduino Nano
- LoRa confirmed delivery (ACK + retry) for start/stop commands
- Historical data logging (InfluxDB + Grafana)
- Solar panel + LiPo battery backup for gateway node
- Multi-node support (second engine)
