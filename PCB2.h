
//RJ45 RJ2 Pin Variables
int Relay5=15;
int Relay6=16;
int Relay7=17;
int Relay8=18;
int VoltageSensor2=A5;
int Pressure2=A6;
int Caudal2=2;
int FuelLevel2=12;

//Variables PCB2
int StateStart2;
int StateOff2;
float sumVoltage2 = 0;
float avgVoltage2 = 0;
float sumPresion2 = 0;
float avgPresion2 = 0;
int timeoff2 = 60000; //minutes for activation of Relay4

//AnalogReadings

void VoltageFunction2() {
  sumVoltage2 = 0;
  avgVoltage2 = 0;
  for (i = 0; i <= 100; i++) {
    int   rawVsensor2 = analogRead(VoltageSensor2);
    float voltage2 = rawVsensor2 * 5.0 * (22.0 + 4.7) / (1023.0 * 4.7); //Vin=((AnalogRead(A0)*5*(R1+R2))/(R2*1023). R1=22K. R2=4.7K. From Voltage Divider calculator
    sumVoltage2 += voltage2;
  }
  avgVoltage2 = sumVoltage2 / 100;                                     //Find average
}
void PressionFunction2() {
  sumPresion2 = 0;
  avgPresion2 = 0;
  for (i = 0; i <= 100; i++) {
    int   rawPresion2 = analogRead(Pressure2);
    float Presion2 = ((rawPresion2 * 5000.0 * 0.0034) - 1.7237)/1023.0; //Presion=((AnalogRead(A7)*5000*0.0034)-1.7237))/1023
    sumPresion2 += Presion2;
  }
  avgPresion2 = sumPresion2/100;                                     //Find average
}

//Activating Relays PCB2
/*
BLYNK_WRITE(V9) {
  timeoff2 = param.asInt() * 60000; // assigning incoming value from pin V0 to timeoff variable. Input in minutes.
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  //Serial.print("V0 input value is: ");
  //Serial.println(timeoff);
}
*/
void Relay8Off() {                               //Relay 8 OFF
  digitalWrite(Relay8, HIGH);
  Serial.println("Relay8 disabled");
  Blynk.virtualWrite(V12, LOW);
}
void turnRelay7OFF() {                           //Relay 7 OFF
  digitalWrite(Relay7, HIGH);
  Serial.println("Relay3 disabled");
  Blynk.virtualWrite(V11, LOW);
}
void turnRelay7ON() {                             //Relay 7 ON
  digitalWrite(Relay7, LOW);
  //led5.on();
  Serial.println("Relay3 enabled");
  timer.setTimeout(3000, turnRelay7OFF);          //Time to off Relay 7 3000ms
}
void turnRelay6Off() {                            //Relay 6 OFF
  digitalWrite(Relay6, HIGH);
 // led5.on();
  Serial.println("Relay 6 disabled");
  timer.setTimeout(5000, turnRelay7ON);           //Time to OFF Relay 7
}
void turnRelay5Off() {                           //Relay 5 OFF
  digitalWrite(Relay5, HIGH);
  Blynk.virtualWrite(V10, LOW);
  Serial.println("Relay 5 disabled");
}
BLYNK_WRITE(V12) {                                 //Relay8 ON
  if (param.asInt() == 1) {
    Serial.println("Relay 8 enabled");
    digitalWrite(Relay8, LOW);
    Blynk.virtualWrite(V12, HIGH);
    //timer.setTimeout(timeoff2, Relay8Off);         //Time to off Relay8 Input in minutes.
  } else {
    Blynk.virtualWrite(V12, LOW);
    digitalWrite(Relay8, HIGH);
  }
}
BLYNK_WRITE(V11) {                                 //Relay6 ON
  if (param.asInt() == 1) {
    Serial.println("Relay 6 enabled");
    digitalWrite(Relay6, LOW);
   // led5.on();
    // timer.setTimeout(timeoff, turnMotorOff);
    timer.setTimeout(600, turnRelay6Off);         //Time to off Relay5 600ms
  } else {
    digitalWrite(Relay6, HIGH);
  }
}
BLYNK_WRITE(V10) {                                 //Relay5 ON
  if (param.asInt() == 1) {
    Serial.println("Relay 5 enabled");
    digitalWrite(Relay5, LOW);
    timer.setTimeout(5000, turnRelay5Off);        //Time to off Relay5 5000ms
  } else {
    digitalWrite(Relay5, HIGH);
  }
}
BLYNK_WRITE(V19) {                                 //Timer ON&OFF. ON Relay5 & OFF Relay6
  if (param.asInt() == 1) {
    Serial.println("Relay 5 enabled");
    digitalWrite(Relay5, LOW);
    Blynk.virtualWrite(V10, HIGH);
    timer.setTimeout(5000, turnRelay5Off);        //Time to off Relay5 5000ms
    //LOW for V11
  }
  if (param.asInt() == 0) {
    Serial.println("Relay 6 enabled");
    digitalWrite(Relay6, LOW);
    Blynk.virtualWrite(V10, LOW);
    // timer.setTimeout(timeoff, turnMotorOff);
    timer.setTimeout(600, turnRelay6Off);         //Time to off Relay5 600ms
  }
}
BLYNK_WRITE(V21) {
  if (param.asInt() == 1) {
    Serial.println("Relay 8 enabled");
    Blynk.virtualWrite(V12, HIGH);
    digitalWrite(Relay8, LOW);
  }
  if (param.asInt() == 0) {
    Serial.println("Relay 8 disabled");
    Blynk.virtualWrite(V12, LOW);
    digitalWrite(Relay8, HIGH);
  }
}

WidgetLED led4(V13);//Led Start 2
WidgetLED led5(V14);//Led OffMotor 2
WidgetLED led6(V15);//Led Level 2
WidgetLED led8(V23);//Led Relay8


void sendSensor3() {
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  led4.setValue(!digitalRead(Relay5) * 255);
  led5.setValue(!digitalRead(Relay7) * 2552);
  led6.setValue(!digitalRead(FuelLevel2) * 255);
  led8.setValue(!digitalRead(Relay8) * 255);
}
void sendSensor4() {
  VoltageFunction2();
  PressionFunction2();
  //Caudal();
  Blynk.virtualWrite(V1, avgPresion1);
  Blynk.virtualWrite(V2, avgVoltage1);
  // Blynk.virtualWrite(V24, volume);//Litros
 // Blynk.virtualWrite(V25, flow_Lmin);//L/min
  
}

void setup_PCB2() {

  pinMode(Pressure2, INPUT);
  pinMode(VoltageSensor2, INPUT);
  pinMode(FuelLevel2, INPUT_PULLUP);
  pinMode(Relay5, OUTPUT);
  pinMode(Relay6, OUTPUT);
  pinMode(Relay7, OUTPUT);
  pinMode(Relay8, OUTPUT);
  timer.setInterval(1000L, sendSensor3);
  timer.setInterval(3000L, sendSensor4);
}
