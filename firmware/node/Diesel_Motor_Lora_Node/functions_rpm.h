SimpleTimer timer;
//RPM variables//
#define RPM_PIN 3
long minRPM = 1000.0;
long maxRPM = 75000.0;
volatile float pulseCount3 = 0;
unsigned long RPM_Target;
int RPM_Slider = 0.0;
unsigned long TotalDuration;
volatile float frequency;
volatile float totalRPM = 0;
unsigned long lastmillis = 10000;
//////

////Pins definitions////
#define Relay_1 8   //Start
#define Relay_2 7   //OFF
#define Relay_3 6   
#define Relay_4 5   //OilPump
#define Relay_5 A0  //Throttle
#define Relay_6 A1  //Brake
#define Relay_7 A2  //ElectroValve
#define Relay_8 A3
#define VoltageSensor A7
#define VoltageSensor2 A6
#define Pressure A5
#define Pressure2 A4
#define FuelLevel 4

int StateRelay_1 = 0;
int StateRelay_2 = 0;
int StateRelay_3 = 0;
int StateRelay_4 = 0;
int StateRelay_5 = 0;
int StateRelay_6 = 0;
int StateRelay_7 = 0;
int StateRelay_8 = 0;
int LedRelay_1 = 0;  //Led_Relay_1
int LedRelay_2 = 0;  //Led_Relay_1
int LedRelay_3 = 0;  //Led_Relay_1
int LedRelay_4 = 0;  //Led_Relay_1
int LedRelay_5 = 0;  //Led_Relay_1
int LedRelay_6 = 0;  //Led_Relay_1
int LedRelay_7 = 0;  //Led_Relay_7 Electrovalve
int LedRelay_8 = 0;  //Led_Relay_1
float sumVoltage1 = 0;
float avgVoltage1 = 0;
float sumPresion1 = 0;
float avgPresion1 = 0;
float sumVoltage2 = 0;
float avgVoltage2 = 0;
float sumPresion2 = 0;
float avgPresion2 = 0;
int StateLevel = 0;  //Led Level 1
int addr = 0;
int addr1 = 1;
byte started;
byte executed;
bool stopped = true;
int TimeOffRelay8;
bool throttling = false;
bool braking = false;

uint8_t i = 0;
//Modified from LoRaDuplexCallback example
String LoRaData;
String jsonBuffer;
float rs;  //Rssi details
////Local Address
//String outgoing;              // outgoing message
//byte localAddress = 0xBB;     // address of this device
//////////////////
////Local Address & Destination Gateway
//String outgoing;              // outgoing message
byte msgCount = 0;         // count of outgoing messages
byte destination = 0xFF;   // destination to send to
byte localAddress = 0xBB;  // address of this device

//////////////////Deserialization//////
void onReceive(int packetSize) {
  if (packetSize == 0) return;        // if there's no packet, return
                                      // read packet header bytes:
  int recipient = LoRa.read();        // recipient address
  byte sender = LoRa.read();          // sender address
  byte incomingMsgId = LoRa.read();   // incoming msg ID
  byte incomingLength = LoRa.read();  // incoming msg length

  LoRaData = "";  // payload of packet

  while (LoRa.available()) {                // can't use readString() in callback, so
    LoRaData += (String)LoRa.readString();  // add bytes one by one
  }
  /*
    if (incomingLength != LoRaData.length()) {   // check length for error
    Serial.println("error: message length does not match length");
    return;                             // skip rest of function
  }
  */

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    return;  // skip rest of function
  }

  // if message is for this device, or broadcast, print details:
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + LoRaData);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  //Serial.println();

  StaticJsonDocument<200> doc;
  // parse a JSON array
  DeserializationError error = deserializeJson(doc, LoRaData);
  if (error == DeserializationError::Ok) {
    StateRelay_1 = (doc["V1"].as<int>());
    StateRelay_2 = (doc["V2"].as<int>());
    StateRelay_3 = (doc["V3"].as<int>());
    StateRelay_4 = (doc["V4"].as<int>());
    StateRelay_5 = (doc["V5"].as<int>());
    StateRelay_6 = (doc["V6"].as<int>());
    StateRelay_7 = (doc["V7"].as<int>());
    StateRelay_8 = (doc["V8"].as<int>());
    RPM_Slider = (doc["V24"].as<int>());
    Serial.print("  StateRelay_1: ");
    Serial.print(StateRelay_1);
    Serial.println();
    Serial.print("  StateRelay_2: ");
    Serial.print(StateRelay_2);
    Serial.println();
    Serial.print("  StateRelay_3: ");
    Serial.print(StateRelay_3);
    Serial.println();
    Serial.print("  StateRelay_4: ");
    Serial.print(StateRelay_4);
    Serial.println();
    Serial.print("  StateRelay_5: ");
    Serial.print(StateRelay_5);
    Serial.println();
    Serial.print("  StateRelay_6: ");
    Serial.print(StateRelay_6);
    Serial.println();
    Serial.print("  StateRelay_7: ");
    Serial.print(StateRelay_7);
    Serial.println();
    Serial.print("  StateRelay_8: ");
    Serial.print(StateRelay_8);
    Serial.println();
    Serial.print("  RPM_Slider: ");
    Serial.print(RPM_Slider);
    Serial.println();
    onReceive(LoRa.parsePacket());
  }
}
//////////End of Deserialization////////////

