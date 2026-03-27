# Blynk Setup

## Template Configuration

Create a new template in [Blynk Cloud](https://blynk.cloud):
- **Hardware:** ESP32
- **Connection type:** WiFi

## Virtual Pin Mapping

### Control (Cloud → Node)

| Virtual Pin | Widget | Settings | Function |
|-------------|--------|----------|----------|
| V1 | Button | Push, 0/1 | Start engine |
| V2 | Button | Push, 0/1 | Stop engine (triggers cooldown sequence) |
| V3 | Button | Switch, 0/1 | Relay 3 (manual cooldown) |
| V4 | Button | Switch, 0/1 | Relay 4 (oil pump manual) |
| V5 | Button | Push, 0/1 | Throttle (linear actuator +) |
| V6 | Button | Push, 0/1 | Brake (linear actuator −) |
| V7 | Button | Switch, 0/1 | Electrovalve (fertiliser) |
| V8 | Button | Switch, 0/1 | Relay 8 (auxiliary) |
| V9 | Button | Switch, 0/1 | Timer-based start (auto stop) |
| V10 | Button | Switch, 0/1 | Timer-based valve |
| V11 | Button | Push, 0/1 | Node soft-reset |
| V24 | Slider | 0–10, step 1 | RPM target |

### Telemetry (Node → Cloud)

| Virtual Pin | Widget | Label | Unit |
|-------------|--------|-------|------|
| V13 | Value Display | Battery Voltage | V |
| V14 | Value Display | Alternator Voltage | V |
| V15 | Gauge | Oil Pressure | bar |
| V17 | LED | Engine OFF | — |
| V18 | LED | Engine ON | — |
| V19 | LED | Electrovalve | — |
| V20 | LED | Fuel Level | — |
| V23 | Value Display | RPM | rpm |

## Blynk Energy Costs

Each widget consumes Blynk energy credits (legacy Blynk 1.x model):

| Widget | Energy |
|--------|--------|
| Button | 200 per widget |
| LED | 100 per widget |
| Timer | 200 per widget |
| SuperChart | 900 |
| Value Display | 200 per widget |
| Gauge | 300 per widget |
| Slider | 200 per widget |

**Approximate total for this project:** ~2,400 energy units (single PCB configuration)

See `config/Virtual_Pins_Blynk.xlsx` for the original spreadsheet with exact counts.

## Datastream Configuration

In the Blynk console, create a datastream for each virtual pin above.
Recommended settings for numerical pins:
- Min: 0
- Max: appropriate for the sensor (e.g. 0–15 V for voltage, 0–10 for pressure)
- Default value: 0
