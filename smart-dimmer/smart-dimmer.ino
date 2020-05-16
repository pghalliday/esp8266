#include "RotaryEncoder.h"

ICACHE_RAM_ATTR void knobInterruptDispatch();
void knobOnChange(int direction);
RotaryEncoder knob(D5, D6, knobInterruptDispatch, knobOnChange);

void setup() {
  Serial.begin(9600);
  knob.setup();
  config_setup(D2, 5000);
}

void loop() {
  knob.loop();
  config_loop();
}

void knobInterruptDispatch() {
  knob.interrupt();
}

void knobOnChange(int direction) {
  Serial.print("knobOnChange: ");
  Serial.println(direction);
}
