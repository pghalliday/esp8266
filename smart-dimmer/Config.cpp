#include "Arduino.h"
#include "Config.h"

Config::Config() {
}

void Config::init(const char *path, ConfigField *fields, unsigned int fieldsLength) {
  _path = path;
  _fields = fields;
  _fieldsLength = fieldsLength;
}

void Config::read() {
  // TODO
}

void Config::write() {
  // TODO
}

void Config::reset() {
  // TODO
}
