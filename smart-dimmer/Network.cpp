#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Network.h"

/*
 * Uncomment the next line to enable
 * debug output to serial for this file
 */
//#define DEBUG
#include "debug.h"

const char *Network::_stationConfigFilePath = "/config/network/station.json";
const char *Network::_apConfigFilePath = "/config/network/ap.json";

const char *Network::_defaultStationSsid = "";
const char *Network::_defaultStationPassword = "";
const char *Network::_defaultApSsid = "smart-dimmer";
const char *Network::_defaultApPassword = "12345678";

Network *Network::getInstance() {
  static Network network;
  return &network;
}

void Network::setup(f_onStateChange onStateChange) {
  _onStateChange = onStateChange;

  using namespace std::placeholders; // for _1, _2
  _stationConfig.init(
      std::bind(&Network::_onStationConfigChange, this, _1, _2),
      Network::_stationConfigFilePath,
      Network::_defaultStationSsid,
      Network::_defaultStationPassword);
  _apConfig.init(
      std::bind(&Network::_onApConfigChange, this, _1, _2),
      Network::_apConfigFilePath,
      Network::_defaultApSsid,
      Network::_defaultApPassword);

  _setState(Network::State::IDLE);
}

void Network::loop() {
  // TODO: Check status and notify state changes
  if (_state == Network::State::IDLE) {
    if (WiFi.softAP(_apConfig.getSsid(), _apConfig.getPassword())) {
      DEBUG_VAL(F("started AP"), F("IP"), WiFi.softAPIP());
      _setState(Network::State::AP_ACTIVE);
    } else {
      DEBUG_MSG(F("failed to start AP"));
    }
  }
}

Network::State Network::getState() {
  return _state;
}

void Network::_setState(Network::State state) {
  _state = state;
  _onStateChange(_state);
}

void Network::setMode(Network::Mode mode) {
  _mode = mode;
  // TODO
}

Network::Mode Network::getMode() {
  return _mode;
}

void Network::setStationConfig(const char *ssid, const char *password) {
  _stationConfig.setConfig(ssid, password);
}

void Network::resetStationConfig() {
  _stationConfig.reset();
}

char *Network::getStationSsid() {
  return _stationConfig.getSsid();
}

char *Network::getStationPassword() {
  return _stationConfig.getPassword();
}

void Network::_onStationConfigChange(const char *ssid, const char *password) {
  DEBUG_LIST_START(F("new station config"));
  DEBUG_LIST_VAL(F("ssid"), ssid);
  DEBUG_LIST_VAL(F("password"), password);
  DEBUG_LIST_END;
  // TODO: restart station?
}

void Network::setApConfig(const char *ssid, const char *password) {
  _apConfig.setConfig(ssid, password);
}

void Network::resetApConfig() {
  _apConfig.reset();
}

char *Network::getApSsid() {
  return _apConfig.getSsid();
}

char *Network::getApPassword() {
  return _apConfig.getPassword();
}

void Network::_onApConfigChange(const char *ssid, const char *password) {
  DEBUG_LIST_START(F("new AP config"));
  DEBUG_LIST_VAL(F("ssid"), ssid);
  DEBUG_LIST_VAL(F("password"), password);
  DEBUG_LIST_END;
  // TODO: restart AP?
}

void Network::scan() {
  // TODO
}
