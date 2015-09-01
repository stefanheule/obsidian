#include <pebble.h>

// I don't know how to pass parameters to the compiler, so I'm using this file
// for various configurations
#include "config.h"


////////////////////////////////////////////
//// Configuration constants
////////////////////////////////////////////

#define NUM_CONFIG 24

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
#define CONFIG_COLOR_BATTERY_30 13
#define CONFIG_COLOR_BATTERY_20 14
#define CONFIG_COLOR_BATTERY_10 15
#define CONFIG_COLOR_BLUETOOTH_LOGO 16
#define CONFIG_COLOR_BLUETOOTH_LOGO_2 17
#define CONFIG_BLUETOOTH_LOGO 18
#define CONFIG_VIBRATE_DISCONNECT 19
#define CONFIG_VIBRATE_RECONNECT 20
#define CONFIG_MESSAGE_DISCONNECT 21
#define CONFIG_MESSAGE_RECONNECT 22
#define CONFIG_MINUTE_TICKS 23
#define CONFIG_HOUR_TICKS 24

////////////////////////////////////////////
//// Default values for the configuration
////////////////////////////////////////////

static uint8_t config_color_outer_background = GColorDarkGrayARGB8;
static uint8_t config_color_inner_background = GColorWhiteARGB8;
static uint8_t config_color_minute_hand = GColorBlackARGB8;
static uint8_t config_color_inner_minute_hand = GColorLightGrayARGB8;
static uint8_t config_color_hour_hand = GColorJaegerGreenARGB8;
static uint8_t config_color_inner_hour_hand = GColorLightGrayARGB8;
static uint8_t config_color_circle = GColorBlackARGB8;
static uint8_t config_color_ticks = GColorBlackARGB8;
static uint8_t config_color_day_of_week = GColorJaegerGreenARGB8;
static uint8_t config_color_date = GColorBlackARGB8;
static uint8_t config_battery_logo = 1;
static uint8_t config_color_battery_logo = GColorBlackARGB8;
static uint8_t config_color_battery_30 = GColorYellowARGB8;
static uint8_t config_color_battery_20 = GColorOrangeARGB8;
static uint8_t config_color_battery_10 = GColorRedARGB8;
static uint8_t config_color_bluetooth_logo = GColorJaegerGreenARGB8;
static uint8_t config_color_bluetooth_logo_2 = GColorWhiteARGB8;
static uint8_t config_bluetooth_logo = true;
static uint8_t config_vibrate_disconnect = true;
static uint8_t config_vibrate_reconnect = true;
static uint8_t config_message_disconnect = true;
static uint8_t config_message_reconnect = true;
static uint8_t config_minute_ticks = 1;
static uint8_t config_hour_ticks = 1;


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


////////////////////////////////////////////
//// Global variables
////////////////////////////////////////////

/** A pointer to our window, for later deallocation. */
static Window *window;

/** All layers */
static Layer *layer_background;

/** Buffers for strings */
static char buffer_1[30];
static char buffer_2[30];

/** The center of the watch */
static GPoint center;

#ifdef OBSIDIAN_SHOW_NUMBERS
/** Open Sans font. */
static GFont font_open_sans;
#endif

/** System font. */
static GFont font_system_18px_bold;

/** Is the bluetooth popup current supposed to be shown? */
static bool show_bluetooth_popup;

/** The timer for the bluetooth popup */
AppTimer *timer_bluetooth_popup;

/** Sync state */
static AppSync sync;

/** Buffer for syncing */
static uint8_t sync_buffer[NUM_CONFIG * 8 + 2];


////////////////////////////////////////////
//// screenshot configurations
////////////////////////////////////////////

#ifdef SCREENSHOT_MENU_ICON
#define DEBUG_NICE_TIME
#define DEBUG_NO_DATE
#define DEBUG_NO_BATTERY_ICON
#endif

#ifdef SCREENSHOT_MAIN
#define DEBUG_NICE_TIME
#define DEBUG_NICE_DATE
#endif

#ifdef SCREENSHOT_BATTERY_LOW_1
#define DEBUG_NICE_TIME
#define DEBUG_NICE_DATE
#endif

#ifdef SCREENSHOT_BATTERY_LOW_2
#define DEBUG_NICE_TIME
#define DEBUG_NICE_DATE
#endif

#ifdef SCREENSHOT_BATTERY_LOW_3
#define DEBUG_NICE_TIME
#define DEBUG_NICE_DATE
#endif

#ifdef SCREENSHOT_DATE_1
#define DEBUG_NICE_DATE
#endif

#ifdef SCREENSHOT_DATE_2
#define DEBUG_NICE_DATE
#endif

#ifdef SCREENSHOT_BLUETOOTH_ICON
#define DEBUG_NICE_TIME
#define DEBUG_NICE_DATE
#define DEBUG_NO_BLUETOOTH
#endif

#ifdef SCREENSHOT_BLUETOOTH_POPUP_1
#define DEBUG_NICE_TIME
#define DEBUG_NICE_DATE
#define DEBUG_BLUETOOTH_POPUP
#endif

#ifdef SCREENSHOT_BLUETOOTH_POPUP_2
#define DEBUG_NICE_TIME
#define DEBUG_NICE_DATE
#define DEBUG_NO_BLUETOOTH
#define DEBUG_BLUETOOTH_POPUP
#endif

#ifdef SCREENSHOT_ALT_THEME_1
#define DEBUG_NICE_TIME
#define DEBUG_NICE_DATE
#endif

#ifdef SCREENSHOT_ALT_THEME_2
#define DEBUG_NICE_TIME
#define DEBUG_NICE_DATE
#endif

