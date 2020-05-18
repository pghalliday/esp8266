#ifndef RotaryEncoder_h
#define RotaryEncoder_h

#include <Arduino.h>
#include <functional>

#define ROTARY_ENCODER_BUFFER_SIZE 8
#define ROTARY_ENCODER_LEFT (-1)
#define ROTARY_ENCODER_RIGHT 1

class RotaryEncoder {
  using f_onChange = std::function<void(int direction)>;

  // can't use <functional> stuff with attachInterrupt
  using f_interruptDispatch = void(*)();

  public:
    RotaryEncoder(int pinA, int pinB, f_interruptDispatch interruptDispatch, f_onChange onChange);
    void setup();
    void loop();
    ICACHE_RAM_ATTR void interrupt();
  private:
    int _pinA;
    int _pinB;
    volatile unsigned int _pinAState;
    volatile unsigned int _pinBState;
    volatile unsigned int _buffer[ROTARY_ENCODER_BUFFER_SIZE];
    volatile unsigned int _writePos;
    unsigned int _readPos;
    unsigned int _state;
    f_interruptDispatch _interruptDispatch;
    f_onChange _onChange;
};

#endif
