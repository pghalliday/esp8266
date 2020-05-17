#include "Arduino.h"
#include "ConfigField.h"

ConfigField::ConfigField() {
}

void ConfigField::init(const char *name, void *value, const void *defaultValue, int type, unsigned int size) {
  _name = name;
  _value = value;
  _defaultValue = defaultValue;
  _type = type;
  if (_type == CONFIG_FIELD_TYPE_STRING) {
    _size = size;
  }
}