#ifdef SCREENSHOT_ALT_THEME_3
#define DEBUG_NICE_TIME
#define DEBUG_NICE_DATE
#endif

#ifdef SCREENSHOT_ALT_THEME_4
#define DEBUG_NICE_TIME
#define DEBUG_NICE_DATE
#endif

#ifdef SCREENSHOT_ALT_THEME_5
#define DEBUG_NICE_TIME
#define DEBUG_NICE_DATE
#endif

#ifdef SCREENSHOT_ALT_THEME_6
#define DEBUG_NICE_TIME
#define DEBUG_NICE_DATE
#endif

#ifdef SCREENSHOT_ALT_THEME_7
#define DEBUG_NICE_TIME
#define DEBUG_NICE_DATE
#endif

#ifdef SCREENSHOT_ALT_THEME_8
#define DEBUG_NICE_TIME
#define DEBUG_NICE_DATE
#endif



////////////////////////////////////////////
//// Implementation
////////////////////////////////////////////

#define LOG(fmt, args...) \
  do { \
    char buffer[80]; \
    snprintf(buffer, ARRAY_LENGTH(buffer), fmt, ## args); \
    graphics_context_set_text_color(ctx, COLOR_ACCENT); \
    graphics_context_set_fill_color(ctx, GColorWhite); \
    graphics_fill_rect(ctx, GRect(0, 0, 144, 60), 0, GCornerNone); \
    graphics_draw_text(ctx, buffer, font_system_18px_bold, GRect(5, 0, 144-2*5, 50), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL); \
  } while (0)
#define LOG2(fmt, args...) \
  do { \
    char buffer[80]; \
    snprintf(buffer, ARRAY_LENGTH(buffer), fmt, ## args); \
    graphics_context_set_text_color(ctx, COLOR_ACCENT); \
    graphics_draw_text(ctx, buffer, font_system_18px_bold, GRect(5, 21, 144-2*5, 50), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL); \
  } while (0)
#define LOG3(fmt, args...) \
  do { \
    char buffer[80]; \
    snprintf(buffer, ARRAY_LENGTH(buffer), fmt, ## args); \
    graphics_context_set_text_color(ctx, COLOR_ACCENT); \
    graphics_draw_text(ctx, buffer, font_system_18px_bold, GRect(5, 21+21, 144-2*5, 50), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL); \
  } while (0)

/**
 * Returns a point on the line from the center away at an angle specified by tick/maxtick, at a specified distance
 */
static GPoint get_radial_point(const int16_t distance_from_center, const int32_t angle) {
    GPoint result = {
            .x = (int16_t) (sin_lookup(angle) * (int32_t) distance_from_center / TRIG_MAX_RATIO) + center.x,
            .y = (int16_t) (-cos_lookup(angle) * (int32_t) distance_from_center / TRIG_MAX_RATIO) + center.y,
    };
    return result;
}

/**
 * Do the line segments a0->a1 and b0->b1 intersect?
 * Loosely based on http://stackoverflow.com/questions/4977491/determining-if-two-line-segments-intersect/4977569#4977569
 */
static bool intersect(const GPoint a0, const GPoint a1, const GPoint b0, const GPoint b1) {
    GPoint va = GPoint(a1.x - a0.x, a1.y - a0.y);
    GPoint vb = GPoint(b1.x - b0.x, b1.y - b0.y);

    // test for parallel line segments
    int16_t det = vb.x * va.y - va.x * vb.y;
    if (det == 0) {
        if ((b0.x - a0.x) * va.y == (b0.y - a0.y) * va.x) {
            // the two lines are parallel, and might overlap (if the segments were extended infinitely, they would be the same line)
            return (0 <= b0.x - a0.x && b0.x - a0.x <= va.x) || (0 <= a0.x - b0.x && a0.x - b0.x <= vb.x);
        } else {
            // the two lines are parallel, and not overlapping
            return false;
        }
    }

    int16_t s = ((a0.x - b0.x) * va.y - (a0.y - b0.y) * va.x);
    int16_t t = -(-(a0.x - b0.x) * vb.y + (a0.y - b0.y) * vb.x);

    if (det < 0) {
        det = -det;
        s = -s;
        t = -t;
    }
    return 0 <= s && s <= det && 0 <= t && t <= det;
}

/**
 * Draws a bluetooth logo at a given position (top-left).
 */
static void draw_bluetooth_logo(GContext *ctx, GPoint origin) {
#define BLUETOOTH_LOGO_STEP 3

    if (!config_bluetooth_logo) return;

    // background
    graphics_context_set_fill_color(ctx, COLOR(config_color_bluetooth_logo));
    graphics_fill_rect(ctx, GRect(origin.x - 2, origin.y - 2, BLUETOOTH_LOGO_STEP * 2 + 5, BLUETOOTH_LOGO_STEP * 4 + 5),
                       2, GCornersAll);

    // logo on the inside
    graphics_context_set_antialiased(ctx, false);
    graphics_context_set_stroke_color(ctx, COLOR(config_color_bluetooth_logo_2));
    graphics_context_set_stroke_width(ctx, 1);

    graphics_draw_line(ctx, GPoint(origin.x + BLUETOOTH_LOGO_STEP, origin.y + 0),
                       GPoint(origin.x + BLUETOOTH_LOGO_STEP, origin.y + 4 * BLUETOOTH_LOGO_STEP));

    graphics_draw_line(ctx, GPoint(origin.x + 0, origin.y + BLUETOOTH_LOGO_STEP),
                       GPoint(origin.x + 2 * BLUETOOTH_LOGO_STEP, origin.y + 3 * BLUETOOTH_LOGO_STEP));
    graphics_draw_line(ctx, GPoint(origin.x + 0, origin.y + 3 * BLUETOOTH_LOGO_STEP),
                       GPoint(origin.x + 2 * BLUETOOTH_LOGO_STEP, origin.y + BLUETOOTH_LOGO_STEP));

    graphics_draw_line(ctx, GPoint(origin.x + BLUETOOTH_LOGO_STEP, origin.y + 0),
                       GPoint(origin.x + 2 * BLUETOOTH_LOGO_STEP, origin.y + BLUETOOTH_LOGO_STEP));
    graphics_draw_line(ctx, GPoint(origin.x + BLUETOOTH_LOGO_STEP, origin.y + 4 * BLUETOOTH_LOGO_STEP),
                       GPoint(origin.x + 2 * BLUETOOTH_LOGO_STEP, origin.y + 3 * BLUETOOTH_LOGO_STEP));
}

/**
 * Returns true if the line and rectangle intersect.
 */
bool line_rect_intersect(GPoint line0, GPoint line1, GPoint rect0, GPoint rect1) {
    return intersect(line0, line1, rect0, GPoint(rect1.x, rect0.y)) ||
           intersect(line0, line1, rect0, GPoint(rect0.x, rect1.y)) ||
           intersect(line0, line1, rect1, GPoint(rect1.x, rect0.y)) ||
           intersect(line0, line1, rect1, GPoint(rect0.x, rect1.y));
}

/**
 * Returns true if either of two lines intersects with the rectangle.
 */
bool line2_rect_intersect(GPoint lineA0, GPoint lineA1, GPoint lineB0, GPoint lineB1, GPoint rect0, GPoint rect1) {
    return line_rect_intersect(lineA0, lineA1, rect0, rect1) || line_rect_intersect(lineB0, lineB1, rect0, rect1);
}

/**
 * Draws a popup about the bluetooth connection
 */
static void bluetooth_popup(GContext *ctx, bool connected) {
#ifndef DEBUG_BLUETOOTH_POPUP
    if (!show_bluetooth_popup) return;
#endif

    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_context_set_stroke_color(ctx, GColorBlack);
    graphics_context_set_stroke_width(ctx, 4);
    GRect notification_rect = GRect(-10, 168 - 50 - 7, 144 + 20, 50);
    graphics_fill_rect(ctx, notification_rect, 0, GCornersAll);
    graphics_draw_round_rect(ctx, notification_rect, 8);
    graphics_context_set_text_color(ctx, GColorBlack);
    graphics_draw_text(ctx, connected ? "Bluetooth Connected" : "Bluetooth Disconnected", font_system_18px_bold,
                       GRect(2, notification_rect.origin.y + 4, 105, 40),
                       GTextOverflowModeWordWrap, GTextAlignmentCenter,
                       NULL);
    if (connected) {
        graphics_context_set_fill_color(ctx, GColorGreen);
    } else {
        graphics_context_set_fill_color(ctx, GColorRed);
    }

    GPoint icon_center = GPoint(120 + 3, notification_rect.origin.y + notification_rect.size.h - 26);
    graphics_fill_circle(ctx, icon_center, 9);
    graphics_context_set_stroke_width(ctx, 2);
    graphics_context_set_stroke_color(ctx, GColorWhite);

    if (connected) {
        graphics_draw_line(ctx,
                           GPoint(icon_center.x + 4, icon_center.y - 3),
                           GPoint(icon_center.x - 2, icon_center.y + 3));
        graphics_draw_line(ctx,
                           GPoint(icon_center.x - 4, icon_center.y + 0),
                           GPoint(icon_center.x - 2, icon_center.y + 3));
    } else {
        graphics_draw_line(ctx,
                           GPoint(icon_center.x + 3, icon_center.y - 3),
                           GPoint(icon_center.x - 3, icon_center.y + 3));
        graphics_draw_line(ctx,
                           GPoint(icon_center.x - 3, icon_center.y - 3),
                           GPoint(icon_center.x + 3, icon_center.y + 3));
    }
}

//#define DEBUG_DATE_POSITION
#ifdef DEBUG_DATE_POSITION
static int debug_iter = 0;
#endif

/**
 * Update procedure for the background
 */
static void background_update_proc(Layer *layer, GContext *ctx) {
    GRect bounds = layer_get_bounds(layer);
    int16_t radius = bounds.size.w / 2;
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
    t->tm_mday = 8;
    t->tm_mon = 4;
    t->tm_year = 2015;
    t->tm_wday = 6;
#endif
#ifdef SCREENSHOT_DATE_1
    t->tm_min = 26;
    t->tm_hour = 10;
#endif
#ifdef SCREENSHOT_DATE_2
    t->tm_min = 31;
    t->tm_hour = 2;
#endif

    // background
    uint8_t outer_color = config_color_outer_background;
    if (battery_state.charge_percent <= 10 && !battery_state.is_charging && !battery_state.is_plugged) {
        outer_color = config_color_battery_10;
    } else if (battery_state.charge_percent <= 20 && !battery_state.is_charging && !battery_state.is_plugged) {
        outer_color = config_color_battery_20;
    } else if (battery_state.charge_percent <= 30 && !battery_state.is_charging && !battery_state.is_plugged) {
        outer_color = config_color_battery_30;
    }

    graphics_context_set_fill_color(ctx, COLOR(outer_color));
    graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);

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
    graphics_context_set_fill_color(ctx, COLOR(config_color_inner_background));
    graphics_fill_circle(ctx, center, radius);
    graphics_context_set_stroke_color(ctx, COLOR(config_color_circle));
    graphics_context_set_stroke_width(ctx, 4);
    graphics_draw_circle(ctx, center, radius + 3);

    // numbers
#if defined(OBSIDIAN_SHOW_NUMBERS) || defined(OBSIDIAN_ONLY_RELEVANT_NUMBER)
    static const GPoint number_points[] = {
            {144 / 2 - 9,       26}, // 12
            {144 / 2 + 23,      28}, // 1
            {144 / 2 + 45,      47}, // 2
            {144 / 2 + 49,      77}, // 3
            {144 / 2 + 45,      102}, // 4
            {144 / 2 + 24,      124}, // 5
            {144 / 2 - 4,       128}, // 6
            {144 / 2 - 6 - 23,  124}, // 7
            {144 / 2 - 6 - 43,  103}, // 8
            {144 / 2 - 6 - 51,  77}, // 9
            {144 / 2 - 12 - 43, 48}, // 10
            {144 / 2 - 12 - 23, 28}, // 11
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
    if (config_hour_ticks != 3) {
        graphics_context_set_stroke_color(ctx, COLOR(config_color_ticks));
        graphics_context_set_stroke_width(ctx, 2);
        for (int i = 0; i < 12; ++i) {
            if (config_hour_ticks == 2 && (i % 3) != 0) continue;

            int32_t angle = i * TRIG_MAX_ANGLE / 12;
            int tick_length = 6;
#ifdef OBSIDIAN_LONG_TICKS
            if (i % 3 == 0) {
                tick_length = 10;
#ifdef OBSIDIAN_FAT_TICKS
                graphics_context_set_stroke_width(ctx, 4);
            } else {
                graphics_context_set_stroke_width(ctx, 2);
#endif
            }
#endif
            graphics_draw_line(ctx, get_radial_point(radius, angle), get_radial_point(radius - tick_length, angle));
        }
    }


    if (config_minute_ticks == 2) {
        // only relevant minute ticks
        int start_min_tick = (t->tm_min / 5) * 5;
        graphics_context_set_stroke_width(ctx, 1);
        for (int i = start_min_tick; i < start_min_tick + 5; ++i) {
            int32_t angle = i * TRIG_MAX_ANGLE / 60;
            graphics_draw_line(ctx, get_radial_point(radius, angle), get_radial_point(radius - 3, angle));
        }
    } else if (config_minute_ticks == 1) {
        // all minute ticks
        graphics_context_set_stroke_width(ctx, 1);
        for (int i = 0; i < 60; ++i) {
            int32_t angle = i * TRIG_MAX_ANGLE / 60;
            graphics_draw_line(ctx, get_radial_point(radius, angle), get_radial_point(radius - 3, angle));
        }
    }

#ifdef DEBUG_DATE_POSITION
    t->tm_hour = 12;
    t->tm_min = (debug_iter + 55) % 60;
#endif

    // compute angles
    int32_t minute_angle = t->tm_min * TRIG_MAX_ANGLE / 60;
    GPoint minute_hand = get_radial_point(radius - 10, minute_angle);
    int hour_tick = ((t->tm_hour % 12) * 6) + (t->tm_min / 10);
    int32_t hour_angle = hour_tick * TRIG_MAX_ANGLE / (12 * 6);
    GPoint hour_hand = get_radial_point(radius * 55 / 100, hour_angle);

    // format date strings
    strftime(buffer_1, sizeof(buffer_1), "%b %d", t);
    // remove leading zeros
    if (buffer_1[4] == '0') {
        memcpy(&buffer_1[4], &buffer_1[5], 2);
    }
    strftime(buffer_2, sizeof(buffer_2), "%a", t);

    // determine where we can draw the date without overlap
    const GPoint d_points[] = {
            // array of candidate points to draw the date strings at
            GPoint(0, 0),
            GPoint(10, -3),
            GPoint(17, -7),
            GPoint(26, -13),
            GPoint(29, -19),
            GPoint(33, -25),
            GPoint(33, -32),
            GPoint(33, -39),
    };
    const int d_offset = 15;
    const int d_height = 21;
    const int d_y_start = 100;
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
        date_pos = GRect(d_center.x, d_y_start + d_center.y + d_offset, 144, d_height);
        GSize date_size = graphics_text_layout_get_content_size(buffer_1, font_system_18px_bold, date_pos,
                                                                GTextOverflowModeWordWrap, GTextAlignmentCenter);
        day_pos = GRect(d_center.x, d_y_start + d_center.y, 144, d_height);
        GSize day_size = graphics_text_layout_get_content_size(buffer_2, font_system_18px_bold, day_pos,
                                                               GTextOverflowModeWordWrap, GTextAlignmentCenter);
        if (!(line2_rect_intersect(center, hour_hand, center, minute_hand,
                                   GPoint(72 + d_center.x - day_size.w / 2 - border, d_y_start + d_center.y - border),
                                   GPoint(72 + d_center.x + day_size.w / 2 + border,
                                          d_y_start + d_center.y + d_height + border)) ||
              line2_rect_intersect(center, hour_hand, center, minute_hand,
                                   GPoint(72 + d_center.x - date_size.w / 2 - border,
                                          d_y_start + d_center.y + d_offset - border),
                                   GPoint(72 + d_center.x + date_size.w / 2 + border,
                                          d_y_start + d_center.y + d_height + d_offset + border)))) {
            found = true;
            break;
        }
    }

    // this should not happen, but if it does, then use the default position
    if (!found) {
        d_center = d_points[0];
        date_pos = GRect(d_center.x, d_y_start + d_center.y + d_offset, 144, d_height);
        day_pos = GRect(d_center.x, d_y_start + d_center.y, 144, d_height);
    }

    // actuallyl draw the date text
#ifndef DEBUG_NO_DATE
    graphics_context_set_text_color(ctx, COLOR(config_color_day_of_week));
    graphics_draw_text(ctx, buffer_2, font_system_18px_bold, day_pos, GTextOverflowModeWordWrap, GTextAlignmentCenter,
                       NULL);
    graphics_context_set_text_color(ctx, COLOR(config_color_date));
    graphics_draw_text(ctx, buffer_1, font_system_18px_bold, date_pos, GTextOverflowModeWordWrap, GTextAlignmentCenter,
                       NULL);
#endif

    // bluetooth status
    if (!bluetooth) {
        // determine where we can draw the bluetooth logo without overlap
        const GPoint b_points[] = {
                // array of candidate points to draw the bluetooth logo at
                GPoint(0, 0),
                GPoint(4, 1),
                GPoint(8, 2),
                GPoint(12, 4),
                GPoint(16, 6),
                GPoint(20, 9),
                GPoint(24, 12),
        };
        GPoint b_center;
        const int b_border = 3;
        const int b_x = 144 / 2;
        const int b_y = 38;
        // loop through all points and use the first one that doesn't overlap with the watch hands
        for (i = 0; i < 1 + (ARRAY_LENGTH(b_points) - 1) * 2; i++) {
            b_center = b_points[(i + 1) / 2];
            if (i % 2 == 0) {
                b_center.x = -b_center.x;
            }
            if (!line2_rect_intersect(center, hour_hand, center, minute_hand,
                                      GPoint(b_x + b_center.x - 5 - b_border, b_y + b_center.y - b_border),
                                      GPoint(b_x + b_center.x + 5 + b_border, b_y + b_center.y + 17 + b_border))) {
                found = true;
                break;
            }
        }

        draw_bluetooth_logo(ctx, GPoint(b_x + b_center.x - 3, b_y + 2 + b_center.y));
    }

    // second hand
//    GPoint second_hand = get_radial_point_basic(radius, t->tm_sec, 60);
//    graphics_context_set_stroke_width(ctx, 4);
//    graphics_context_set_stroke_color(ctx, GColorBlack);
//    graphics_draw_line(ctx, second_hand, center);
//    graphics_context_set_stroke_width(ctx, 3);
//    graphics_context_set_stroke_color(ctx, GColorWhite);
//    graphics_draw_line(ctx, second_hand, center);

    // minute hand
//    graphics_context_set_stroke_width(ctx, 5);
//    graphics_context_set_stroke_color(ctx, COLOR_BACKGROUND);
//    graphics_draw_line(ctx, minute_hand, center);
    graphics_context_set_stroke_width(ctx, 4);
    graphics_context_set_stroke_color(ctx, COLOR(config_color_minute_hand));
    graphics_draw_line(ctx, minute_hand, center);
    graphics_context_set_stroke_width(ctx, 1);
    graphics_context_set_stroke_color(ctx, COLOR(config_color_inner_minute_hand));
    graphics_draw_line(ctx, get_radial_point(radius - 12, minute_angle), center);

    // hour hand
//    graphics_context_set_stroke_width(ctx, 5);
//    graphics_context_set_stroke_color(ctx, COLOR_BACKGROUND);
//    graphics_draw_line(ctx, hour_hand, center);
    graphics_context_set_stroke_width(ctx, 4);
    graphics_context_set_stroke_color(ctx, COLOR(config_color_hour_hand));
    graphics_draw_line(ctx, hour_hand, center);
    graphics_context_set_stroke_width(ctx, 1);
    graphics_context_set_stroke_color(ctx, COLOR(config_color_inner_hour_hand));
    graphics_draw_line(ctx, get_radial_point(radius * 55 / 100 - 2, hour_angle), center);

    // dot in the middle
    graphics_context_set_fill_color(ctx, COLOR(config_color_minute_hand));
    graphics_fill_circle(ctx, center, 5);
    graphics_context_set_fill_color(ctx, COLOR(config_color_inner_background));
    graphics_fill_circle(ctx, center, 2);

    // battery status
    if (config_battery_logo == 1 ||
        (config_battery_logo == 2 && battery_state.charge_percent <= 30 && !battery_state.is_charging &&
         !battery_state.is_plugged)) {
        const GRect battery = GRect(125, 3, 14, 8);
        graphics_context_set_stroke_color(ctx, COLOR(config_color_battery_logo));
        graphics_context_set_fill_color(ctx, COLOR(config_color_battery_logo));
        graphics_draw_rect(ctx, battery);
        graphics_fill_rect(ctx, GRect(battery.origin.x + 2, battery.origin.y + 2, battery_state.charge_percent / 10, 4),
                           0, GCornerNone);
        graphics_context_set_stroke_width(ctx, 1);
        graphics_draw_line(ctx, GPoint(battery.origin.x + battery.size.w, battery.origin.y + 2),
                           GPoint(battery.origin.x + battery.size.w, battery.origin.y + 5));
    }

    // draw the bluetooth popup
    bluetooth_popup(ctx, bluetooth);
}

static void handle_battery(BatteryChargeState new_state) {
    layer_mark_dirty(layer_background);
}

/**
 * Handler for time ticks.
 */
static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
    layer_mark_dirty(layer_background);
#ifdef DEBUG_DATE_POSITION
    debug_iter += 1;
#endif
}

static void timer_callback_bluetooth_popup(void *data) {
    show_bluetooth_popup = false;
    timer_bluetooth_popup = NULL;
    layer_mark_dirty(layer_background);
}

static void handle_bluetooth(bool connected) {
    // redraw background (to turn on/off the logo)
    layer_mark_dirty(layer_background);

    bool show_popup = false;
    bool vibrate = false;
    if ((config_message_reconnect && connected) || (config_message_disconnect && !connected)) {
        show_popup = true;
    }
    if ((config_vibrate_reconnect && connected) || (config_vibrate_disconnect && !connected)) {
        vibrate = true;
    }

    // vibrate
    if (vibrate) {
        vibes_double_pulse();
    }

    // turn light on
    if (show_popup) {
        light_enable_interaction();
    }

    // show popup
    if (show_popup) {
        show_bluetooth_popup = true;
        if (timer_bluetooth_popup) {
            app_timer_reschedule(timer_bluetooth_popup, OBSIDIAN_BLUETOOTH_POPUP_MS);
        } else {
            timer_bluetooth_popup = app_timer_register(OBSIDIAN_BLUETOOTH_POPUP_MS, timer_callback_bluetooth_popup,
                                                       NULL);
        }
    }
}

/**
 * Window load callback.
 */
static void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    // create layer
    layer_background = layer_create(bounds);
    layer_set_update_proc(layer_background, background_update_proc);
    layer_add_child(window_layer, layer_background);

    // load fonts
#ifdef OBSIDIAN_SHOW_NUMBERS
    font_open_sans = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_OPEN_SANS_12));
