//#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
// Default heartbeat interval for GSM is 60
// If you want override this value, uncomment and set this option:
//#define BLYNK_HEARTBEAT 30
// Select your modem:
#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
#include <BlynkSimpleSIM800.h>

// Your GPRS credentials
// Leave empty, if missing user or pass
//const char apn[]  = "lowi.private.omv.es";
//const char apn[]  = "orangeworld";
//const char user[] = "";
//const char pass[] = "";
const char apn[]  = "tuenti.com";
const char user[] = "tuenti";
const char pass[] = "tuenti";
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
const char auth[] = "dwdeawxiDx9MHFYI2h3aq3sKgVvVE6uA";
TinyGsm modem(Serial);
BlynkTimer timer;
