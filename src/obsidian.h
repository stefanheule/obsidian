// Copyright 2015-16 Stefan Heule
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OBSIDIAN_OBSIDIAN_H
#define OBSIDIAN_OBSIDIAN_H

#include <pebble.h>

#include "settings.h"
#include "drawing.h"
#include "geometry.h"

//#define DEBUG_DATE_POSITION
//#define DEBUG_WEATHER_POSITION

////////////////////////////////////////////
//// Configuration constants
////////////////////////////////////////////

// config keys are also duplicated in src/obsidian.h, appinfo.json, src/js/pebble-js-app.js and config/index.html
#define CONFIG_COLOR_OUTER_BACKGROUND 1
#define CONFIG_COLOR_INNER_BACKGROUND 2
#define CONFIG_COLOR_MINUTE_HAND 3
#define CONFIG_COLOR_INNER_MINUTE_HAND 4
#define CONFIG_COLOR_HOUR_HAND 5
#define CONFIG_COLOR_INNER_HOUR_HAND 6
#define CONFIG_COLOR_CIRCLE 7
#define CONFIG_COLOR_TICKS 8
#define CONFIG_COLOR_DAY_OF_WEEK 9
#define CONFIG_COLOR_DATE 10
#define CONFIG_BATTERY_LOGO 11
#define CONFIG_COLOR_BATTERY_LOGO 12
#define CONFIG_COLOR_BATTERY_BG_30 13
#define CONFIG_COLOR_BATTERY_BG_20 14
#define CONFIG_COLOR_BATTERY_BG_10 15
#define CONFIG_COLOR_BLUETOOTH_LOGO 16
#define CONFIG_COLOR_BLUETOOTH_LOGO_2 17
#define CONFIG_BLUETOOTH_LOGO 18
#define CONFIG_VIBRATE_DISCONNECT 19
#define CONFIG_VIBRATE_RECONNECT 20
#define CONFIG_MESSAGE_DISCONNECT 21
#define CONFIG_MESSAGE_RECONNECT 22
#define CONFIG_MINUTE_TICKS 23
#define CONFIG_HOUR_TICKS 24
#define CONFIG_COLOR_BATTERY_30 25
#define CONFIG_COLOR_BATTERY_20 26
#define CONFIG_COLOR_BATTERY_10 27
// not defining local keys here (so some IDs are missing)
#define CONFIG_COLOR_WEATHER 29
#define CONFIG_WEATHER_REFRESH 35
#define CONFIG_WEATHER_EXPIRATION 36

// message keys (also duplicated in appinfo.json)
#define MSG_KEY_WEATHER_TEMP 100
#define MSG_KEY_WEATHER_ICON 101
#define MSG_KEY_FETCH_WEATHER 102
#define MSG_KEY_JS_READY 103

// persitant storage keys (in additiona to config keys above)
#define PERSIST_KEY_WEATHER 201


////////////////////////////////////////////
//// Configuration values
////////////////////////////////////////////

extern uint8_t config_color_outer_background;
extern uint8_t config_color_inner_background;
extern uint8_t config_color_minute_hand;
extern uint8_t config_color_inner_minute_hand;
extern uint8_t config_color_hour_hand;
extern uint8_t config_color_inner_hour_hand;
extern uint8_t config_color_circle;
extern uint8_t config_color_ticks;
extern uint8_t config_color_day_of_week;
extern uint8_t config_color_date;
extern uint8_t config_battery_logo;
extern uint8_t config_color_battery_logo;
extern uint8_t config_color_battery_30;
extern uint8_t config_color_battery_20;
extern uint8_t config_color_battery_10;
extern uint8_t config_color_battery_bg_30;
extern uint8_t config_color_battery_bg_20;
extern uint8_t config_color_battery_bg_10;
extern uint8_t config_color_bluetooth_logo;
extern uint8_t config_color_bluetooth_logo_2;
extern uint8_t config_bluetooth_logo;
extern uint8_t config_vibrate_disconnect;
extern uint8_t config_vibrate_reconnect;
extern uint8_t config_message_disconnect;
extern uint8_t config_message_reconnect;
extern uint8_t config_minute_ticks;
extern uint8_t config_hour_ticks;
extern uint8_t config_color_weather;
extern uint16_t config_weather_refresh;
extern uint16_t config_weather_expiration;


////////////////////////////////////////////
//// Global variables
////////////////////////////////////////////

extern Window *window;
extern Layer *layer_background;
extern char buffer_1[30];
extern char buffer_2[30];
extern GPoint center;
extern int16_t height;
extern int16_t width;
#ifdef OBSIDIAN_SHOW_NUMBERS
/** Open Sans font. */
extern GFont font_open_sans;
#endif
extern GFont font_system_18px_bold;
extern GFont font_nupe;
#ifdef PBL_ROUND
extern GFont font_system_24px_bold;
#endif
extern bool show_bluetooth_popup;
extern AppTimer *timer_bluetooth_popup;

#if defined(DEBUG_WEATHER_POSITION) || defined(DEBUG_DATE_POSITION)
#define DEBUG_ITER_COUNTER
#endif
#ifdef DEBUG_ITER_COUNTER
extern int debug_iter;
#endif

typedef struct {
    time_t timestamp;
    int8_t icon;
    int8_t temperature;
} __attribute__((__packed__)) Weather;

extern Weather weather;
extern bool js_ready;
extern AppTimer * weather_request_timer;

////////////////////////////////////////////
//// Static configuration and useful macros
////////////////////////////////////////////

#define COLOR(c) ((GColor8) { .argb = (c) })

//#define OBSIDIAN_SHOW_NUMBERS
#define OBSIDIAN_LONG_TICKS
#define OBSIDIAN_FAT_TICKS
//#define OBSIDIAN_ONLY_RELEVANT_NUMBER
//#define OBSIDIAN_BATTERY_USE_TEXT

#define OBSIDIAN_BLUETOOTH_POPUP_MS 5000


#endif //OBSIDIAN_OBSIDIAN_H
