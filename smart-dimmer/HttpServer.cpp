#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
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
  if (path.endsWith(".html")) return "text/html";
  if (path.endsWith(".css")) return "text/css";
  if (path.endsWith(".js")) return "application/javascript";
  if (path.endsWith(".json")) return "application/json";
  if (path.endsWith(".png")) return "image/png";
  if (path.endsWith(".gif")) return "image/gif";
  if (path.endsWith(".jpg")) return "image/jpeg";
  if (path.endsWith(".jpeg")) return "image/jpeg";
  if (path.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

void HttpServer::_handleFileRead(String path) {
  if (path.endsWith("/")) {
    path += "index.html";
  }
  Serial.print(F("HttpServer::_handleFileRead - path: "));
  Serial.println(path);
  String contentType = HttpServer::_getContentType(path);
  if (LittleFS.exists(path)) {
    File file = LittleFS.open(path, "r");
    size_t sent = _pServer->streamFile(file, contentType);
    file.close();
    return;
  }
  Serial.println(F("HttpServer::_handleFileRead - file not found"));
  _pServer->send(404, "text/plain", "404: Not Found");
}

void HttpServer::_handleFileUpload() {
  static File fsUploadFile;
  Serial.println(F("HttpServer::_handleFileUpload"));
  HTTPUpload &upload = _pServer->upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) filename = "/" + filename;
    Serial.print(F("HttpServer::_handleFileUpload - Name: "));
    Serial.println(filename);
    fsUploadFile = LittleFS.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (fsUploadFile) {
      fsUploadFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
      Serial.print(F("HttpServer::_handleFileUpload - Size: "));
      Serial.println(upload.totalSize);
      _pServer->sendHeader("Location", "/success.html");
      _pServer->send(303);
    } else {
      _pServer->send(500, "text/plain", "500: couldn't create file");
    }
  }
}
