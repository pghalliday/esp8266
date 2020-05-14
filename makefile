.PHONY: all compile upload listen clean

FQBN:=esp8266:esp8266:generic
FQBN_FILE:=$(subst :,.,${FQBN})

DEVICE:=usbserial-A100AVYN
BAUDRATE:=9600

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
	arduino-cli upload -p /dev/cu.${DEVICE} --fqbn ${UPLOAD_FQBN} ${PROJECT}

listen:
	stty -f /dev/cu.${DEVICE} ${BAUDRATE}
	cat /dev/cu.${DEVICE}

clean:
	rm ${PROJECT}/${PROJECT}.${FQBN_FILE}.elf
	rm ${PROJECT}/${PROJECT}.${FQBN_FILE}.bin
