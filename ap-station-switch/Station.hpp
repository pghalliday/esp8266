#ifndef Station_hpp
#define Station_hpp

#include <Arduino.h>
#include <functional>
#include <ESP8266WiFi.h>

class Station {
  public:
    enum Result {
      SUCCESS
    };

    using f_callback = std::function<void(Result result)>;

    Station(
      char * ssid,
      char * password
    ) :
      _ssid(ssid),
      _password(password) {
      _mode = WiFi.getMode();
      _status = WiFi.status();
    }

    static const __FlashStringHelper * resultString(Result result) {
      switch (result) {
        case Result::SUCCESS:
          return F("SUCCESS");
        default:
          return F("UNKNOWN");
      }
    }

    void setup() {
      _printDiag("Setup");
    }

    void loop() {
      WiFiMode_t mode = WiFi.getMode();
      if (mode != _mode) {
        _mode = mode;
        _printDiag("mode changed");
      }
      wl_status_t status = WiFi.status();
      if (status != _status) {
        _status = status;
        _printDiag("Status changed");
        switch (_status) {
          case WL_NO_SHIELD:
            break;
          case WL_IDLE_STATUS:
            break;
          case WL_NO_SSID_AVAIL:
            break;
          case WL_SCAN_COMPLETED:
            break;
          case WL_CONNECTED:
            break;
          case WL_CONNECT_FAILED:
            break;
          case WL_CONNECTION_LOST:
            break;
          case WL_DISCONNECTED:
            break;
          default:
            break;
        } 
      }
    }

    bool connect(f_callback callback) {
      Serial.println("Station::connect");
      bool result = WiFi.begin(_ssid, _password);
      Serial.printf("WiFi.begin: [%s]\n", result ? "Success" : "Failed");
      return result;
    }

    bool wpsConnect(f_callback callback) {
      Serial.println("Station::wpsConnect");
      // WiFi.beginWPSConfig blocks until it succeeds or times out
      // However it always seems to return success. On failure the
      // Wifi.status() will eventually (not immediately) be set to
      // WL_IDLE_STATUS
      // Also WPS will not work if AP mode is on (even if no AP started)
      WiFi.mode(WIFI_STA);
      bool result = WiFi.beginWPSConfig();
      Serial.printf("Wifi.beginWPSConfig: [%s]\n", result ? "Success" : "Failed");
      Serial.printf("ssid: [%s]: password: [%s]\n", WiFi.SSID().c_str(), WiFi.psk().c_str());
      return result;
    }

    bool disconnect(f_callback callback) {
      Serial.println("Station::disconnect");
      return false;
    }

    bool scan(f_callback callback) {
      Serial.println("Station::scan");
      // WiFi.scan cancels calls to WiFi.begin that have not connected
      WiFi.scanNetworksAsync([=](int networksFound) {
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
      }, false);
      return true;
    }

  private:
    wl_status_t _status;
    WiFiMode_t _mode;
    char * _ssid;
    char * _password;

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

    void _printDiag(char * msg) {
      Serial.println();
      Serial.println("****************************");
      Serial.println(msg);
      Serial.println("****************************");
      Serial.printf("ssid: [%s]\n", _ssid);
      Serial.printf("password: [%s]\n", _password);
      Serial.printf("WiFi status: [%s]\n", Station::_statusString(_status));
      Serial.println("****************************");
      WiFi.printDiag(Serial);
      Serial.println("****************************");
      Serial.println();
    }
};

#endif
