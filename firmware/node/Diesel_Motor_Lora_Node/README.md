# Node Firmware — Arduino Nano + RFM95W

## Role

The field-side controller mounted directly at the engine:
- Reads 5 sensors every loop cycle (voltage ×2, pressure, fuel, RPM)
- Drives 8 relay channels for engine control
- Sends telemetry to gateway every 60 s (also after any state change)
- Receives and executes commands from gateway (individual JSON key-value pairs)
- Persists start/stop and electrovalve state in EEPROM (survives power loss)

## Hardware

| Component | Notes |
|-----------|-------|
| Arduino Nano (ATmega328P) | Old Bootloader variant — set in Arduino IDE |
| RFM95W LoRa module | SPI, 868 MHz, connected via custom PCB |

## Pin Assignments

### Relay Outputs (active LOW — relay energises when pin is LOW)

| Pin | Relay | Function |
|-----|-------|----------|
| D8 | Relay 1 | Engine START — 500 ms key pulse |
| D7 | Relay 2 | Engine STOP — 550 ms pulse, triggers cooldown |
| D6 | Relay 3 | Cooldown run — 3 s after 5 s stop delay |
| D5 | Relay 4 | Oil pump / run-state indicator |
| A0 | Relay 5 | Throttle (linear actuator) |
| A1 | Relay 6 | Brake (linear actuator) |
| A2 | Relay 7 | Electrovalve (fertiliser injection) |
| A3 | Relay 8 | Auxiliary |

### Sensor Inputs

| Pin | Sensor | Conditioning |
|-----|--------|-------------|
| A7 | Battery voltage | 22 kΩ / 4.7 kΩ resistor divider, 100-sample average |
| A6 | Alternator voltage | Same divider |
| A5 | Oil pressure | Analog transducer 0–5 V, 100-sample average |
| D4 | Fuel level | Digital float switch, INPUT_PULLUP |
| D3 | Engine RPM | Hall effect sensor, INT1 hardware interrupt |

### RFM95W SPI

| Signal | Arduino Pin |
|--------|-------------|
| SCK | D13 |
| MISO | D12 |
| MOSI | D11 |
| NSS (CS) | D10 |
| RST | D9 |
| DIO0 | D2 |

## File Descriptions

| File | Purpose |
|------|---------|
| `Diesel_Motor_Lora_Node.ino` | Main sketch — setup (LoRa init, pin modes, timers), loop |
| `functions_rpm.h` | All sensor functions, relay control, start/stop state machine, `LoRaSender()`, `onReceive()` |
| `functions_flow.h` | Legacy flow-meter code (disabled) — earlier prototype version |
| `JSON.h` | Earlier JSON implementation (superseded by `functions_rpm.h`) |

## Telemetry Packet (Node → Gateway)

JSON array sent every 60 s and after relay state changes:

```
[voltage1, voltage2, pressure, electrovalve, motor_off_led, motor_on_led, fuel_level, rpm, rpm_slider]
[  0     ,    1    ,    2    ,      3       ,      4        ,      5      ,     6     ,  7 ,     8     ]
```

## Command Packet (Gateway → Node)

Individual JSON objects per virtual pin:

```json
{"V1": 1}   // Start
{"V2": 1}   // Stop
{"V5": 1}   // Throttle pulse
{"V6": 1}   // Brake pulse
{"V24": 5}  // RPM slider target (0–10)
```

## Start/Stop Sequence

```
START: V1=1
  → Relay 1 LOW (500 ms) — starter motor
  → Relay 4 LOW          — oil pump ON
  → EEPROM.update(addr, 1)

STOP: V2=1
  → Relay 2 LOW (550 ms) — stop relay
  → wait 5000 ms
  → Relay 3 LOW (3000 ms) — cooldown run
  → Relay 3 HIGH, Relay 4 HIGH
  → EEPROM.update(addr, 0)
```
