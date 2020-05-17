#ifndef Network_h
#define Network_h

#include "Arduino.h"
#include "Config.h"

#define NETWORK_MODE_AP 1
#define NETWORK_MODE_STATION 2
#define NETWORK_MODE_BOTH 3

#define NETWORK_CONFIG_SSID_BUFFER_SIZE 33
#define NETWORK_CONFIG_PASSWORD_BUFFER_SIZE 64

#define NETWORK_CONFIG_FIELDS_LENGTH 2

class Network {
  using f_onStateChange = void(*)(int state);
  using t_config = struct {
    char ssid[NETWORK_CONFIG_SSID_BUFFER_SIZE];
    char password[NETWORK_CONFIG_PASSWORD_BUFFER_SIZE];
  };

  public:
    Network(f_onStateChange onStateChange);
    void setup();
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
    static const char *_stationConfigFilePath;
    static const char *_apConfigFilePath;
    static const char *_ssidFieldName;
    static const char *_passwordFieldName;
    static const char *_defaultStationSsid;
    static const char *_defaultStationPassword;
    static const char *_defaultApSsid;
    static const char *_defaultApPassword;
    f_onStateChange _onStateChange;
    t_config _stationConfig;
    ConfigField _stationConfigFields[NETWORK_CONFIG_FIELDS_LENGTH];
    Config _stationConfigFile;
    t_config _apConfig;
    ConfigField _apConfigFields[NETWORK_CONFIG_FIELDS_LENGTH];
    int _mode;
    Config _apConfigFile;
};

#endif
