#ifndef Light_h
#define Light_h

#include <Arduino.h>
#include <functional>

#define LIGHT_MAX_BRIGHTNESS 25

class Light {
  using f_onUpdate = std::function<void(bool on, int brightness)>;

  public:
    Light(int pin, f_onUpdate onUpdate);
    void setup();
    void toggle();
    void setOn(bool on);
    void setBrightness(int brightness);
    void changeBrightness(int change);
  private:
    int _pin;
    bool _on;
    int _brightness;
    void _update();
    f_onUpdate _onUpdate;
};

#endif
