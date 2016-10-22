
OBSIDIAN_CONFIG=""
OBSIDIAN_FILE="out"

# platform
P="basalt"

VERSION=$(shell cat appinfo.json | grep version | grep -o "[0-9]*\.[0-9]*")

all: build install_emulator

deploy: install_deploy

build:
	# copy fonts
	cp resources/fonts/nupe2.ttf config/fonts/nupe2.ttf
	cp resources/fonts/nupe2-small.ttf config/fonts/nupe2-small.ttf
	pebble build

build_os3:
	sed -i "s/ef42caba-0c65-4879-ab23-edd2bde68824/529f2905-ac36-4991-90c6-fc25f2381cac/g" appinfo.json
	pebble sdk activate 3.14
	$(MAKE) write_header OBSIDIAN_CONFIG="__COMPILE_OS3"
	# copy fonts
	cp resources/fonts/nupe2.ttf config/fonts/nupe2.ttf
	cp resources/fonts/nupe2-small.ttf config/fonts/nupe2-small.ttf
	pebble build
	$(MAKE) clean_header
	sed -i "s/529f2905-ac36-4991-90c6-fc25f2381cac/ef42caba-0c65-4879-ab23-edd2bde68824/g" appinfo.json
	@echo ""
	@echo ""
	@echo ""
	@echo "Now activate the latest SKD again!"

build_quiet:
	@scripts/build_quiet.sh

config:
	pebble emu-app-config --emulator $(P)

log:
	pebble logs --emulator $(P)

travis_build:
	yes | ~/pebble-dev/${PEBBLE_SDK}/bin/pebble build

install_emulator:
	pebble install --emulator $(P)

install_deploy: build
	pebble install --phone 192.168.1.6

menu_icon:
	$(MAKE) write_header OBSIDIAN_CONFIG="SCREENSHOT_MENU_ICON"
	$(MAKE) build
	$(MAKE) install_emulator
	$(MAKE) clean_header

resources:
	scripts/assemble_resources.sh

screenshots: screenshot_config
	pebble kill
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_MAIN" OBSIDIAN_FILE="main"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_SQUARE" OBSIDIAN_FILE="square"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_BATTERY_LOW_1" OBSIDIAN_FILE="battery-low-1"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_BATTERY_LOW_2" OBSIDIAN_FILE="battery-low-2"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_BATTERY_LOW_3" OBSIDIAN_FILE="battery-low-3"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_DATE_1" OBSIDIAN_FILE="date-1"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_DATE_2" OBSIDIAN_FILE="date-2"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_WEATHER_1" OBSIDIAN_FILE="weather-1"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_WEATHER_2" OBSIDIAN_FILE="weather-2"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_BLUETOOTH_ICON" OBSIDIAN_FILE="bluetooth-icon"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_BLUETOOTH_POPUP_1" OBSIDIAN_FILE="bluetooth-popup-1"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_BLUETOOTH_POPUP_2" OBSIDIAN_FILE="bluetooth-popup-2"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_ALT_THEME_1" OBSIDIAN_FILE="theme-1"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_ALT_THEME_2" OBSIDIAN_FILE="theme-2"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_ALT_THEME_3" OBSIDIAN_FILE="theme-3"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_ALT_THEME_4" OBSIDIAN_FILE="theme-4"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_ALT_THEME_5" OBSIDIAN_FILE="theme-5"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_ALT_THEME_6" OBSIDIAN_FILE="theme-6"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_ALT_THEME_7" OBSIDIAN_FILE="theme-7"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_ALT_THEME_8" OBSIDIAN_FILE="theme-8"
	scripts/assemble_screenshots.sh
	scripts/assemble_resources.sh
	pebble kill

screenshot_config:
	rm -f screenshots/aplite/config.png
	rm -f screenshots/basalt/config.png
	rm -f screenshots/chalk/config.png
	phantomjs scripts/capture-settings-screenshot.js http://obsidian.local.com/index.html?platform=aplite&version=$(VERSION)
	sleep 2
	pngcrush -q -rem time tmp.png screenshots/aplite/config.png
	rm tmp.png
	phantomjs scripts/capture-settings-screenshot.js http://obsidian.local.com/index.html?platform=basalt&version=$(VERSION)
	sleep 2
	pngcrush -q -rem time tmp.png screenshots/basalt/config.png
	rm tmp.png
	phantomjs scripts/capture-settings-screenshot.js http://obsidian.local.com/index.html?platform=chalk&version=$(VERSION)
	sleep 2
	pngcrush -q -rem time tmp.png screenshots/chalk/config.png
	rm tmp.png

screenshot: write_header build_quiet
	scripts/take_screenshot.sh $(OBSIDIAN_FILE)

write_header:
	@echo "#define $(OBSIDIAN_CONFIG)" > src/config.h

clean: clean_header
	pebble clean

clean_header:
	echo "" > src/config.h

updated_config:
	src/scripts/updated_config.sh

font_build:
	tools/fctx-compiler/fctx-compiler.js -r "[0-9a-zA-Z.:\-/]" resources/fonts/Oswald-Regular.svg

.PHONY: all deploy build build_quiet config log resources install_emulator install_deploy menu_icon screenshots screenshot screenshot_config write_header clean clean_header