////////////Serialization/////////
void LoRaSender() {
  // compute the required size
  const size_t CAPACITY = JSON_ARRAY_SIZE(9);
  // allocate the memory for the document
  StaticJsonDocument<CAPACITY> doc;
  // create an empty array
  JsonArray array = doc.to<JsonArray>();
  // add some values
  array.add(avgVoltage1);
  array.add(avgVoltage2);
  array.add(avgPresion1);
  array.add(LedRelay_7);
  array.add(LedRelay_3);
  array.add(LedRelay_4);
  array.add(StateLevel);
  array.add(totalRPM);
  array.add(RPM_Slider);
  //array.add(avgVoltage1);
  // serialize the array and send the result to Serial
  //serializeJson(doc, Serial);
  char output[200];
  serializeJson(doc, output, sizeof(output));
  LoRa.beginPacket();
  LoRa.write(destination);     // add destination address
  LoRa.write(localAddress);    // add sender address
  LoRa.write(msgCount);        // add message ID
  LoRa.write(sizeof(output));  // add payload length
  //LoRa.write(output.length());        // add payload length
  LoRa.print(output);
  LoRa.endPacket();
  msgCount++;
  Serial.print("Sending packet: ");
  Serial.print("  Received from: 0x" + String(localAddress, HEX));
  Serial.print("  Sent to: 0x" + String(destination, HEX));
  Serial.print("  Message ID: " + String(msgCount));
  Serial.print("  Message length: ");
  Serial.print(String(sizeof(output)));
  Serial.println("  Message: ");
  Serial.print(output);
  Serial.println();
  onReceive(LoRa.parsePacket());
}

///////End of Serialization/////////////
////////////////////////////////////////////////////

///////////RPM Function////////////////
// Funtion for RPM counter by hall sensor attacher to alternator fan.

void ISRCountPulse3() {
  pulseCount3++;
}

void setup_rpm() {
  attachInterrupt(digitalPinToInterrupt(RPM_PIN), ISRCountPulse3, RISING);
}

void loop_rpm() {
  /*
if ((millis() - lastmillis) >= 2000){
 // RPM_Target = constrain(RPM_Target,minRPM,maxRPM);
  RPM_Target = map(RPM_Slider, 0, 10, minRPM, maxRPM);
  unsigned long DurationHigh = pulseIn(3, HIGH);
  unsigned long DurationLow = pulseIn(3,LOW);
  TotalDuration = DurationHigh + DurationLow;
  frequency = 1000000/TotalDuration;
  totalRPM = frequency *60;
  Serial.print("RPM: ");
  Serial.print(totalRPM);
  Serial.print(" \t Hz: \t ");
  Serial.print(frequency);
  Serial.print(" \t RPM_Target: \t ");
  Serial.println(RPM_Target);
  lastmillis = millis();
}
  */
  if ((millis() - lastmillis) >= 1000) {              // Only process counters once per second. Reading frequency in Hz. So, PulseCount3 will be frequency in Hz.
    detachInterrupt(digitalPinToInterrupt(RPM_PIN));  //Disable Interrupt when calculating
    unsigned long interval = millis() - lastmillis;
    totalRPM = 60.0 * (pulseCount3 / (interval / 1000));  //Convert frecuency to RPM, note: this works for one interruption per full rotation. For two interrups per full rotation use rpmcount * 30.
   /*
  Serial.print("interval:  ");
  Serial.print(interval);
  Serial.print(" Counter: ");
  Serial.print(pulseCount3);
  Serial.print(" RPM: ");
  Serial.print(totalRPM);
  Serial.print("  Hz:  ");
  Serial.println(pulseCount3/(interval/1000));  
  */
    lastmillis = millis();
    pulseCount3 = 0;                                                          // Reset the pulse counter so we can start incrementing again
    attachInterrupt(digitalPinToInterrupt(RPM_PIN), ISRCountPulse3, RISING);  // Enable the interrupt again now that we've finished sending output
  }
}
//////////////Throttle and Brake Motor//////////
/////Throttle and brake by pushbutton
void STOPPush(){
  digitalWrite(Relay_5, HIGH);
  digitalWrite(Relay_6, HIGH),
  throttling = false; 
  braking = false;
  return;  
  }

