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
  Serial.print(F("Network::_onStationConfigChange - ssid: ["));
  Serial.print(ssid);
  Serial.print(F("], password: ["));
  Serial.print(password);
  Serial.println(F("]"));
  // TODO: restart station?
}

void Network::setApConfig(char *ssid, char *password) {
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
  Serial.print(F("Network::_onApConfigChange - ssid: ["));
  Serial.print(ssid);
  Serial.print(F("], password: ["));
  Serial.print(password);
  Serial.println(F("]"));
  // TODO: restart AP?
}

void Network::scan() {
  // TODO
}
