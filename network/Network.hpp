#ifndef Network_hpp
#define Network_hpp

#include <ESP8266WiFi.h>

#include "Debug.hpp"

class Network {
  public:
    struct StationIpConfig {
      IPAddress localIp;
      IPAddress gateway;
      IPAddress subnet;
      IPAddress dns1 = (uint32_t)0x00000000;
      IPAddress dns2 = (uint32_t)0x00000000;
    };

    struct StationConfig {
      const char * hostname;
      const char * ssid;
      const char * passphrase = NULL;
      int32_t channel = 0;
      uint8_t * bssid = NULL;
      StationIpConfig * ipConfig = NULL;
    };

    struct ApIpConfig {
      IPAddress localIp;
      IPAddress gateway;
      IPAddress subnet;
    };

    struct ApConfig {
      unsigned long fallbackMs = 0;
      const char * ssid;
      const char * passphrase = NULL;
      int channel = 1;
      int ssidHidden = 0;
      int maxConnections = 4;
      ApIpConfig * ipConfig;
    };

    Network() {
    }

    void setup(StationConfig stationConfig, ApConfig apConfig) {
      // We want to manage the status
      // ourselves so on start we:
      // - set persistent off
      // - set auto connect off
      // - set auto reconnect off
      WiFi.persistent(false);
      WiFi.setAutoConnect(false);
      WiFi.setAutoReconnect(true);

      _status = WiFi.status();
      DEBUG_PRINT("Initial status: [%s]", Network::_statusString(_status));
      setApConfig(apConfig);
      setStationConfig(stationConfig);
    }

    void loop() {
      wl_status_t status = WiFi.status();
      if (status != _status) {
        // WIFI Status changed
        _status = status;
        DEBUG_PRINT("Status changed: [%s]", Network::_statusString(_status));
        switch (_status) {
          case WL_IDLE_STATUS:
            _checkIdle();
            break;
          case WL_DISCONNECTED:
            break;
          case WL_CONNECT_FAILED:
          case WL_NO_SSID_AVAIL:
            _startFallback();
            break;
          case WL_CONNECTED:
            _endFallback();
            break;
          default:
            break;
        }
      }
      _checkForcedAp();
      _checkFallback();
    }

    void setStationConfig(StationConfig config) {
      // first disconnect to clear any saved configuration
      bool result = WiFi.disconnect(true);
      DEBUG_PRINT("WiFi.disconnect: [%s]", result ? "SUCCESS" : "FAIL");
      _stationConfig = config;
      // We changed the station config
      // so apply immediately and stop
      // any forced AP
      stopForcedAp();
    }

    void setApConfig(ApConfig config) {
      // first disconnect to clear any saved configuration
      bool result = WiFi.softAPdisconnect(true);
      DEBUG_PRINT("WiFi.softAPdisconnect: [%s]", result ? "SUCCESS" : "FAIL");
      _apConfig = config;
      // restart AP if we are in fallback or forced AP mode
      if (_fallbackActive || _forcedApStart) {
        _apBegin();
      }
    }

    void forceAp(unsigned long timeout) {
      bool result = WiFi.disconnect(true);
      DEBUG_PRINT("WiFi.disconnect: [%s]", result ? "SUCCESS" : "FAIL");
      result = WiFi.softAPdisconnect(true);
      DEBUG_PRINT("WiFi.softAPdisconnect: [%s]", result ? "SUCCESS" : "FAIL");
      _forcedApStart = millis();
      _forcedApTimeout = timeout;
      _apBegin();
    }

    void stopForcedAp() {
      _forcedApStart = 0;
      _stationBegin();
    }

    void wpsConfig() {
      // first disconnect everything to clear any saved
      // configuration and ensure the correct mode is set
      // (WPS won't work with AP mode on)
      bool result = WiFi.disconnect(true);
      DEBUG_PRINT("WiFi.disconnect: [%s]", result ? "SUCCESS" : "FAIL");
      _endFallback();
      // and stop any forced AP
      _forcedApStart = 0;
      result = WiFi.beginWPSConfig();
      DEBUG_PRINT("Wifi.beginWPSConfig: [%s]", result ? "Success" : "Failed");
      DEBUG_PRINT("ssid: [%s]: password: [%s]", WiFi.SSID().c_str(), WiFi.psk().c_str());
      if (WiFi.SSID().length() > 0) {
        // Now create a new StationConfig with these parameters
        DEBUG_PRINT("TODO: store new configuration from WPS!!");
      }
    }

    void scan() {
      // WiFi.scan cancels calls to WiFi.begin that have
      // not connected we can tell as we will get
      // a WL_IDLE_STATUS
      _scanning = true;
      WiFi.scanNetworksAsync([=](int networksFound) {
        _scanning = false;
        Serial.printf("Scan complete, found %d networks\n", networksFound);
        for (int i = 0; i < networksFound; i++) {
          Serial.printf(
            "%d: %s, Ch:%d (%ddBm) %s\n",
            i + 1,
            WiFi.SSID(i).c_str(),
            WiFi.channel(i),
            WiFi.RSSI(i),
            WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : ""
          );
        }
        WiFi.scanDelete();
        if (_restartAfterScan) {
          _restartAfterScan = false;
          _stationBegin();
        }
      }, false);
    }

