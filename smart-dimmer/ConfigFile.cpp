#include "Arduino.h"
#include "ArduinoJson.h"
#include "LittleFS.h"
#include "ConfigFile.h"

void ConfigFile::init(const char *path, JsonDocument *pDoc) {
  _path = path;

  if (LittleFS.exists(_path)) {
    return _read(pDoc);
  }

  Serial.print(F("ConfigFile::init - File does not exist ["));
  Serial.print(_path);
  Serial.println(F("]"));
}

void ConfigFile::_read(JsonDocument *pDoc) {
  File file = LittleFS.open(_path, "r");
  if (file) {
    DeserializationError error = deserializeJson(*pDoc, file);
    if (error) {
      Serial.print(F("ConfigFile::read - Failed to deserialize file ["));
      Serial.print(_path);
      Serial.print(F("], error: ["));
      Serial.print(error.c_str());
      Serial.println(F("]"));
    }
    file.close();
    return;
  }

  Serial.print(F("ConfigFile::_read - Failed to open file ["));
  Serial.print(_path);
  Serial.println(F("]"));
  return;
}

void ConfigFile::write(JsonDocument *pDoc) {
  File file = LittleFS.open(_path, "w");
  if (file) {
    if (serializeJson(*pDoc, file) == 0) {
      Serial.print(F("ConfigFile::write - Failed to write file ["));
      Serial.print(_path);
      Serial.println(F("]"));
    }
    file.close();
  } else {
    Serial.print(F("ConfigFile::write - Failed to open file ["));
    Serial.print(_path);
    Serial.println(F("]"));
  }
}

void ConfigFile::remove() {
  if (!LittleFS.remove(_path)) {
    Serial.print(F("ConfigFile::remove - Failed to remove file ["));
    Serial.print(_path);
    Serial.println(F("]"));
  }
}
