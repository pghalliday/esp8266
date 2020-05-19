#include "Light.h"
#include "Button.h"
#include "RotaryEncoder.h"
#include "Storage.h"
#include "Network.h"
#include "HttpServer.h"

/*
 * Uncomment the next line to enable
 * debug output to serial for this file
 */
//#define DEBUG
#include "debug.h"

Storage *pStorage = Storage::getInstance();

void lightOnUpdate(bool on, int brightness);
Light light(D1, lightOnUpdate);

void buttonOnRelease();
Button button(D7, buttonOnRelease);

ICACHE_RAM_ATTR void knobInterruptDispatch();
void knobOnChange(int direction);
RotaryEncoder knob(D5, D6, knobInterruptDispatch, knobOnChange);

void networkOnStateChange(Network::State state);
Network *pNetwork = Network::getInstance();

void httpServerOnSettings(const char *ssid, const char *password);
HttpServer *pHttpServer = HttpServer::getInstance();

void setup() {
  Serial.begin(115200);
  pStorage->setup();
  light.setup();
  button.setup();
  knob.setup();
  pNetwork->setup(networkOnStateChange);
  pHttpServer->setup(httpServerOnSettings);
}

void loop() {
  button.loop();
  knob.loop();
  pNetwork->loop();
  pHttpServer->loop();
}

void lightOnUpdate(bool on, int brightness) {
  DEBUG_LIST_START(F("new value"));
  DEBUG_LIST_VAL(F("on"), on);
  DEBUG_LIST_VAL(F("brightness"), brightness);
  DEBUG_LIST_END;
}

void buttonOnRelease() {
  DEBUG_MSG(F("toggle the light"));
  light.toggle();
}

void knobInterruptDispatch() {
  knob.interrupt();
}

void knobOnChange(int direction) {
  DEBUG_VAL(F("change the light brightness"), F("change"), direction);
  light.changeBrightness(direction);
}

void networkOnStateChange(Network::State state) {
  DEBUG_VAL(F("new state"), F("state"), static_cast<int>(state));
}

void httpServerOnSettings(const char *ssid, const char *password) {
  DEBUG_LIST_START(F("new settings"));
  DEBUG_LIST_VAL(F("ssid"), ssid);
  DEBUG_LIST_VAL(F("password"), password);
  DEBUG_LIST_END;
  pNetwork->setStationConfig(ssid, password);
}
