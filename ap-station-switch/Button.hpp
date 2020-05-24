#ifndef Button_hpp
#define Button_hpp

#include <Arduino.h>
#include <functional>

class Button {
  using f_onRelease = std::function<void()>;

  int _pin;
  f_onRelease _onRelease;
  unsigned long _debounceDelay;
  unsigned long _lastDebounceTime;
  int _state;
  int _lastState;

  public:
    Button(
      int pin,
      unsigned long debounceDelay,
      f_onRelease onRelease
    ) :
      _pin(pin),
      _debounceDelay(debounceDelay),
      _onRelease(onRelease),
      _state(HIGH),
      _lastState(HIGH),
      _lastDebounceTime(0) {
    }

    void setup() {
      pinMode(_pin, INPUT_PULLUP);
    }

    void loop() {
      bool notify = false;
      int reading = digitalRead(_pin);
      if (reading != _lastState) {
        _lastDebounceTime = millis();
      }

      if (reading != _state) {
        if ((millis() - _lastDebounceTime) > _debounceDelay) {
          _state = reading;
          if (_state == HIGH) {
            notify = true;
          }
        }
      }

      _lastState = reading;

      if (notify) {
        _onRelease();
      }
    }
};

#endif
