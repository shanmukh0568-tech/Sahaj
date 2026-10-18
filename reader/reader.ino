// SAHAJ reader — Rev B. One firmware, two modes.
#define CROP_RICE   // comment out for soil mode (cotton / wheat / maize)

const int PIN_SOIL=34, PIN_RELAY=26, PIN_GRID=27, PIN_LED=2;
const int PIN_TRIG=25, PIN_ECHO=33;              // ultrasonic (rice)

int RAW_DRY=4095, RAW_WET=0;                      // Wokwi pot calibration
const int MOIST_START=30, MOIST_STOP=60;

const float SENSOR_TO_SOIL_CM=25.0;              // sensor height above soil
const float AWD_START_CM=-15.0;                  // 15 cm below soil -> pump ON
const float AWD_STOP_CM=5.0;                     // 5 cm above soil  -> pump OFF
bool KEEP_FLOODED=false;                          // true: first ~2 wks + flowering

const unsigned long MAX_RUN_MS=60UL*1000, COOLDOWN_MS=30UL*1000, READ_EVERY_MS=2000;
bool pumpOn=false; unsigned long pumpStartedAt=0, cooldownUntil=0, lastRead=0;

void setPump(bool on){ pumpOn=on; digitalWrite(PIN_RELAY,on?HIGH:LOW); digitalWrite(PIN_LED,on?HIGH:LOW); }
bool gridPowerOn(){ return digitalRead(PIN_GRID)==LOW; }

int readMoisturePct(){
  int raw=analogRead(PIN_SOIL);
  int pct=map(raw,RAW_DRY,RAW_WET,0,100);
  if(pct<0)pct=0; if(pct>100)pct=100; return pct;
}
float readWaterLevelCm(){
  digitalWrite(PIN_TRIG,LOW); delayMicroseconds(2);
  digitalWrite(PIN_TRIG,HIGH); delayMicroseconds(10);
  digitalWrite(PIN_TRIG,LOW);
  long dur=pulseIn(PIN_ECHO,HIGH,30000UL);
  if(dur==0) return -999;                         // no echo -> unknown
  float distCm=dur*0.0343/2.0;
  return SENSOR_TO_SOIL_CM - distCm;              // + above soil, - below soil
}

void setup(){
  Serial.begin(115200);
  pinMode(PIN_RELAY,OUTPUT); pinMode(PIN_LED,OUTPUT);
  pinMode(PIN_GRID,INPUT_PULLUP);
  pinMode(PIN_TRIG,OUTPUT); pinMode(PIN_ECHO,INPUT);
  setPump(false);
#ifdef CROP_RICE
  Serial.println("SAHAJ reader: RICE / AWD mode");
#else
  Serial.println("SAHAJ reader: SOIL-MOISTURE mode");
#endif
}

void loop(){
  unsigned long now=millis();
  if(now-lastRead<READ_EVERY_MS) return;
  lastRead=now;

  if(pumpOn && (now-pumpStartedAt>MAX_RUN_MS)){
    setPump(false); cooldownUntil=now+COOLDOWN_MS;
    Serial.println("SAFETY: max run -> pump OFF, cooling down. (Check the pump!)"); return;
  }
  if(now<cooldownUntil){ Serial.println("Cooling down..."); return; }

  if(!gridPowerOn()){
    if(pumpOn) setPump(false);
    Serial.println("No grid power. Waiting for the next window."); return;
  }

#ifdef CROP_RICE
  float level=readWaterLevelCm();
  if(level==-999){ Serial.println("AWD: no reading from tube."); return; }
  Serial.print("AWD level: "); Serial.print(level,1); Serial.println(" cm vs soil");
  if(KEEP_FLOODED){
    if(level<AWD_STOP_CM && !pumpOn){ setPump(true); pumpStartedAt=now; Serial.println("KEEP-FLOODED -> pump ON"); }
    else if(level>=AWD_STOP_CM && pumpOn){ setPump(false); Serial.println("KEEP-FLOODED at target -> pump OFF"); }
  } else {
    if(level<=AWD_START_CM && !pumpOn){ setPump(true); pumpStartedAt=now; Serial.println("AWD: safe line -> pump ON"); }
    else if(level>=AWD_STOP_CM && pumpOn){ setPump(false); Serial.println("AWD: re-flooded -> pump OFF"); }
  }
#else
  int pct=readMoisturePct();
  Serial.print("Soil moisture: "); Serial.print(pct); Serial.println("%");
  if(pct<=MOIST_START && !pumpOn){ setPump(true); pumpStartedAt=now; Serial.println("Soil dry -> pump ON"); }
  else if(pct>=MOIST_STOP && pumpOn){ setPump(false); Serial.println("Soil wet enough -> pump OFF"); }
#endif
}
