SimpleTimer timer;
//Taken from https://github.com/pkarun/Blynk-Flow-Sensor-Water-Meter
//& https://www.luisllamas.es/caudal-consumo-de-agua-con-arduino-y-caudalimetro/
/*
 *Flowmeter
Modelo    Conexión  Caudales    K-factor
YF-S201   1/2"      1-30 L/min  7.5
FS300A    3/4"      1-60 L/min  5.5
FS400A    1”        1-60 L/min  3.5
*/
/*
//Flow Variables//
#define CAUDAL_PIN 3
#define FLOW_CALIBRATION 7.5 //K-factor
int VPIN_RESET = 0;
volatile long pulseCount3 = 0;
float         flowRate;
unsigned int  flowMilliLitres;
unsigned long totalMilliLitres;
float         totalLitres;
unsigned long oldTime;
////////
*/
//RPM variables//
#define RPM_PIN 3
volatile long pulseCount3 = 0;
float         totalRPM;
unsigned long oldTime;
//////
int TimeOffRelay8;
bool executed = false;
////Pins definitions////
#define Relay_1 8
#define Relay_2 7
#define Relay_3 6
#define Relay_4 5
#define Relay_5 15
#define Relay_6 16
#define Relay_7 17
#define Relay_8 18
#define VoltageSensor A0
#define Pressure A7
#define FuelLevel 4
int StateRelay_1 = 0; 
int StateRelay_2 = 0;
int StateRelay_3 = 0;
int StateRelay_4 = 0;
int StateRelay_5 = 0;
int StateRelay_6 = 0;
int StateRelay_7 = 0;
int StateRelay_8 = 0;
int LedRelay_1 = 0; //Led_Relay_1
int LedRelay_2 = 0; //Led_Relay_1
int LedRelay_3 = 0; //Led_Relay_1
int LedRelay_4 = 0; //Led_Relay_1
int LedRelay_5 = 0; //Led_Relay_1
int LedRelay_6 = 0; //Led_Relay_1
int LedRelay_7 = 0; //Led_Relay_1
int LedRelay_8 = 0; //Led_Relay_1
float sumVoltage1 = 0;
float avgVoltage1 = 0;
float sumPresion1 = 0;
float avgPresion1 = 0;
int StateLevel = 0; //Led Level 1
int addr = 0;
byte started;
uint8_t i = 0;
//Modified from LoRaDuplexCallback example
String LoRaData;
String jsonBuffer;
float rs; //Rssi details
////Local Address
//String outgoing;              // outgoing message
//byte localAddress = 0xBB;     // address of this device
//////////////////
////Local Address & Destination Gateway
//String outgoing;              // outgoing message
byte msgCount = 0;            // count of outgoing messages
byte destination = 0xFF;      // destination to send to
byte localAddress = 0xBB;     // address of this device

//////////////////Deserialization//////
void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return
   // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  LoRaData="";     // payload of packet
  
  while (LoRa.available()) {            // can't use readString() in callback, so
    LoRaData += (String)LoRa.readString();      // add bytes one by one
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
    return;                             // skip rest of function
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
if (error == DeserializationError::Ok)
{
  StateRelay_1 = (doc["V1"].as<int>()); 
  StateRelay_2 = (doc["V2"].as<int>()); 
  StateRelay_3 = (doc["V3"].as<int>()); 
  StateRelay_4 = (doc["V4"].as<int>()); 
  StateRelay_5 = (doc["V5"].as<int>()); 
  StateRelay_6 = (doc["V6"].as<int>()); 
  StateRelay_7 = (doc["V7"].as<int>()); 
  StateRelay_8 = (doc["V8"].as<int>()); 
  //VPIN_RESET = (doc["V11"].as<int>());
      
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
      //Serial.print("  VPIN_RESET: ");
      //Serial.print(VPIN_RESET);
      //Serial.println();
    onReceive(LoRa.parsePacket());
  }
 }
//////////End of Deserialization////////////
 
