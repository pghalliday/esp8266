#include <Arduino.h>
#include "Light.h"

/*
 * Uncomment the next line to enable
 * debug output to serial for this file
 */
//#define DEBUG
#include "debug.h"

#define LIGHT_DEFAULT_BRIGHTNESS LIGHT_MAX_BRIGHTNESS
#define LIGHT_DEFAULT_ON false

#define LIGHT_LIMIT_BRIGHTNESS(BRIGHTNESS) ((BRIGHTNESS) < 0 ? 0 : ((BRIGHTNESS) > LIGHT_MAX_BRIGHTNESS ? LIGHT_MAX_BRIGHTNESS : (BRIGHTNESS))) 
#define LIGHT_PWM_LEVEL(BRIGHTNESS) (int((float(BRIGHTNESS) / float(LIGHT_MAX_BRIGHTNESS)) * 255))

Light::Light(int pin, f_onUpdate onUpdate) {
  _pin = pin;
  _onUpdate = onUpdate;
  _brightness = LIGHT_DEFAULT_BRIGHTNESS;
  _on = LIGHT_DEFAULT_ON;
}

void Light::setup() {
  pinMode(_pin, OUTPUT);
  _update();
}

void Light::_update() {
  if (_brightness == 0) {
    _on = false;
    _brightness = LIGHT_MAX_BRIGHTNESS;
  }

  if (!_on) {
    digitalWrite(_pin, LOW);
  } else {
    int pwmLevel = LIGHT_PWM_LEVEL(_brightness);
    DEBUG_VAL(F("setting output"), F("pwmLevel"), pwmLevel);
    analogWrite(_pin, pwmLevel);
  }

  _onUpdate(_on, _brightness);
}

void Light::toggle() {
  _on = !_on;
  _update();
}

void Light::setOn(bool on) {
  _on = on;
  _update();
}

void Light::setBrightness(int brightness) {
  _on = true;
  _brightness = LIGHT_LIMIT_BRIGHTNESS(brightness);
  _update();
}

void Light::changeBrightness(int change) {
  if (_on) {
    _brightness = LIGHT_LIMIT_BRIGHTNESS(_brightness + change);
    _update();
  } else if (change > 0) {
    _on = true;
    _brightness = LIGHT_LIMIT_BRIGHTNESS(change);
    _update();
  }
}
