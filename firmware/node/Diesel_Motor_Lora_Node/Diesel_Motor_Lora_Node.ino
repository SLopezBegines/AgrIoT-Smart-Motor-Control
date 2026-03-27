#include <ArduinoJson.h>
#include <SimpleTimer.h>
#include <LoRa.h>
#include <SPI.h>
#include <EEPROM.h>
#include "functions_rpm.h"
//#include "FlowMeter.h"
//#include "JSON.h"

//////LORA CONFIG////
//Modified from LoRaDuplexCallback example
//#define SCK 13    
//#define MISO 12   
//#define MOSI 11   
#define SS 10   
#define RST 9
#define DIO0 2
#define LORABAND  868E6 //frequency in Hz (433E6, 868E6, 915E6)


//SimpleTimer timer;

void setup() {
//PCB1  
  pinMode(Pressure, INPUT);
  pinMode(VoltageSensor, INPUT);
  pinMode(VoltageSensor2, INPUT);
  pinMode(RPM_PIN, INPUT);
  pinMode(FuelLevel, INPUT_PULLUP);
  pinMode(Relay_1, OUTPUT);
  pinMode(Relay_2, OUTPUT);
  pinMode(Relay_3, OUTPUT);
  pinMode(Relay_4, OUTPUT);
  pinMode(Relay_5, OUTPUT);
  pinMode(Relay_6, OUTPUT);
  pinMode(Relay_7, OUTPUT);
  pinMode(Relay_8, OUTPUT);
  // Set console baud rate
  Serial.begin(115200);

  //Setting all relays OFF.
  for (int q = 5; q < 9 ; q++) {
    digitalWrite(q, HIGH);
  }
  for (int s = 14; s < 19 ; s++) {
    digitalWrite(s, HIGH);
  }

  //Starting LORA
 LoRa.setPins(SS, RST, DIO0);
 if (!LoRa.begin(LORABAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

LoRa.setSyncWord(0xF3); //byte value to use as the sync word, defaults to 0x12
LoRa.setTxPower(20);
/*
 TX Power
Change the TX power of the radio.

LoRa.setTxPower(txPower);

LoRa.setTxPower(txPower, outputPin);
txPower - TX power in dB, defaults to 17
outputPin - (optional) PA output pin, supported values are PA_OUTPUT_RFO_PIN and PA_OUTPUT_PA_BOOST_PIN, defaults to PA_OUTPUT_PA_BOOST_PIN.
Supported values are 2 to 20 for PA_OUTPUT_PA_BOOST_PIN, and 0 to 14 for PA_OUTPUT_RFO_PIN.

Most modules have the PA output pin connected to PA BOOST.
*/
 //LoRa.onReceive(onReceive);//Register a callback function for when a packet is received.
 //LoRa.receive();//Puts the radio in continuous receive mode.
 
 Serial.println("LoRa init succeeded.");
 
timer.setInterval(60000L, LoRaSender);
//setupFlow();
setup_rpm();
}

void loop() {
timer.run();
loop_rpm();
//loop_flow();
loop_funtions();
  // parse for a packet, and call onReceive with the result:
onReceive(LoRa.parsePacket());
}
