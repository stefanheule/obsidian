#!/bin/bash

# Take a screenshot of the emulator (for all platforms)

function take_screenshot {
  pebble install --emulator $2 &> /dev/null
  pebble screenshot --emulator aplite screenshots/aplite/$1.png &> /dev/null
}

pebble kill
(take_screenshot $1 "aplite") &
(take_screenshot $1 "basalt") &
(take_screenshot $1 "chalk") &
# (pebble install --emulator "basalt" && pebble screenshot --emulator basalt screenshots/basalt/$1.png) &
# (pebble install --emulator "chalk" && pebble screenshot --emulator chalk screenshots/chalk/$1.png) &
wait
echo "" > src/config.h
