#include "Arduino.h"
#include "Config.h"
#include "Network.h"

const char *Network::_stationConfigFilePath = "/network/station.json";
const char *Network::_apConfigFilePath = "/network/ap.json";
const char *Network::_ssidFieldName = "ssid";
const char *Network::_passwordFieldName = "password";
const char *Network::_defaultStationSsid = "";
const char *Network::_defaultStationPassword = "";
const char *Network::_defaultApSsid = "smart-dimmer";
const char *Network::_defaultApPassword = "12345678";

Network::Network(f_onStateChange onStateChange) {
  _onStateChange = onStateChange;
  _stationConfigFields[0].init(
      Network::_ssidFieldName,
      _stationConfig.ssid,
      Network::_defaultStationSsid,
      CONFIG_FIELD_TYPE_STRING,
      NETWORK_CONFIG_SSID_BUFFER_SIZE
      );
  _stationConfigFields[1].init(
      Network::_passwordFieldName,
      _stationConfig.password,
      Network::_defaultStationPassword,
      CONFIG_FIELD_TYPE_STRING,
      NETWORK_CONFIG_PASSWORD_BUFFER_SIZE
      );
  _stationConfigFile.init(
      Network::_stationConfigFilePath,
      _stationConfigFields,
      NETWORK_CONFIG_FIELDS_LENGTH
      );
  _apConfigFields[0].init(
      Network::_ssidFieldName,
      _apConfig.ssid,
      Network::_defaultApSsid,
      CONFIG_FIELD_TYPE_STRING,
      NETWORK_CONFIG_SSID_BUFFER_SIZE
      );
  _apConfigFields[1].init(
      Network::_passwordFieldName,
      _apConfig.password,
      Network::_defaultApPassword,
      CONFIG_FIELD_TYPE_STRING,
      NETWORK_CONFIG_PASSWORD_BUFFER_SIZE
      );
  _apConfigFile.init(
      Network::_apConfigFilePath,
      _apConfigFields,
      NETWORK_CONFIG_FIELDS_LENGTH
      );
}

void Network::setup() {
  _stationConfigFile.read();
  _apConfigFile.read();
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
  strlcpy(_stationConfig.ssid, ssid, NETWORK_CONFIG_SSID_BUFFER_SIZE);
  strlcpy(_stationConfig.password, password, NETWORK_CONFIG_PASSWORD_BUFFER_SIZE);
  _stationConfigFile.write();
}

void Network::resetStationConfig() {
  _stationConfigFile.reset();
}

char *Network::getStationSsid() {
  return _stationConfig.ssid;
}

char *Network::getStationPassword() {
  return _stationConfig.password;
}

void Network::setApConfig(char *ssid, char *password) {
  strlcpy(_apConfig.ssid, ssid, NETWORK_CONFIG_SSID_BUFFER_SIZE);
  strlcpy(_apConfig.password, password, NETWORK_CONFIG_PASSWORD_BUFFER_SIZE);
  _apConfigFile.write();
}

void Network::resetApConfig() {
  _apConfigFile.reset();
}

char *Network::getApSsid() {
  return _apConfig.ssid;
}

char *Network::getApPassword() {
  return _apConfig.password;
}

void Network::scan() {
  // TODO
}
