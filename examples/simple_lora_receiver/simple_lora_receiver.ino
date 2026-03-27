/*
 * Simple LoRa Receiver — Standalone Example
 * Receives JSON array packets from simple_lora_sender and prints
 * decoded values with RSSI and SNR.
 *
 * Tested on: ESP32 TTGO LoRa v2
 */

#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>

// ── Pins (ESP32 TTGO LoRa v2) ──────────────────────────────────
#define SS_PIN   18
#define RST_PIN  14
#define DIO0_PIN 26

const long  FREQUENCY = 868E6;
const byte  SYNC_WORD = 0xF3;
const byte  MY_ADDR   = 0xFF;   // this device (gateway address)

void setup() {
  Serial.begin(115200);
  while (!Serial);

  LoRa.setPins(SS_PIN, RST_PIN, DIO0_PIN);
  if (!LoRa.begin(FREQUENCY)) {
    Serial.println("LoRa init failed!");
    while (1);
  }

  LoRa.setSyncWord(SYNC_WORD);
  Serial.println("LoRa receiver ready — waiting for packets...");
  Serial.println("Idx | Voltage | AltVolt | Pressure | Valve | OFF | ON | Fuel |  RPM | Slider | RSSI | SNR");
}

void loop() {
  int pktSize = LoRa.parsePacket();
  if (pktSize == 0) return;

  // Read header
  int  dest    = LoRa.read();
  byte src     = LoRa.read();
  byte msgId   = LoRa.read();
  byte length  = LoRa.read();

  String data = "";
  while (LoRa.available()) data += (char)LoRa.read();

  if (dest != MY_ADDR && dest != 0xFF) return;  // not for us

  StaticJsonDocument<JSON_ARRAY_SIZE(9)> doc;
  DeserializationError err = deserializeJson(doc, data);
  if (err) {
    Serial.printf("Parse error: %s | raw: %s\n", err.c_str(), data.c_str());
    return;
  }

  Serial.printf("%3d | %7.2f | %7.2f | %8.2f | %5d | %3d | %2d | %4d | %4.0f | %6d | %4d | %+.1f\n",
    msgId,
    doc[0].as<float>(),   // battery V
    doc[1].as<float>(),   // alternator V
    doc[2].as<float>(),   // pressure bar
    doc[3].as<int>(),     // electrovalve
    doc[4].as<int>(),     // motor-off
    doc[5].as<int>(),     // motor-on
    doc[6].as<int>(),     // fuel
    doc[7].as<float>(),   // RPM
    doc[8].as<int>(),     // slider
    LoRa.packetRssi(),
    LoRa.packetSnr()
  );
}
