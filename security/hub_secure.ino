#include <esp_now.h>
#include <WiFi.h>
#include "mbedtls/aes.h"

const uint8_t KEY[16] = {0x53,0x41,0x48,0x41,0x4A,0x5F,0x44,0x45,0x4D,0x4F,0x5F,0x4B,0x45,0x59,0x30,0x31};

void aesDec(const uint8_t in[16], uint8_t out[16]){
  mbedtls_aes_context a; mbedtls_aes_init(&a);
  mbedtls_aes_setkey_dec(&a, KEY, 128);
  mbedtls_aes_crypt_ecb(&a, MBEDTLS_AES_DECRYPT, in, out);
  mbedtls_aes_free(&a);
}

void onRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len){
  if(len != 16){ Serial.println("REJECTED: wrong size (malformed packet)"); return; }
  uint8_t plain[16];
  aesDec(data, plain);
  if(plain[0]!='S'||plain[1]!='H'||plain[2]!='J'||plain[3]!='1'){
    Serial.println("REJECTED: bad key -- fake reader blocked");
    return;
  }
  uint32_t farmId; memcpy(&farmId,&plain[4],4);
  int moist=plain[8]; bool pump=plain[9];
  Serial.printf("ACCEPTED farm=%u moist=%d pump=%s\n", farmId, moist, pump?"on":"off");
}

void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if(esp_now_init()!=ESP_OK){ Serial.println("init failed"); return; }
  esp_now_register_recv_cb(onRecv);
  Serial.println("SAHAJ secure hub: only readers with the key are accepted.");
}

void loop(){ delay(1000); }
