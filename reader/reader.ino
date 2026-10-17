// ---- pins ----
const int PIN_SOIL  = 34;   // analog in (ADC1) — pot wiper
const int PIN_RELAY = 26;   // pump relay IN
const int PIN_GRID  = 27;   // grid-power switch (INPUT_PULLUP: LOW = power on)
const int PIN_LED   = 2;    // mirrors pump state

// ---- calibration (change these in Step B below) ----
int RAW_DRY = 4095;   // raw number at the DRY end
int RAW_WET = 0;      // raw number at the WET end

// ---- rules ----
const int MOIST_START = 30;              // below this % -> start
const int MOIST_STOP  = 60;              // at/above -> stop (gap stops chatter)
const unsigned long MAX_RUN_MS    = 60UL * 1000;  // demo: 60 s hard cap
const unsigned long COOLDOWN_MS   = 30UL * 1000;  // demo: 30 s. Real farm: much longer
const unsigned long READ_EVERY_MS = 2000;

bool pumpOn = false;
unsigned long pumpStartedAt = 0, lastRead = 0;
unsigned long cooldownUntil = 0;
bool cooldownNotified = false;

int readMoisturePct() {
  long raw = analogRead(PIN_SOIL);
  long pct = map(raw, RAW_DRY, RAW_WET, 0, 100);
  return constrain(pct, 0, 100);
}
bool gridPowerOn() { return digitalRead(PIN_GRID) == LOW; }

void setPump(bool on) {
  pumpOn = on;
  digitalWrite(PIN_RELAY, on);
  digitalWrite(PIN_LED, on);
  if (on) pumpStartedAt = millis();
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_GRID, INPUT_PULLUP);
  setPump(false);
  Serial.println("SAHAJ reader ready.");
}

void loop() {
  unsigned long now = millis();

  if (pumpOn && now - pumpStartedAt >= MAX_RUN_MS) {
    setPump(false);
    cooldownUntil = now + COOLDOWN_MS;
    cooldownNotified = false;
    Serial.println("SAFETY: max run time reached -> pump OFF");
    Serial.println("ALERT: ran full time without reaching target. Check the pump. (farmer message goes here)");
  }

  if (now - lastRead >= READ_EVERY_MS) {
    lastRead = now;

    int moist = readMoisturePct();
    bool power = gridPowerOn();
    bool inCooldown = now < cooldownUntil;

    if (!pumpOn && moist <= MOIST_START && power && !inCooldown) {
      setPump(true);  Serial.println("DECIDE: dry + power -> pump ON");
    } else if (!pumpOn && moist <= MOIST_START && power && inCooldown && !cooldownNotified) {
      cooldownNotified = true;
      Serial.println("COOLDOWN: soil dry but waiting after a safety stop");
    } else if (pumpOn && moist >= MOIST_STOP) {
      setPump(false); Serial.println("DECIDE: target reached -> pump OFF");
    } else if (pumpOn && !power) {
      setPump(false); Serial.println("DECIDE: power lost -> pump OFF (will wait)");
    }

    Serial.printf("{\"moist\":%d,\"power\":%s,\"pump\":%s,\"cooldown\":%s}\n",
      moist, power ? "true" : "false", pumpOn ? "true" : "false",
      inCooldown ? "true" : "false");
  }
}
