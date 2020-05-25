#include "WebServer.hpp"
/* #include "Ap.hpp" */
#include "Station.hpp"
#include "Button.hpp"
#include <functional>

#define DEBOUNCE_DELAY 50
#define LONG_DELAY 2000
#define RESET_DELAY 1000

/*
#define WIFI_SSID
#define WIFI_PASS
#define AP_SSID
#define AP_PASS
*/
#include "secrets.h"

WebServer webServer(80);
/* Ap ap; */
Station station(WIFI_SSID, WIFI_PASS);

#define CALLBACK(CLASS, INSTANCE, METHOD) []() {\
  Serial.printf(\
    "%s.%s: [%s]\n",\
    #INSTANCE,\
    #METHOD,\
    INSTANCE.METHOD([](CLASS::Result result) {\
      Serial.printf(\
        "%s.%s result: [%s]\n",\
        #INSTANCE,\
        #METHOD,\
        CLASS::resultString(result)\
      );\
    }) ? "SUCCESS" : "FAIL"\
  );\
}

Button stationButton(
  D2,
  DEBOUNCE_DELAY,
  CALLBACK(Station, station, connect),
  LONG_DELAY,
  CALLBACK(Station, station, wpsConnect)
);

/* Button apButton(D3, DEBOUNCE_DELAY, std::bind(&Ap::toggle, &ap), LONG_DELAY, [](){ Serial.println("ap long"); }); */

Button scanButton(
  D4,
  DEBOUNCE_DELAY,
  CALLBACK(Station, station, scan),
  LONG_DELAY,
  [](){
    Serial.printf("Resetting in %dms...", RESET_DELAY);
    delay(RESET_DELAY);
    ESP.reset();
  }
);

void setup() {
  Serial.begin(115200);
  /* Serial.println("Turn off persistent WiFi settings"); */
  /* WiFi.persistent(false); */
  webServer.setup();
  /* ap.setup(); */
  station.setup();
  stationButton.setup();
  /* apButton.setup(); */
  scanButton.setup();
}

void loop() {
  webServer.loop();
  /* ap.loop(); */
  station.loop();
  stationButton.loop();
  /* apButton.loop(); */
  scanButton.loop();
}
