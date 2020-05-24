#ifndef WebServer_hpp
#define WebServer_hpp

#include <Arduino.h>
#include <ESP8266WebServer.h>

class WebServer {
  ESP8266WebServer _server;

  public:
    WebServer(int port) :
      _server(port) {
    }

    void setup() {
      _server.on("/", [=]() {
        Serial.printf("hello %S\n", _server.client().remoteIP().toString().c_str());
        _server.send(200, "text/plain", "hello");
      });
      _server.begin();
    }

    void loop() {
      _server.handleClient();
    }
};

#endif
