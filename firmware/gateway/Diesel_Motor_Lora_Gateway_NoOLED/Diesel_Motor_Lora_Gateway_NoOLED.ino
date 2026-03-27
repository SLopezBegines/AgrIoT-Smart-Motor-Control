// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLfqlfDody"
#define BLYNK_DEVICE_NAME "MotorLoraGateway"
#define BLYNK_AUTH_TOKEN "YE3JSMYSyZiVRS_iVfMLEqJzJWK1pnom"
//#define BLYNK_AUTH_TOKEN "tEoqBekxOVQrfscHwa7MmkOC86-A5OKC"
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <AsyncElegantOTA.h>
#include <ArduinoJson.h>
#include <SPI.h>              // include libraries
#include <LoRa.h>

#include "BlynkFunctions.h"
#include "OTA.h"
#include "Lora.h"
#include "WiFiManager.h"
#include "mDNS.h"

// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "SLB-LAD";
//char pass[] = "Wichita2019";
//char ssid[] = "TP-LINK_B702";
//char pass[] = "34309049";

BlynkTimer timer;


void setup() {
  // put your setup code here, to run once:
  // Debug console
  Serial.begin(115200);
  Serial.println("Booting");
  //  Wire.begin();
  
//////Starting LoRa////  
  Lora();
//////End Lora Code//////  
//////WiFiManager Code////////
  WifiManager();
//////END WIFI MANAGER CODE///////////////
//////OTA WIFI CODE/////////////////////
  InitOTA();
//////END OTA WIFI CODE/////////////////////
//////mDNS/////
initmDNS();
//////END mDNS/////
  
Blynk.config(BLYNK_AUTH_TOKEN);
timer.setInterval(1000L, sendServer);
}

void loop() {
  ArduinoOTA.handle();
  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
  Blynk.run();
  timer.run();
loopmDNS();
}
