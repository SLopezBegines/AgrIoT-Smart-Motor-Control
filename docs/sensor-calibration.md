# Sensor Calibration

## Voltage Sensors (Battery and Alternator)

**Circuit:** Resistor voltage divider, R1 = 22 kΩ, R2 = 4.7 kΩ

```
Vin ─── R1 (22kΩ) ─── Vout ─── R2 (4.7kΩ) ─── GND
                         │
                       Arduino ADC pin (A7 or A6)
```

**Calibration formula (from Coste.xlsx):**
```
Vin = Vout × (R1 + R2) / R2
Vin = ADC_raw × (5.0 / 1023) × (22000 + 4700) / 4700
Vin = ADC_raw × 0.02778
```

**Maximum measurable voltage:** 5 V × 26700/4700 ≈ 28.4 V (safe for 12 V lead-acid)

**Calibration procedure:**
1. Connect a known voltage (e.g. bench supply at 12.00 V)
2. Open Serial Monitor and read `avgVoltage1`
3. If the displayed value differs by more than ±0.2 V, adjust the formula constant
   in `VoltageFunction1()` in `functions_rpm.h`

**Expected values:**
- Battery at rest: 12.4–12.7 V
- Engine running (alternator charging): 13.8–14.4 V

---

## Oil Pressure Sensor

**Circuit:** Analog transducer, 0–5 V output (linear)

**Calibration formula (from Coste.xlsx):**
```
Pressure (bar) = ADC_raw × 5000 mV × 0.0028 / 1023
```

The constant `0.0028` was derived from the sensor datasheet:
- Full range: 200 PSI = 13.78 bar
- At 5000 mV: ADC_raw = 1023
- Constant = 13.78 / 1023 / (5000 × 0.001) ≈ 0.0028 bar/(mV·ADC_step)

**Alternative formula (with offset, not used in current firmware):**
```
Pressure = (ADC_raw × 5000 × 0.0034 − 1.7237) / 1023
```

**Calibration procedure:**
1. With engine OFF and pressure known (use external gauge), compare displayed value
2. Adjust the constant in `PressionFunction1()` if needed

**Expected values:**
- Engine OFF: 0 bar
- Engine idling: 2.5–4.0 bar (typical diesel, varies by engine)

---

## Engine RPM (Hall Effect)

**Circuit:** Hall effect sensor on alternator fan, digital output to D3 (INT1)

The alternator fan has multiple blades. The sensor counts one pulse per magnet
pass. **Verify pulses-per-revolution for your specific alternator before use.**

**Current firmware assumes 1 pulse per revolution:**
```cpp
totalRPM = 60.0 × (pulseCount / (interval_ms / 1000.0));
```

If the alternator fan has N magnets, divide `totalRPM` by N.

**Calibration procedure:**
1. Run engine at known idle RPM (use a tachometer or OBD reader)
2. Compare to displayed `totalRPM` in Serial Monitor
3. If off by factor N, adjust the multiplier accordingly

---

## Fuel Level Sensor

**Circuit:** Digital float switch, INPUT_PULLUP

```
D4 (INPUT_PULLUP) ─── switch ─── GND
```

- Switch OPEN (float up / fuel present) → pin reads HIGH → `StateLevel = 0`
- Switch CLOSED (float down / low fuel) → pin reads LOW → `StateLevel = 1`

The firmware reads:
```cpp
StateLevel = (!digitalRead(FuelLevel));  // 1 = low fuel warning
```

No calibration needed — binary sensor. Verify polarity with a multimeter
(check continuity when tank is full vs. empty).
