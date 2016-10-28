#!/bin/bash

# Take a screenshot of the emulator (for all platforms)

function take_screenshot {
  pebble install --emulator $2 &> /dev/null
  pebble screenshot --emulator $2 screenshots/$2/$1.png &> /dev/null
}

pebble wipe
pebble kill
pebble wipe
(take_screenshot $1 "aplite") &
(take_screenshot $1 "basalt") &
(take_screenshot $1 "chalk") &
wait
echo "" > src/config.h
pebble wipe
pebble kill
pebble wipe
