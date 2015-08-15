
OBSIDIAN_CONFIG=""
OBSIDIAN_FILE="out"

all: install_emulator

deploy: install_deploy

build:
	pebble build

install_emulator: build
	pebble install --emulator basalt

install_deploy: build
	pebble install --phone 10.0.0.5

menu_icon:
	$(MAKE) config OBSIDIAN_CONFIG="SCREENSHOT_MENU_ICON"
	$(MAKE) build
	$(MAKE) install_emulator
	$(MAKE) clean_config

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

screenshot:
	$(MAKE) config
	$(MAKE) build
	$(MAKE) install_emulator
	scripts/take_screenshot.sh screenshots/$(OBSIDIAN_FILE).png
	$(MAKE) clean_config

config:
	echo "#define $(OBSIDIAN_CONFIG)" > src/config.h

clean: clean_config
	pebble clean

clean_config:
	echo "" > src/config.h

.PHONY: all deploy build install_emulator install_deploy menu_icon screenshots screenshot config clean clean_config