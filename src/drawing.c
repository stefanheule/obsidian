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

#include "drawing.h"
#include "obsidian.h"

void draw_pointer(GContext *ctx, GPoint target, int16_t width, int16_t height, int32_t angle, GColor color) {
    GPoint vertices[3];
    vertices[0].x = 0;
    vertices[0].y = 0;
    vertices[1].x = -width / 2;
    vertices[1].y = height;
    vertices[2].x = width / 2;
    vertices[2].y = height;
    GPathInfo T;
    T.num_points = 3;
    T.points = vertices;
    GPath *triangle = gpath_create(&T);

    gpath_rotate_to(triangle, angle);
    gpath_move_to(triangle, target);

    // Fill the path:
    graphics_context_set_fill_color(ctx, color);
    gpath_draw_filled(ctx, triangle);
    // Stroke the path:
    graphics_context_set_stroke_width(ctx, 1);
    graphics_context_set_stroke_color(ctx, color);
    gpath_draw_outline(ctx, triangle);

    gpath_destroy(triangle);

    graphics_context_set_stroke_color(ctx, COLOR(config_color_ticks));
    graphics_context_set_fill_color(ctx, COLOR(config_color_ticks));
}


#ifdef __COMPILE_OS3
GRect layer_get_unobstructed_bounds(Layer* layer) {
    return layer_get_bounds(layer);
}
#endif

/**
 * Is something obstructing our layer?
 */
bool is_obstructed() {
    Layer *layer = layer_background;
    GRect full = layer_get_bounds(layer);
    GRect partial = layer_get_unobstructed_bounds(layer);

    return full.size.h != partial.size.h || full.size.w != partial.size.w;
}

/**
 * Draw a line with a given width.
 */
void graphics_draw_line_with_width(GContext *ctx, GPoint p0, GPoint p1, uint8_t width) {
    graphics_context_set_stroke_width(ctx, width);
    graphics_draw_line(ctx, p0, p1);
}

/**
 * Draws a bluetooth logo at a given position (top-left).
 */
void draw_bluetooth_logo(GContext *ctx, GPoint origin) {
#define BLUETOOTH_LOGO_STEP 3

    if (!config_bluetooth_logo) return;
    if (is_obstructed()) return;

    // background
    graphics_context_set_fill_color(ctx, COLOR(config_color_bluetooth_logo));
    graphics_fill_rect(ctx, GRect(origin.x - 2, origin.y - 2, BLUETOOTH_LOGO_STEP * 2 + 5, BLUETOOTH_LOGO_STEP * 4 + 5),
                       2, GCornersAll);

    // logo on the inside
#ifdef PBL_COLOR
    graphics_context_set_antialiased(ctx, false);
#endif
    graphics_context_set_stroke_color(ctx, COLOR(config_color_bluetooth_logo_2));

    graphics_draw_line_with_width(ctx, GPoint(origin.x + BLUETOOTH_LOGO_STEP, origin.y + 0),
                                  GPoint(origin.x + BLUETOOTH_LOGO_STEP, origin.y + 4 * BLUETOOTH_LOGO_STEP), 1);

    graphics_draw_line_with_width(ctx, GPoint(origin.x + 0, origin.y + BLUETOOTH_LOGO_STEP),
                                  GPoint(origin.x + 2 * BLUETOOTH_LOGO_STEP, origin.y + 3 * BLUETOOTH_LOGO_STEP), 1);
    graphics_draw_line_with_width(ctx, GPoint(origin.x + 0, origin.y + 3 * BLUETOOTH_LOGO_STEP),
                                  GPoint(origin.x + 2 * BLUETOOTH_LOGO_STEP, origin.y + BLUETOOTH_LOGO_STEP), 1);

    graphics_draw_line_with_width(ctx, GPoint(origin.x + BLUETOOTH_LOGO_STEP, origin.y + 0),
                                  GPoint(origin.x + 2 * BLUETOOTH_LOGO_STEP, origin.y + BLUETOOTH_LOGO_STEP), 1);
    graphics_draw_line_with_width(ctx, GPoint(origin.x + BLUETOOTH_LOGO_STEP, origin.y + 4 * BLUETOOTH_LOGO_STEP),
                                  GPoint(origin.x + 2 * BLUETOOTH_LOGO_STEP, origin.y + 3 * BLUETOOTH_LOGO_STEP), 1);
#ifdef PBL_COLOR
    graphics_context_set_antialiased(ctx, true);
#endif
}

/**
 * Draws a popup about the bluetooth connection
 */
