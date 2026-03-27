/*
 * AgrIoT Sensor Test — Arduino Nano Node
 * Reads all sensors and prints raw + calibrated values every second.
 * Use this to verify sensor wiring and calibration before full firmware.
 *
 * Does NOT activate any relays.
 */

#define VOLTAGE_PIN  A7   // Battery voltage divider
#define VOLTAGE2_PIN A6   // Alternator voltage divider
#define PRESSURE_PIN A5   // Oil pressure transducer
#define FUEL_PIN      4   // Fuel level (digital, INPUT_PULLUP)
#define RPM_PIN       3   // Hall effect RPM sensor (INT1)

// Voltage divider constants  (R1=22kΩ, R2=4.7kΩ)
const float R1 = 22000.0;
const float R2 =  4700.0;

// Pressure calibration constant
const float P_CAL = 0.0028;

volatile unsigned long pulseCount = 0;
unsigned long lastRpmMillis = 0;
float totalRPM = 0;

void ISRPulse() { pulseCount++; }

void setup() {
  Serial.begin(115200);
  pinMode(FUEL_PIN, INPUT_PULLUP);
  pinMode(RPM_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(RPM_PIN), ISRPulse, RISING);
  Serial.println("AgrIoT Sensor Test — reading every 1s");
  Serial.println("Pin | Sensor           | Raw ADC | Calibrated");
  Serial.println("----+------------------+---------+-----------");
}

void loop() {
  // ── Voltage averaging (100 samples) ──────────────────────────
  float sumV1 = 0, sumV2 = 0, sumP = 0;
  for (int i = 0; i < 100; i++) {
    sumV1 += analogRead(VOLTAGE_PIN)  * 5.0 * (R1 + R2) / (R2 * 1023.0);
    sumV2 += analogRead(VOLTAGE2_PIN) * 5.0 * (R1 + R2) / (R2 * 1023.0);
    sumP  += analogRead(PRESSURE_PIN) * 5000.0 * P_CAL / 1023.0;
  }
  float voltage1  = sumV1 / 100.0;
  float voltage2  = sumV2 / 100.0;
  float pressure  = sumP  / 100.0;

  // ── Fuel level ───────────────────────────────────────────────
  bool fuelLow = (digitalRead(FUEL_PIN) == LOW);

  // ── RPM (1-second window) ────────────────────────────────────
  unsigned long now = millis();
  if (now - lastRpmMillis >= 1000) {
    detachInterrupt(digitalPinToInterrupt(RPM_PIN));
    unsigned long interval = now - lastRpmMillis;
    totalRPM = 60.0 * ((float)pulseCount / (interval / 1000.0));
    pulseCount = 0;
    lastRpmMillis = now;
    attachInterrupt(digitalPinToInterrupt(RPM_PIN), ISRPulse, RISING);
  }

  // ── Print ─────────────────────────────────────────────────────
  Serial.printf("A7  | Battery Voltage  | %4d    | %.2f V\n",
                analogRead(VOLTAGE_PIN), voltage1);
  Serial.printf("A6  | Alternator Volts | %4d    | %.2f V\n",
                analogRead(VOLTAGE2_PIN), voltage2);
  Serial.printf("A5  | Oil Pressure     | %4d    | %.2f bar\n",
                analogRead(PRESSURE_PIN), pressure);
  Serial.printf("D4  | Fuel Level       | %s       | %s\n",
                fuelLow ? "LOW " : "HIGH",
                fuelLow ? "LOW FUEL WARNING" : "OK");
  Serial.printf("D3  | Engine RPM       | pulses/s| %.0f RPM\n", totalRPM);
  Serial.println("---------------------------------------------------");

  delay(1000);
}
