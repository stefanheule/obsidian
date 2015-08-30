
OBSIDIAN_CONFIG=""
OBSIDIAN_FILE="out"

all: install_emulator

deploy: install_deploy

build:
	pebble build

config:
	pebble emu-app-config --emulator basalt

travis_build:
	~/pebble-dev/${PEBBLE_SDK}/bin/pebble build

install_emulator: build
	pebble install --emulator basalt

install_deploy: build
	pebble install --phone 10.0.0.5

menu_icon:
	$(MAKE) write_header OBSIDIAN_CONFIG="SCREENSHOT_MENU_ICON"
	$(MAKE) build
	$(MAKE) install_emulator
	$(MAKE) clean_header

screenshots:
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

screenshot:
	$(MAKE) write_header
	$(MAKE) build
	$(MAKE) install_emulator
	scripts/take_screenshot.sh screenshots/$(OBSIDIAN_FILE).png
	$(MAKE) clean_header

write_header:
	echo "#define $(OBSIDIAN_CONFIG)" > src/config.h

clean: clean_header
	pebble clean

clean_header:
	echo "" > src/config.h

.PHONY: all deploy build config install_emulator install_deploy menu_icon screenshots screenshot write_header clean clean_header
