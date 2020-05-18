#include "Light.h"
#include "Button.h"
#include "RotaryEncoder.h"
#include "LittleFS.h"
#include "Network.h"
#include "HttpServer.h"

void lightOnUpdate(bool on, int brightness);
Light light(D1, lightOnUpdate);

void buttonOnRelease();
Button button(D7, buttonOnRelease);

ICACHE_RAM_ATTR void knobInterruptDispatch();
void knobOnChange(int direction);
RotaryEncoder knob(D5, D6, knobInterruptDispatch, knobOnChange);

void networkOnStateChange(Network::State state);
Network *network = Network::getInstance();

void httpServerOnSettings();
HttpServer *httpServer = HttpServer::getInstance();

void setup() {
  Serial.begin(9600);
  LittleFS.begin();
  light.setup();
  button.setup();
  knob.setup();
  network->setup(networkOnStateChange);
  httpServer->setup(httpServerOnSettings);
}

void loop() {
  button.loop();
  knob.loop();
  network->loop();
  httpServer->loop();
}

void lightOnUpdate(bool on, int brightness) {
  Serial.print(F("lightOnUpdate - on: "));
  Serial.print(on);
  Serial.print(F(": brightness: "));
  Serial.println(brightness);
}

void buttonOnRelease() {
  Serial.println(F("buttonOnRelease"));
  light.toggle();
}

void knobInterruptDispatch() {
  knob.interrupt();
}

void knobOnChange(int direction) {
  Serial.print(F("knobOnChange - direction: "));
  Serial.println(direction);
  light.changeBrightness(direction);
}

void networkOnStateChange(Network::State state) {
  Serial.print(F("networkOnStateChange - state: "));
  Serial.println(static_cast<int>(state));
}

void httpServerOnSettings() {
  Serial.println(F("httpServerOnSettings"));
}
