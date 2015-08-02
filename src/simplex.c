#include <pebble.h>
#include "simplex.h"

#include "pebble.h"


////////////////////////////////////////////
//// Configuration constants
////////////////////////////////////////////

#define COLOR_BACKGROUND GColorWhite
#define COLOR_NORMAL GColorBlack
//#define COLOR_ACCENT GColorBlue
#define COLOR_ACCENT GColorIslamicGreen
#define COLOR_BATTERY GColorDarkGray
#define COLOR_WARNING GColorSunsetOrange


////////////////////////////////////////////
//// Global variables
////////////////////////////////////////////

/** A pointer to our window, for later deallocation. */
static Window *window;

/** All layers */
static Layer *layer_time, *layer_text, *layer_background, *layer_battery;

/** All text layers */
static TextLayer *label_dayofweek, *label_day;

/** Buffers for date strings */
static char buffer_dayofweek[4], buffer_day[6];

/** The center of the watch */
static GPoint center;


////////////////////////////////////////////
//// Implementation
////////////////////////////////////////////

/**
 * Returns a point on the line from the center away at an angle specified by tick/maxtick, at a specified distance
 */
static GPoint get_radial_point(const int16_t distance_from_center, const int32_t tick,
                               const int32_t maxtick) {
    int32_t angle = TRIG_MAX_ANGLE * tick / maxtick;
    GPoint result = {
            .x = (int16_t) (sin_lookup(angle) * (int32_t) distance_from_center / TRIG_MAX_RATIO) + center.x,
            .y = (int16_t) (-cos_lookup(angle) * (int32_t) distance_from_center / TRIG_MAX_RATIO) + center.y,
    };
    return result;
}

static void draw_bluetooth_logo(GContext *ctx, GColor color, GPoint origin) {
    graphics_context_set_stroke_color(ctx, color);
    graphics_context_set_stroke_width(ctx, 1);

    graphics_draw_line(ctx, GPoint(origin.x + 3, origin.y + 0), GPoint(origin.x + 3, origin.y +12));

    graphics_draw_line(ctx, GPoint(origin.x + 0, origin.y + 3), GPoint(origin.x + 6, origin.y + 9));
    graphics_draw_line(ctx, GPoint(origin.x + 0, origin.y + 9), GPoint(origin.x + 6, origin.y + 3));

    graphics_draw_line(ctx, GPoint(origin.x + 3, origin.y + 0), GPoint(origin.x + 6, origin.y + 3));
    graphics_draw_line(ctx, GPoint(origin.x + 3, origin.y +12), GPoint(origin.x + 6, origin.y + 9));
}

/**
 * Update procedure for the background
 */
static void background_update_proc(Layer *layer, GContext *ctx) {
    GRect bounds = layer_get_bounds(layer);
    int16_t radius = bounds.size.w / 2;

    // background
    graphics_context_set_fill_color(ctx, GColorDarkGray);
    graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
    graphics_context_set_fill_color(ctx, COLOR_BACKGROUND);
    graphics_fill_circle(ctx, center, radius);
    graphics_context_set_stroke_color(ctx, COLOR_NORMAL);
    graphics_context_set_stroke_width(ctx, 4);
    graphics_draw_circle(ctx, center, radius+3);

    // hour ticks
    graphics_context_set_stroke_color(ctx, COLOR_NORMAL);
    graphics_context_set_stroke_width(ctx, 2);
    for (int i = 0; i < 12; ++i) {
        graphics_draw_line(ctx, get_radial_point(radius, i, 12), get_radial_point(radius - 6, i, 12));
    }

    // minute ticks
    graphics_context_set_stroke_width(ctx, 1);
    for (int i = 0; i < 60; ++i) {
        graphics_draw_line(ctx, get_radial_point(radius, i, 60), get_radial_point(radius - 3, i, 60));
    }

    //draw_bluetooth_logo(ctx, COLOR_WARNING, GPoint(3, 3));
}

/**
 * Update procedure for text
 */
static void text_update_proc(Layer *layer, GContext *ctx) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    // time and date strings
    strftime(buffer_day, sizeof(buffer_day), "%m/%d", t);
    // remove leading zeros
    if (buffer_day[0] == '0') {
        memcpy(&buffer_day[0], &buffer_day[1], 5);
    }
    if (buffer_day[2] == '0') {
        memcpy(&buffer_day[2], &buffer_day[3], 2);
    }
    if (buffer_day[3] == '0') {
        memcpy(&buffer_day[3], &buffer_day[4], 2);
    }
//    buffer_day[0] = '1';
//    buffer_day[1] = '2';
//    buffer_day[2] = '/';
//    buffer_day[3] = '2';
//    buffer_day[4] = '8';
//    buffer_day[5] = 0;
    strftime(buffer_dayofweek, sizeof(buffer_dayofweek), "%a", t);
    text_layer_set_text(label_day, buffer_day);
    text_layer_set_text(label_dayofweek, buffer_dayofweek);
}

/**
 * Update procedure for the battery level
 */
static void battery_update_proc(Layer *layer, GContext *ctx) {
    GRect bounds = layer_get_bounds(layer);

    BatteryChargeState state = battery_state_service_peek();
    graphics_context_set_stroke_width(ctx, 1);
    graphics_context_set_stroke_color(ctx, COLOR_BATTERY);
    graphics_draw_line(ctx, GPoint(0, bounds.size.h - 1),
                       GPoint(bounds.size.w * state.charge_percent / 100, bounds.size.h - 1));
}

/**
 * Update procedure for the time
 */
