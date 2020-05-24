#ifndef Ap_hpp
#define Ap_hpp

/*
#define AP_SSID
#define AP_PASS
*/
#include "secrets.h"

#include <Arduino.h>
#include <ESP8266WiFi.h>

class Ap {
  bool _started;
  unsigned int _connected;

  void _checkConnections(unsigned int connected) {
    if (connected != _connected) {
      _connected = connected;
      Serial.printf("Connected stations: %d\n", _connected);
    }
  }

  public:
    Ap() :
      _started(false),
      _connected(0) {
    }

    void setup() {
      Serial.println("Ensure that Soft AP is stopped");
      WiFi.softAPdisconnect(true);
      WiFi.printDiag(Serial);
    }

    void loop() {
      _checkConnections(WiFi.softAPgetStationNum());
    }

    void stop() {
      if (_started) {
        Serial.println("Disconnecting and setting soft-AP mode to off");
        WiFi.softAPdisconnect(true);
        WiFi.printDiag(Serial);
        _started = false;
      }
    }

    void start() {
      if (!_started) {
        Serial.printf(
          "Starting AP with ssid [%s] and password [%s]\n",
          AP_SSID,
          AP_PASS
        );
        WiFi.softAP(AP_SSID, AP_PASS);
        Serial.printf("AP IP Address: %s\n", WiFi.softAPIP().toString().c_str());
        WiFi.printDiag(Serial);
        _started = true;
      }
    }

    void toggle() {
      if (_started) {
        stop();
      } else {
        start();
      }
    }
};

#endif
