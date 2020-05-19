#include <Arduino.h>
#include <ArduinoJson.h>
#include "Storage.h"
#include "ConfigFile.h"

/*
 * Uncomment the next line to enable
 * debug output to serial for this file
 */
//#define DEBUG
#include "debug.h"

void ConfigFile::init(const char *path, JsonDocument *pDoc) {
  _pStorage = Storage::getInstance();

  DEBUG_VAL(F("check file"), F("path"), path);
  _path = path;

  if (_pStorage->exists(_path)) {
    return _read(pDoc);
  }

  DEBUG_VAL(F("File does not exist"), F("_path"), _path);
}

void ConfigFile::_read(JsonDocument *pDoc) {
  File file = _pStorage->open(_path, "r");
  if (file) {
    DEBUG_LIST_START(F("file opened"));
    DEBUG_LIST_VAL(F("size"), file.size());
    // This call to readString moves the file stream to the end
    // when DEBUG is enabled
    DEBUG_LIST_VAL(F("contents"), file.readString());
    DEBUG_LIST_END;
    // This call to seek moves the file stream back to the beginning
    // when DEBUG is enabled so that the deserialize can work
    DEBUG_DO(file.seek(0, SeekSet));
    DeserializationError error = deserializeJson(*pDoc, file);
    if (error) {
      DEBUG_LIST_START(F("Failed to deserialize file"));
      DEBUG_LIST_VAL(F("_path"), _path);
      DEBUG_LIST_VAL(F("error"), error.c_str());
      DEBUG_LIST_END;
    }
    file.close();
    return;
  }

  DEBUG_VAL(F("Failed to open file"), F("_path"), _path);
  return;
}

void ConfigFile::write(JsonDocument *pDoc) {
  File file = _pStorage->open(_path, "w");
  DEBUG_VAL(F("opened file"), F("handle"), file);
  if (file) {
    if (serializeJson(*pDoc, file) == 0) {
      DEBUG_VAL(F("Failed to write file"), F("_path"), _path);
    }
    file.close();
  } else {
    DEBUG_VAL(F("Failed to open file"), F("_path"), _path);
  }
}

void ConfigFile::remove() {
  if (!_pStorage->remove(_path)) {
    DEBUG_VAL(F("Failed to remove file"), F("_path"), _path);
  }
}
