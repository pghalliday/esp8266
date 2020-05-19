#include <Arduino.h>
#include <LittleFS.h>
#include "Storage.h"

/*
 * Uncomment the next line to enable
 * debug output to serial for this file
 */
//#define DEBUG
#include "debug.h"

Storage *Storage::getInstance() {
  static Storage storage;
  return &storage;
}

void Storage::setup() {
  LittleFS.begin();
}

File Storage::open(String path, const char *mode) {
  return LittleFS.open(path, mode);
}

bool Storage::exists(String path) {
  return LittleFS.exists(path);
}

bool Storage::remove(String path) {
  return LittleFS.remove(path);
}