////////////Serialization/////////
void LoRaSender(){
  // compute the required size
const size_t CAPACITY = JSON_ARRAY_SIZE(9);
// allocate the memory for the document
StaticJsonDocument<CAPACITY> doc;
// create an empty array
JsonArray array = doc.to<JsonArray>();
// add some values
array.add(avgVoltage1);
array.add(avgPresion1);
array.add(LedRelay_4);
array.add(LedRelay_3);
array.add(LedRelay_8);
array.add(StateLevel);
//array.add(totalLitres);
//array.add(flowRate);
array.add(totalRPM);
// serialize the array and send the result to Serial
//serializeJson(doc, Serial);
char output[200];
  serializeJson(doc, output, sizeof(output));
  LoRa.beginPacket();
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(sizeof(output));        // add payload length
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
///////////////////////////////////////////////////
/*
/////Flow Meter Functions///////////////
// Funtion for water flowmeter in water line to count fertilizer liters. 
void flow(){
  if ((millis() - oldTime) > 1000)   // Only process counters once per second
  {
    detachInterrupt(CAUDAL_PIN);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount3) / FLOW_CALIBRATION;
    oldTime = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
    totalLitres = totalMilliLitres * 0.001;
    unsigned int frac;
    frac = (flowRate - int(flowRate)) * 10; // Determine the fractional part. The 10 multiplier gives us 1 decimal place.
    pulseCount3 = 0;  // Reset the pulse counter so we can start incrementing again
    attachInterrupt(digitalPinToInterrupt(CAUDAL_PIN), ISRCountPulse3, FALLING);    // Enable the interrupt again now that we've finished sending output
  }
}
void resetflow(){
   Serial.println("Clearing Data");
   flowRate = 0;
   flowMilliLitres = 0;
   totalMilliLitres = 0;
   totalLitres = 0;
   VPIN_RESET = 0;
   return;
   }

void ISRCountPulse3(){
  pulseCount3++;
}

void setupFlow(){
  pulseCount3        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;
  pinMode(CAUDAL_PIN, INPUT);  // Initialization of the variable "CAUDAL_PIN" as INPUT (D3 pin)
  attachInterrupt(digitalPinToInterrupt(CAUDAL_PIN), ISRCountPulse3, FALLING);
 }
void loop_flow(){
if (VPIN_RESET == 1){
  resetflow();
}
if (StateRelay_8 == 1 && executed==false) {
  FertilizerON();
  resetflow();
  }
if(StateRelay_8 == 0 && executed==true) {
  FertilizerOFF();  
  }
flow();
}
*/
///////////RPM Function////////////////
// Funtion for RPM counter by hall sensor attacher to alternator fan.
void rpm(){
  if ((millis() - oldTime) > 900)   // Only process counters once per 900ms
  {
    detachInterrupt(RPM_PIN);
    totalRPM = 60000.0/(millis()-oldTime)*pulseCount3;
    oldTime = millis();
    pulseCount3 = 0;  // Reset the pulse counter so we can start incrementing again
    attachInterrupt(digitalPinToInterrupt(RPM_PIN), ISRCountPulse2, FALLING);    // Enable the interrupt again now that we've finished sending output
  }
  
}
void setup_rpm(){
   pulseCount3 = 0;
   attachInterrupt(digitalPinToInterrupt(CAUDAL_PIN), ISRCountPulse3, CHANGE);
   oldTime = 0;
}
void loop_rpm(){
  rpm();
  
}
void FertilizerON(){
    Serial.println("Relay 8 enabled");
    digitalWrite(Relay_8, LOW);
    LedRelay_8 = (!digitalRead(Relay_8) * 255);
    //resetflow();
    executed=true;
    LoRaSender();
    return;
}
void FertilizerOFF(){
    Serial.println("Relay 8 disabled");
    digitalWrite(Relay_8, HIGH);
    LedRelay_8 = (!digitalRead(Relay_8) * 255);
    executed=false;
    LoRaSender();
    return;
}
//////////////////////////////////////

//AnalogReadings
void VoltageFunction1() {
  sumVoltage1 = 0;
  avgVoltage1 = 0;
  for (i = 0; i <= 100; i++) {
    int   rawVsensor1 = analogRead(VoltageSensor);
    float voltage1 = rawVsensor1 * 5.0 * (22.0 + 4.7) / (1023.0 * 4.7); //Vin=((AnalogRead(A0)*5Volts*(R1+R2))/(R2*1023). R1=22K. R2=4.7K. From Voltage Divider calculator
    sumVoltage1 += voltage1;
  }
  avgVoltage1 = sumVoltage1 / 100;                                     //Find average
}

void PressionFunction1() {
  sumPresion1 = 0;
  avgPresion1 = 0;
  for (i = 0; i <= 100; i++) {
    int   rawPresion1 = analogRead(Pressure);
//    float Presion1 = ((rawPresion1 * 5000.0 * 0.0034) - 1.7237) / 1023.0; //Presion=((AnalogRead(A7)*5000miliVolts*0.0034)-1.7237))/1023
    float Presion1 = (rawPresion1 * 5000.0 * 0.0028) / 1023.0; //Presion=((AnalogRead(A7)*5000miliVolts*0.0028))/1023

    sumPresion1 += Presion1;
  }
  avgPresion1 = sumPresion1 / 100;                                   //Find average

}
//Activating Relays PCB1
void turnRelay3OFF() {                           //Relay 3 OFF
  digitalWrite(Relay_3, HIGH);
  digitalWrite(Relay_4, HIGH);
  Serial.println("Relay3 disabled");
  LedRelay_3 = (!digitalRead(Relay_3) * 255);
  LoRaSender();
  return;
    }
void turnRelay3ON() {                             //Relay 3 ON
  digitalWrite(Relay_3, LOW);
  digitalWrite(Relay_4, HIGH);
  LedRelay_3 = (!digitalRead(Relay_3) * 255);
  Serial.println("Relay3 enabled");
  timer.setTimeout(3000, turnRelay3OFF);          //Time to off Relay3 3000ms
  LoRaSender();
  }
void turnRelay2Off() {                            //Relay 2 OFF
  digitalWrite(Relay_2, HIGH);
  Serial.println("Relay2 disabled");
  timer.setTimeout(5000, turnRelay3ON);///Time to OFF Relay 3
  return;
 }
void OffMotor() {           //Relay2 ON
    Serial.println("Relay2 enabled");
    digitalWrite(Relay_4, HIGH);
    digitalWrite(Relay_2, LOW);
    started = 0;
    EEPROM.update(addr, started);
    Serial.print("Started?: "); 
    Serial.println(EEPROM.get(addr, started));
    LedRelay_2 = (!digitalRead(Relay_2) * 255);
    LedRelay_4 = (!digitalRead(Relay_4) * 255);
    StateRelay_2 = 0;
    LoRaSender();
    timer.setTimeout(580, turnRelay2Off);         //Time to off Relay2 580ms
    return;
  } 
void turnRelay1Off() {                           //Relay 1 OFF
  digitalWrite(Relay_1, HIGH);
  digitalWrite(Relay_4, LOW);
  StateRelay_1 = 0;
  Serial.println("Relay1 disabled");
return;
}
void Start() {      //Relay1 ON
    Serial.println("Relay 1 enabled");
    digitalWrite(Relay_1, LOW);
    digitalWrite(Relay_4, LOW);
    started = 1;
    EEPROM.update(addr, started);
    Serial.print("Started?: ");
    Serial.println(EEPROM.get(addr, started));
    LedRelay_1 = (!digitalRead(Relay_1) * 255);
    LedRelay_4 = (!digitalRead(Relay_4) * 255);
    LoRaSender();
    timer.setTimeout(500, turnRelay1Off);        //Time to off Relay2 500ms
    return;
    }

void loop_funtions() {
  started = EEPROM.get(addr, started);
  VoltageFunction1();
  delay(2);
  PressionFunction1();
  //timer2.setInterval(730,VoltageFunction1);
  //timer3.setInterval(890,PressionFunction1);
  LedRelay_3 = (!digitalRead(Relay_3) * 255);
  LedRelay_4 = (!digitalRead(Relay_4) * 255);
  StateLevel = (!digitalRead(FuelLevel) * 255);
if (StateRelay_1 == 1) {
    Start();
    }
if (StateRelay_2 == 1) {
    OffMotor();
    }
if (StateRelay_8 == 1){
  FertilizerON();
}
if (StateRelay_8 == 0 && executed==true){
  FertilizerOFF();
}
}
