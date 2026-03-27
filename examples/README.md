# Examples

Standalone sketches to test individual subsystems without the full firmware.

## simple_lora_sender

Sends a sample telemetry JSON array every 5 seconds.
- Upload to: Arduino Nano + RFM95W (node hardware)
- Pair with: `simple_lora_receiver`

## simple_lora_receiver

Receives telemetry packets and prints decoded values + RSSI/SNR.
- Upload to: ESP32 TTGO LoRa v2 (gateway hardware)
- Pair with: `simple_lora_sender`

## Usage

1. Upload `simple_lora_sender` to the node hardware
2. Upload `simple_lora_receiver` to the gateway hardware
3. Open Serial Monitor (115200 baud) on the receiver
4. Verify packets are received with good RSSI (> −100 dBm at bench distance)
5. Move devices apart to characterise range (or use `tools/lora-range-test.ino`
   for a proper link budget test with packet loss statistics)
