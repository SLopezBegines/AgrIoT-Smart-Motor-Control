/*
 * AgrIoT JSON Parsing Test
 * Verifies that the telemetry serialisation and command deserialisation
 * produce correct results.
 *
 * Upload to any Arduino board (or run as desktop C++ with ArduinoJson).
 * Expected output: all tests PASS.
 */

#include <ArduinoJson.h>

int passed = 0;
int failed = 0;

void assert_float(const char* name, float actual, float expected, float tol = 0.01) {
  if (abs(actual - expected) <= tol) {
    Serial.print("  PASS  "); Serial.println(name);
    passed++;
  } else {
    Serial.print("  FAIL  "); Serial.print(name);
    Serial.print(" — expected "); Serial.print(expected);
    Serial.print(", got "); Serial.println(actual);
    failed++;
  }
}

void assert_int(const char* name, int actual, int expected) {
  if (actual == expected) {
    Serial.print("  PASS  "); Serial.println(name);
    passed++;
  } else {
    Serial.print("  FAIL  "); Serial.print(name);
    Serial.print(" — expected "); Serial.print(expected);
    Serial.print(", got "); Serial.println(actual);
    failed++;
  }
}

// ── Test 1: Telemetry serialisation (Node → Gateway) ─────────────
void test_telemetry_serialise() {
  Serial.println("\n[Test 1] Telemetry serialisation");

  float voltage1 = 12.8, voltage2 = 14.1, pressure = 3.2;
  int   elecValve = 0, motorOff = 1, motorOn = 0, fuel = 1;
  float rpm = 1850; int rpmSlider = 5;

  StaticJsonDocument<JSON_ARRAY_SIZE(9)> doc;
  JsonArray arr = doc.to<JsonArray>();
  arr.add(voltage1); arr.add(voltage2); arr.add(pressure);
  arr.add(elecValve); arr.add(motorOff); arr.add(motorOn);
  arr.add(fuel); arr.add(rpm); arr.add(rpmSlider);

  char buf[200];
  serializeJson(doc, buf, sizeof(buf));

  // Deserialise and check round-trip
  StaticJsonDocument<JSON_ARRAY_SIZE(9)> doc2;
  DeserializationError err = deserializeJson(doc2, buf);
  assert_int("no parse error", err == DeserializationError::Ok ? 1 : 0, 1);
  assert_float("voltage1", doc2[0].as<float>(), 12.8f);
  assert_float("voltage2", doc2[1].as<float>(), 14.1f);
  assert_float("pressure", doc2[2].as<float>(), 3.2f);
  assert_int("electrovalve", doc2[3].as<int>(), 0);
  assert_int("motorOff", doc2[4].as<int>(), 1);
  assert_int("fuel", doc2[6].as<int>(), 1);
  assert_float("rpm", doc2[7].as<float>(), 1850.0f, 1.0);
  assert_int("rpmSlider", doc2[8].as<int>(), 5);
}

// ── Test 2: Command deserialisation (Gateway → Node) ─────────────
void test_command_deserialise() {
  Serial.println("\n[Test 2] Command deserialisation");

  const char* cmds[] = {
    "{\"V1\":1}", "{\"V2\":1}", "{\"V5\":1}", "{\"V24\":7}", "{\"V7\":0}"
  };
  int expected_keys[]  = {1, 2, 5, 24, 7};
  int expected_vals[]  = {1, 1, 1,  7, 0};

  for (int i = 0; i < 5; i++) {
    StaticJsonDocument<64> doc;
    DeserializationError err = deserializeJson(doc, cmds[i]);
    assert_int("no parse error", err == DeserializationError::Ok ? 1 : 0, 1);

    String key = "V" + String(expected_keys[i]);
    assert_int(key.c_str(), doc[key].as<int>(), expected_vals[i]);
  }
}

// ── Test 3: Voltage calibration formula ──────────────────────────
void test_voltage_formula() {
  Serial.println("\n[Test 3] Voltage divider formula");

  // At ADC = 435: expected Vin ≈ 11.8 V (from Coste.xlsx sensor sheet)
  const float R1 = 22000.0, R2 = 4700.0;
  int raw = 435;
  float vin = raw * 5.0 * (R1 + R2) / (R2 * 1023.0);
  assert_float("ADC 435 → ~11.8V", vin, 11.8f, 0.3f);

  // At ADC = 0: must be 0 V
  assert_float("ADC 0 → 0V", 0 * 5.0 * (R1 + R2) / (R2 * 1023.0), 0.0f);
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("=== AgrIoT JSON Parsing Tests ===");

  test_telemetry_serialise();
  test_command_deserialise();
  test_voltage_formula();

  Serial.println("\n================================");
  Serial.print("Results: ");
  Serial.print(passed); Serial.print(" passed, ");
  Serial.print(failed); Serial.println(" failed");
  if (failed == 0) Serial.println("ALL TESTS PASSED");
}

void loop() {}
