/*
 * AgrIoT LoRa Range Test
 * Upload to either gateway or node to measure link quality at distance.
 *
 * Sender: transmits a numbered packet every 2 seconds.
 * Receiver: prints RSSI and SNR for each received packet.
 *
 * Set SENDER true on one device, false on the other.
 * Both must use the same frequency and sync word.
 */

#include <SPI.h>
#include <LoRa.h>

// ── Set to true on the transmitting device ───────────────────────
const bool SENDER = true;

// ── Match these to your firmware ────────────────────────────────
const long  LORA_FREQ     = 868E6;
const int   LORA_TX_POWER = 20;
const int   LORA_SF       = 7;
const byte  SYNC_WORD     = 0xF3;

// ── Pin definitions — Node (Arduino Nano) ───────────────────────
//const int PIN_SS   = 10;
//const int PIN_RST  = 9;
//const int PIN_DIO0 = 2;

// ── Pin definitions — Gateway (ESP32 TTGO LoRa v2) ──────────────
const int PIN_SS   = 18;
const int PIN_RST  = 14;
const int PIN_DIO0 = 26;

int packetCount = 0;
int received    = 0;
int lost        = 0;
int lastId      = -1;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  LoRa.setPins(PIN_SS, PIN_RST, PIN_DIO0);
  if (!LoRa.begin(LORA_FREQ)) {
    Serial.println("LoRa init failed!");
    while (1);
  }

  LoRa.setTxPower(LORA_TX_POWER);
  LoRa.setSpreadingFactor(LORA_SF);
  LoRa.setSyncWord(SYNC_WORD);

  Serial.printf("LoRa range test — %s mode\n", SENDER ? "SENDER" : "RECEIVER");
  Serial.printf("Freq: %.0f MHz  SF%d  TX: %d dBm  Sync: 0x%02X\n",
                LORA_FREQ / 1e6, LORA_SF, LORA_TX_POWER, SYNC_WORD);
  Serial.println("---------------------------------------------------");
}

void loop() {
  if (SENDER) {
    String msg = "PING:" + String(packetCount);
    LoRa.beginPacket();
    LoRa.print(msg);
    LoRa.endPacket();
    Serial.printf("[%05d] Sent: %s\n", packetCount, msg.c_str());
    packetCount++;
    delay(2000);

  } else {
    int pkt = LoRa.parsePacket();
    if (pkt > 0) {
      String data = "";
      while (LoRa.available()) data += (char)LoRa.read();

      int id = -1;
      if (data.startsWith("PING:")) id = data.substring(5).toInt();

      if (lastId >= 0 && id > lastId + 1) {
        lost += (id - lastId - 1);
        Serial.printf("  ⚠ Lost %d packet(s)\n", id - lastId - 1);
      }
      lastId = id;
      received++;

      float pdr = 100.0 * received / (received + lost);
      Serial.printf("[%05d] RSSI: %3d dBm  SNR: %+.1f dB  PDR: %.1f%%  Data: %s\n",
                    id, LoRa.packetRssi(), LoRa.packetSnr(), pdr, data.c_str());
    }
  }
}
