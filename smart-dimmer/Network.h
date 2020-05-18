#ifndef Network_h
#define Network_h

#include <Arduino.h>
#include <functional>
#include "WifiConfig.h"

#define NETWORK_MODE_AP 1
#define NETWORK_MODE_STATION 2
#define NETWORK_MODE_BOTH 3

#define NETWORK_CONFIG_SSID_BUFFER_SIZE 33
#define NETWORK_CONFIG_PASSWORD_BUFFER_SIZE 64

#define NETWORK_CONFIG_FIELDS_LENGTH 2

class Network {
  using f_onStateChange = std::function<void(int state)>;

  public:
    static Network *getInstance();
    void setup(f_onStateChange onStateChange);
    void loop();
    int getState();
    void setMode(int mode);
    int getMode();
    void setStationConfig(char *ssid, char *password);
    void resetStationConfig();
    char *getStationSsid();
    char *getStationPassword();
    void setApConfig(char *ssid, char *password);
    void resetApConfig();
    char *getApSsid();
    char *getApPassword();
    void scan();
  private:
    Network(){};
    Network(Network const&){};
    Network& operator=(Network const&){};
    static const char *_stationConfigFilePath;
    static const char *_apConfigFilePath;
    static const char *_defaultStationSsid;
    static const char *_defaultStationPassword;
    static const char *_defaultApSsid;
    static const char *_defaultApPassword;
    f_onStateChange _onStateChange;
    WifiConfig _stationConfig;
    void _onStationConfigChange(const char *ssid, const char *password);
    WifiConfig _apConfig;
    void _onApConfigChange(const char *ssid, const char *password);
    int _mode;
};

#endif
