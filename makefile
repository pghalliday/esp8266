.PHONY: all compile upload listen clean

FQBN:=esp8266:esp8266:generic
FQBN_FILE:=$(subst :,.,${FQBN})

ifdef UPLOAD_FLAGS
	UPLOAD_FQBN:=${FQBN}:${UPLOAD_FLAGS}
else
	UPLOAD_FQBN:=${FQBN}
endif

all: upload listen

${PROJECT}/${PROJECT}.${FQBN_FILE}.elf: ${PROJECT}/${PROJECT}.ino
	arduino-cli compile --fqbn ${FQBN} ${PROJECT}

compile: ${PROJECT}/${PROJECT}.${FQBN_FILE}.elf

upload: compile
	arduino-cli upload -p /dev/cu.usbserial-A100AVYN --fqbn ${UPLOAD_FQBN} ${PROJECT}

listen:
	stty -f /dev/cu.usbserial-A100AVYN 9600
	cat /dev/cu.usbserial-A100AVYN

clean:
	rm ${PROJECT}/${PROJECT}.${FQBN_FILE}.elf
	rm ${PROJECT}/${PROJECT}.${FQBN_FILE}.bin
