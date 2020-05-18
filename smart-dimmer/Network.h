#ifndef Network_h
#define Network_h

#include <Arduino.h>
#include <functional>
#include "WifiConfig.h"

class Network {
  public:
    enum class Mode {
      AP,
      STATION,
      BOTH
    };

    enum class State {
      IDLE,
      AP_ACTIVE
    };

  using f_onStateChange = std::function<void(State state)>;

  public:
    static Network *getInstance();
    void setup(f_onStateChange onStateChange);
    void loop();
    State getState();
    void setMode(Network::Mode);
    Mode getMode();
    void setStationConfig(const char *ssid, const char *password);
    void resetStationConfig();
    char *getStationSsid();
    char *getStationPassword();
    void setApConfig(const char *ssid, const char *password);
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
    Mode _mode;
    State _state;
    void _setState(State state);
};

#endif
