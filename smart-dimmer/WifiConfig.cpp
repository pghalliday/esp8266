#include <Arduino.h>
#include <ArduinoJson.h>
#include "WifiConfig.h"

/*
 * Uncomment the next line to enable
 * debug output to serial for this file
 */
//#define DEBUG
#include "debug.h"

#define WIFI_CONFIG_JSON_DOCUMENT_SIZE 256

const char WifiConfig::_ssidFieldName[] = "ssid";
const char WifiConfig::_passwordFieldName[] = "password";

void WifiConfig::init(f_onChange onChange, const char *path, const char *defaultSsid, const char *defaultPassword) {
  _onChange = onChange;
  _defaultSsid = defaultSsid;
  _defaultPassword = defaultPassword;
  StaticJsonDocument<WIFI_CONFIG_JSON_DOCUMENT_SIZE> doc;
  _configFile.init(path, &doc);
  _applyDoc(&doc);
}

void WifiConfig::_applyDoc(JsonDocument *pDoc) {
  const char *ssid = _defaultSsid;
  const char *password = _defaultPassword;
  if (!pDoc->isNull()) {
    ssid = (*pDoc)[WifiConfig::_ssidFieldName] | _defaultSsid;
    password = (*pDoc)[WifiConfig::_passwordFieldName] | _defaultPassword;
  } else {
    DEBUG_MSG(F("JSON document is NULL, applying defaults"));
  }
  // Always notify last as we don't know what will happen
  // in the onChange callback
  _applyConfig(ssid, password);
}

void WifiConfig::_applyConfig(const char *ssid, const char *password) {
  strlcpy(_ssid, ssid, WIFI_CONFIG_SSID_BUFFER_SIZE);
  strlcpy(_password, password, WIFI_CONFIG_PASSWORD_BUFFER_SIZE);
  // Always notify last as we don't know what will happen
  // in the onChange callback
  _onChange(_ssid, _password);
}

void WifiConfig::setConfig(const char *ssid, const char *password) {
  StaticJsonDocument<WIFI_CONFIG_JSON_DOCUMENT_SIZE> doc;
  doc[WifiConfig::_ssidFieldName] = ssid;
  doc[WifiConfig::_passwordFieldName] = password;
  DEBUG_VAL(F("JSON document created"), F("field count"), doc.size());
  _configFile.write(&doc);
  // Always notify last as we don't know what will happen
  // in the onChange callback
  _applyConfig(ssid, password);
}

char *WifiConfig::getSsid() {
  return _ssid;
}

char *WifiConfig::getPassword() {
  return _password;
}

void WifiConfig::reset() {
  _configFile.remove();
  // Always notify last as we don't know what will happen
  // in the onChange callback
  _applyConfig(_defaultSsid, _defaultPassword);
}