#endif
    font_system_18px_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);

    // initialize
    show_bluetooth_popup = false;
}

/**
 * Window unload callback.
 */
static void window_unload(Window *window) {
    layer_destroy(layer_background);
#ifdef OBSIDIAN_SHOW_NUMBERS
    fonts_unload_custom_font(font_open_sans);
#endif
}

/**
 * Helper to process new configuration.
 */
static bool sync_helper(const uint32_t key, const Tuple *new_tuple, uint8_t *value) {
    if ((*value) != new_tuple->value->uint8) {
        (*value) = new_tuple->value->uint8;
        persist_write_int(key, *value);
        return true;
    }

    return false;
}

/**
 * Handle the receipt of a new configuration
 */
static void sync_changed_handler(const uint32_t key, const Tuple *new_tuple, const Tuple *old_tuple, void *context) {
    bool dirty = false;
    switch (key) {
        case CONFIG_COLOR_OUTER_BACKGROUND:
            dirty = sync_helper(key, new_tuple, &config_color_outer_background);
            break;
        case CONFIG_COLOR_INNER_BACKGROUND:
            dirty = sync_helper(key, new_tuple, &config_color_inner_background);
            break;
        case CONFIG_COLOR_MINUTE_HAND:
            dirty = sync_helper(key, new_tuple, &config_color_minute_hand);
            break;
        case CONFIG_COLOR_INNER_MINUTE_HAND:
            dirty = sync_helper(key, new_tuple, &config_color_inner_minute_hand);
            break;
        case CONFIG_COLOR_HOUR_HAND:
            dirty = sync_helper(key, new_tuple, &config_color_hour_hand);
            break;
        case CONFIG_COLOR_INNER_HOUR_HAND:
            dirty = sync_helper(key, new_tuple, &config_color_inner_hour_hand);
            break;
        case CONFIG_COLOR_CIRCLE:
            dirty = sync_helper(key, new_tuple, &config_color_circle);
            break;
        case CONFIG_COLOR_TICKS:
            dirty = sync_helper(key, new_tuple, &config_color_ticks);
            break;
        case CONFIG_COLOR_DAY_OF_WEEK:
            dirty = sync_helper(key, new_tuple, &config_color_day_of_week);
            break;
        case CONFIG_COLOR_DATE:
            dirty = sync_helper(key, new_tuple, &config_color_date);
            break;
        case CONFIG_BATTERY_LOGO:
            dirty = sync_helper(key, new_tuple, &config_battery_logo);
            break;
        case CONFIG_COLOR_BATTERY_LOGO:
            dirty = sync_helper(key, new_tuple, &config_color_battery_logo);
            break;
        case CONFIG_COLOR_BATTERY_30:
            dirty = sync_helper(key, new_tuple, &config_color_battery_30);
            break;
        case CONFIG_COLOR_BATTERY_20:
            dirty = sync_helper(key, new_tuple, &config_color_battery_20);
            break;
        case CONFIG_COLOR_BATTERY_10:
            dirty = sync_helper(key, new_tuple, &config_color_battery_10);
            break;
        case CONFIG_COLOR_BLUETOOTH_LOGO:
            dirty = sync_helper(key, new_tuple, &config_color_bluetooth_logo);
            break;
        case CONFIG_COLOR_BLUETOOTH_LOGO_2:
            dirty = sync_helper(key, new_tuple, &config_color_bluetooth_logo_2);
            break;
        case CONFIG_BLUETOOTH_LOGO:
            dirty = sync_helper(key, new_tuple, &config_bluetooth_logo);
            break;
        case CONFIG_VIBRATE_DISCONNECT:
            dirty = sync_helper(key, new_tuple, &config_vibrate_disconnect);
            break;
        case CONFIG_VIBRATE_RECONNECT:
            dirty = sync_helper(key, new_tuple, &config_vibrate_reconnect);
            break;
        case CONFIG_MESSAGE_DISCONNECT:
            dirty = sync_helper(key, new_tuple, &config_message_disconnect);
            break;
        case CONFIG_MESSAGE_RECONNECT:
            dirty = sync_helper(key, new_tuple, &config_message_reconnect);
            break;

        default:
            // ignore unknown keys
            break;
    }
    if (dirty && layer_background) {
        layer_mark_dirty(layer_background);
    }
}

