.PHONY: all compile upload listen clean

FQBN:=esp8266:esp8266:d1_mini
FQBN_FILE:=$(subst :,.,${FQBN})

DEVICE:=usbserial-01EF8BB1
BAUDRATE:=115200

ifdef UPLOAD_FLAGS
	UPLOAD_FQBN:=${FQBN}:${UPLOAD_FLAGS}
else
	UPLOAD_FQBN:=${FQBN}
endif

all: upload listen

compile:
	arduino-cli compile --fqbn ${FQBN} ${PROJECT}

upload: compile
	arduino-cli upload -p /dev/cu.${DEVICE} --fqbn ${UPLOAD_FQBN} ${PROJECT}

listen:
	./serial.sh ${DEVICE} ${BAUDRATE}

clean:
	rm ${PROJECT}/${PROJECT}.${FQBN_FILE}.elf
	rm ${PROJECT}/${PROJECT}.${FQBN_FILE}.bin
