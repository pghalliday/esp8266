#include "WebServer.hpp"
#include "Ap.hpp"
#include "Station.hpp"
#include "Button.hpp"
#include <functional>

WebServer webServer(80);
Ap ap;
Station station;

Button stationButton(D2, 50, std::bind(&Station::toggle, &station));
Button apButton(D3, 50, std::bind(&Ap::toggle, &ap));
Button scanButton(D4, 50, std::bind(&Station::scan, &station));

void setup() {
  Serial.begin(115200);
  Serial.println("Turn off persistent WiFi settings");
  WiFi.persistent(false);
  webServer.setup();
  ap.setup();
  station.setup();
  stationButton.setup();
  apButton.setup();
  scanButton.setup();
}

void loop() {
  webServer.loop();
  ap.loop();
  station.loop();
  stationButton.loop();
  apButton.loop();
  scanButton.loop();
}
