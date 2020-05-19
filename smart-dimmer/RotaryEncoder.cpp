#include <Arduino.h>
#include "RotaryEncoder.h"

/*
 * Uncomment the next line to enable
 * debug output to serial for this file
 */
//#define DEBUG
#include "debug.h"

#define ROTARY_ENCODER_A_RISING 1
#define ROTARY_ENCODER_A_FALLING 2
#define ROTARY_ENCODER_B_RISING 3
#define ROTARY_ENCODER_B_FALLING 4

RotaryEncoder::RotaryEncoder(int pinA, int pinB, f_interruptDispatch interruptDispatch, f_onChange onChange) {
  _pinA = pinA;
  _pinB = pinB;
  _interruptDispatch = interruptDispatch;
  _onChange = onChange;
}

void RotaryEncoder::setup() {
  pinMode(_pinA, INPUT);
  pinMode(_pinB, INPUT);
  _pinAState = digitalRead(_pinA);
  _pinBState = digitalRead(_pinB);
  attachInterrupt(digitalPinToInterrupt(_pinA), _interruptDispatch, CHANGE);
  attachInterrupt(digitalPinToInterrupt(_pinB), _interruptDispatch, CHANGE);
}

void RotaryEncoder::loop() {
  if (_writePos != _readPos) {
    int state = _buffer[_readPos];
    _readPos = (_readPos + 1) % ROTARY_ENCODER_BUFFER_SIZE;
    if (
      (_state == ROTARY_ENCODER_A_FALLING && state == ROTARY_ENCODER_B_FALLING) ||
      (_state == ROTARY_ENCODER_A_RISING && state == ROTARY_ENCODER_B_RISING)
    ) {
      _onChange(ROTARY_ENCODER_RIGHT);
    }
    if (
      (_state == ROTARY_ENCODER_B_FALLING && state == ROTARY_ENCODER_A_FALLING) ||
      (_state == ROTARY_ENCODER_B_RISING && state == ROTARY_ENCODER_A_RISING)
    ) {
      _onChange(ROTARY_ENCODER_LEFT);
    }
    _state = state;
  }
}

void RotaryEncoder::interrupt() {
  int pinAState = digitalRead(_pinA);
  int pinBState = digitalRead(_pinB);
  int state = 0;
  if (pinAState != _pinAState) {
    if (pinAState == HIGH) {
      state = ROTARY_ENCODER_A_RISING;
    } else {
      state = ROTARY_ENCODER_A_FALLING;
    }
    _pinAState = pinAState;
  } else if (pinBState != _pinBState) {
    if (pinBState == HIGH) {
      state = ROTARY_ENCODER_B_RISING;
    } else {
      state = ROTARY_ENCODER_B_FALLING;
    }
    _pinBState = pinBState;
  }
  if (state != 0) {
    _buffer[_writePos] = state;
    _writePos = (_writePos + 1) % ROTARY_ENCODER_BUFFER_SIZE;
  }
}
