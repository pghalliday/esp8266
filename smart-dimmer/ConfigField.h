#ifndef ConfigField_h
#define ConfigField_h

#include "Arduino.h"

#define CONFIG_FIELD_TYPE_PRIMITIVE 1
#define CONFIG_FIELD_TYPE_STRING 2

class ConfigField {
  public:
    ConfigField();
    void init(const char *name, void *value, const void *defaultValue, int type, unsigned int size);
  private:
    const char *_name;
    void *_value;
    const void *_defaultValue;
    int _type;
    unsigned int _size;
};

#endif
