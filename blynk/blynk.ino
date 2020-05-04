#define BLYNK_PRINT Serial

#include "secrets.h"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH, WIFI_SSID, WIFI_PASS, BLYNK_HOST, BLYNK_PORT);
}

void loop() {
  Blynk.run();
}
