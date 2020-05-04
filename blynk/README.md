# blynk

## Prerequisites

- Blynk server
- Blynk app on phone

## Create a secrets file containing your network settings

Create a file at `blynk/secrets.h` with the following content

```c
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASS "YOUR_WIFI_PASSPHRASE"

#define BLYNK_AUTH "YOUR_BLYNK_AUTHENTICATION_CODE"
#define BLYNK_HOST "YOUR_BLYNK_SERVER_HOSTNAME"
#define BLYNK_PORT YOUR_BLYNK_SERVER_PORT
```

## Install the `blynk` library

Download the library from https://github.com/blynkkk/blynk-library/releases

Then add the library to your Arduino libraries manually by unzipping it and copying as follows:

- The contents of the `libraries` folder should be copied to `~/Documents/Arduino/libraries/`
- The contents of the `tools` folder should be copied to `~/Documents/Arduino/tools/`

## Pins

FTDI:

- GND - White
- CTS - Black
- VCC - Brown
- TX - Red
- RX - Orange
- DTR - Yellow

ESP8266:

- GND - Brown
- IO2 - Red
- IO0 - Orange
- RX - Yellow
- TX - Grey
- EN - Purple
- RST - Blue
- 3V3 - Green


