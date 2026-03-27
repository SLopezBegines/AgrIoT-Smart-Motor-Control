# Troubleshooting

## LoRa Communication

### No packets received by gateway

1. Check that both gateway and node use the same sync word (`0xF3`)
2. Verify LoRa antennas are connected — transmitting without antenna can damage the RF frontend
3. Check RSSI in Serial Monitor (`LoRa.packetRssi()`):
   - \> −90 dBm: excellent
   - −90 to −110 dBm: good
   - \< −110 dBm: marginal, move devices closer or increase TX power
4. Confirm `LORABAND` matches on both devices (`868E6`)
5. Check SPI wiring on the Arduino Nano node (SCK=D13, MISO=D12, MOSI=D11, NSS=D10)

### Packets received but JSON parse fails

- Gateway logs `deserializeJson() failed`
- Usually caused by a truncated payload. The node sends a fixed 200-byte buffer;
  check that the LoRa packet length is correct
- Try reducing `char output[200]` to `char output[128]` if buffer is too large

### Intermittent packet loss

- Expected: LoRa SF7 has ~1% packet loss at range limits
- Engine ignition system creates strong 868 MHz interference — add a ferrite bead
  on the sensor cables near the engine
- If RSSI is good but SNR is negative (< −5 dB), there is RF noise. Increase
  spreading factor to SF9 or SF10 (update in `LoRa.h` and `functions_rpm.h`)

---

## Gateway (ESP32)

### WiFi fails to connect

- First boot: connect to `AutoConnectAP` and configure via captive portal
- Subsequent boots: if saved credentials are wrong, press the reset button
  for 3 s during boot to clear WiFiManager settings (depends on WiFiManager config)
- Check router is on 2.4 GHz (ESP32 does not support 5 GHz)

### Blynk not connecting

- Verify `BLYNK_AUTH_TOKEN` in `config.h` is correct
- Check Blynk template ID matches the console
- Ensure the device is online in Blynk console

### OTA update page not reachable

- Confirm device is on same network as your computer
- Try IP address instead of `motorgateway.local` (mDNS may not resolve on some routers)
- Check OTA credentials in `config.h`

---

## Node (Arduino Nano)

### Upload fails

- Switch processor setting: **ATmega328P (Old Bootloader)**
- Reduce upload speed to 57600 baud
- Try a different USB cable / USB-serial adapter
- Check that nothing is connected to D0/D1 (RX/TX) during upload

### Relay stuck ON after power cycle

- The node reads EEPROM state on boot and may re-activate a relay
- Connect Serial Monitor — it prints the restored `started` value
- To reset EEPROM: upload a small sketch that writes 0 to addresses 0 and 1, then re-upload the main firmware

### Engine RPM reads 0 or wildly incorrect

- Verify Hall effect sensor alignment with alternator fan magnets
- Check 5V supply to sensor
- Confirm interrupt is on D3 (INT1) — D2 is INT0 on the Nano
- If reading is double the actual RPM, the sensor is triggering on both edges; change `RISING` to `FALLING`

### Pressure reads 0 or saturated

- Check 5V power to the transducer
- Verify signal wire connected to A5
- If reading is 0 at engine idle, check the calibration constant in `PressionFunction1()`

---

## PCB and Power

### 3.3V rail missing

- Check LM1117T-3.3 orientation (TO-220: GND, Output, Input)
- Verify input capacitor (10 µF) on LM2596 output
- Check for shorts on the 3.3V plane

### Relays click but don't switch the load

- Relay modules are typically active-LOW; verify control pin goes LOW when activated
- Check relay COM and NO/NC wiring for the specific load circuit
- Verify load voltage/current is within relay rating (10A, 250VAC)

### High current draw on 12V rail

- Normal: ~200 mA at idle (Arduino + relay coils standby)
- With all 8 relays energised: ~400–600 mA
- If drawing > 1A at idle, check for a shorted relay coil or reversed LM2596 output
