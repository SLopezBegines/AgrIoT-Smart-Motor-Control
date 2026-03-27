
//#define SCK     5    // GPIO5  -- SX1278's SCK
//#define MISO    19   // GPIO19 -- SX1278's MISO
//#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     23   // GPIO14 -- SX1278's RESET
#define DIO0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define SDA 21
#define SCL 22
#define ADDRESS 0x3C
#define LORABAND  868E6 //frequency in Hz (433E6, 868E6, 915E6)

void Lora(){
//Starting LORA
  LoRa.setPins(SS, RST, DIO0);
  while (!Serial);
  Serial.println("LoRa Sender");

  if (!LoRa.begin(LORABAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
LoRa.setSyncWord(0xF3); //byte value to use as the sync word, defaults to 0x12
/*
 * LoRa.setTxPower(txPower,RFOUT_pin);
 * txPower -- 0 ~ 20
 * RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
 *   - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
 *   - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
*/
  LoRa.setTxPower(20);
  LoRa.setSpreadingFactor(7);           // ranges from 6-12,default 7 see API docs

  //LoRa.onReceive(onReceive);//Register a callback function for when a packet is received.
  LoRa.receive();//Puts the radio in continuous receive mode.
  Serial.println("LoRa init succeeded.");
}
