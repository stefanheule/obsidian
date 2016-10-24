cmake_minimum_required(VERSION 3.2)
project(obsidian)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        src/obsidian.c
        src/obsidian.h
        src/config.h
        src/settings.c
        src/settings.h
        src/geometry.c
        src/geometry.h
        src/drawing.c
        src/drawing.h)

add_executable(obsidian ${SOURCE_FILES})

include_directories(~/.pebble-sdk/SDKs/4.0.1/sdk-core/pebble/basalt/include)
include_directories(~/dev/projects/obsidian/node_modules/pebble-fctx/dist/include)
