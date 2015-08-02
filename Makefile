
all: install_emulator

phone: install_phone

build: src/*.c src/*.h
	pebble build

install_emulator: build
	pebble install --emulator basalt

install_phone: build
	pebble install --phone 10.0.0.5
