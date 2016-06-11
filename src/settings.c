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

#include <pebble.h>
#include "settings.h"


static void update_weather_helper(void* unused);

/**
 * Update the weather information (and schedule a periodic timer to update again)
 */
void update_weather() {
    // return if we don't want weather information
    if (config_weather_refresh == 0) return;

    const uint32_t timeout_min = config_weather_refresh;
    const uint32_t timeout_ms = timeout_min * 1000 * 60;
    if (weather_request_timer) {
        if (!app_timer_reschedule(weather_request_timer, timeout_ms)) {
            weather_request_timer = app_timer_register(timeout_ms, update_weather_helper, NULL);
        }
    } else {
        weather_request_timer = app_timer_register(timeout_ms, update_weather_helper, NULL);
    }

    // actually update the weather by sending a request
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    dict_write_uint8(iter, MSG_KEY_FETCH_WEATHER, 1);
    app_message_outbox_send();
    APP_LOG(APP_LOG_LEVEL_INFO, "requesting weather update");
}

/**
 * Utility function.
 */
static void update_weather_helper(void* unused) {
    update_weather();
}

/**
 * Helper to process new configuration.
 */
bool sync_helper(const uint32_t key, DictionaryIterator *iter, uint8_t *value) {
    Tuple *new_tuple = dict_find(iter, key);
    if (new_tuple == NULL) return false;
    if ((*value) != new_tuple->value->uint8) {
        (*value) = new_tuple->value->uint8;
        persist_write_int(key, *value);
        return true;
    }
    return false;
}

/**
 * Helper to process new configuration.
 */
bool sync_helper_2(const uint32_t key, DictionaryIterator *iter, uint16_t *value) {
    Tuple *new_tuple = dict_find(iter, key);
    if (new_tuple == NULL) return false;
    if ((*value) != new_tuple->value->uint16) {
        (*value) = new_tuple->value->uint16;
        persist_write_int(key, *value);
        return true;
    }
    return false;
}

void inbox_received_handler(DictionaryIterator *iter, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "received message");
    bool dirty = false;
    dirty |= sync_helper(CONFIG_COLOR_OUTER_BACKGROUND, iter, &config_color_outer_background);
    dirty |= sync_helper(CONFIG_COLOR_INNER_BACKGROUND, iter, &config_color_inner_background);
    dirty |= sync_helper(CONFIG_COLOR_MINUTE_HAND, iter, &config_color_minute_hand);
    dirty |= sync_helper(CONFIG_COLOR_INNER_MINUTE_HAND, iter, &config_color_inner_minute_hand);
    dirty |= sync_helper(CONFIG_COLOR_HOUR_HAND, iter, &config_color_hour_hand);
    dirty |= sync_helper(CONFIG_COLOR_INNER_HOUR_HAND, iter, &config_color_inner_hour_hand);
    dirty |= sync_helper(CONFIG_COLOR_CIRCLE, iter, &config_color_circle);
    dirty |= sync_helper(CONFIG_COLOR_TICKS, iter, &config_color_ticks);
    dirty |= sync_helper(CONFIG_COLOR_DAY_OF_WEEK, iter, &config_color_day_of_week);
    dirty |= sync_helper(CONFIG_COLOR_DATE, iter, &config_color_date);
    dirty |= sync_helper(CONFIG_BATTERY_LOGO, iter, &config_battery_logo);
    dirty |= sync_helper(CONFIG_COLOR_BATTERY_LOGO, iter, &config_color_battery_logo);
    dirty |= sync_helper(CONFIG_COLOR_BATTERY_30, iter, &config_color_battery_30);
    dirty |= sync_helper(CONFIG_COLOR_BATTERY_20, iter, &config_color_battery_20);
    dirty |= sync_helper(CONFIG_COLOR_BATTERY_10, iter, &config_color_battery_10);
    dirty |= sync_helper(CONFIG_COLOR_BATTERY_BG_30, iter, &config_color_battery_bg_30);
    dirty |= sync_helper(CONFIG_COLOR_BATTERY_BG_20, iter, &config_color_battery_bg_20);
    dirty |= sync_helper(CONFIG_COLOR_BATTERY_BG_10, iter, &config_color_battery_bg_10);
    dirty |= sync_helper(CONFIG_COLOR_BLUETOOTH_LOGO, iter, &config_color_bluetooth_logo);
    dirty |= sync_helper(CONFIG_COLOR_BLUETOOTH_LOGO_2, iter, &config_color_bluetooth_logo_2);
    dirty |= sync_helper(CONFIG_BLUETOOTH_LOGO, iter, &config_bluetooth_logo);
    dirty |= sync_helper(CONFIG_VIBRATE_DISCONNECT, iter, &config_vibrate_disconnect);
    dirty |= sync_helper(CONFIG_VIBRATE_RECONNECT, iter, &config_vibrate_reconnect);
    dirty |= sync_helper(CONFIG_MESSAGE_DISCONNECT, iter, &config_message_disconnect);
    dirty |= sync_helper(CONFIG_MESSAGE_RECONNECT, iter, &config_message_reconnect);
    dirty |= sync_helper(CONFIG_MINUTE_TICKS, iter, &config_minute_ticks);
    dirty |= sync_helper(CONFIG_HOUR_TICKS, iter, &config_hour_ticks);
    dirty |= sync_helper(CONFIG_COLOR_WEATHER, iter, &config_color_weather);
    dirty |= sync_helper_2(CONFIG_WEATHER_REFRESH, iter, &config_weather_refresh);
    dirty |= sync_helper_2(CONFIG_WEATHER_EXPIRATION, iter, &config_weather_expiration);

    if (dirty) {
        update_weather();
    }

    Tuple *icon_tuple = dict_find(iter, MSG_KEY_WEATHER_ICON);
    Tuple *temp_tuple = dict_find(iter, MSG_KEY_WEATHER_TEMP);
    if (icon_tuple && temp_tuple) {
        weather.timestamp = time(NULL);
        weather.icon = icon_tuple->value->int8;
        weather.temperature = temp_tuple->value->int8;
        persist_write_data(PERSIST_KEY_WEATHER, &weather, sizeof(Weather));
        dirty = true;
    }

    if (dict_find(iter, MSG_KEY_JS_READY)) {
        js_ready = true;
        update_weather();
    }
    if (dirty) {
        layer_mark_dirty(layer_background);
    }
}

