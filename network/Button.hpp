#ifndef Button_hpp
#define Button_hpp

#include <Arduino.h>
#include <functional>

class Button {
  using f_callback = std::function<void()>;

  int _pin;
  unsigned long _debounceDelay;
  f_callback _onRelease;
  f_callback _onLongHold;
  unsigned long _longDelay = 0;
  unsigned long _longTime = 0;
  unsigned long _lastDebounceTime = 0;
  int _state = HIGH;
  int _lastState = HIGH;
  bool _notified = false;

  public:
    Button(
      int pin,
      unsigned long debounceDelay,
      f_callback onRelease
    ) :
      _pin(pin),
      _debounceDelay(debounceDelay),
      _onRelease(onRelease) {
    }

    Button(
      int pin,
      unsigned long debounceDelay,
      f_callback onRelease,
      unsigned long longDelay,
      f_callback onLongHold
    ) :
      _pin(pin),
      _debounceDelay(debounceDelay),
      _onRelease(onRelease),
      _longDelay(longDelay),
      _onLongHold(onLongHold) {
    }

    void setup() {
      pinMode(_pin, INPUT_PULLUP);
    }

    void loop() {
      bool notify = false;
      bool longNotify = false;
      int reading = digitalRead(_pin);
      if (reading != _lastState) {
        _lastDebounceTime = millis();
      }

      unsigned long now = millis();
      if ((now - _lastDebounceTime) > _debounceDelay) {
        if (reading != _state) {
          _state = reading;
          if (_longDelay) {
            if (_state == LOW) {
              _notified = false;
              _longTime = now;
            }
          }

          if (!_notified) {
            if (_state == HIGH) {
              notify = true;
            }
          }
        } else {
          // check for long hold
          if (_longDelay) {
            if (!_notified) {
              if (_state == LOW) {
                if ((now - _longTime) > _longDelay) {
                  _notified = true;
                  longNotify = true;
                }
              }
            }
          }
        }
      }

      _lastState = reading;

      if (longNotify) {
        _onLongHold();
      } else if (notify) {
        _onRelease();
      }
    }
};

#endif
