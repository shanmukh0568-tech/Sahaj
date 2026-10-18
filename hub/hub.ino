/*  SAHAJ — Village Hub. Reads weather + receives reader data.
    ESP-NOW does NOT simulate in Wokwi — test the radio on the two
    real boards. In Wokwi, you're only checking the BME280 read. */
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <esp_now.h>
#include <WiFi.h>

Adafruit_BME280 bme;   // I2C: SDA=21, SCL=22
typedef struct { int moist; float soilC; bool power; bool pump; } ReaderMsg;

void onData(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  if (len != sizeof(ReaderMsg)) return;   // drops malformed packets — NOT security yet
  ReaderMsg m; memcpy(&m, data, sizeof(m));
  Serial.printf("HUB got: moist=%d pump=%s\n", m.moist, m.pump ? "on" : "off");
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