void ThrottlePush(){
  digitalWrite(Relay_6,HIGH);
  digitalWrite(Relay_5,LOW);
  throttling = true; 
  Serial.println("Throttling for 1s... ");
  timer.setTimeout(1000, STOPPush);  
  }
void BrakePush(){
  digitalWrite(Relay_5, HIGH);
  digitalWrite(Relay_6, LOW);
  braking = true;
  Serial.println("Braking for 1s... ");
  timer.setTimeout(1000, STOPPush);
  }


////Throttle///
void Throttle() {
  digitalWrite(Relay_6, HIGH);
  digitalWrite(Relay_5, LOW);
  Serial.println("Throttling... ");
  stopped = false;
  return;
}

/////Brake//////
void Brake() {
  digitalWrite(Relay_5, HIGH);
  digitalWrite(Relay_6, LOW);
  Serial.println("Braking... ");
  stopped = false;
  return;
}
//STOP///
void STOP() {
  digitalWrite(Relay_5, HIGH);
  digitalWrite(Relay_6, HIGH);
  stopped = true;
  Serial.println("Stopping Linear Actuator... ");
  return;
}

void AdjustRPM() {
  RPM_Target = map(RPM_Slider, 1, 10, minRPM, maxRPM);
    ///Throttle and Brake

  if (totalRPM >= 0) {
    if (started == 1 && totalRPM < 1.1 * RPM_Target && totalRPM > 0.9 * RPM_Target) {
      STOP();
    }
    if (started == 1 && totalRPM > RPM_Target) {
      Brake();
    }
    if (started == 1 && totalRPM < RPM_Target) {
      Throttle();
    }
  } 
  
  //else {
    //if (started == 1 && stopped == false) {
      //STOP();
      //Add LoraSender Notify for fail in RPM counter
    //}
  //}
}
void FertilizerON() {
  Serial.println("Relay 7 enabled");
  digitalWrite(Relay_7, LOW);
  LedRelay_7 = (!digitalRead(Relay_7));
  executed = 1;
  EEPROM.update(addr1, executed);
  LoRaSender();
  return;
}
void FertilizerOFF() {
  Serial.println("Relay 7 disabled");
  digitalWrite(Relay_7, HIGH);
  LedRelay_7 = (!digitalRead(Relay_7));
  executed = 0;
  EEPROM.update(addr1, executed);
  LoRaSender();
  return;
}
//////////////////////////////////////

//AnalogReadings
void VoltageFunction1() {
  sumVoltage1 = 0;
  avgVoltage1 = 0;
  for (i = 0; i <= 100; i++) {
    int rawVsensor1 = analogRead(VoltageSensor);
    float voltage1 = rawVsensor1 * 5.0 * (22.0 + 4.7) / (1023.0 * 4.7);  //Vin=((AnalogRead(A0)*5Volts*(R1+R2))/(R2*1023). R1=22K. R2=4.7K. From Voltage Divider calculator
    sumVoltage1 += voltage1;
  }
  avgVoltage1 = sumVoltage1 / 100;  //Find average
}
void VoltageFunction2() {
  sumVoltage2 = 0;
  avgVoltage2 = 0;
  for (i = 0; i <= 100; i++) {
    int rawVsensor2 = analogRead(VoltageSensor2);
    float voltage2 = rawVsensor2 * 5.0 * (22.0 + 4.7) / (1023.0 * 4.7);  //Vin=((AnalogRead(A0)*5Volts*(R1+R2))/(R2*1023). R1=22K. R2=4.7K. From Voltage Divider calculator
    sumVoltage2 += voltage2;
  }
  avgVoltage2 = sumVoltage2 / 100;  //Find average
}
void PressionFunction1() {
  sumPresion1 = 0;
  avgPresion1 = 0;
  for (i = 0; i <= 100; i++) {
    int rawPresion1 = analogRead(Pressure);
    //    float Presion1 = ((rawPresion1 * 5000.0 * 0.0034) - 1.7237) / 1023.0; //Presion=((AnalogRead(A7)*5000miliVolts*0.0034)-1.7237))/1023
    float Presion1 = (rawPresion1 * 5000.0 * 0.0028) / 1023.0;  //Presion=((AnalogRead(A7)*5000miliVolts*0.0028))/1023

    sumPresion1 += Presion1;
  }
  avgPresion1 = sumPresion1 / 100;  //Find average
}
//Activating Relays PCB1