/**
 * Sync error handler
 */
static void sync_error_handler(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
    // there's not much we can do
}

/**
 * Config initialization.
 */
static void config_init() {
    app_message_open(
            app_message_inbox_size_maximum(),
            app_message_outbox_size_maximum());

    Tuplet initial_values[] = {
            TupletInteger(CONFIG_COLOR_OUTER_BACKGROUND, config_color_outer_background),
            TupletInteger(CONFIG_COLOR_INNER_BACKGROUND, config_color_inner_background),
            TupletInteger(CONFIG_COLOR_MINUTE_HAND, config_color_minute_hand),
            TupletInteger(CONFIG_COLOR_INNER_MINUTE_HAND, config_color_inner_minute_hand),
            TupletInteger(CONFIG_COLOR_HOUR_HAND, config_color_hour_hand),
            TupletInteger(CONFIG_COLOR_INNER_HOUR_HAND, config_color_inner_hour_hand),
            TupletInteger(CONFIG_COLOR_CIRCLE, config_color_circle),
            TupletInteger(CONFIG_COLOR_TICKS, config_color_ticks),
            TupletInteger(CONFIG_COLOR_DAY_OF_WEEK, config_color_day_of_week),
            TupletInteger(CONFIG_COLOR_DATE, config_color_date),
            TupletInteger(CONFIG_BATTERY_LOGO, config_battery_logo),
            TupletInteger(CONFIG_COLOR_BATTERY_LOGO, config_color_battery_logo),
            TupletInteger(CONFIG_COLOR_BATTERY_30, config_color_battery_30),
            TupletInteger(CONFIG_COLOR_BATTERY_20, config_color_battery_20),
            TupletInteger(CONFIG_COLOR_BATTERY_10, config_color_battery_10),
            TupletInteger(CONFIG_COLOR_BLUETOOTH_LOGO, config_color_bluetooth_logo),
            TupletInteger(CONFIG_COLOR_BLUETOOTH_LOGO_2, config_color_bluetooth_logo_2),
            TupletInteger(CONFIG_BLUETOOTH_LOGO, config_bluetooth_logo),
            TupletInteger(CONFIG_VIBRATE_DISCONNECT, config_vibrate_disconnect),
            TupletInteger(CONFIG_VIBRATE_RECONNECT, config_vibrate_reconnect),
            TupletInteger(CONFIG_MESSAGE_DISCONNECT, config_message_disconnect),
            TupletInteger(CONFIG_MESSAGE_RECONNECT, config_message_reconnect),
            TupletInteger(CONFIG_MINUTE_TICKS, config_minute_ticks),
            TupletInteger(CONFIG_HOUR_TICKS, config_hour_ticks),
    };

    app_sync_init(
            &sync, sync_buffer, sizeof(sync_buffer),
            initial_values, ARRAY_LENGTH(initial_values),
            sync_changed_handler, sync_error_handler, NULL);
}