void bluetooth_popup(GContext *ctx, bool connected) {
#ifndef DEBUG_BLUETOOTH_POPUP
    if (!show_bluetooth_popup) return;
#endif

    int16_t yoffset = PBL_IF_ROUND_ELSE(70, 50);
    graphics_context_set_fill_color(ctx, GColorBlack);
    GRect notification_rect = GRect(-10, height - yoffset - 7, width + 20, 50);
    graphics_fill_rect(ctx, notification_rect, 0, GCornersAll);
    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_fill_rect(ctx, GRect(-10, height - yoffset - 3, width + 20, 50 - 8), 0, GCornersAll);
    graphics_context_set_text_color(ctx, GColorBlack);
    // TODO
//    graphics_draw_text(ctx, connected ? "Bluetooth Connected" : "Bluetooth Disconnected", font_system_18px_bold,
//                       GRect(PBL_IF_ROUND_ELSE(22, 2), notification_rect.origin.y + 4, 105, 40),
//                       GTextOverflowModeWordWrap, GTextAlignmentCenter,
//                       NULL);
    if (connected) {
        graphics_context_set_fill_color(ctx, COLOR_FALLBACK(GColorGreen, GColorBlack));
    } else {
        graphics_context_set_fill_color(ctx, COLOR_FALLBACK(GColorRed, GColorBlack));
    }

    GPoint icon_center = GPoint(PBL_IF_ROUND_ELSE(135, 120) + 3,
                                notification_rect.origin.y + notification_rect.size.h - 26);
    graphics_fill_circle(ctx, icon_center, 9);
    graphics_context_set_stroke_color(ctx, GColorWhite);

    if (connected) {
        graphics_draw_line_with_width(ctx,
                                      GPoint(icon_center.x + 4, icon_center.y - 3),
                                      GPoint(icon_center.x - 2, icon_center.y + 3), 2);
        graphics_draw_line_with_width(ctx,
                                      GPoint(icon_center.x - 4, icon_center.y + 0),
                                      GPoint(icon_center.x - 2, icon_center.y + 3), 2);
    } else {
        graphics_draw_line_with_width(ctx,
                                      GPoint(icon_center.x + 3, icon_center.y - 3),
                                      GPoint(icon_center.x - 3, icon_center.y + 3), 2);
        graphics_draw_line_with_width(ctx,
                                      GPoint(icon_center.x - 3, icon_center.y - 3),
                                      GPoint(icon_center.x + 3, icon_center.y + 3), 2);
    }
}

#ifdef DEBUG_ITER_COUNTER
int debug_iter = 0;
#endif

/** Array of candidate points to draw the weather string at */
static GPoint w_points[] = {
        {0,                            0},
        {10 + PBL_IF_ROUND_ELSE(2, 0), 3},
        {17 + PBL_IF_ROUND_ELSE(3, 0), 7},
        {26 + PBL_IF_ROUND_ELSE(4, 0), 13},
        {29 + PBL_IF_ROUND_ELSE(5, 0), 19},
        {33 + PBL_IF_ROUND_ELSE(6, 0), 25},
        {33 + PBL_IF_ROUND_ELSE(6, 0), 32},
        {33 + PBL_IF_ROUND_ELSE(6, 0), 39},
};
/** Array of candidate points to draw the date string at */
static GPoint d_points[] = {
        {0,  16},
        {10, 16 - 3},
        {17, 16 - 7},
        {26, 16 - 13},
        {29, 16 - 19},
        {33, 16 - 25},
        {33, 16 - 32},
        {33, 16 - 39},
};

void draw_string(FContext *fctx, char *str, GPoint position, FFont *font, GColor color, uint8_t size, bool center) {
    fctx_begin_fill(fctx);
    fctx_set_fill_color(fctx, color);
    fctx_set_color_bias(fctx, 0);
    fctx_set_pivot(fctx, FPointZero);
    FPoint pos;
    pos.x = INT_TO_FIXED(position.x + width / 2);
    pos.y = INT_TO_FIXED(position.y + 5);
    fctx_set_offset(fctx, pos);
    fctx_set_rotation(fctx, 0);
    fctx_set_text_em_height(fctx, font, size);
    fctx_draw_string(fctx, str, font, center ? GTextAlignmentCenter : GTextAlignmentLeft, FTextAnchorCapTop);
    fctx_end_fill(fctx);
}

fixed_t string_width(FContext *fctx, char* str, FFont* font, int size) {
    if (str[0] == 0) return 0;
    fctx_set_text_em_height(fctx, font, size);
    return FIXED_TO_INT(fctx_string_width(fctx, str, font));
}

