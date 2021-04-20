/* CAUDAL FUNCTION
//Caudal. From https://www.luisllamas.es/caudal-consumo-de-agua-con-arduino-y-caudalimetro/
const int measureInterval = 2500;
volatile int pulseConter;
// YF-S201
const float factorK = 7.5;
// FS300A
//const float factorK = 5.5;
// FS400A
//const float factorK = 3.5;
float volume = 0;
long t0 = 0;
float flow_Lmin;

void ISRCountPulse() {
  pulseConter++;
}
float GetFrequency() {
  pulseConter = 0;
  interrupts();
  delay(measureInterval);
  noInterrupts();
  return (float)pulseConter * 1000 / measureInterval;
}
void SumVolume(float dV) {
  volume += dV / 60 * (millis() - t0) / 1000.0;
  t0 = millis();
}
void Caudal() {
  // obtener frecuencia en Hz
  float frequency = GetFrequency();
  // calcular caudal L/min
  flow_Lmin = frequency / factorK;
  SumVolume(flow_Lmin);
 
  Serial.print("Frecuencia: ");
  Serial.print(frequency, 0);
  Serial.print(" (Hz)\tCaudal: ");
  Serial.print(flow_Lmin, 3);
  Serial.println(" (L/min)");
  Serial.print(volume, 1);
  Serial.println(" (L)");

}
*/

//AnalogReadings
void VoltageFunction1() {
  sumVoltage1 = 0;
  avgVoltage1 = 0;
  for (i = 0; i <= 100; i++) {
    int   rawVsensor1 = analogRead(VoltageSensor);
    float voltage1 = rawVsensor1 * 5.0 * (22.0 + 4.7) / (1023.0 * 4.7); //Vin=((AnalogRead(A0)*5*(R1+R2))/(R2*1023). R1=22K. R2=4.7K. From Voltage Divider calculator
    sumVoltage1 += voltage1;
  }
  avgVoltage1 = sumVoltage1 / 100;                                     //Find average
}

void PressionFunction1() {
  sumPresion1 = 0;
  avgPresion1 = 0;
  for (i = 0; i <= 100; i++) {
    int   rawPresion1 = analogRead(Pressure);
    float Presion1 = ((rawPresion1 * 5000.0 * 0.0034) - 1.7237)/1023.0; //Presion=((AnalogRead(A7)*5000*0.0034)-1.7237))/1023
    sumPresion1 += Presion1;
  }
  avgPresion1 = sumPresion1/100;                                     //Find average
}
/*///Sync Virtual Pins values from stored values in server
  BLYNK_CONNECTED() {
  //get data stored in virtual pin V0 from server
  Blynk.syncVirtual(V0,V1,V2,V3,V4,V5,V6,V7,V8,V9,V10,V11,V12,V13,V14,V15,V16,V17,V18,V19,V20,V21);
  }
*/

//Activating Relays PCB1
/*
BLYNK_WRITE(V0) { // assigning incoming value from pin V0 to timeoff variable. Input in minutes.
  timeoff = param.asInt() * 60000;//Input in minutes. Conversion to ms
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  //Serial.print("V0 input value is: ");
  //Serial.println(timeoff);
}
*/
void Relay4Off() {                               //Relay 4 OFF
  digitalWrite(Relay4, HIGH);
  Serial.println("Relay4 disabled");
  Blynk.virtualWrite(V8, LOW);
}
void turnRelay3OFF() {                           //Relay 3 OFF
  digitalWrite(Relay3, HIGH);
  Serial.println("Relay3 disabled");
  Blynk.virtualWrite(V7, LOW);
}
void turnRelay3ON() {                             //Relay 3 ON
  digitalWrite(Relay3, LOW);
  //led2.on();
  Serial.println("Relay3 enabled");
  timer.setTimeout(3000, turnRelay3OFF);          //Time to off Relay3 3000ms
}
void turnRelay2Off() {                            //Relay 2 OFF
  digitalWrite(Relay2, HIGH);
  //led2.on();
  Serial.println("Relay2 disabled");
  timer.setTimeout(5000, turnRelay3ON);///Time to OFF Relay 3
}
void turnRelay1Off() {                           //Relay 1 OFF
  digitalWrite(Relay1, HIGH);
  Blynk.virtualWrite(V6, LOW);
  Serial.println("Relay disabled");
}
BLYNK_WRITE(V8) {                                 //Relay4 ON
  if (param.asInt() == 1) {
    Serial.println("Relay 4 enabled");
    digitalWrite(Relay4, LOW);
    Blynk.virtualWrite(V8, HIGH);
    // timer.setTimeout(timeoff, Relay4Off);         //Time to off Relay4 Input in minutes.
  } else {
    digitalWrite(Relay4, HIGH);
     Blynk.virtualWrite(V8, LOW);
  }
}
BLYNK_WRITE(V7) {                                 //Relay2 ON
  if (param.asInt() == 1) {
    Serial.println("Relay2 enabled");
    digitalWrite(Relay2, LOW);
    //led2.on();
    // timer.setTimeout(timeoff, turnMotorOff);
    timer.setTimeout(600, turnRelay2Off);         //Time to off Relay2 600ms
  } else {
    digitalWrite(Relay2, HIGH);
  }
}
BLYNK_WRITE(V6) {                                 //Relay1 ON
  if (param.asInt() == 1) {
    Serial.println("Relay 1 enabled");
    digitalWrite(Relay1, LOW);
    timer.setTimeout(5000, turnRelay1Off);        //Time to off Relay2 5000ms
  } else {
    digitalWrite(Relay1, HIGH);
  }
}
BLYNK_WRITE(V18) {                                 //Timer ON&OFF. ON Relay1 & OFF Relay3
  if (param.asInt() == 1) {
    Serial.println("Relay 1 enabled");
    digitalWrite(Relay1, LOW);
    Blynk.virtualWrite(V6, HIGH);
    timer.setTimeout(5000, turnRelay1Off);        //Time to off Relay2 5000ms
  }
  if (param.asInt() == 0) {
    Serial.println("Relay2 enabled");
    digitalWrite(Relay2, LOW);
    Blynk.virtualWrite(V7, LOW);
    // timer.setTimeout(timeoff, turnMotorOff);
    timer.setTimeout(600, turnRelay2Off);         //Time to off Relay2 600ms
  }
}

BLYNK_WRITE(V20) {
  if (param.asInt() == 1) {
    Serial.println("Relay 4 enabled");
    Blynk.virtualWrite(V8, HIGH);
    digitalWrite(Relay4, LOW);
  }
  if (param.asInt() == 0) {
    Serial.println("Relay 4 enabled");
    Blynk.virtualWrite(V8, LOW);
    digitalWrite(Relay4, HIGH);
  }
}

WidgetLED led1(V3);//Led Start
WidgetLED led2(V4);//Led OffMotor
WidgetLED led3(V5);//Led Level
WidgetLED led7(V22);//Led Relay4
void sendSensor1() {
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  led1.setValue(!digitalRead(Relay1) * 255);
  led2.setValue(!digitalRead(Relay3) * 255);
  led3.setValue(!digitalRead(FuelLevel) * 255);
  led7.setValue(!digitalRead(Relay4) * 255);
}
void sendSensor2() {
  VoltageFunction1();
  PressionFunction1();
  //Caudal();
  Blynk.virtualWrite(V1, avgPresion1);
  Blynk.virtualWrite(V2, avgVoltage1);
 // Blynk.virtualWrite(V24, volume);//Litros
 // Blynk.virtualWrite(V25, flow_Lmin);//L/min
  
}
