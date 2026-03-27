# System Architecture

## Overview

Two-node LoRa network bridging a smartphone app to a diesel engine 2+ km away.

```
┌─────────────────────────────────────────────────────────────┐
│                      Blynk Cloud                            │
│              Virtual pins V1–V24 (HTTPS/WiFi)               │
└─────────────────────┬───────────────────────────────────────┘
                      │ WiFi / HTTPS
                      ▼
┌─────────────────────────────────────────────────────────────┐
│               Gateway Node (ESP32 TTGO LoRa v2)             │
│  Address: 0xFF                                              │
│  • Receives BLYNK_WRITE callbacks for each virtual pin      │
│  • Serialises command as JSON → LoRa packet to node         │
│  • Receives telemetry array from node → pushes to Blynk     │
│  • OTA updates via ArduinoOTA + AsyncElegantOTA             │
│  • mDNS: motorgateway.local                                 │
└─────────────────────┬───────────────────────────────────────┘
                      │ LoRa 868 MHz, 20 dBm, SF7, BW125, 0xF3
                      │ 2.3 km range (tested, rural terrain)
                      │ ~60 bytes payload, bidirectional
                      ▼
┌─────────────────────────────────────────────────────────────┐
│          Control Node (Arduino Nano + RFM95W)               │
│  Address: 0xBB                                              │
│  • Reads sensors every loop (voltage, pressure, RPM, fuel)  │
│  • Drives 8 relay channels for engine control               │
│  • Sends telemetry every 60 s + after any state change      │
│  • Persists start/electrovalve state in EEPROM              │
└─────────────────────┬───────────────────────────────────────┘
                      │ 12V relay wiring
                      ▼
         1972 Barreiros Diesel Engine
         (start, stop, throttle, brake, oil pump, valve)
```

## Data Flow

### Command path (Cloud → Engine)

1. User taps button in Blynk mobile app
2. Blynk cloud triggers `BLYNK_WRITE(Vx)` on gateway ESP32
3. Gateway serialises a JSON object `{"Vx": value}` (~30 bytes)
4. Gateway transmits LoRa packet addressed to node 0xBB
5. Node's `onReceive()` deserialises JSON, updates `StateRelay_x`
6. Node's `loop_funtions()` checks guard conditions and activates relay sequence

### Telemetry path (Engine → Cloud)

1. Node's `LoRaSender()` called every 60 s (SimpleTimer) or after a relay event
2. Serialises 9-value JSON array (~55 bytes)
3. Transmits LoRa packet addressed to gateway 0xFF
4. Gateway's `onReceive()` deserialises array, updates local variables
5. Gateway's `sendServer()` (1 s interval) writes values to Blynk virtual pins

## Motor Control State Machine

```
         ┌─────────────────────────────────────────────────────┐
         │                    STOPPED                          │
         │          started=0 (EEPROM addr 0)                  │
         └──────────────┬──────────────────────────────────────┘
                        │ V1=1 received AND started==0
                        ▼
               Relay 1 LOW (500 ms)     ← starter motor
               Relay 4 LOW              ← oil pump ON
               started=1 → EEPROM
                        │
                        ▼
         ┌─────────────────────────────────────────────────────┐
         │                    RUNNING                          │
         │          started=1 (EEPROM addr 0)                  │
         │   RPM control via Relay 5 (throttle) / Relay 6 (brake)│
         └──────────────┬──────────────────────────────────────┘
                        │ V2=1 received AND started==1
                        ▼
               Relay 2 LOW (550 ms)     ← stop relay
               started=0 → EEPROM
               wait 5000 ms
               Relay 3 LOW (3000 ms)    ← cooldown run
               Relay 3 HIGH, Relay 4 HIGH
                        │
                        ▼
                    STOPPED

```

## EEPROM Layout

| Address | Variable | Values |
|---------|----------|--------|
| 0 | `started` | 0 = stopped, 1 = running |
| 1 | `executed` | 0 = electrovalve OFF, 1 = electrovalve ON |

State is read at the top of every `loop_funtions()` call so power-cycle recovery
is automatic — the engine resumes its last known state on node reboot.

## Power Architecture

```
Engine battery 12 V DC
        │
        ├── 1N5819 (reverse polarity protection)
        │
        ▼
LM2596 buck converter → 5 V / 3 A
        │
        ├── Arduino Nano Vin (5 V)
        ├── 4-channel relay module VCC (5 V)
        ├── 4-channel relay module 2 VCC (5 V)
        │
        ▼
LM1117T-3.3 LDO → 3.3 V / 800 mA
        │
        └── RFM95W LoRa module (3.3 V)
```
