#!/bin/bash

# Take a screenshot of the emulator (for all platforms)

function take_screenshot {
  pebble install --emulator $2 &> /dev/null
  pebble screenshot --emulator $2 screenshots/$2/$1.png &> /dev/null
}

(take_screenshot $1 "diorite")
pebble wipe
pebble kill
(take_screenshot $1 "basalt")
pebble wipe
pebble kill
(take_screenshot $1 "chalk")
pebble wipe
pebble kill
(take_screenshot $1 "emery")
pebble wipe
pebble kill


echo "" > src/config.h
