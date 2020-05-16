#include "Arduino.h"
#include "RotaryEncoder.h"

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
    /* Serial.print("state: "); */
    /* Serial.println(state); */
    // 7, 12 (A -> LOW, B -> LOW) or 6, 9 (A -> HIGH, B -> HIGH) sequences are right
    if (
      (_state == 7 && state == 12) ||
      (_state == 6 && state == 9)
    ) {
      _onChange(ROTARY_ENCODER_RIGHT);
    }
    // 11, 5 (B -> HIGH, A -> HIGH) or 10, 8 (B -> LOW, A -> LOW) sequences are left
    if (
      (_state == 11 && state == 5) ||
      (_state == 10 && state == 8)
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
    if (pinBState != _pinBState) {
      if (pinAState == HIGH) {
        if (pinBState == HIGH) {
          // state 01 - A and B both changed to HIGH
          state = 1;
        } else {
          // state 02 - A changed to HIGH, B changed to LOW
          state = 2;
        }
      } else {
        if (pinBState == HIGH) {
          // state 03 - A changed to LOW, B changed to HIGH
          state = 3;
        } else {
          // state 04 - A and B both changed to LOW
          state = 4;
        }
      }
    } else {
      if (pinAState == HIGH) {
        if (pinBState == HIGH) {
          // state 05 - A changed to HIGH, B stayed HIGH
          state = 5;
        } else {
          // state 06 - A changed to HIGH, B stayed LOW
          state = 6;
        }
      } else {
        if (pinBState == HIGH) {
          // state 07 - A changed to LOW, B stayed HIGH
          state = 7;
        } else {
          // state 08 - A changed to LOW, B stayed LOW
          state = 8;
        }
      }
    }
  } else {
    if (pinBState != _pinBState) {
      if (pinAState == HIGH) {
        if (pinBState == HIGH) {
          // state 09 - A stayed HIGH, B changed to HIGH
          state = 9;
        } else {
          // state 10 - A stayed HIGH, B changed to LOW
          state = 10;
        }
      } else {
        if (pinBState == HIGH) {
          // state 11 - A stayed LOW, B changed to HIGH
          state = 11;
        } else {
          // state 12 - A stayed LOW, B changed to LOW
          state = 12;
        }
      }
    }
  }

  if (state != 0) {
    _buffer[_writePos] = state;
    _writePos = (_writePos + 1) % ROTARY_ENCODER_BUFFER_SIZE;
  }

  _pinAState = pinAState;
  _pinBState = pinBState;
}
