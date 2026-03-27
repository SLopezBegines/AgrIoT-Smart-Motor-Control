# Communication Protocol

## LoRa Physical Layer

| Parameter | Value |
|-----------|-------|
| Frequency | 868 MHz (EU ISM band) |
| TX power | 20 dBm (100 mW) — PA_BOOST pin |
| Spreading factor | SF7 |
| Bandwidth | 125 kHz |
| Coding rate | 4/5 |
| Sync word | 0xF3 (private network) |
| Max payload | ~255 bytes |

## Packet Header

Each LoRa packet carries a 4-byte header prepended by the sender:

| Byte | Field | Description |
|------|-------|-------------|
| 0 | Destination | 0xFF = gateway, 0xBB = node |
| 1 | Source | 0xFF = gateway, 0xBB = node |
| 2 | Message ID | Rolling counter (byte, wraps at 255) |
| 3 | Length | sizeof(payload buffer) — note: fixed 200 on node |

The receiver checks byte 0 against its own address and discards mismatched packets.

## Payload Formats

### Command: Gateway → Node

One JSON object per Blynk virtual pin event. Sent immediately on `BLYNK_WRITE()`.

```json
{"V1": 1}    // Start engine
{"V2": 1}    // Stop engine
{"V3": 1}    // Relay 3
{"V4": 1}    // Relay 4
{"V5": 1}    // Throttle (linear actuator pulse)
{"V6": 1}    // Brake    (linear actuator pulse)
{"V7": 1}    // Electrovalve ON
{"V7": 0}    // Electrovalve OFF
{"V8": 1}    // Relay 8 (auxiliary)
{"V11": 1}   // Soft reset node
{"V24": 5}   // RPM target slider (integer 0–10)
```

All values are integers. The node deserialises with `ArduinoJson` and maps
each key to its corresponding `StateRelay_x` variable.

### Telemetry: Node → Gateway

JSON array of 9 floats/integers, sent every 60 s and after any relay event:

```json
[12.8, 14.1, 3.2, 0, 1, 0, 1, 1850, 5]
```

| Index | Variable | Unit | Description |
|-------|----------|------|-------------|
| 0 | `avgVoltage1` | V | Battery voltage (22kΩ/4.7kΩ divider, A7) |
| 1 | `avgVoltage2` | V | Alternator voltage (same divider, A6) |
| 2 | `avgPresion1` | bar | Oil pressure (analog transducer, A5) |
| 3 | `LedRelay_7` | 0/1 | Electrovalve state |
| 4 | `LedRelay_3` | 0/1 | Engine OFF indicator (Relay 3 state) |
| 5 | `LedRelay_4` | 0/1 | Engine ON indicator (Relay 4 / oil pump) |
| 6 | `StateLevel` | 0/1 | Fuel level (1 = sufficient) |
| 7 | `totalRPM` | RPM | Engine RPM (Hall effect, 1 s window) |
| 8 | `RPM_Slider` | 0–10 | Current RPM target slider position |

## Sensor Formulas

### Voltage (battery and alternator)
```
Vin = raw_adc × 5.0 × (R1 + R2) / (R2 × 1023)
    = raw_adc × 5.0 × (22000 + 4700) / (4700 × 1023)
    = raw_adc × 0.02778
```
100 samples averaged per reading.

### Oil pressure
```
Presion (bar) = raw_adc × 5000 mV × 0.0028 / 1023
```
100 samples averaged per reading.

### Engine RPM
```
Interrupt on D3 (INT1, RISING edge) counts Hall effect pulses.
Every 1000 ms:
    totalRPM = 60.0 × (pulseCount / (interval_ms / 1000))
Interrupt disabled during calculation, re-enabled after.
```

## Timing

| Event | Interval |
|-------|----------|
| Telemetry (node → gateway) | 60 s (SimpleTimer) + on any relay change |
| Gateway → Blynk push | 1 s (BlynkTimer `sendServer()`) |
| Sensor averaging loop | Every loop() call (~few ms) |
| RPM window | 1000 ms |
