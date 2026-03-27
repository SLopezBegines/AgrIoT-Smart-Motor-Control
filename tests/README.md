# Tests

Unit tests for firmware logic that can be run without hardware.

## test_json_parsing

Verifies that the JSON serialisation/deserialisation used in the telemetry
protocol works correctly. Run on any Arduino-compatible board or in a
desktop C++ environment with ArduinoJson.

## Running on Desktop (without hardware)

ArduinoJson supports standard C++:

```bash
# Install ArduinoJson
git clone https://github.com/bblanchon/ArduinoJson.git

# Compile test (adjust include path)
g++ -std=c++17 -I ArduinoJson/src tests/test_json_parsing/test_json_parsing.ino -o test_json
./test_json
```

## Planned Tests

- `test_voltage_calibration` — verify voltage divider formula across ADC range
- `test_pressure_calibration` — verify pressure formula matches known values
- `test_rpm_calculation` — verify RPM formula for different pulse counts/intervals
- `test_eeprom_persistence` — verify state machine restores correctly after reset
