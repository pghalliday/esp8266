# smart-dimmer

## Prerequisites

- Wemos D1 mini pro ESP8266 board
- `esp8266` package for Arduino
  - Add `https://arduino.esp8266.com/stable/package_esp8266com_index.json` to `Additional Boards Manager URLs` in Arduino IDE `preferences`
  - Go to `Tools/Board/Boards Manager` and install the `esp8266` package
- ArduinoJson - https://arduinojson.org/
- ESP8266LittleFS plugin for Arduino
  - Download from https://github.com/earlephilhower/arduino-esp8266littlefs-plugin/releases
  - Unzip to `~/Documents/Arduino/tools`

## Uploading the LittleFS file system

To upload the contents of `data/` to the ESP8266

- Open the Arduino IDE to the sketch
- Select `Tools/Board/Generic ESP8266 Module` - see note on flash size as to why we don't use `LOLIN(Wemos) D1 mini pro`
- Select `Tools/Flash Size/4MB (FS:2MB OTA:~1019KB)` - this seems to work but was actually chosen pretty randomly as the upload tool detected 4MB of flash even though the `LOLIN(Wemos) D1 mini pro` board options seem to only allow a selection of 16MB (which fails to upload)
- Select `Tools/ESP8266 LittleFS Data Upload` to upload.

