#ifndef Config_h
#define Config_h

#include "Arduino.h"
#include "ConfigField.h"

class Config {
  public:
    Config();
    void init(const char *path, ConfigField *fields, unsigned int fieldsLength);
    void read();
    void write();
    void reset();
  private:
    const char *_path;
    ConfigField *_fields;
    unsigned int _fieldsLength;
};

#endif
