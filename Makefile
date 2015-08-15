
all: install_emulator

deploy: install_deploy

build:
	pebble build

install_emulator: build
	pebble install --emulator basalt

install_deploy: build
	pebble install --phone 10.0.0.5

menu_icon:
	echo "#define DEBUG_MENU_ICON" > src/config.h
	pebble build
	$(MAKE) install_emulator
	$(MAKE) clean_config

clean_config:
	echo "" > src/config.h

.PHONY: all deploy build install_emulator install_deploy menu_icon clean_config