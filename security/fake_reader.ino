#include <esp_now.h>
#include <WiFi.h>

uint8_t bcast[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if(esp_now_init()!=ESP_OK){ Serial.println("init failed"); return; }
  esp_now_peer_info_t p={}; memcpy(p.peer_addr,bcast,6); p.channel=0; p.encrypt=false;
  esp_now_add_peer(&p);
  randomSeed(micros());
  Serial.println("FAKE reader: dumping junk packets at the hub (no key)");
}

void loop(){
  uint8_t junk[16];
  for(int i=0;i<16;i++) junk[i]=random(0,256);  // correct size, but no key
  esp_now_send(bcast, junk, 16);
  Serial.println("dumped a fake packet");
  delay(1500);
}
