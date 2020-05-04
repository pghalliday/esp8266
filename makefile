.PHONY: all compile deploy listen

all: deploy listen

${PROJECT}/${PROJECT}.esp8266.esp8266.generic.elf: ${PROJECT}/${PROJECT}.ino
	arduino-cli compile --fqbn esp8266:esp8266:generic ${PROJECT}

compile: ${PROJECT}/${PROJECT}.esp8266.esp8266.generic.elf

deploy: compile
	arduino-cli upload -p /dev/cu.usbserial-A100AVYN --fqbn esp8266:esp8266:generic ${PROJECT}

listen:
	stty -f /dev/cu.usbserial-A100AVYN 9600
	cat /dev/cu.usbserial-A100AVYN
