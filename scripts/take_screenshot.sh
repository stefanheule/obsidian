#!/bin/bash

# Take a screenshot of the emulator, and save the file as '$1'

# Ideally, we would just use the pebble tool, but that appears to be broken
# for taking screenshots, so I'll use shutter for now.

set -e

script=`basename "$0"`
if [ $# -eq 0 ]; then
  echo "usage: $script <outfile.png>"
  exit 1
fi

file=$1

# wait just a little bit before proceeding
sleep 0.1

# bring emulator to top
xdotool search --sync "QEMU" windowactivate
sleep 0.1

# send left keystroke to light up the display
xdotool key Left
sleep 0.1

# take the screenshot
shutter -e -n --window QEMU -o $file

# check image dimensions (sanity check)
if ! identify $file | grep 150x197 > /dev/null; then
  identify $file
  echo "ERROR: unexpected image size"
  exit 1
fi

# clip the image (i.e., remove the border from the emulator window)
convert $file -crop 144x168+3+26 $file
