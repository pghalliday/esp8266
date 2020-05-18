#include "Light.h"
#include "Button.h"
#include "RotaryEncoder.h"
#include "LittleFS.h"
#include "Network.h"

void lightOnUpdate(bool on, int brightness);
Light light(D1, lightOnUpdate);

void buttonOnRelease();
Button button(D7, buttonOnRelease);

ICACHE_RAM_ATTR void knobInterruptDispatch();
void knobOnChange(int direction);
RotaryEncoder knob(D5, D6, knobInterruptDispatch, knobOnChange);

void networkOnStateChange(int state);
Network *network = Network::getInstance();

void setup() {
  Serial.begin(9600);
  LittleFS.begin();
  light.setup();
  button.setup();
  knob.setup();
  network->setup(networkOnStateChange);
}

void loop() {
  button.loop();
  knob.loop();
  network->loop();
}

void lightOnUpdate(bool on, int brightness) {
  Serial.print("lightOnUpdate: on: ");
  Serial.print(on);
  Serial.print(": brightness: ");
  Serial.println(brightness);
}

void buttonOnRelease() {
  Serial.println("buttonOnRelease");
  light.toggle();
}

void knobInterruptDispatch() {
  knob.interrupt();
}

void knobOnChange(int direction) {
  Serial.print("knobOnChange: direction: ");
  Serial.println(direction);
  light.changeBrightness(direction);
}

void networkOnStateChange(int state) {
  Serial.print("networkOnStateChange: state: ");
  Serial.println(state);
}
