#include "Arduino.h"
#include "Network.h"

const char *Network::_stationConfigFilePath = "/network/station.json";
const char *Network::_apConfigFilePath = "/network/ap.json";
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
  _stationConfig.init(
      Network::_stationConfigFilePath,
      Network::_defaultStationSsid,
      Network::_defaultStationPassword);
  _apConfig.init(
      Network::_apConfigFilePath,
      Network::_defaultApSsid,
      Network::_defaultApPassword);
  // TODO: Connect or start AP using setMode
}

void Network::loop() {
  // TODO: Check status and notify state changes
}

int Network::getState() {
  // TODO
}

void Network::setMode(int mode) {
  _mode = mode;
  // TODO
}

int Network::getMode() {
  return _mode;
}

void Network::setStationConfig(char *ssid, char *password) {
  _stationConfig.setConfig(ssid, password);
  // TODO: connect
}

void Network::resetStationConfig() {
  _stationConfig.reset();
  // TODO: disconnect and start AP?
}

char *Network::getStationSsid() {
  return _stationConfig.getSsid();
}

char *Network::getStationPassword() {
  return _stationConfig.getPassword();
}

void Network::setApConfig(char *ssid, char *password) {
  _apConfig.setConfig(ssid, password);
  // TODO: connect
}

void Network::resetApConfig() {
  _apConfig.reset();
  // TODO: restart AP with defaults?
}

char *Network::getApSsid() {
  return _apConfig.getSsid();
}

char *Network::getApPassword() {
  return _apConfig.getPassword();
}

void Network::scan() {
  // TODO
}
