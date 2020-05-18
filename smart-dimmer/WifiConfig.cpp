#include "Arduino.h"
#include "ArduinoJson.h"
#include "LittleFS.h"
#include "WifiConfig.h"

const char *WifiConfig::_ssidFieldName = "ssid";
const char *WifiConfig::_passwordFieldName = "password";

void WifiConfig::init(const char *path, const char *defaultSsid, const char *defaultPassword) {
  _path = path;
  _defaultSsid = defaultSsid;
  _defaultPassword = defaultPassword;

  if (LittleFS.exists(_path)) {
    _read();
  } else {
    Serial.print(F("WifiConfig::init - File does not exist ["));
    Serial.print(_path);
    Serial.println(F("], using default configuration"));
    _applyDefaults();
  }
}

void WifiConfig::_read() {
  File file = LittleFS.open(_path, "r");
  if (file) {
    const size_t capacity =
      JSON_OBJECT_SIZE(2) + // Object with 2 fields
      sizeof(WifiConfig::_ssidFieldName) + // ssid field name
      WIFI_CONFIG_SSID_BUFFER_SIZE + // ssid value
      sizeof(WifiConfig::_passwordFieldName) + // password field name
      WIFI_CONFIG_PASSWORD_BUFFER_SIZE; // password value
    Serial.print("WifiConfig::read -  JSON capacity: ");
    Serial.println(capacity);

    StaticJsonDocument<capacity> doc;
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
      Serial.print(F("WifiConfig::read - Failed to deserialize file ["));
      Serial.print(_path);
      Serial.print(F("], error: ["));
      Serial.print(error.c_str());
      Serial.println(F("], using default configuration"));
      _applyDefaults();
    } else {
      strlcpy(_ssid, doc[WifiConfig::_ssidFieldName] | _defaultSsid, WIFI_CONFIG_SSID_BUFFER_SIZE);
      strlcpy(_password, doc[WifiConfig::_passwordFieldName] | _defaultPassword, WIFI_CONFIG_PASSWORD_BUFFER_SIZE);
    }
    file.close();
  } else {
    Serial.print(F("WifiConfig::_read - Failed to open file ["));
    Serial.print(_path);
    Serial.println(F("], using default configuration"));
    _applyDefaults();
  }
}

void WifiConfig::_applyDefaults() {
  strlcpy(_ssid, _defaultSsid, WIFI_CONFIG_SSID_BUFFER_SIZE);
  strlcpy(_password, _defaultPassword, WIFI_CONFIG_PASSWORD_BUFFER_SIZE);
}

void WifiConfig::setConfig(const char *ssid, const char *password) {
  strlcpy(_ssid, ssid, WIFI_CONFIG_SSID_BUFFER_SIZE);
  strlcpy(_password, password, WIFI_CONFIG_PASSWORD_BUFFER_SIZE);
  File file = LittleFS.open(_path, "w");
  if (file) {
    const size_t capacity =
      JSON_OBJECT_SIZE(2); // Object with 2 fields
    Serial.print("WifiConfig::setConfig -  JSON capacity: ");
    Serial.println(capacity);
    StaticJsonDocument<capacity> doc;
    doc[WifiConfig::_ssidFieldName] = _ssid;
    doc[WifiConfig::_passwordFieldName] = _password;
    if (serializeJson(doc, file) == 0) {
      Serial.print(F("WifiConfig::setConfig - Failed to write file ["));
      Serial.print(_path);
      Serial.println(F("]"));
    }
    file.close();
  } else {
    Serial.print(F("WifiConfig::setConfig - Failed to open file ["));
    Serial.print(_path);
    Serial.println(F("]"));
  }
}

char *WifiConfig::getSsid() {
  return _ssid;
}

char *WifiConfig::getPassword() {
  return _password;
}

void WifiConfig::reset() {
  if (!LittleFS.remove(_path)) {
    Serial.print(F("WifiConfig::reset - Failed to remove file ["));
    Serial.print(_path);
    Serial.println(F("]"));
  }
  _applyDefaults();
}
