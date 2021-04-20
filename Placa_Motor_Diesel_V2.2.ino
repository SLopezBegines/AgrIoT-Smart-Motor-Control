/**************************************************************
   For this example, you need to install Blynk library:
     https://github.com/blynkkk/blynk-library/releases/latest
   TinyGSM Getting Started guide:
     https://tiny.cc/tinygsm-readme
 **************************************************************
   Blynk is a platform with iOS and Android apps to control
   Arduino, Raspberry Pi and the likes over the Internet.
   You can easily build graphic interfaces for all your
   projects by simply dragging and dropping widgets.
   Blynk supports many development boards with WiFi, Ethernet,
   GSM, Bluetooth, BLE, USB/Serial connection methods.
   See more in Blynk library examples and community forum.
                  http://www.blynk.io/
   Change GPRS apm, user, pass, and Blynk auth token to run :)
 **************************************************************/
//////////////
//Version for PCB and Extender PCB from JLCPCB. October 2020
/////////////
/*///VIRTUAL PIN DEFINITIONS
  Blynk Virtual PINs
  V0  timeOff Input. Minutes to off Relay 4
  V1 Presion PCB1
  V2 Voltage PCB1
  V3 Led Start
  V4 Led OffMotor
  V5 Led Level
  V6  Relay 1
  V7  Relay 2
  V8  Relay 4
  V9  timeOff Input. Minutes to off Relay 8
  V10 Relay 5
  V11 Relay 6
  V12 Relay 8
  V13 Led Start PCB2
  V14 Led OffMotor PCB2
  V15 Led Level PCB2
  V16 Presion PCB2
  V17 Voltage PCB2
  V18 Timer Relay1&2-3  Timer ON/OFF Motor PCB1
  V19 Timer Relay5&6-7  Timer ON/OFF Motor PCB2
  V20 Timer ON/OFF Relay4
  V21 Timer ON/OFF Relay8
  V22 Led Relay4
  V23 Led Relay8
*////
#include "Global.h"
#include "blynk.h"
#include "functions.h"
#include "PCB2.h"


void setup() {
/*
  pinMode(Pressure2, INPUT);
  pinMode(VoltageSensor2, INPUT);
  pinMode(FuelLevel2, INPUT_PULLUP);
  pinMode(Relay5, OUTPUT);
  pinMode(Relay6, OUTPUT);
  pinMode(Relay7, OUTPUT);
  pinMode(Relay8, OUTPUT);
    timer.setInterval(1000L, sendSensor3);
  timer.setInterval(3000L, sendSensor4);
*/
  
  pinMode(Pressure, INPUT);
  pinMode(VoltageSensor, INPUT);
  pinMode(FuelLevel, INPUT_PULLUP);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  // Set console baud rate
  Serial.begin(57600);
  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  //  Serial.println("Initializing modem...");
  for (int q = 5; q < 9 ; q++) {
    digitalWrite(q, HIGH);
  }
  for (int s = 15; s < 19 ; s++) {
    digitalWrite(s, HIGH);
  }
  
  modem.restart();
  delay(3000);
  String modemInfo = modem.getModemInfo();
  // Serial.print("Modem: ");
  // Serial.println(modemInfo);
  // Unlock your SIM card with a PIN
  //modem.simUnlock("1234");
  Blynk.begin(auth, modem, apn, user, pass);
  timer.setInterval(1000L, sendSensor1);
  timer.setInterval(3000L, sendSensor2);

///Activate PCB2
  setup_PCB2();
//attachInterrupt(digitalPinToInterrupt(CaudalPin), ISRCountPulse, RISING);
//  t0 = millis();
}
void loop()
{
  Blynk.run();
  timer.run();
  /*
  if (!Blynk.connected()) {
    // modem.restart();
    Blynk.begin(auth, modem, apn, user, pass);
    //Serial.println("Restarting Modem...");
  }
  */
}
