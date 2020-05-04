# esp8266

## makefile targets

- `make all PROJECT=<PROJECT_NAME>` - compile, deploy and listen on serial output 
- `make deploy PROJECT=<PROJECT_NAME>` - compile and deploy 
- `make compile PROJECT=<PROJECT_NAME>` - compile
- `make listen` - listen on serial output

## Add ESP8266 core to arduino-cli

Add the following to the `arduino-cli` config at `~/Library/Arduino15/arduino-cli.yaml`

NB. if the file does not exist then run `arduino-cli config init`

```yaml
board_manager:
    additional_urls:
    - https://arduino.esp8266.com/stable/package_esp8266com_index.json
```

Then run

```
arduino-cli core update-index
arduino-cli core search esp8266
```

Verify that the `esp8266` core is found, eg:

```
ID              Version Name
esp8266:esp8266 2.7.0   esp8266
```

Install the core with

```
arduino-cli core install esp8266:esp8266
```

You can verify the installation with

```
arduino-cli core list
```

## Notes

Setup:

- Using breadboard with 3.3V power supply attached
- Using the ESP8266 01
- Using a FTDI USB/Serial chip
- Connect FTDI RX -> ESP8266 TX
- Connect FTDI TX -> ESP8266 RX
- Connect ESP8266 GND -> breadboard -
- Connect ESP8266 VCC -> breadboard +
- Connect ESP8266 EN -> breadboard + (Enable the chip)
- Connect FTDI GND -> breadboard - (Without this the serial communication does not work - I have an idea that the chips all have to have the same GND and that's why?)

To reset the ESP8266:

- Connect then disconnect ESP8266 RST -> breadboard -

To flash the ESP8266:

- Connect the ESP8266 IO0 -> breadboard -
- Reset the ESP8266 as above
- Upload the code
- Disconnect ESP8266 IO0 from breadboard -
- Reset the ESP8266 as above

After flashing ESP8266 IO0 can be used as an output, etc