/**
 * Config de-initialization.
 */
static void config_deinit() {
    app_sync_deinit(&sync);
}

/**
 * Initialization.
 */
static void init() {
    if (persist_exists(CONFIG_COLOR_OUTER_BACKGROUND)) {
        config_color_outer_background = persist_read_int(CONFIG_COLOR_OUTER_BACKGROUND);
        config_color_inner_background = persist_read_int(CONFIG_COLOR_INNER_BACKGROUND);
        config_color_minute_hand = persist_read_int(CONFIG_COLOR_MINUTE_HAND);
        config_color_inner_minute_hand = persist_read_int(CONFIG_COLOR_INNER_MINUTE_HAND);
        config_color_hour_hand = persist_read_int(CONFIG_COLOR_HOUR_HAND);
        config_color_inner_hour_hand = persist_read_int(CONFIG_COLOR_INNER_HOUR_HAND);
        config_color_circle = persist_read_int(CONFIG_COLOR_CIRCLE);
        config_color_ticks = persist_read_int(CONFIG_COLOR_TICKS);
        config_color_day_of_week = persist_read_int(CONFIG_COLOR_DAY_OF_WEEK);
        config_color_date = persist_read_int(CONFIG_COLOR_DATE);
        config_battery_logo = persist_read_int(CONFIG_BATTERY_LOGO);
        config_color_battery_logo = persist_read_int(CONFIG_COLOR_BATTERY_LOGO);
        config_color_battery_30 = persist_read_int(CONFIG_COLOR_BATTERY_30);
        config_color_battery_20 = persist_read_int(CONFIG_COLOR_BATTERY_20);
        config_color_battery_10 = persist_read_int(CONFIG_COLOR_BATTERY_10);
        config_color_bluetooth_logo = persist_read_int(CONFIG_COLOR_BLUETOOTH_LOGO);
        config_color_bluetooth_logo_2 = persist_read_int(CONFIG_COLOR_BLUETOOTH_LOGO_2);
        config_bluetooth_logo = persist_read_int(CONFIG_BLUETOOTH_LOGO);
        config_vibrate_disconnect = persist_read_int(CONFIG_VIBRATE_DISCONNECT);
        config_vibrate_reconnect = persist_read_int(CONFIG_VIBRATE_RECONNECT);
        config_message_disconnect = persist_read_int(CONFIG_MESSAGE_DISCONNECT);
        config_message_reconnect = persist_read_int(CONFIG_MESSAGE_RECONNECT);
        config_minute_ticks = persist_read_int(CONFIG_MINUTE_TICKS);
        config_hour_ticks = persist_read_int(CONFIG_HOUR_TICKS);
    }

// some alternative themes (for screenshots)
#ifdef SCREENSHOT_ALT_THEME_1
    uint8_t accent_col = GColorRedARGB8;
#endif
#ifdef SCREENSHOT_ALT_THEME_2
    uint8_t accent_col = GColorBlueARGB8;
#endif
#if defined(SCREENSHOT_ALT_THEME_1) || defined(SCREENSHOT_ALT_THEME_2)
    config_color_day_of_week = accent_col;
    config_color_hour_hand = accent_col;
#endif
#ifdef SCREENSHOT_ALT_THEME_3
    config_hour_ticks = 3;
    config_minute_ticks = 3;
    config_color_outer_background = GColorPurpleARGB8;
    config_color_inner_background = GColorPurpleARGB8;
    config_color_minute_hand = GColorBlackARGB8;
    config_color_hour_hand = GColorBlackARGB8;
    config_color_circle = GColorBlackARGB8;
    config_color_ticks = GColorBlackARGB8;
    config_color_day_of_week = GColorPurpleARGB8;
    config_color_date = GColorPurpleARGB8;
    config_battery_logo = 3;
    config_color_inner_minute_hand = GColorBlackARGB8;
    config_color_inner_hour_hand = GColorBlackARGB8;
#endif
#ifdef SCREENSHOT_ALT_THEME_4
    config_color_outer_background = GColorBlackARGB8;
    config_color_inner_background = GColorBlackARGB8;
    config_color_minute_hand = GColorWhiteARGB8;
    config_color_hour_hand = GColorBabyBlueEyesARGB8;
    config_color_circle = GColorBlackARGB8;
    config_color_ticks = GColorBlackARGB8;
    config_color_day_of_week = GColorBabyBlueEyesARGB8;
    config_color_date = GColorWhiteARGB8;
    config_battery_logo = 3;
    config_color_inner_minute_hand = GColorLightGrayARGB8;
    config_color_inner_hour_hand = config_color_inner_minute_hand;
#endif
#ifdef SCREENSHOT_ALT_THEME_5
    uint8_t col1 = GColorChromeYellowARGB8;
    uint8_t col2 = GColorVividCeruleanARGB8;
    config_hour_ticks = 3;
    config_minute_ticks = 3;
    config_color_outer_background = col2;
    config_color_inner_background = col2;
    config_color_minute_hand = col1;
    config_color_hour_hand = col1;
    config_color_circle = col2;
    config_color_ticks = col1;
    config_color_day_of_week = col2;
    config_color_date = col2;
    config_battery_logo = 3;
    config_color_inner_minute_hand = col1;
    config_color_inner_hour_hand = config_color_inner_minute_hand;
#endif
#ifdef SCREENSHOT_ALT_THEME_6
    config_hour_ticks = 1;
    config_minute_ticks = 2;
    config_color_outer_background = GColorWhiteARGB8;
    config_color_inner_background = GColorWhiteARGB8;
    config_color_minute_hand = GColorRedARGB8;
    config_color_hour_hand = GColorBlackARGB8;
    config_color_circle = GColorBlackARGB8;
    config_color_ticks = GColorBlackARGB8;
    config_color_day_of_week = GColorWhiteARGB8;
    config_color_date = GColorWhiteARGB8;
    config_battery_logo = 3;
    config_color_inner_minute_hand = GColorRedARGB8;
    config_color_inner_hour_hand = GColorBlackARGB8;
#endif
#ifdef SCREENSHOT_ALT_THEME_7
    config_hour_ticks = 1;
    config_minute_ticks = 2;
    config_color_outer_background = GColorWhiteARGB8;
    config_color_inner_background = GColorWhiteARGB8;
    config_color_minute_hand = GColorBlackARGB8;
    config_color_hour_hand = GColorBlackARGB8;
    config_color_circle = GColorBlackARGB8;
    config_color_ticks = GColorBlackARGB8;
    config_color_day_of_week = GColorWhiteARGB8;
    config_color_date = GColorBlackARGB8;
    config_battery_logo = 3;
    config_color_inner_minute_hand = GColorBlackARGB8;
    config_color_inner_hour_hand = GColorBlackARGB8;
#endif
#ifdef SCREENSHOT_ALT_THEME_8
    config_hour_ticks = 1;
    config_minute_ticks = 2;
    config_color_outer_background = GColorWhiteARGB8;
    config_color_inner_background = GColorIcterineARGB8;
    config_color_minute_hand = GColorBlueARGB8;
    config_color_hour_hand = GColorBlackARGB8;
    config_color_circle = GColorBlackARGB8;
    config_color_ticks = GColorBlackARGB8;
    config_color_day_of_week = GColorIcterineARGB8;
    config_color_date = GColorBlueARGB8;
    config_battery_logo = 3;
    config_color_inner_minute_hand = GColorBlueARGB8;
    config_color_inner_hour_hand = GColorBlackARGB8;
#endif
#ifdef DEBUG_NO_BATTERY_ICON
    config_battery_logo = 3;
#endif

    window = window_create();
    window_set_window_handlers(window, (WindowHandlers) {
            .load = window_load,
            .unload = window_unload,
    });
    window_stack_push(window, true);

    GRect bounds = layer_get_bounds(window_get_root_layer(window));
    center = grect_center_point(&bounds);

    TimeUnits unit = MINUTE_UNIT;
#ifdef DEBUG_DATE_POSITION
    unit = SECOND_UNIT;
#endif
    tick_timer_service_subscribe(unit, handle_second_tick);
    battery_state_service_subscribe(handle_battery);
    bluetooth_connection_service_subscribe(handle_bluetooth);

    config_init();
}

/**
 * De-initialisation.
 */
static void deinit() {
    tick_timer_service_unsubscribe();
    battery_state_service_unsubscribe();
    bluetooth_connection_service_unsubscribe();

    window_destroy(window);

    config_deinit();
}

/**
 * Main entry point.
 */
int main() {
    init();
    app_event_loop();
    deinit();
}
