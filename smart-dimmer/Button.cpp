#include <Arduino.h>
#include "Button.h"

/*
 * Uncomment the next line to enable
 * debug output to serial for this file
 */
//#define DEBUG
#include "debug.h"

#define BUTTON_DEBOUNCE_DELAY 50

Button::Button(int pin, f_onRelease onRelease) {
  _pin = pin;
  _onRelease = onRelease;
  _lastDebounceTime = 0;
}

void Button::setup() {
  pinMode(_pin, INPUT_PULLUP);
  _pinState = digitalRead(_pin);
  _lastPinState = _pinState;
}

void Button::loop() {
  int pinState = digitalRead(_pin);
  if (_lastPinState != pinState) {
    _lastDebounceTime = millis();
    _lastPinState = pinState;
  }

  if (_pinState != pinState) {
    if ((millis() - _lastDebounceTime) > BUTTON_DEBOUNCE_DELAY) {
      _pinState = pinState;
      if (_pinState == HIGH) {
        _onRelease();
      }
    }
  }
}
