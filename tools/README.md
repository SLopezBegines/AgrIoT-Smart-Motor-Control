# Tools

Utility scripts for development, testing, and debugging.

## serial-monitor.py

Timestamped serial monitor with event highlighting.

**Requirements:** `pip install pyserial`

```bash
python3 serial-monitor.py /dev/ttyUSB0        # Linux
python3 serial-monitor.py COM3                 # Windows
python3 serial-monitor.py /dev/ttyUSB0 115200  # Custom baud
```

Highlights LoRa packet events, relay activations, RSSI values, and errors.

## lora-range-test.ino

Upload to two devices (one as SENDER, one as RECEIVER) to characterise RF link
quality at distance. Reports RSSI, SNR, and packet delivery rate (PDR).

- Edit `SENDER = true/false` before uploading
- Adjust pin definitions for gateway (ESP32) vs node (Nano)
- Walk the range while monitoring the receiver's Serial output

## sensor-test.ino

Upload to the Arduino Nano node to verify all sensors before deploying the
main firmware. Reads voltage, pressure, fuel level, and RPM every second,
printing raw ADC values alongside calibrated engineering units.

No relays are activated by this sketch.
