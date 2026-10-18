#include <esp_now.h>
#include <WiFi.h>
#include "mbedtls/aes.h"

const uint8_t KEY[16] = {0x53,0x41,0x48,0x41,0x4A,0x5F,0x44,0x45,0x4D,0x4F,0x5F,0x4B,0x45,0x59,0x30,0x31};
uint8_t bcast[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

void aesEnc(const uint8_t in[16], uint8_t out[16]){
  mbedtls_aes_context a; mbedtls_aes_init(&a);
  mbedtls_aes_setkey_enc(&a, KEY, 128);
  mbedtls_aes_crypt_ecb(&a, MBEDTLS_AES_ENCRYPT, in, out);
  mbedtls_aes_free(&a);
}

void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if(esp_now_init()!=ESP_OK){ Serial.println("init failed"); return; }
  esp_now_peer_info_t p={}; memcpy(p.peer_addr,bcast,6); p.channel=0; p.encrypt=false;
  esp_now_add_peer(&p);
  Serial.println("REAL reader: sending AES-encrypted readings");
}

void loop(){
  uint8_t plain[16]={0}, cipher[16];
  plain[0]='S'; plain[1]='H'; plain[2]='J'; plain[3]='1';   // marker
  uint32_t farmId=4821; memcpy(&plain[4],&farmId,4);
  plain[8]=27;   // moisture %
  plain[9]=1;    // pump on
  aesEnc(plain, cipher);
  esp_now_send(bcast, cipher, 16);
  Serial.println("sent encrypted reading");
  delay(2000);
}
