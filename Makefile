
OBSIDIAN_CONFIG=""
OBSIDIAN_FILE="out"

all: install_emulator

deploy: install_deploy

build:
	pebble build

config:
	pebble emu-app-config --emulator basalt

config_aplite:
	pebble emu-app-config --emulator aplite

log:
	pebble logs --emulator basalt

log_aplite:
	pebble logs --emulator aplite

travis_build:
	~/pebble-dev/${PEBBLE_SDK}/bin/pebble build

install_emulator: build
	pebble install --emulator basalt

install_emulator_aplite: build
	pebble install --emulator aplite

install_deploy: build
	pebble install --phone 10.0.0.5

menu_icon:
	$(MAKE) write_header OBSIDIAN_CONFIG="SCREENSHOT_MENU_ICON"
	$(MAKE) build
	$(MAKE) install_emulator
	$(MAKE) clean_header

resources:
	scripts/assemble_resources.sh

screenshots: screenshot_config
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_MAIN" OBSIDIAN_FILE="main"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_BATTERY_LOW_1" OBSIDIAN_FILE="battery-low-1"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_BATTERY_LOW_2" OBSIDIAN_FILE="battery-low-2"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_BATTERY_LOW_3" OBSIDIAN_FILE="battery-low-3"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_DATE_1" OBSIDIAN_FILE="date-1"
	$(MAKE) screenshot OBSIDIAN_CONFIG="SCREENSHOT_DATE_2" OBSIDIAN_FILE="date-2"
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

screenshot_config:
	phantomjs scripts/capture-settings-screenshot.js config/index.html?platform=basalt
	pngcrush -q -rem time tmp.png screenshots/config.png
	rm tmp.png
	phantomjs scripts/capture-settings-screenshot.js config/index.html?platform=aplite
	pngcrush -q -rem time tmp.png screenshots/config_aplite.png
	rm tmp.png

screenshot:
	$(MAKE) write_header
	$(MAKE) build
	pebble kill
	$(MAKE) install_emulator
	scripts/take_screenshot.sh screenshots/$(OBSIDIAN_FILE).png
	pebble kill
	$(MAKE) install_emulator_aplite
	scripts/take_screenshot.sh screenshots/aplite_$(OBSIDIAN_FILE).png
	pebble kill
	$(MAKE) clean_header

write_header:
	echo "#define $(OBSIDIAN_CONFIG)" > src/config.h

clean: clean_header
	pebble clean

clean_header:
	echo "" > src/config.h

.PHONY: all deploy build config log resources install_emulator install_deploy menu_icon screenshots screenshot screenshot_config write_header clean clean_header
