#ifndef Storage_h
#define Storage_h

#include <Arduino.h>
#include <LittleFS.h>

class Storage {
  public:
    static Storage *getInstance();
    void setup();
    File open(String path, const char *mode);
    bool exists(String path);
    bool remove(String path);
  private:
    Storage(){};
    Storage(Storage const&){};
    Storage& operator=(Storage const&){};
};

#endif
