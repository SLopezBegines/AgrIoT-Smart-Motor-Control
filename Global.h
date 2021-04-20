//pin variables PCB1
int VoltageSensor=A0;
int Pressure=A7;
int Relay4=5;   //Relay4
int Relay3=6;  //Relay3
int Relay2=7;   //Relay2
int Relay1=8;   //Relay1
int CaudalPin=3;
int FuelLevel=4;

//Varialbes PCB1
uint8_t i = 0;
int StateStart1;
int StateOff1;
float sumVoltage1 = 0;
float avgVoltage1 = 0;
float sumPresion1 = 0;
float avgPresion1 = 0;
int timeoff = 60000; //minutes for activation of Relay4