/**
 * Read a value from the persistent storage (or load the default value).
 */
void read_config(const uint32_t key, uint8_t *value) {
    if (persist_exists(key)) {
        *value = persist_read_int(key);
    } else {
        persist_write_int(key, *value);
    }
}

/**
 * Read a value from the persistent storage (or load the default value).
 */
void read_config_2(const uint32_t key, uint16_t *value) {
    if (persist_exists(key)) {
        *value = persist_read_int(key);
    } else {
        persist_write_int(key, *value);
    }
}


/**
 * Read all items from the configuration storage.
 */
void read_config_all() {
    read_config(CONFIG_COLOR_OUTER_BACKGROUND, &config_color_outer_background);
    read_config(CONFIG_COLOR_INNER_BACKGROUND, &config_color_inner_background);
    read_config(CONFIG_COLOR_MINUTE_HAND, &config_color_minute_hand);
    read_config(CONFIG_COLOR_INNER_MINUTE_HAND, &config_color_inner_minute_hand);
    read_config(CONFIG_COLOR_HOUR_HAND, &config_color_hour_hand);
    read_config(CONFIG_COLOR_INNER_HOUR_HAND, &config_color_inner_hour_hand);
    read_config(CONFIG_COLOR_CIRCLE, &config_color_circle);
    read_config(CONFIG_COLOR_TICKS, &config_color_ticks);
    read_config(CONFIG_COLOR_DAY_OF_WEEK, &config_color_day_of_week);
    read_config(CONFIG_COLOR_DATE, &config_color_date);
    read_config(CONFIG_BATTERY_LOGO, &config_battery_logo);
    read_config(CONFIG_COLOR_BATTERY_LOGO, &config_color_battery_logo);
    read_config(CONFIG_COLOR_BATTERY_30, &config_color_battery_30);
    read_config(CONFIG_COLOR_BATTERY_20, &config_color_battery_20);
    read_config(CONFIG_COLOR_BATTERY_10, &config_color_battery_10);
    read_config(CONFIG_COLOR_BATTERY_BG_30, &config_color_battery_bg_30);
    read_config(CONFIG_COLOR_BATTERY_BG_20, &config_color_battery_bg_20);
    read_config(CONFIG_COLOR_BATTERY_BG_10, &config_color_battery_bg_10);
    read_config(CONFIG_COLOR_BLUETOOTH_LOGO, &config_color_bluetooth_logo);
    read_config(CONFIG_COLOR_BLUETOOTH_LOGO_2, &config_color_bluetooth_logo_2);
    read_config(CONFIG_BLUETOOTH_LOGO, &config_bluetooth_logo);
    read_config(CONFIG_VIBRATE_DISCONNECT, &config_vibrate_disconnect);
    read_config(CONFIG_VIBRATE_RECONNECT, &config_vibrate_reconnect);
    read_config(CONFIG_MESSAGE_DISCONNECT, &config_message_disconnect);
    read_config(CONFIG_MESSAGE_RECONNECT, &config_message_reconnect);
    read_config(CONFIG_MINUTE_TICKS, &config_minute_ticks);
    read_config(CONFIG_HOUR_TICKS, &config_hour_ticks);
    read_config(CONFIG_COLOR_WEATHER, &config_color_weather);
    read_config_2(CONFIG_WEATHER_REFRESH, &config_weather_refresh);
    read_config_2(CONFIG_WEATHER_EXPIRATION, &config_weather_expiration);

    if (persist_exists(PERSIST_KEY_WEATHER)) {
        persist_read_data(PERSIST_KEY_WEATHER, &weather, sizeof(Weather));
    } else {
        weather.timestamp = 0;
    }

    js_ready = false;
}
