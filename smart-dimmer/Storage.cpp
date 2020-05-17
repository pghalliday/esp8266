#include "Arduino.h"
#include "LittleFS.h"
#include "Storage.h"

Storage::Storage() {
}

void Storage::setup() {
  LittleFS.begin();
}
