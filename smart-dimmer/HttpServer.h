#ifndef HttpServer_h
#define HttpServer_h

#include <Arduino.h>
#include <functional>
#include <ESP8266WebServer.h>

class HttpServer {
  using f_onSettings = std::function<void()>;

  public:
    static HttpServer *getInstance();
    void setup(f_onSettings onSettings);
    void loop();
  private:
    HttpServer(ESP8266WebServer *pServer);
    HttpServer(HttpServer const&){};
    HttpServer& operator=(HttpServer const&){};
    f_onSettings _onSettings;
    ESP8266WebServer *_pServer;
    void _handleFileRead(String path);
    void _handleFileUpload();
    static String _getContentType(String path);
};

#endif
