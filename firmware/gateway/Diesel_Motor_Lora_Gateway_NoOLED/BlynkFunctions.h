int StateRelay_1 = 0; 
int StateRelay_2 = 0;
int StateRelay_3 = 0;
int StateRelay_4 = 0;
int StateRelay_5 = 0;
int StateRelay_6 = 0;
int StateRelay_7 = 0;
int StateRelay_8 = 0;
int StateTimerON = 0;
int StateTimerValve = 0;
int VPIN_RESET = 0;
int RPM_Slider=0;
float   Voltage;
float   Voltage2;
float   Presion;
//float   Voltage2;
//float   Presion2;
float   LedRelay_7; 
float   LedRelay_3; 
float   LedRelay_4; 
float   StateLevel; 
float   TotalLitres; 
float   FlowRate; 
float   RPM; 
int recipient;          // recipient address
byte sender;            // sender address
byte incomingMsgId;     // incoming msg ID
byte incomingLength;    // incoming msg length
int times=0;
WidgetLED ledON(V18);//Led Start
WidgetLED ledOFF(V17);//Led OffMotor
WidgetLED ledValve(V19);//Led Level
WidgetLED ledFuel(V20);//Led Relay4
WidgetTerminal terminal(V0);
//modified from https://www.hackster.io/vinayyn/lora-blynk-app-interfacing-1e1851
//Modified from LoRaDuplexCallback example

////Local Address & Destination Gateway
//String outgoing;              // outgoing message
byte msgCount = 0;            // count of outgoing messages
byte destination = 0xBB;      // destination to send to
byte destination2 = 0xBC;      // destination to send to
byte localAddress = 0xFF;     // address of this device

