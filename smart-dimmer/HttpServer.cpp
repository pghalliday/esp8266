#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "WifiConfig.h"
#include "Storage.h"
#include "HttpServer.h"

/*
 * Uncomment the next line to enable
 * debug output to serial for this file
 */
//#define DEBUG
#include "debug.h"

#define HTTP_SERVER_PORT 80

HttpServer *HttpServer::getInstance() {
  static ESP8266WebServer server(HTTP_SERVER_PORT);
  static HttpServer httpServer(&server);
  return &httpServer;
}

HttpServer::HttpServer(ESP8266WebServer *pServer) {
  _pStorage = Storage::getInstance();
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

  DEBUG_VAL(F("HTTP Server started"), F("port"), HTTP_SERVER_PORT);
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
  DEBUG_VAL(F("start"), F("path"), path);
  if (path.endsWith("/")) {
    path += "index.html";
  }
  DEBUG_VAL(F("adjusted"), F("path"), path);
  String contentType = HttpServer::_getContentType(path);
  DEBUG_VAL(F("checked content type"), F("contentType"), contentType);
  if (_pStorage->exists(path)) {
    File file = _pStorage->open(path, "r");
    size_t sent = _pServer->streamFile(file, contentType);
    DEBUG_VAL(F("file streamed"), F("sent bytes"), sent);
    file.close();
    return;
  }
  DEBUG_MSG(F("file not found"));
  _pServer->send(404, "text/plain", "404: Not Found");
}

void HttpServer::_handleSettings() {
  char ssid[WIFI_CONFIG_SSID_BUFFER_SIZE] = "";
  char password[WIFI_CONFIG_PASSWORD_BUFFER_SIZE] = "";
  DEBUG_VAL(F("POST data received"), F("raw"), _pServer->arg("plain")); 
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
  HTTPUpload &upload = _pServer->upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    DEBUG_VAL(F("started upload"), F("filename"), filename);
    if (!filename.startsWith("/")) filename = "/" + filename;
    DEBUG_VAL(F("adjusted filename"), F("filename"), filename);
    _uploadFile = _pStorage->open(filename, "w");
    DEBUG_VAL(F("opened file"), F("handle"), _uploadFile);
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    DEBUG_LIST_START(F("received chunk"));
    DEBUG_LIST_VAL(F("bytes"), upload.currentSize);
    DEBUG_LIST_VAL(F("file handle"), _uploadFile);
    DEBUG_LIST_END;
    if (_uploadFile) {
      size_t bytesWritten = _uploadFile.write(upload.buf, upload.currentSize);
      DEBUG_VAL(F("bytes written"), F("bytesWritten"), bytesWritten);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (_uploadFile) {
      DEBUG_VAL(F("upload complete"), F("total size"), upload.totalSize);
      _uploadFile.close();
      _pServer->sendHeader("Location", "/success.html");
      _pServer->send(303);
    } else {
      _pServer->send(500, "text/plain", "500: couldn't create file");
    }
  }
}
