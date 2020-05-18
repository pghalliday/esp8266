#ifndef WifiConfig_h
#define WifiConfig_h

#include <Arduino.h>
#include <functional>
#include "ConfigFile.h"

#define WIFI_CONFIG_SSID_BUFFER_SIZE 33
#define WIFI_CONFIG_PASSWORD_BUFFER_SIZE 64

class WifiConfig {
  using f_onChange = std::function<void(const char *ssid, const char *password)>;

  public:
    WifiConfig(){};
    void init(f_onChange onChange, const char *path, const char *defaultSsid, const char *defaultPassword);
    void setConfig(const char *ssid, const char *password);
    char *getSsid();
    char *getPassword();
    void reset();
  private:
    f_onChange _onChange;
    ConfigFile _configFile;
    void _read();
    void _applyDoc(JsonDocument *pDoc);
    void _applyConfig(const char *ssid, const char *password);
    const char *_defaultSsid;
    const char *_defaultPassword;
    char _ssid[WIFI_CONFIG_SSID_BUFFER_SIZE];
    char _password[WIFI_CONFIG_PASSWORD_BUFFER_SIZE];
    static const char _ssidFieldName[];
    static const char _passwordFieldName[];
};

#endif
