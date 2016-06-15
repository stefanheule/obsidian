#!/bin/bash

# assemble the screenshots for the readme

set -e

for platform in "aplite" "basalt" "chalk"; do

  watch_width=144
  if [ $platform == "chalk" ]; then
    watch_width=180
  fi
  watch_height=168
  if [ $platform == "chalk" ]; then
    watch_height=180
  fi

  n=3
  hspace=50
  border=0
  width=$((n*watch_width+(n-1)*hspace+2*border))
  height=$((watch_height-168+194+2*border))
  normalheight=$((watch_height+2*border))
  x1=$((0*(watch_width+hspace)+border))
  x2=$((1*(watch_width+hspace)+border))
  x3=$((2*(watch_width+hspace)+border))
  textx1=$(((watch_width-144)/2+8+x1))
  textx2=$(((watch_width-144)/2+8+x2))
  textx3=$(((watch_width-144)/2+8+x3))
  texty=$((watch_height-168+190+border))

  # low battery look
  convert data/white.png -background white -resize ${width}x${height}! tmp.png
  composite -compose atop -geometry +$x1+$border screenshots/$platform/battery-low-1.png tmp.png tmp.png
  composite -compose atop -geometry +$x2+$border screenshots/$platform/battery-low-2.png tmp.png tmp.png
  composite -compose atop -geometry +$x3+$border screenshots/$platform/battery-low-3.png tmp.png tmp.png
  convert tmp.png -pointsize 15 -font /usr/share/fonts/truetype/msttcorefonts/verdana.ttf -annotate +$textx1+$texty '30%% battery left' tmp.png
  convert tmp.png -pointsize 15 -font /usr/share/fonts/truetype/msttcorefonts/verdana.ttf -annotate +$textx2+$texty '20%% battery left' tmp.png
  convert tmp.png -pointsize 15 -font /usr/share/fonts/truetype/msttcorefonts/verdana.ttf -annotate +$textx3+$texty '10%% battery left' tmp.png
  mv tmp.png screenshots/$platform/battery.png

  textx1=$(((watch_width-144)/2+12+x1))
  textx2=$(((watch_width-144)/2-5+x2))
  textx3=$(((watch_width-144)/2+15+x3))

  # bluetooth
  convert data/white.png -background white -resize ${width}x${height}! tmp.png
  composite -compose atop -geometry +$x1+$border screenshots/$platform/bluetooth-popup-2.png tmp.png tmp.png
  composite -compose atop -geometry +$x2+$border screenshots/$platform/bluetooth-popup-1.png tmp.png tmp.png
  composite -compose atop -geometry +$x3+$border screenshots/$platform/bluetooth-icon.png tmp.png tmp.png
  convert tmp.png -pointsize 15 -font /usr/share/fonts/truetype/msttcorefonts/verdana.ttf -annotate +$textx1+$texty 'Connection lost' tmp.png
  convert tmp.png -pointsize 15 -font /usr/share/fonts/truetype/msttcorefonts/verdana.ttf -annotate +$textx2+$texty 'Connection regained' tmp.png
  convert tmp.png -pointsize 15 -font /usr/share/fonts/truetype/msttcorefonts/verdana.ttf -annotate +$textx3+$texty 'No connection' tmp.png
  mv tmp.png screenshots/$platform/bluetooth.png

  textx1=$(((watch_width-144)/2+12+x1))
  textx2=$(((watch_width-144)/2+10+x2))
  textx3=$(((watch_width-144)/2+4+x3))

  # date
  convert data/white.png -background white -resize ${width}x${height}! tmp.png
  composite -compose atop -geometry +$x1+$border screenshots/$platform/main.png tmp.png tmp.png
  composite -compose atop -geometry +$x2+$border screenshots/$platform/date-1.png tmp.png tmp.png
  composite -compose atop -geometry +$x3+$border screenshots/$platform/date-2.png tmp.png tmp.png
  convert tmp.png -pointsize 15 -font /usr/share/fonts/truetype/msttcorefonts/verdana.ttf -annotate +$textx1+$texty 'Normal position' tmp.png
  convert tmp.png -pointsize 15 -font /usr/share/fonts/truetype/msttcorefonts/verdana.ttf -annotate +$textx2+$texty 'Date moved left' tmp.png
  convert tmp.png -pointsize 15 -font /usr/share/fonts/truetype/msttcorefonts/verdana.ttf -annotate +$textx3+$texty 'Date moved right' tmp.png
  mv tmp.png screenshots/$platform/date.png

  # main
  convert data/white.png -background white -resize ${width}x${normalheight}! tmp.png
  composite -compose atop -geometry +$x1+$border screenshots/$platform/main.png tmp.png tmp.png
  composite -compose atop -geometry +$x2+$border screenshots/$platform/theme-1.png tmp.png tmp.png
  composite -compose atop -geometry +$x3+$border screenshots/$platform/theme-2.png tmp.png tmp.png
  mv tmp.png screenshots/$platform/overview.png

  # themes
  convert data/white.png -background white -resize ${width}x${normalheight}! tmp.png
  composite -compose atop -geometry +$x1+$border screenshots/$platform/theme-6.png tmp.png tmp.png
  composite -compose atop -geometry +$x2+$border screenshots/$platform/theme-4.png tmp.png tmp.png
  composite -compose atop -geometry +$x3+$border screenshots/$platform/theme-3.png tmp.png tmp.png
  mv tmp.png screenshots/$platform/theme-overview-1.png

  # themes
  convert data/white.png -background white -resize ${width}x${normalheight}! tmp.png
  composite -compose atop -geometry +$x1+$border screenshots/$platform/theme-5.png tmp.png tmp.png
  composite -compose atop -geometry +$x2+$border screenshots/$platform/theme-7.png tmp.png tmp.png
  composite -compose atop -geometry +$x3+$border screenshots/$platform/theme-8.png tmp.png tmp.png
  mv tmp.png screenshots/$platform/theme-overview-2.png

  # weather
  convert data/white.png -background white -resize ${width}x${normalheight}! tmp.png
  composite -compose atop -geometry +$x1+$border screenshots/$platform/main.png tmp.png tmp.png
  composite -compose atop -geometry +$x2+$border screenshots/$platform/weather-1.png tmp.png tmp.png
  composite -compose atop -geometry +$x3+$border screenshots/$platform/weather-2.png tmp.png tmp.png
  mv tmp.png screenshots/$platform/weather.png
done

for platform in "aplite" "basalt" "chalk"; do
  for f in screenshots/$platform/*.png; do
    pngcrush -rem time $f tmp.png &> /dev/null
    mv tmp.png $f
  done
done