  private:
    wl_status_t _status;
    StationConfig _stationConfig;
    ApConfig _apConfig;
    unsigned long _fallbackStart = 0;
    bool _fallbackActive = false;
    bool _scanning = false;
    bool _restartAfterScan = false;
    unsigned long _forcedApTimeout = 0;
    unsigned long _forcedApStart = 0;

    static const __FlashStringHelper * _statusString(wl_status_t status) {
      switch (status) {
        case WL_NO_SHIELD:
          return F("WL_NO_SHIELD");
        case WL_IDLE_STATUS:
          return F("WL_IDLE_STATUS");
        case WL_NO_SSID_AVAIL:
          return F("WL_NO_SSID_AVAIL");
        case WL_SCAN_COMPLETED:
          return F("WL_SCAN_COMPLETED");
        case WL_CONNECTED:
          return F("WL_CONNECTED");
        case WL_CONNECT_FAILED:
          return F("WL_CONNECT_FAILED");
        case WL_CONNECTION_LOST:
          return F("WL_CONNECTION_LOST");
        case WL_DISCONNECTED:
          return F("WL_DISCONNECTED");
        default:
          return F("UNKNOWN");
      } 
    }

    void _stationBegin() {
      // only start the station if not in forced AP mode
      if (!_forcedApStart) {
        // only start the station if a ssid is set
        if (strlen(_stationConfig.ssid)) {
          DEBUG_PRINT("Starting the station with ssid [%s]", _stationConfig.ssid);
          // set the hostname
          WiFi.hostname(_stationConfig.hostname);
          // if IP config is supplied then set that first
          if (_stationConfig.ipConfig) {
            bool result = WiFi.config(
              _stationConfig.ipConfig->localIp,
              _stationConfig.ipConfig->gateway,
              _stationConfig.ipConfig->subnet,
              _stationConfig.ipConfig->dns1,
              _stationConfig.ipConfig->dns2
            );
            DEBUG_PRINT("WiFi.config: [%s]", result ? "SUCCESS" : "FAIL");
          }
          bool result  = WiFi.begin(
            _stationConfig.ssid,
            _stationConfig.passphrase,
            _stationConfig.channel,
            _stationConfig.bssid
          );
          DEBUG_PRINT("WiFi.begin: [%s]", result ? "SUCCESS" : "FAIL");
        } else {
          _fallback();
        }
      }
    }

    void _apBegin() {
      // only start the AP if a ssid is set
      if (strlen(_apConfig.ssid)) {
        DEBUG_PRINT("Starting the AP with ssid [%s]", _apConfig.ssid);
        // if IP config is supplied then set that first
        if (_apConfig.ipConfig) {
          bool result = WiFi.softAPConfig(
            _apConfig.ipConfig->localIp,
            _apConfig.ipConfig->gateway,
            _apConfig.ipConfig->subnet
          );
          DEBUG_PRINT("WiFi.softAPConfig: [%s]", result ? "SUCCESS" : "FAIL");
        }
        bool result = WiFi.softAP(
          _apConfig.ssid,
          _apConfig.passphrase,
          _apConfig.channel,
          _apConfig.ssidHidden,
          _apConfig.maxConnections
        );
        DEBUG_PRINT("WiFi.softAP: [%s]", result ? "SUCCESS" : "FAIL");
      }
    }

    void _startFallback() {
      // Don't use fallback if the timeout is set to 0
      // Will still fallback if station SSID is blank
      if (_apConfig.fallbackMs > 0) {
        // Don't use fallback if forced AP is active 
        if (!_forcedApStart) {
          if (!_fallbackActive) {
            if (!_fallbackStart) {
              _fallbackStart = millis();
            }
          }
        }
      }
    }

    void _checkFallback() {
      if (!_fallbackActive) {
        if (_fallbackStart) {
          if ((millis() - _fallbackStart) > _apConfig.fallbackMs) {
            _fallbackStart = 0;
            _fallback();
          }
        }
      }
    }

    void _fallback() {
      if (!_fallbackActive) {
        _fallbackActive = true;
        _apBegin();
      }
    }

    void _endFallback() {
      _fallbackStart = 0;
      _fallbackActive = false;
      bool result = WiFi.softAPdisconnect(true);
      DEBUG_PRINT("WiFi.softAPdisconnect: [%s]", result ? "SUCCESS" : "FAIL");
    }

    void _checkForcedAp() {
      if (_forcedApStart) {
        // Only timeout forced AP if timeout is > 0
        // otherwise it must be stopped explicitly
        if (_forcedApTimeout) {
          if ((millis() - _forcedApStart) > _forcedApTimeout) {
            stopForcedAp();
          }
        }
      }
    }

    void _checkIdle() {
      if (_scanning) {
        _restartAfterScan = true;
      } else {
        _stationBegin();
      }
    }
};

#endif
