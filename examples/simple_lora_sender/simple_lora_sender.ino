/*
 * Simple LoRa Sender — Standalone Example
 * Sends a JSON array packet every 5 seconds.
 * Use with simple_lora_receiver to verify a fresh LoRa link.
 *
 * Tested on: Arduino Nano + RFM95W
 */

#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>

// ── Pins (Arduino Nano) ───────────────────────────────────────────
#define SS_PIN   10
#define RST_PIN   9
#define DIO0_PIN  2

const long  FREQUENCY = 868E6;
const byte  SYNC_WORD = 0xF3;
const byte  DEST_ADDR = 0xFF;   // gateway
const byte  SRC_ADDR  = 0xBB;  // this node

byte msgCount = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  LoRa.setPins(SS_PIN, RST_PIN, DIO0_PIN);
  if (!LoRa.begin(FREQUENCY)) {
    Serial.println("LoRa init failed!");
    while (1);
  }

  LoRa.setSyncWord(SYNC_WORD);
  LoRa.setTxPower(20);
  Serial.println("LoRa sender ready (868 MHz, SF7, 20 dBm)");
}

void loop() {
  // Build a sample telemetry packet
  StaticJsonDocument<JSON_ARRAY_SIZE(9)> doc;
  JsonArray arr = doc.to<JsonArray>();
  arr.add(12.6f);  // battery voltage
  arr.add(14.2f);  // alternator voltage
  arr.add(3.1f);   // oil pressure
  arr.add(0);      // electrovalve off
  arr.add(1);      // motor-off indicator
  arr.add(0);      // motor-on indicator
  arr.add(1);      // fuel OK
  arr.add(1800);   // RPM
  arr.add(5);      // RPM slider

  char payload[200];
  serializeJson(doc, payload, sizeof(payload));

  LoRa.beginPacket();
  LoRa.write(DEST_ADDR);
  LoRa.write(SRC_ADDR);
  LoRa.write(msgCount);
  LoRa.write(sizeof(payload));
  LoRa.print(payload);
  LoRa.endPacket();

  Serial.printf("[%03d] Sent: %s\n", msgCount, payload);
  msgCount++;
  delay(5000);
}
