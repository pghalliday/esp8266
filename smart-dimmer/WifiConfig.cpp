#include "Arduino.h"
#include "ArduinoJson.h"
#include "LittleFS.h"
#include "WifiConfig.h"

#define WIFI_CONFIG_FIELD_COUNT 2
const char *WifiConfig::_ssidFieldName = "ssid";
const char *WifiConfig::_passwordFieldName = "password";

void WifiConfig::init(f_onChange onChange, const char *path, const char *defaultSsid, const char *defaultPassword) {
  _onChange = onChange;
  _defaultSsid = defaultSsid;
  _defaultPassword = defaultPassword;
  const size_t capacity =
    WIFI_CONFIG_FIELD_COUNT +
    sizeof(WifiConfig::_ssidFieldName) +
    WIFI_CONFIG_SSID_BUFFER_SIZE +
    sizeof(WifiConfig::_passwordFieldName) +
    WIFI_CONFIG_PASSWORD_BUFFER_SIZE;
  Serial.print("WifiConfig::init - JSON capacity: ");
  Serial.println(capacity);
  StaticJsonDocument<capacity> doc;
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
    Serial.println("WifiConfig::_applyDoc - JSON document is NULL, applying defaults");
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
  const size_t capacity = WIFI_CONFIG_FIELD_COUNT;
  Serial.print("WifiConfig::setConfig -  JSON capacity: ");
  Serial.println(capacity);
  StaticJsonDocument<capacity> doc;
  doc[WifiConfig::_ssidFieldName] = ssid;
  doc[WifiConfig::_passwordFieldName] = password;
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
