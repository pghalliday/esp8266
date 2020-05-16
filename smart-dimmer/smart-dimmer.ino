#include "Button.h"
#include "RotaryEncoder.h"

void buttonOnRelease();
Button button(D7, buttonOnRelease);

ICACHE_RAM_ATTR void knobInterruptDispatch();
void knobOnChange(int direction);
RotaryEncoder knob(D5, D6, knobInterruptDispatch, knobOnChange);

void setup() {
  Serial.begin(9600);
  button.setup();
  knob.setup();
  config_setup(D2, 5000);
}

void loop() {
  button.loop();
  knob.loop();
  config_loop();
}

void buttonOnRelease() {
  Serial.println("buttonOnRelease");
}

void knobInterruptDispatch() {
  knob.interrupt();
}

void knobOnChange(int direction) {
  Serial.print("knobOnChange: ");
  Serial.println(direction);
}