#ifdef PBL_ROUND
/** Array of candidate points to draw the battery information */
static GPoint b_points[] = {
        {0, 0},
        {4, 1},
        {8, 2},
        {12, 4},
        {16, 6},
        {20, 9},
        {24, 12},
};
#endif

void remove_leading_zero(char* buffer, size_t length) {
    bool last_was_space = true;
    int i = 0;
    while (buffer[i] != 0) {
        if (buffer[i] == '0' && last_was_space) {
            memcpy(&buffer[i], &buffer[i+1], length - (i + 1));
        }
        last_was_space = buffer[i] == ' ' || buffer[i] == '.' || buffer[i] == '/';
        i += 1;
    }
}

/**
 * Update procedure for the background
 */
void background_update_proc(Layer *layer, GContext *ctx) {

    // initialize fctx
    FContext fctx;
    fctx_init_context(&fctx, ctx);

//    APP_LOG(APP_LOG_LEVEL_DEBUG, "drawing...");

    // update bounds
    GRect bounds = layer_get_unobstructed_bounds(layer_background);
    center = grect_center_point(&bounds);
    height = bounds.size.h;
    width = bounds.size.w;

    int16_t radius = width / 2;
    bool bluetooth = bluetooth_connection_service_peek();
#ifdef DEBUG_NO_BLUETOOTH
    bluetooth = false;
#endif
    BatteryChargeState battery_state = battery_state_service_peek();
#ifdef SCREENSHOT_BATTERY_LOW_1
    battery_state.charge_percent = 30;
#endif
#ifdef SCREENSHOT_BATTERY_LOW_2
    battery_state.charge_percent = 20;
#endif
#ifdef SCREENSHOT_BATTERY_LOW_3
    battery_state.charge_percent = 10;
#endif
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

#ifdef DEBUG_NICE_TIME
    t->tm_min = 10;
    t->tm_hour = 10;
#endif
#ifdef DEBUG_NICE_DATE
#define DEBUG_NICE_WEATHER
    t->tm_mday = 8;
    t->tm_mon = 4;
    t->tm_year = 2015;
    t->tm_wday = 6;
#endif
#ifdef DEBUG_NICE_WEATHER
    weather.timestamp = now;
    weather.icon = (uint8_t)'a';
    weather.temperature = 74;
#endif
#ifdef SCREENSHOT_DATE_1
    t->tm_min = 26;
    t->tm_hour = 10;
#endif
#ifdef SCREENSHOT_DATE_2
    t->tm_min = 31;
    t->tm_hour = 0;
#endif
#ifdef SCREENSHOT_WEATHER_1
    weather.icon = (uint8_t)'B';
    weather.temperature = 59;
#endif
#ifdef SCREENSHOT_WEATHER_2
    weather.icon = (uint8_t)'h';
    weather.temperature = -12;
#endif

    // background
#ifndef PBL_ROUND
    if (!config_square) {
        uint8_t outer_color = config_color_outer_background;
        if (battery_state.charge_percent <= 10 && !battery_state.is_charging && !battery_state.is_plugged) {
            outer_color = config_color_battery_bg_10;
        } else if (battery_state.charge_percent <= 20 && !battery_state.is_charging && !battery_state.is_plugged) {
            outer_color = config_color_battery_bg_20;
        } else if (battery_state.charge_percent <= 30 && !battery_state.is_charging && !battery_state.is_plugged) {
            outer_color = config_color_battery_bg_30;
        }
        graphics_context_set_fill_color(ctx, COLOR(outer_color));
        graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
    } else {
        graphics_context_set_fill_color(ctx, COLOR(config_color_inner_background));
        graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
    }
#endif

    // battery
//    const int max_battery_points = 50;
//    graphics_context_set_stroke_color(ctx, COLOR_NORMAL);
//    graphics_context_set_fill_color(ctx, COLOR_NORMAL);
//    graphics_context_set_stroke_width(ctx, 1);
//    const int cur_battery_points = battery_state_service_peek().charge_percent * max_battery_points / 100;
//    for (int i = 0; i < cur_battery_points; ++i) {
//        int32_t angle = i * TRIG_MAX_ANGLE / max_battery_points;
////        graphics_draw_line(ctx, get_radial_point(radius, angle), get_radial_point(radius + 9, angle));
//        graphics_fill_circle(ctx, get_radial_point(radius+5, angle), 3);
//    }

    // background
#if !defined(PBL_ROUND)
    if (!config_square) {
        graphics_context_set_fill_color(ctx, COLOR(config_color_circle));
        graphics_fill_circle(ctx, center, (uint16_t) (radius + 3 + 2));
        graphics_context_set_fill_color(ctx, COLOR(config_color_inner_background));
        graphics_fill_circle(ctx, center, (uint16_t) radius);
    }
#else
    uint8_t inner_color = config_color_inner_background;
    if (battery_state.charge_percent <= 10 && !battery_state.is_charging && !battery_state.is_plugged) {
        inner_color = config_color_battery_bg_10;
    } else if (battery_state.charge_percent <= 20 && !battery_state.is_charging && !battery_state.is_plugged) {
        inner_color = config_color_battery_bg_20;
    } else if (battery_state.charge_percent <= 30 && !battery_state.is_charging && !battery_state.is_plugged) {
        inner_color = config_color_battery_bg_30;
    }
    graphics_context_set_fill_color(ctx, COLOR(inner_color));
    graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
#endif

    // numbers
#if defined(OBSIDIAN_SHOW_NUMBERS) || defined(OBSIDIAN_ONLY_RELEVANT_NUMBER)
    static const GPoint number_points[] = {
            {width / 2 - 9,       168 - height + 26}, // 12
            {width / 2 + 23,      168 - height + 28}, // 1
            {width / 2 + 45,      168 - height + 47}, // 2
            {width / 2 + 49,      168 - height + 77}, // 3
            {width / 2 + 45,      168 - height + 102}, // 4
            {width / 2 + 24,      168 - height + 124}, // 5
            {width / 2 - 4,       168 - height + 128}, // 6
            {width / 2 - 6 - 23,  168 - height + 124}, // 7
            {width / 2 - 6 - 43,  168 - height + 103}, // 8
            {width / 2 - 6 - 51,  168 - height + 77}, // 9
            {width / 2 - 12 - 43, 168 - height + 48}, // 10
            {width / 2 - 12 - 23, 168 - height + 28}, // 11
    };
    static const char *numbers[] = {
            "12",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10",
            "11",
    };
    graphics_context_set_text_color(ctx, GColorDarkGray);
#ifdef OBSIDIAN_ONLY_RELEVANT_NUMBER
    {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        int i = t->tm_hour % 12;
#else
    for (unsigned i = 0; i < ARRAY_LENGTH(number_points); i++) {
#endif
        graphics_draw_text(ctx, numbers[i], font_open_sans,
                           GRect(number_points[i].x, number_points[i].y, strlen(numbers[i]) > 1 ? 18 : 9, 22),
                           GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
    }
#endif

    // hour ticks
    uint8_t tick_width = 2;
    if (config_hour_ticks != 3) {
        if (!config_square) {
            for (int i = 0; i < 12; ++i) {
                if (config_hour_ticks == 2 && (i % 3) != 0) continue;

                int32_t angle = i * TRIG_MAX_ANGLE / 12;
                int tick_length = PBL_IF_ROUND_ELSE(8, 6);
#ifdef OBSIDIAN_LONG_TICKS
                if (i % 3 == 0) {
                    tick_length = PBL_IF_ROUND_ELSE(12, 10);
#ifdef OBSIDIAN_FAT_TICKS
                    tick_width = 4;
                } else {
                    tick_width = 2;
#endif
                }
#endif

                graphics_draw_line_with_width(ctx, get_radial_point(radius + PBL_IF_ROUND_ELSE(3, 0), angle),
                                              get_radial_point(radius - tick_length, angle),
                                              tick_width);
            }
        } else {
            for (int i = 0; i < 12; ++i) {
                if (config_hour_ticks == 2 && (i % 3) != 0) continue;
                int32_t angle = i * TRIG_MAX_ANGLE / 12;
                int tick_length = 8;
                graphics_draw_line_with_width(ctx, get_radial_border_point(0, angle),
                                              get_radial_border_point(tick_length, angle), 4);
            }
        }
    }

    // minute ticks
    graphics_context_set_stroke_color(ctx, COLOR(config_color_ticks));
    int square_minute_tick = 4;
    if (config_minute_ticks == 2) {
        // only relevant minute ticks
        int start_min_tick = (t->tm_min / 5) * 5;
        for (int i = start_min_tick; i < start_min_tick + 5; ++i) {
            int32_t angle = i * TRIG_MAX_ANGLE / 60;
            if (!config_square) {
                if (i == t->tm_sec % 60 && config_seconds != 0) {
                    draw_pointer(ctx, get_radial_point(radius + PBL_IF_ROUND_ELSE(3, 0), angle), 10, 12, angle,
                                 COLOR(config_color_seconds));
                } else {
                    graphics_draw_line_with_width(ctx, get_radial_point(radius + PBL_IF_ROUND_ELSE(3, 0), angle),
                                                  get_radial_point(radius - PBL_IF_ROUND_ELSE(5, 3), angle), 1);
                }
            } else {
                graphics_draw_line_with_width(ctx, get_radial_border_point(0, angle),
                                              get_radial_border_point(square_minute_tick, angle), 1);
            }

        }
    } else if (config_minute_ticks == 1) {
        // all minute ticks
        for (int i = 0; i < 60; ++i) {
            int32_t angle = i * TRIG_MAX_ANGLE / 60;
            if (!config_square) {
                if (i == t->tm_sec % 60 && config_seconds != 0) {
                    draw_pointer(ctx, get_radial_point(radius + PBL_IF_ROUND_ELSE(3, 0), angle), 10, 12, angle,
                                 COLOR(config_color_seconds));
                } else {
                    graphics_draw_line_with_width(ctx, get_radial_point(radius + PBL_IF_ROUND_ELSE(3, 0), angle),
                                                  get_radial_point(radius - PBL_IF_ROUND_ELSE(5, 3), angle), 1);
                }
            } else {
                graphics_draw_line_with_width(ctx, get_radial_border_point(0, angle),
                                              get_radial_border_point(square_minute_tick, angle), 1);
            }
        }
    }

#ifdef DEBUG_DATE_POSITION
    t->tm_hour = 5;
    t->tm_min = (debug_iter + 25) % 60;
    t->tm_mon = 4;
    t->tm_mon = 10;
    t->tm_mday = 10;
#endif
#ifdef DEBUG_WEATHER_POSITION
    t->tm_hour = 6;
    t->tm_min = (debug_iter + 55) % 60;
#endif

// determining which numbers/months are widest
//#define DEBUG_WIDTH_OF_STRINGS
#ifdef DEBUG_WIDTH_OF_STRINGS
    int widest_num = 0;
    int widest_num3 = 0;
    int widest_num_tmp = 0;
    int widest_num_tmp3 = 0;
    for (int i = 0; i < 10; i++) {
        snprintf(buffer_1, 10, "%d", i);
        int w = string_width(&fctx, buffer_1, font_main, 18);
        if (w > widest_num_tmp) {
            widest_num_tmp = w;
            widest_num = i;
        }
        if (i > 0 && i < 4 && w > widest_num_tmp3) {
            widest_num_tmp3 = w;
            widest_num3 = i;
        }
    }
    char* months[] = {
            "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    int widest_month_tmp = 0;
    char* widest_month = NULL;
    int widest_month_int = 0;
    int widest_month_int_tmp = 0;
    for (int i = 0; i < 12; i++) {
        snprintf(buffer_1, 10, "%s %d%d", months[i], widest_num3, widest_num);
        int w = string_width(&fctx, buffer_1, font_main, 18);
        if (w > widest_month_tmp) {
            widest_month_tmp = w;
            widest_month = months[i];
        }

        snprintf(buffer_1, 10, "%d/%d%d", i+1, widest_num3, widest_num);
        w = string_width(&fctx, buffer_1, font_main, 18);
        if (w > widest_month_int_tmp) {
            widest_month_int_tmp = w;
            widest_month_int = i + 1;
        }
    }
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Widest number: %d", widest_num); // 0
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Widest number below 3: %d", widest_num3); // 1
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Widest month: %s (%d)", widest_month, widest_month_tmp); // May 10 at 51
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Widest month: %d (%d)", widest_month_int, widest_month_int_tmp); // 10/10 at 40
    APP_LOG(APP_LOG_LEVEL_DEBUG, "OK width: %d", (int)string_width(&fctx, "May 10", font_main, 20)); // 57
    APP_LOG(APP_LOG_LEVEL_DEBUG, "OK width: %d", (int)string_width(&fctx, "10/10", font_main, 24));
    APP_LOG(APP_LOG_LEVEL_DEBUG, "OK width: %d", (int)string_width(&fctx, "10:10", font_main, 24));
#endif

    // compute angles
    int32_t minute_angle = t->tm_min * TRIG_MAX_ANGLE / 60;
    GPoint minute_hand = get_radial_point(radius - PBL_IF_ROUND_ELSE(16, 10), minute_angle);
    int hour_tick = ((t->tm_hour % 12) * 6) + (t->tm_min / 10);
    int32_t hour_angle = hour_tick * TRIG_MAX_ANGLE / (12 * 6);
    GPoint hour_hand = get_radial_point(radius * 55 / 100, hour_angle);

    // format date strings
    char* format_1 = NULL;
    char* format_2 = NULL;
    int format = 9;
    switch(format) {
        case 0: // Mon // Oct 22 (date)
            format_1 = "%b %d";
            format_2 = "%a";
            break;
        case 1: // Oct 22 (date)
            format_1 = "%b %d";
            break;
        case 2: // 10/22 (date)
            format_1 = "%m/%d";
            break;
        case 3: // 22.10. (date)
            format_1 = "%d.%m.";
            break;
        case 4: // 22 (date)
            format_1 = "%d";
            break;
        case 5: // Mon 22 (date)
            format_1 = "%a %d";
            break;
        case 6: // Mon (day)
            format_1 = "%a";
            break;
        case 7: // 14:10 (time 24h)
            format_1 = "%H:%M";
            break;
        case 8: // 2:10 (time 12h)
            format_1 = "%I:%M";
            break;
        case 9: // 2:10 // 10/22 (date/time)
            format_1 = "%m/%d";
            format_2 = "%I:%M";
            break;
        case 10: // 14:10 // 10/22 (date/time)
            format_1 = "%m/%d";
            format_2 = "%H:%M";
            break;
        case 11: // Mon // 10/22 (date/time)
            format_1 = "%m/%d";
            format_2 = "%a";
            break;
        case 12: // 2:10 // 22.10. (date/time)
            format_1 = "%d.%m.";
            format_2 = "%I:%M";
            break;
        case 13: // 14:10 // 22.10. (date/time)
            format_1 = "%d.%m.";
            format_2 = "%H:%M";
            break;
        case 14: // Mon // 22.10. (date/time)
            format_1 = "%d.%m.";
            format_2 = "%a";
            break;
    }

    setlocale(LC_ALL, "");
    strftime(buffer_1, sizeof(buffer_1), format_1, t);
    if (format_2 == NULL) {
        buffer_2[0] = 0;
    } else {
        strftime(buffer_2, sizeof(buffer_2), format_2, t);
    }
    // remove leading zeros
    if (format != 7) {
        remove_leading_zero(buffer_1, sizeof(buffer_1));
        remove_leading_zero(buffer_2, sizeof(buffer_1));
    }

    bool big = buffer_2[0] == 0;
    int date_font_size = 18;
    if (big) {
        if (format == 0) {
            // Jan 1
            date_font_size = 20;
        } else {
            // other one-line formats
            date_font_size = 24;
        }
    }

    // determine size
    const int d_w1 = string_width(&fctx, buffer_2, font_main, date_font_size);
    const int d_w2 = string_width(&fctx, buffer_1, font_main, date_font_size);

    // determine where we can draw the date without overlap
    const int d_offset = PBL_IF_ROUND_ELSE(20, 15) + (big ? -10 : 0);
    const int d_height = date_font_size;
    const int d_y_start = height / 2;
    bool found = false;
    uint16_t i;
    GPoint d_center;
    GRect date_pos;
    GRect day_pos;
    const int border = 2;
    // loop through all points and use the first one that doesn't overlap with the watch hands
    for (i = 0; i < 1 + (ARRAY_LENGTH(d_points) - 1) * 2; i++) {
        d_center = d_points[(i + 1) / 2];
        if (i % 2 == 0) {
            d_center.x = -d_center.x;
        }
#ifdef PBL_ROUND
        d_center.x = d_center.x * 7 / 6;
        //d_center.y = d_center.y * 7 / 6;
#endif
        date_pos = GRect(d_center.x, d_y_start + d_center.y + d_offset, width, d_height);
        day_pos = GRect(d_center.x, d_y_start + d_center.y, width, d_height);

        if (!(line2_rect_intersect(center, hour_hand, center, minute_hand,
                                   GPoint(width / 2 + d_center.x - d_w1 / 2 - border,
                                          d_y_start + d_center.y),
                                   GPoint(width / 2 + d_center.x + d_w1 / 2 + border,
                                          d_y_start + d_center.y + d_height + 2*border)) ||
              line2_rect_intersect(center, hour_hand, center, minute_hand,
                                   GPoint(width / 2 + d_center.x - d_w2 / 2 - border,
                                          d_y_start + d_center.y + d_offset),
                                   GPoint(width / 2 + d_center.x + d_w2 / 2 + border,
                                          d_y_start + d_center.y + d_height + d_offset + 2*border)))) {
            found = true;
            break;
        }
    }

    // this should not happen, but if it does, then use the default position
    if (!found) {
        d_center = d_points[0];
        day_pos = GRect(d_center.x, d_y_start + d_center.y, width, d_height);
        date_pos = GRect(d_center.x, d_y_start + d_center.y + d_offset, width, d_height);
    }

//    // show bounding box
//    if (!big) {
//        graphics_draw_rect(ctx, GRect(width / 2 + d_center.x - d_w1 / 2 - border,
//                                      d_y_start + d_center.y,
//                                      d_w1 + 2*border,
//                                      d_height+ 2* border));
//    }
//    graphics_draw_rect(ctx, GRect(width / 2 + d_center.x - d_w2 / 2 - border,
//                                  d_y_start + d_center.y + d_offset,
//                                  d_w2 + 2*border,
//                                  d_height+ 2* border));

    // actuallyl draw the date text
#ifndef DEBUG_NO_DATE
    if (!big) {
        draw_string(&fctx, buffer_2, day_pos.origin, font_main, COLOR(config_color_day_of_week), 18, true);
    }
#endif
    draw_string(&fctx, buffer_1, date_pos.origin, font_main, COLOR(config_color_date), date_font_size, true);

    // weather information
    bool weather_is_on = config_weather_refresh > 0;
    bool weather_is_available = weather.timestamp > 0;
    bool weather_is_outdated = (time(NULL) - weather.timestamp) > (config_weather_expiration * 60);
    bool show_weather = weather_is_on && weather_is_available && !weather_is_outdated;
    if (show_weather ||
        #ifdef PBL_ROUND
        (!bluetooth && config_bluetooth_logo)
        #else
        false
#endif
            ) {
        int temp = weather.temperature;
        if (weather.failed) {
#ifdef PBL_ROUND
            if (!show_weather) {
                snprintf(buffer_1, 10, "z");
                buffer_2[0] = 0;
            } else if (!bluetooth && config_bluetooth_logo) {
                snprintf(buffer_1, 10, "z%c", weather.icon);
                snprintf(buffer_2, 10, "%d", temp);
            } else {
#endif
            snprintf(buffer_1, 10, "%c", weather.icon);
            snprintf(buffer_2, 10, "%d", temp);
#ifdef PBL_ROUND
            }
#endif
        } else {
#ifdef PBL_ROUND
            if (!show_weather) {
                snprintf(buffer_1, 10, "z");
                buffer_2[0] = 0;
            } else if (!bluetooth && config_bluetooth_logo) {
                snprintf(buffer_1, 10, "z%c", weather.icon);
                snprintf(buffer_1, 10, "%d°", temp);
            } else {
#endif
            snprintf(buffer_1, 10, "%c", weather.icon);
            snprintf(buffer_2, 10, "%d°", temp);
#ifdef PBL_ROUND
            }
#endif
        }
        GPoint w_center;
        const int w_border = 2;
        const int w_x = width / 2;
        const int w_y = PBL_IF_ROUND_ELSE(36, height / 2 - 48);

        const int w_font_size2 = 18;
        const int w_font_size1 = w_font_size2 + w_font_size2*10/34;
        const int w_height = w_font_size2;
        const int w_w1 = string_width(&fctx, buffer_1, font_weather, w_font_size1);
        const int w_w2 = string_width(&fctx, buffer_2, font_main, w_font_size2);
        const int w_w = w_w1 + w_w2 + 1;

        GSize weather_size = GSize(w_w, w_height);
        // loop through all points and use the first one that doesn't overlap with the watch hands
        for (i = 0; i < 1 + (ARRAY_LENGTH(w_points) - 1) * 2; i++) {
//            i = debug_iter % ARRAY_LENGTH(w_points);
//            i = i*2+1;
            w_center = w_points[(i + 1) / 2];
            if (i % 2 == 0) {
                w_center.x = -w_center.x;
            }
//            break;
            if (!line2_rect_intersect(center, hour_hand, center, minute_hand,
                                      GPoint(w_x + w_center.x - weather_size.w / 2 - w_border,
                                             w_y + w_center.y),
                                      GPoint(w_x + w_center.x + weather_size.w / 2 + w_border,
                                             w_y + w_center.y + w_height + 2 * w_border))) {
                found = true;
                break;
            }
        }

        // this should not happen, but if it does, then use the default position
        if (!found) {
            w_center = w_points[0];
        }
        GPoint pos1 = GPoint(w_center.x - w_w / 2, w_y + w_center.y + 2);
        GPoint pos2 = GPoint(w_center.x - w_w / 2 + w_w1 + 1, w_y + w_center.y);
        draw_string(&fctx, buffer_1, pos1, font_weather, COLOR(config_color_weather), w_font_size1, false);
        draw_string(&fctx, buffer_2, pos2, font_main, COLOR(config_color_weather), w_font_size2, false);

//        // show bounding box
//        graphics_draw_rect(ctx, GRect(w_x + w_center.x - weather_size.w / 2 - w_border,
//                                      w_y + w_center.y,
//                                      weather_size.w+2*w_border,
//                                      weather_size.h+2*w_border));
    }

    snprintf(buffer_1, 5, "21");

    // bluetooth status
    if (!bluetooth && config_bluetooth_logo) {
#ifndef PBL_ROUND
        draw_bluetooth_logo(ctx, GPoint(9, 9));
#endif
    }

    // minute hand
    graphics_context_set_stroke_color(ctx, COLOR(config_color_minute_hand));
    graphics_draw_line_with_width(ctx, minute_hand, center, 4);
    graphics_context_set_stroke_color(ctx, COLOR(config_color_inner_minute_hand));
    graphics_draw_line_with_width(ctx, get_radial_point(radius - PBL_IF_ROUND_ELSE(20, 12), minute_angle), center, 1);

    // hour hand
    graphics_context_set_stroke_color(ctx, COLOR(config_color_hour_hand));
    graphics_draw_line_with_width(ctx, hour_hand, center, 4);
    graphics_context_set_stroke_color(ctx, COLOR(config_color_inner_hour_hand));
    graphics_draw_line_with_width(ctx, get_radial_point(radius * 55 / 100 - 2, hour_angle), center, 1);

    // dot in the middle
    graphics_context_set_fill_color(ctx, COLOR(config_color_minute_hand));
    graphics_fill_circle(ctx, center, 5);
    graphics_context_set_fill_color(ctx, COLOR(config_color_inner_background));
    graphics_fill_circle(ctx, center, 2);

    // battery status
    if (!is_obstructed()) {
        if (config_battery_logo == 1 ||
            (config_battery_logo == 2 && battery_state.charge_percent <= 30 && !battery_state.is_charging &&
             !battery_state.is_plugged)) {
            GRect battery = PBL_IF_ROUND_ELSE(GRect((width
                                                      -14)/2, 21, 14, 8), GRect(125, 3, 14, 8));
            if (config_square) {
                battery.origin.x = 123;
                battery.origin.y = 7;
            }
#ifdef PBL_ROUND
            // determine where we can draw the bluetooth logo without overlap
            GPoint b_center;
            const int b_x = width / 2;
            const int b_y = 25;
            const int b_border = 3;
            // loop through all points and use the first one that doesn't overlap with the watch hands
            for (i = 0; i < 1 + (ARRAY_LENGTH(b_points) - 1) * 2; i++) {
                b_center = b_points[(i + 1) / 2];
                if (i % 2 == 0) {
                    b_center.x = -b_center.x;
                }
                if (!line2_rect_intersect(center, hour_hand, center, minute_hand,
                                          GPoint(b_x + b_center.x - battery.size.w/2 - b_border, b_y + b_center.y - b_border),
                                          GPoint(b_x + b_center.x + battery.size.w/2 + b_border, b_y + b_center.y + battery.size.h + b_border))) {
                    break;
                }
            }
            battery = GRect(b_x + b_center.x - battery.size.w/2, b_y + b_center.y, battery.size.w, battery.size.h);
#endif
            uint8_t battery_color = config_color_battery_logo;
            if (battery_state.charge_percent <= 10 && !battery_state.is_charging && !battery_state.is_plugged) {
                battery_color = config_color_battery_10;
            } else if (battery_state.charge_percent <= 20 && !battery_state.is_charging && !battery_state.is_plugged) {
                battery_color = config_color_battery_20;
            } else if (battery_state.charge_percent <= 30 && !battery_state.is_charging && !battery_state.is_plugged) {
                battery_color = config_color_battery_30;
            }
            graphics_context_set_stroke_color(ctx, COLOR(battery_color));
            graphics_context_set_fill_color(ctx, COLOR(battery_color));
            graphics_draw_rect(ctx, battery);
            graphics_fill_rect(ctx,
                               GRect(battery.origin.x + 2, battery.origin.y + 2, battery_state.charge_percent / 10, 4),
                               0, GCornerNone);
            graphics_draw_line_with_width(ctx, GPoint(battery.origin.x + battery.size.w, battery.origin.y + 2),
                                          GPoint(battery.origin.x + battery.size.w, battery.origin.y + 5), 1);
        }
    }

    // draw the bluetooth popup
    bluetooth_popup(ctx, bluetooth);

    // end fctx
    fctx_deinit_context(&fctx);
}

