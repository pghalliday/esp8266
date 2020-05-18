#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include "WifiConfig.h"
#include "HttpServer.h"

#define HTTP_SERVER_PORT 80

HttpServer *HttpServer::getInstance() {
  static ESP8266WebServer server(HTTP_SERVER_PORT);
  static HttpServer httpServer(&server);
  return &httpServer;
}

HttpServer::HttpServer(ESP8266WebServer *pServer) {
  _pServer = pServer;
}

void HttpServer::setup(f_onSettings onSettings) {
  _onSettings = onSettings;

  _pServer->on(
      "/settings",
      HTTP_GET,
      [=](){ _handleFileRead("/settings.html"); });

  _pServer->on(
      "/settings",
      HTTP_POST,
      std::bind(&HttpServer::_handleSettings, this));

  _pServer->on(
      "/upload",
      HTTP_GET,
      [=](){ _handleFileRead("/upload.html"); });

  _pServer->on(
      "/upload",
      HTTP_POST,
      [=](){ _pServer->send(200); },
      std::bind(&HttpServer::_handleFileUpload, this));

  _pServer->onNotFound([=](){ _handleFileRead(_pServer->uri()); });

  _pServer->begin();
  Serial.print(F("HTTP server started on port: "));
  Serial.println(HTTP_SERVER_PORT);
}

void HttpServer::loop() {
  _pServer->handleClient();
}

String HttpServer::_getContentType(String path) {
  String pathLower = path;
  pathLower.toLowerCase();
  if (pathLower.endsWith(".html")) return "text/html";
  if (pathLower.endsWith(".css")) return "text/css";
  if (pathLower.endsWith(".js")) return "application/javascript";
  if (pathLower.endsWith(".json")) return "application/json";
  if (pathLower.endsWith(".png")) return "image/png";
  if (pathLower.endsWith(".gif")) return "image/gif";
  if (pathLower.endsWith(".jpg")) return "image/jpeg";
  if (pathLower.endsWith(".jpeg")) return "image/jpeg";
  if (pathLower.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

void HttpServer::_handleFileRead(String path) {
  if (path.endsWith("/")) {
    path += "index.html";
  }
  Serial.print(F("HttpServer::_handleFileRead - path: "));
  Serial.println(path);
  String contentType = HttpServer::_getContentType(path);
  Serial.print(F("HttpServer::_handleFileRead - contentType: "));
  Serial.println(contentType);
  if (LittleFS.exists(path)) {
    File file = LittleFS.open(path, "r");
    size_t sent = _pServer->streamFile(file, contentType);
    Serial.print(F("HttpServer::_handleFileRead - sent bytes: "));
    Serial.println(sent);
    file.close();
    return;
  }
  Serial.println(F("HttpServer::_handleFileRead - file not found"));
  _pServer->send(404, "text/plain", "404: Not Found");
}

void HttpServer::_handleSettings() {
  char ssid[WIFI_CONFIG_SSID_BUFFER_SIZE];
  char password[WIFI_CONFIG_SSID_BUFFER_SIZE];
  if (!_pServer->hasArg("ssid") ||
      !_pServer->hasArg("password") ||
      _pServer->arg("ssid") == NULL ||
      _pServer->arg("password") == NULL) {
      _pServer->send(400, "text/plain", "400: Invalid Request");
      return;
  }
  _pServer->sendHeader("Location", "/success.html");
  _pServer->send(303);
  _pServer->arg("ssid").toCharArray(ssid, WIFI_CONFIG_SSID_BUFFER_SIZE);
  _pServer->arg("password").toCharArray(password, WIFI_CONFIG_PASSWORD_BUFFER_SIZE);
  _onSettings(ssid, password);
}

void HttpServer::_handleFileUpload() {
  Serial.println(F("HttpServer::_handleFileUpload"));
  HTTPUpload &upload = _pServer->upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) filename = "/" + filename;
    Serial.print(F("HttpServer::_handleFileUpload - Name: "));
    Serial.println(filename);
    _uploadFile = LittleFS.open(filename, "w");
    Serial.print(F("HttpServer::_handleFileUpload - _uploadFile: "));
    Serial.println(_uploadFile);
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (_uploadFile) {
      Serial.print(F("HttpServer::_handleFileUpload - upload.currentSize: "));
      Serial.println(upload.currentSize);
      Serial.print(F("HttpServer::_handleFileUpload - _uploadFile: "));
      Serial.println(_uploadFile);
      size_t size = _uploadFile.write(upload.buf, upload.currentSize);
      Serial.print(F("HttpServer::_handleFileUpload - bytes written: "));
      Serial.println(size);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (_uploadFile) {
      _uploadFile.close();
      Serial.print(F("HttpServer::_handleFileUpload - Size: "));
      Serial.println(upload.totalSize);
      _pServer->sendHeader("Location", "/success.html");
      _pServer->send(303);
    } else {
      _pServer->send(500, "text/plain", "500: couldn't create file");
    }
  }
}
