#!/bin/bash

# assemble the resources for the pebble app store, etc

set -e

# get png of vector outline
inkscape -z -e pebble_border.png -w 799 -h 799 resources/src/pebble_border.svg
inkscape -z -e pebble_outline.png -w 799 -h 799 resources/src/pebble_outline.svg

# assemble screenshot and outline
convert -crop 272x410+158+125 pebble_outline.png pebble_outline.png
cp pebble_outline.png tmp.png
composite -compose Src_Over -geometry +0+0 pebble_outline.png tmp.png tmp.png
composite -compose Src_Over -geometry +63+116 screenshots/main.png tmp.png tmp.png
composite -compose Src_Over -geometry -20-183 pebble_border.png tmp.png tmp.png
rm -f pebble_border.png
rm -f pebble_outline.png
mv tmp.png resources/images/with_outline.png

# assemble banner
cp resources/src/banner_background.png tmp.png
composite -compose Src_Over -geometry +425-45 resources/images/with_outline.png tmp.png tmp.png
mv tmp.png resources/images/banner.png

# cut out a rectangular banner
cp resources/images/banner.png tmp.png
convert -crop 320x320+400+0 tmp.png tmp.png
mv tmp.png resources/images/banner_rect.png

# 144x144 logo
cp screenshots/main.png tmp.png
convert -page 144x168+0+0 tmp.png tmp.png
convert -crop 144x144+0+12 tmp.png tmp.png
mv tmp.png resources/images/logo_144.png

# 48x48 logo
cp resources/images/logo_144.png tmp.png
convert -resize 48x48+0+0 tmp.png tmp.png
mv tmp.png resources/images/logo_48.png

for f in resources/images/*.png; do
  pngcrush -q -rem time $f tmp.png
  mv tmp.png $f
done