static void time_update_proc(Layer *layer, GContext *ctx) {
    GRect bounds = layer_get_bounds(layer);
    int16_t radius = bounds.size.w / 2;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    // second hand
//    GPoint second_hand = get_radial_point(radius, t->tm_sec, 60);
//    graphics_context_set_stroke_width(ctx, 4);
//    graphics_context_set_stroke_color(ctx, GColorBlack);
//    graphics_draw_line(ctx, second_hand, center);
//    graphics_context_set_stroke_width(ctx, 3);
//    graphics_context_set_stroke_color(ctx, GColorWhite);
//    graphics_draw_line(ctx, second_hand, center);

    // minute hand
    GPoint minute_hand = get_radial_point(radius - 10, t->tm_min, 60);
    graphics_context_set_stroke_width(ctx, 5);
    graphics_context_set_stroke_color(ctx, COLOR_BACKGROUND);
    graphics_draw_line(ctx, minute_hand, center);
    graphics_context_set_stroke_width(ctx, 4);
    graphics_context_set_stroke_color(ctx, COLOR_NORMAL);
    graphics_draw_line(ctx, minute_hand, center);
    graphics_context_set_stroke_width(ctx, 1);
    graphics_context_set_stroke_color(ctx, GColorLightGray);
    graphics_draw_line(ctx, get_radial_point(radius - 12, t->tm_min, 60), center);

    // hour hand
    int hour_tick = ((t->tm_hour % 12) * 6) + (t->tm_min / 10);
    GPoint hour_hand = get_radial_point(radius * 55 / 100, hour_tick, 12 * 6);
    graphics_context_set_stroke_width(ctx, 5);
    graphics_context_set_stroke_color(ctx, COLOR_BACKGROUND);
    graphics_draw_line(ctx, hour_hand, center);
    graphics_context_set_stroke_width(ctx, 4);
    graphics_context_set_stroke_color(ctx, COLOR_ACCENT);
    graphics_draw_line(ctx, hour_hand, center);
    graphics_context_set_stroke_width(ctx, 1);
    graphics_context_set_stroke_color(ctx, GColorLightGray);
    graphics_draw_line(ctx, get_radial_point(radius * 55 / 100 - 2, hour_tick, 12*6), center);

    // dot in the middle
    graphics_context_set_fill_color(ctx, COLOR_NORMAL);
    graphics_fill_circle(ctx, center, 5);
    graphics_context_set_fill_color(ctx, COLOR_BACKGROUND);
    graphics_fill_circle(ctx, center, 2);
}

static void handle_battery(BatteryChargeState new_state) {
    layer_mark_dirty(layer_battery);
}

/**
 * Handler for time ticks.
 */
static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
    layer_mark_dirty(layer_time);
}

/**
 * Window load callback.
 */
static void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    // create background layer
    layer_background = layer_create(bounds);
    layer_set_update_proc(layer_background, background_update_proc);
    layer_add_child(window_layer, layer_background);

    // create the battery layer
    layer_battery = layer_create(bounds);
    layer_set_update_proc(layer_battery, battery_update_proc);
    layer_add_child(window_layer, layer_battery);

    // create text layer
    layer_text = layer_create(bounds);
    layer_set_update_proc(layer_text, text_update_proc);
    layer_add_child(window_layer, layer_text);

    // create time layer
    layer_time = layer_create(bounds);
    layer_set_update_proc(layer_time, time_update_proc);
    layer_add_child(window_layer, layer_time);

    // create dayofweek text layer
    label_dayofweek = text_layer_create(GRect(88, 64, 9 * 5, 20));
    text_layer_set_text(label_dayofweek, buffer_day);
    text_layer_set_background_color(label_dayofweek, COLOR_BACKGROUND);
    text_layer_set_text_color(label_dayofweek, COLOR_NORMAL);
    text_layer_set_font(label_dayofweek, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_text_alignment(label_dayofweek, GTextAlignmentCenter);
    layer_add_child(layer_text, text_layer_get_layer(label_dayofweek));

    // create day text layer
    label_day = text_layer_create(GRect(88, 82, 9 * 5, 20));
    text_layer_set_text(label_day, buffer_dayofweek);
    text_layer_set_background_color(label_day, COLOR_BACKGROUND);
    text_layer_set_text_color(label_day, COLOR_ACCENT);
    text_layer_set_font(label_day, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_text_alignment(label_day, GTextAlignmentCenter);
    layer_add_child(layer_text, text_layer_get_layer(label_day));
}

/**
 * Window unload callback.
 */
static void window_unload(Window *window) {
    layer_destroy(layer_background);
    layer_destroy(layer_time);

    text_layer_destroy(label_dayofweek);
    text_layer_destroy(label_day);
}

/**
 * Initialization.
 */
static void init() {
    window = window_create();
    window_set_window_handlers(window, (WindowHandlers) {
            .load = window_load,
            .unload = window_unload,
    });
    window_stack_push(window, true);

    GRect bounds = layer_get_bounds(window_get_root_layer(window));
    center = grect_center_point(&bounds);

    buffer_day[0] = '\0';
    buffer_dayofweek[0] = '\0';

    tick_timer_service_subscribe(MINUTE_UNIT, handle_second_tick);
    battery_state_service_subscribe(handle_battery);
}

/**
 * De-initialisation.
 */
static void deinit() {
    tick_timer_service_unsubscribe();
    battery_state_service_unsubscribe();

    window_destroy(window);
}

/**
 * Main entry point.
 */
int main() {
    init();
    app_event_loop();
    deinit();
}
