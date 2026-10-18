#include <Wire.h>
#include <Adafruit_BME280.h>
#include <esp_now.h>
#include <WiFi.h>

Adafruit_BME280 bme;   // I2C: SDA=21, SCL=22

typedef struct { int moist; float soilC; bool power; bool pump; } ReaderMsg;

void onData(const uint8_t *mac, const uint8_t *data, int len) {
  if (len != sizeof(ReaderMsg)) return;   // reject wrong-size (fake) packets
  ReaderMsg m; memcpy(&m, data, sizeof(m));
  Serial.printf("HUB got: moist=%d pump=%s\n", m.moist, m.pump ? "on" : "off");
  // TODO: if (m.moist <= 30) call Section C's sendDryAlert()
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  if (!bme.begin(0x76)) Serial.println("BME280 not found (try 0x77)");
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) { Serial.println("ESP-NOW init failed"); return; }
  esp_now_register_recv_cb(onData);
  Serial.println("SAHAJ hub ready.");
}

void loop() {
  Serial.printf("WEATHER: %.1fC  %.0f%%  %.0f hPa\n",
    bme.readTemperature(), bme.readHumidity(), bme.readPressure()/100.0F);
  delay(5000);
}
