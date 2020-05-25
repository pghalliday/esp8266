/*
   WIFI_HOSTNAME
   WIFI_SSID
   WIFI_PASS
   AP_SSID
   AP_PASS
*/
#include "secrets.h"

#include "Network.hpp"
#include "Button.hpp"

Network network;
Network::StationConfig stationConfig;
Network::ApConfig apConfig;

#define BAUDRATE 115200
#define DEBOUNCE_DELAY 50
#define LONG_DELAY 2000
#define RESET_DELAY 1000
#define FALLBACK_TIMEOUT 20000
#define FORCED_AP_TIMEOUT 30000

Button apButton(
  D2,
  DEBOUNCE_DELAY,
  []() {
    network.forceAp(FORCED_AP_TIMEOUT);
  },
  LONG_DELAY,
  []() {
    Serial.printf("Resetting in %dms...\n", RESET_DELAY);
    delay(RESET_DELAY);
    ESP.reset();
  }
);
Button scanButton(D3, DEBOUNCE_DELAY, std::bind(&Network::scan, &network));
Button wpsButton(D4, DEBOUNCE_DELAY, std::bind(&Network::wpsConfig, &network));

void setup() {
  Serial.begin(BAUDRATE);

  apButton.setup();
  scanButton.setup();
  wpsButton.setup();

  stationConfig.hostname = WIFI_HOSTNAME;
  stationConfig.ssid = WIFI_SSID;
  stationConfig.passphrase = WIFI_PASS;
  /* apConfig.fallbackMs = FALLBACK_TIMEOUT; */
  apConfig.ssid = AP_SSID;
  apConfig.passphrase = AP_PASS;
  network.setup(stationConfig, apConfig);
}

void loop() {
  apButton.loop();
  scanButton.loop();
  wpsButton.loop();
  network.loop();
}
