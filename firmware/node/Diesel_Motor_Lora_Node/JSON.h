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
  VPIN_RESET = (doc["V11"].as<int>());
      
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
      Serial.print("  VPIN_RESET: ");
      Serial.print(VPIN_RESET);
      Serial.println();
    onReceive(LoRa.parsePacket());
  }
 }
/*
/////////Deserialization array///////
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
  
    if (incomingLength != LoRaData.length()) {   // check length for error
    Serial.println("error: message length does not match length");
    return;                             // skip rest of function
  }
  
  
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
  
  // compute the required size
const size_t CAPACITY = JSON_ARRAY_SIZE(9);
// allocate the memory for the document
StaticJsonDocument<CAPACITY> doc;
// parse a JSON array
DeserializationError error = deserializeJson(doc, LoRaData);
if (error) {
  Serial.print(F("deserializeJson() failed: "));
  Serial.println(error.f_str());
  return;
}
//deserializeJson(doc, LoRaData);

// extract the values
JsonArray array = doc.as<JsonArray>();
StateRelay_1 = doc[0];
StateRelay_2 = doc[1];
StateRelay_3 = doc[2];
StateRelay_4 = doc[3];
StateRelay_5 = doc[4];
StateRelay_6 = doc[5];
StateRelay_7 = doc[6];
StateRelay_8 = doc[7];
//StateTimerON = doc[8];
//StateTimerValve = doc[9];
VPIN_RESET = doc[10];

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
      //Serial.print("  StateTimerON: ");
      //Serial.print(StateTimerON);
      //Serial.println();
      //Serial.print("  StateTimerValve: ");
      //Serial.print(StateTimerValve);
      //Serial.println();
      Serial.print("  VPIN_RESET: ");
      Serial.print(VPIN_RESET);
      Serial.println();
      onReceive(LoRa.parsePacket());
 }
 */
//////Serialization///////
 //modified from https://www.hackster.io/vinayyn/lora-blynk-app-interfacing-1e1851
//Modified from LoRaDuplexCallback example
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
array.add(totalLitres);
array.add(flowRate);
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