void turnRelay3OFF() {                           //Relay 3 OFF
  digitalWrite(Relay_3, HIGH);
  digitalWrite(Relay_4, HIGH);
  Serial.println("Relay3 disabled");
  LedRelay_3 = (!digitalRead(Relay_3));
  LoRaSender();
  return;
    }
void turnRelay3ON() {                             //Relay 3 ON
  digitalWrite(Relay_3, LOW);
  digitalWrite(Relay_4, HIGH);
  LedRelay_3 = (!digitalRead(Relay_3));
  Serial.println("Relay3 enabled");
  timer.setTimeout(3000, turnRelay3OFF);          //Time to off Relay3 3000ms
  LoRaSender();
  }
  
void turnRelay2Off() {  //Relay 2 OFF
  digitalWrite(Relay_2, HIGH);
  Serial.println("Relay2 disabled");
  timer.setTimeout(5000, turnRelay3ON);///Time to OFF Relay 3
  LedRelay_3 = (!digitalRead(Relay_2));
  RPM_Slider = 0;
  LoRaSender();
  return;
}
void OffMotor() {           //Relay2 ON
  if (totalRPM > minRPM) {  //10% greater of minRPM
    Brake();
  } else {
    STOP();
  }
  Serial.println("Relay2 enabled");
  digitalWrite(Relay_4, HIGH);
  digitalWrite(Relay_2, LOW);
  started = 0;
  EEPROM.update(addr, started);
  Serial.print("Started?: ");
  Serial.println(EEPROM.get(addr, started));
  LedRelay_3 = (!digitalRead(Relay_2));
  LedRelay_4 = (!digitalRead(Relay_4));
  StateRelay_2 = 0;
  //LoRaSender();
  timer.setTimeout(550, turnRelay2Off);  //Time to off Relay2 5000ms
  return;
}
void turnRelay1Off() {  //Relay 1 OFF
  digitalWrite(Relay_1, HIGH);
  digitalWrite(Relay_4, LOW);
  StateRelay_1 = 0;
  LedRelay_1 = (!digitalRead(Relay_1));
  LedRelay_4 = (!digitalRead(Relay_4));
  LoRaSender();
  Serial.println("Relay1 disabled");
  return;
}
void Start() {  //Relay1 ON
  Serial.println("Relay 1 enabled");
  digitalWrite(Relay_1, LOW);
  digitalWrite(Relay_4, LOW);
  started = 1;
  EEPROM.update(addr, started);
  Serial.print("Started?: ");
  Serial.println(EEPROM.get(addr, started));
  timer.setTimeout(500, turnRelay1Off);  //Time to off Relay2 500ms
  return;
}

/*
void TimerValveON() {
  //Timer ON&OFF. ON Relay1 & OFF Relay3
  Serial.println("Relay 8 enabled");
  digitalWrite(Relay_8, LOW);
  LedRelay_8 = (!digitalRead(Relay_8) * 255);
  StateRelay_8 = 1;
 }
void TimerValveOFF() {
  Serial.println("Relay8 disabled");
  digitalWrite(Relay_8, HIGH);
  LedRelay_8 = (!digitalRead(Relay_8) * 255);
  StateRelay_8 = 0;
 }
*/

void loop_funtions() {
  started = EEPROM.get(addr, started);
  executed = EEPROM.get(addr1, executed);

  VoltageFunction1();
  delay(10);
  VoltageFunction2();
  delay(10);
  PressionFunction1();
  delay(10);
  LedRelay_3 = (!digitalRead(Relay_2));  //OFF Led
  LedRelay_4 = (!digitalRead(Relay_4));  //ON Led
  StateLevel = (!digitalRead(FuelLevel));
  if (StateRelay_1 == 1 && started == 0) {
    Start();
  }
  if (StateRelay_2 == 1 && started == 1) {
    OffMotor();
  }
  if (StateRelay_7 == 1 && executed == 0) {

    FertilizerON();
  }
  if (StateRelay_7 == 0 && executed == 1) {
    FertilizerOFF();
  }
  //AdjustRPM();
    if (StateRelay_5 == 1 && throttling == 0) {
    ThrottlePush();
    }
  if (StateRelay_6 == 1 && braking == 0) {
    BrakePush();
  }
  }
