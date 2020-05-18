#include <ESP8266WiFi.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

WiFiManager wifiManager;
bool config_shouldSaveConfig = false;

int config_resetPin = 0;
int config_resetPinState = HIGH;
unsigned int config_resetTimeout = 0;
unsigned int config_resetStart = 0;

void config_configModeCallback(WiFiManager *wifiManager);

void config_setup(int resetPin, unsigned int resetTimeout) {
  Serial.println(WiFi.macAddress());
  config_resetPin = resetPin;
  config_resetTimeout = resetTimeout;
  config_setupReset();
  wifiManager.setAPCallback(config_configModeCallback);
  wifiManager.setSaveConfigCallback(config_saveConfigCallback);
  if (!wifiManager.autoConnect("smart-dimmer", "12345678")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    // reset and try again
    ESP.reset();
    delay(5000);
  }
  Serial.println("Connected :)");
}

void config_loop() {
  config_loopReset();
}

void config_setupReset() {
  pinMode(config_resetPin, INPUT_PULLUP);
  config_resetPinState = digitalRead(config_resetPin);
}

void config_reset() {
  Serial.println("Resetting...");
  WiFi.disconnect();
  delay(2000);
  ESP.reset();
}

void config_loopReset() {
  int resetPinState = digitalRead(config_resetPin);
  if (resetPinState == LOW) {
    if (resetPinState != config_resetPinState) {
      config_resetStart = millis();
    } else {
      if (millis() - config_resetStart > config_resetTimeout) {
        config_reset();
      }
    }
  }
  config_resetPinState = resetPinState;
}

void config_configModeCallback(WiFiManager *wifiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(wifiManager->getConfigPortalSSID());
}

void config_saveConfigCallback() {
  Serial.println("Should save config");
  config_shouldSaveConfig = true;
}
