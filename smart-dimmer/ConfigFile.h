#ifndef ConfigFile_h
#define ConfigFile_h

#include <Arduino.h>
#include <functional>
#include <ArduinoJson.h>
#include "Storage.h"

class ConfigFile {
  public:
    ConfigFile(){};
    void init(const char *path, JsonDocument *pDoc);
    void write(JsonDocument *pDoc);
    void remove();
  private:
    void _read(JsonDocument *pDoc);
    const char *_path;
    Storage *_pStorage;
};

#endif