//////////////////
///////Deserialization////
//Modified from LoRaDuplexCallback example
String LoRaData;
String jsonBuffer;
float rs; //Rssi details
//////////////////
 void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return
    // read packet header bytes:
  recipient = LoRa.read();          // recipient address
  sender = LoRa.read();            // sender address
  incomingMsgId = LoRa.read();     // incoming msg ID
  incomingLength = LoRa.read();    // incoming msg length

  LoRaData="";     // payload of packet
 
  while (LoRa.available()) {            // can't use readString() in callback, so
    LoRaData += (char)LoRa.read();      // add bytes one by one
  }
  Serial.println();
  Serial.print("Received from: 0x" + String(sender, HEX));
  Serial.println("  Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.print("  Message length: " + String(incomingLength));
  Serial.println("Message: " + LoRaData);
  Serial.print("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("  Snr: " + String(LoRa.packetSnr()));
  Serial.println();
  
  // compute the required size
const size_t CAPACITY = JSON_ARRAY_SIZE(11);
// allocate the memory for the document
StaticJsonDocument<CAPACITY> doc;
// parse a JSON array
DeserializationError error = deserializeJson(doc, LoRaData);
if (error) {
  Serial.print(F("deserializeJson() failed: "));
  Serial.println(error.f_str());
  return;
}
/*
// if the recipient isn't this device or broadcast,
 if (recipient != localAddress && recipient != destination  || recipient != destination2) { 
    Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }
  */
  
if (sender==destination){
  // extract the values
JsonArray array = doc.as<JsonArray>();
Voltage = doc[0];
Voltage2 = doc[1];
Presion = doc[2];
LedRelay_7 = doc[3];
LedRelay_3 = doc[4];
LedRelay_4 = doc[5];
StateLevel = doc[6];
RPM = doc[7];
RPM_Slider = doc[8];
//TotalLitres = doc[6];
//FlowRate = doc[7];

      Serial.print("Voltage: ");
      Serial.print(Voltage);
      Serial.println();
      Serial.print("Voltage2: ");
      Serial.print(Voltage2);
      Serial.println();
      Serial.print("Presion: ");
      Serial.print(Presion); 
      Serial.println();
      Serial.print("LedRelay_7: ");
      Serial.print(LedRelay_7);
      Serial.println();
      Serial.print("Led Valve: ");
      Serial.print(LedRelay_3); 
      Serial.println();
      Serial.print("StateRelay_4: ");
      Serial.print(LedRelay_4);
      Serial.println();
      Serial.print("StateLevel: ");
      Serial.print(StateLevel); 
      Serial.println();
      Serial.print("RPM: ");
      Serial.print(RPM);
      Serial.println();
  }
}
///////////
///////////

////////End Serialization Function////////////
BLYNK_WRITE(V1){
  StateRelay_1 = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V1 value is: ");
  Serial.println(StateRelay_1);
  //Blynk.virtualWrite(V1, LOW);
StaticJsonDocument<200> doc;
  doc["V1"] = StateRelay_1;
  char outputV1[200];
  serializeJson(doc, outputV1, sizeof(outputV1));
  LoRa.beginPacket();
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(sizeof(outputV1));        // add payload length
  //LoRa.write(output.length());        // add payload length
  LoRa.print(outputV1);
  LoRa.endPacket();
  Serial.print("Sending packet: ");
  Serial.println(outputV1);
  msgCount++;
}
BLYNK_WRITE(V2){
  StateRelay_2 = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V2 value is: ");
  Serial.println(StateRelay_2);
  Blynk.virtualWrite(V1, LOW);
  Blynk.virtualWrite(V2, LOW);
 StaticJsonDocument<200> doc;
  doc["V2"] = StateRelay_2;
  char outputV2[200];
  serializeJson(doc, outputV2, sizeof(outputV2));
  LoRa.beginPacket();
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(sizeof(outputV2));        // add payload length
  //LoRa.write(output.length());        // add payload length
  LoRa.print(outputV2);
  LoRa.endPacket();
  Serial.print("Sending packet: ");
  Serial.println(outputV2);
  msgCount++;
}
BLYNK_WRITE(V3){
  StateRelay_3 = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V3 value is: ");
  Serial.println(StateRelay_3);
  StaticJsonDocument<200> doc;
  doc["V3"] = StateRelay_3;
  char outputV3[200];
  serializeJson(doc, outputV3, sizeof(outputV3));
  LoRa.beginPacket();
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(sizeof(outputV3));        // add payload length
  //LoRa.write(output.length());        // add payload length
  LoRa.print(outputV3);
  LoRa.endPacket();
  Serial.print("Sending packet: ");
  Serial.println(outputV3);
  msgCount++;
}
BLYNK_WRITE(V4){
  StateRelay_4 = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V4 value is: ");
  Serial.println(StateRelay_4);
  //Blynk.virtualWrite(V4, LOW);
  StaticJsonDocument<200> doc;
  doc["V4"] = StateRelay_4;
  char outputV4[200];
  serializeJson(doc, outputV4, sizeof(outputV4));
  LoRa.beginPacket();
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(sizeof(outputV4));        // add payload length
  //LoRa.write(output.length());        // add payload length
  LoRa.print(outputV4);
  LoRa.endPacket();
  Serial.print("Sending packet: ");
  Serial.println(outputV4);
  msgCount++;
}
BLYNK_WRITE(V5){    //Throttle
  StateRelay_5 = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V5 value is: ");
  Serial.println(StateRelay_5);
  //Blynk.virtualWrite(V5, LOW);
  StaticJsonDocument<200> doc;
  doc["V5"] = StateRelay_5;
  char outputV5[200];
  serializeJson(doc, outputV5, sizeof(outputV5));
  LoRa.beginPacket();
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(sizeof(outputV5));        // add payload length
  //LoRa.write(output.length());        // add payload length
  LoRa.print(outputV5);
  LoRa.endPacket();
  Serial.print("Sending packet: ");
  Serial.println(outputV5);
  msgCount++;
}
BLYNK_WRITE(V6){  //Brake
  StateRelay_6 = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V6 value is: ");
  Serial.println(StateRelay_6);
  //Blynk.virtualWrite(V6, LOW);
  StaticJsonDocument<200> doc;
  doc["V6"] = StateRelay_6;
  char outputV6[200];
  serializeJson(doc, outputV6, sizeof(outputV6));
  LoRa.beginPacket();
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(sizeof(outputV6));        // add payload length
  //LoRa.write(output.length());        // add payload length
  LoRa.print(outputV6);
  LoRa.endPacket();
  Serial.print("Sending packet: ");
  Serial.println(outputV6);
  msgCount++;}
BLYNK_WRITE(V7){  //Electrovalve
  StateRelay_7 = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V7 value is: ");
  Serial.println(StateRelay_7);
  //Blynk.virtualWrite(V7, LOW);
  StaticJsonDocument<200> doc;
  doc["V7"] = StateRelay_7;
  char outputV7[200];
  serializeJson(doc, outputV7, sizeof(outputV7));
  LoRa.beginPacket();
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(sizeof(outputV7));        // add payload length
  //LoRa.write(output.length());        // add payload length
  LoRa.print(outputV7);
  LoRa.endPacket();
  Serial.print("Sending packet: ");
  Serial.println(outputV7);
  msgCount++;}
BLYNK_WRITE(V8){
  StateRelay_8 = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V8 value is: ");
  Serial.println(StateRelay_8);
  StaticJsonDocument<200> doc;
  doc["V8"] = StateRelay_8;
  char outputV8[200];
  serializeJson(doc, outputV8, sizeof(outputV8));
  LoRa.beginPacket();
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(sizeof(outputV8));        // add payload length
  //LoRa.write(output.length());        // add payload length
  LoRa.print(outputV8);
  LoRa.endPacket();
  Serial.print("Sending packet: ");
  Serial.println(outputV8);
  msgCount++;
  }
BLYNK_WRITE(V9){
  StateTimerON = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V9 value is: ");
  Serial.println(StateTimerON);
  if(StateTimerON==1){
StaticJsonDocument<200> doc;
  doc["V1"] = StateTimerON;
  char outputV9[200];
  serializeJson(doc, outputV9, sizeof(outputV9));
  LoRa.beginPacket();
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(sizeof(outputV9));        // add payload length
  //LoRa.write(output.length());        // add payload length
  LoRa.print(outputV9);
  LoRa.endPacket();
  Serial.print("Sending packet: ");
  Serial.println(outputV9);
  msgCount++;
}else{
  StaticJsonDocument<200> doc;
  doc["V2"] = !StateTimerON;
  char outputV9[200];
  serializeJson(doc, outputV9, sizeof(outputV9));
  LoRa.beginPacket();
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(sizeof(outputV9));        // add payload length
  //LoRa.write(output.length());        // add payload length
  LoRa.print(outputV9);
  LoRa.endPacket();
  Serial.print("Sending packet: ");
  Serial.println(outputV9);
  msgCount++;
}
}
BLYNK_WRITE(V10){
  StateTimerValve = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V10 value is: ");
  Serial.println(StateTimerValve);
  if(StateTimerValve==1){
  StaticJsonDocument<200> doc;
  doc["V8"] = StateTimerValve;
  char outputV10[200];
  serializeJson(doc, outputV10, sizeof(outputV10));
  LoRa.beginPacket();
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(sizeof(outputV10));        // add payload length
  //LoRa.write(output.length());        // add payload length
  LoRa.print(outputV10);
  LoRa.endPacket();
  Serial.print("Sending packet: ");
  Serial.println(outputV10);
  msgCount++;
  }else{
  StaticJsonDocument<200> doc;
  doc["V8"] = StateTimerValve;
  char outputV10[200];
  serializeJson(doc, outputV10, sizeof(outputV10));
  LoRa.beginPacket();
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(sizeof(outputV10));        // add payload length
  //LoRa.write(output.length());        // add payload length
  LoRa.print(outputV10);
  LoRa.endPacket();
  Serial.print("Sending packet: ");
  Serial.println(outputV10);
  msgCount++;
}
}
BLYNK_WRITE(V11){
  VPIN_RESET = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V11 value is: ");
  Serial.println(VPIN_RESET);
  StaticJsonDocument<200> doc;
  doc["V11"] = VPIN_RESET;
  char outputV11[200];
  serializeJson(doc, outputV11, sizeof(outputV11));
  LoRa.beginPacket();
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(sizeof(outputV11));        // add payload length
  //LoRa.write(output.length());        // add payload length
  LoRa.print(outputV11);
  LoRa.endPacket();
  Serial.print("Sending packet: ");
  Serial.println(outputV11);
  msgCount++;
}
BLYNK_WRITE(V12){
  StateRelay_1 = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V1 value is: ");
  Serial.println(StateRelay_1);
  Blynk.virtualWrite(V1, LOW);
StaticJsonDocument<200> doc;
  doc["V1"] = StateRelay_1;
  char outputV1[200];
  serializeJson(doc, outputV1, sizeof(outputV1));
  LoRa.beginPacket();
  LoRa.write(destination2);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(sizeof(outputV1));        // add payload length
  //LoRa.write(output.length());        // add payload length
  LoRa.print(outputV1);
  LoRa.endPacket();
  Serial.print("Sending packet: ");
  Serial.println(outputV1);
  msgCount++;
}
/*
BLYNK_WRITE(V13){
  StateRelay_2 = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V2 value is: ");
  Serial.println(StateRelay_2);
  Blynk.virtualWrite(V2, LOW);
  StaticJsonDocument<200> doc;
  doc["V2"] = StateRelay_2;
  char outputV2[200];
  serializeJson(doc, outputV2, sizeof(outputV2));
  LoRa.beginPacket();
  LoRa.write(destination2);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(sizeof(outputV2));        // add payload length
  //LoRa.write(output.length());        // add payload length
  LoRa.print(outputV2);
  LoRa.endPacket();
  Serial.print("Sending packet: ");
  Serial.println(outputV2);
  msgCount++;
}
*/
BLYNK_WRITE(V24){
  RPM_Slider = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V24 value is: ");
  Serial.println(RPM_Slider);
  StaticJsonDocument<200> doc;
  doc["V24"] = RPM_Slider;
  char outputV24[200];
  serializeJson(doc, outputV24, sizeof(outputV24));
  LoRa.beginPacket();
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(sizeof(outputV24));        // add payload length
  //LoRa.write(output.length());        // add payload length
  LoRa.print(outputV24);
  LoRa.endPacket();
  Serial.print("Sending packet: ");
  Serial.println(outputV24);
  msgCount++;
}
void sendServer() {
  ledON.setValue(LedRelay_4);
  ledOFF.setValue(LedRelay_3);
  ledValve.setValue(LedRelay_7);
  ledFuel.setValue(StateLevel);
  Blynk.virtualWrite(V13, Voltage);
  Blynk.virtualWrite(V14, Voltage2);
  Blynk.virtualWrite(V15, Presion);
  Blynk.virtualWrite(V21, TotalLitres);
  Blynk.virtualWrite(V22, FlowRate);
  Blynk.virtualWrite(V23, RPM);
  Blynk.virtualWrite(V24, RPM_Slider);
 // Blynk.virtualWrite(V30, Voltage2);
 // Blynk.virtualWrite(V31, Presion2);
  /*
  terminal.println("connected...yeey :)");
  terminal.print(" SSDI:  ");
  terminal.println(WiFi.SSID());
  terminal.print(" Pass:  ");
  terminal.println(WiFi.psk());
  terminal.print("IP address: ");
  terminal.println(WiFi.localIP());
  times++; 
  Serial.print("Times: ");
  Serial.println(times);
*/
  }
