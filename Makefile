
all: install_emulator

deploy: install_deploy

build: src/*.c src/*.h appinfo.json wscript
	pebble build

install_emulator: build
	pebble install --emulator basalt

install_deploy: build
	pebble install --phone 10.0.0.5
