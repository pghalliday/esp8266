#ifndef Button_h
#define Button_h

#include "Arduino.h"

#define BUTTON_BUFFER_SIZE 8
#define BUTTON_LEFT (-1)
#define BUTTON_RIGHT 1

class Button {
  using f_onRelease = void(*)();

  public:
    Button(int pin, f_onRelease onRelease);
    void setup();
    void loop();
  private:
    int _pin;
    int _pinState;
    int _lastPinState;
    unsigned long _lastDebounceTime;
    f_onRelease _onRelease;
};

#endif
