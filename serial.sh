#!/bin/bash

DEVICE=$1
BAUDRATE=$2

# start cat in background and record the PID
cat /dev/cu.${DEVICE} &
PID=$!

# Have to change baudrate after
# starting cat for it to stick
stty -f /dev/cu.${DEVICE} speed ${BAUDRATE}

cleanup()
{
  kill $PID
}

# loop till ctrl-c then kill cat
trap cleanup 0
read -r -d '' _ </dev/tty
