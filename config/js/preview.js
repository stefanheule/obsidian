var ObsidianPreview = (function () {

    /** Map from canvasIDs to configurations. */
    var configurations = {};
    /** Map from canvasIDs to platforms. */
    var platforms = {};

    function drawConfig(canvasId) {
        var config = configurations[canvasId];
        var platform = platforms[canvasId];

        var PBL_IF_ROUND_ELSE = PebbleHelper.PBL_IF_ROUND_ELSE(platform);

        var GPoint = function (x, y) {
            return {x: x, y: y};
        };
        var GRect = function (x, y, w, h) {
            return {origin: {x: x, y: y}, size: {w: w, h: h}};
        };
        var chalk = platform == "chalk";
        var canvas = document.getElementById(canvasId);
        var ctx = canvas.getContext('2d');
        var w = PBL_IF_ROUND_ELSE(180, 144);
        var h = PBL_IF_ROUND_ELSE(180, 168);
        var radius = w / 2;
        var center = GPoint(w / 2, h / 2);
        var color = function (c) {
            return '#' + GColor.toHex(c);
        };
        var t = {
            tm_min: 10,
            tm_hour: 10
        };

        canvas.height = h;
        canvas.width = w;
        var width = w;
        var height = h;

        var TRIG_MAX_ANGLE = Math.PI * 2;
        var get_radial_point = function (radius, angle) {
            return {
                x: Math.sin(angle) * radius + center.x,
                y: -Math.cos(angle) * radius + center.y
            };
        };
        var get_radial_border_point = function (distance_from_border, angle) {
            var topright_angle = Math.atan2(width, height);
            var top = angle > (TRIG_MAX_ANGLE - topright_angle) || angle <= topright_angle;
            var bottom = angle > TRIG_MAX_ANGLE / 2 - topright_angle && angle <= TRIG_MAX_ANGLE / 2 + topright_angle;
            var left = angle > TRIG_MAX_ANGLE / 2 + topright_angle && angle <= TRIG_MAX_ANGLE - topright_angle;
            var sine = Math.sin(angle);
            var cosine = Math.cos(angle);
            var result;
            if (top || bottom) {
                result = {
                    x: (sine * (height / 2 - distance_from_border) / cosine) +
                    center.x,
                    y: top ? distance_from_border : height - distance_from_border
                };
                return result;
            }
            result = {
                x: left ? distance_from_border : width - distance_from_border,
                y: (cosine * (width / 2 - distance_from_border) / sine) + center.y
            };
            return result;
        };
        var graphics_draw_line_with_width = function (ctx, p0, p1, w) {
            ctx.lineWidth = w + 0.5;
            if (w == 1) {
                ctx.lineWidth = 1;
            }
            ctx.beginPath();
            ctx.moveTo(p0.x, p0.y);
            ctx.lineTo(p1.x, p1.y);
            ctx.stroke();
        };
        var graphics_context_set_stroke_color = function (ctx, c) {
            ctx.strokeStyle = color(c);
        };
        var graphics_context_set_fill_color = function (ctx, c) {
            ctx.fillStyle = color(c);
        };
        var graphics_fill_circle = function (ctx, center, radius) {
            ctx.beginPath();
            ctx.arc(center.x, center.y, radius, 0, 2 * Math.PI, false);
            ctx.fill();
        };
        var graphics_draw_rect = function (ctx, rect) {
            ctx.strokeRect(rect.origin.x, rect.origin.y, rect.size.w, rect.size.h);
        };
        var graphics_fill_rect = function (ctx, rect) {
            ctx.fillRect(rect.origin.x, rect.origin.y, rect.size.w, rect.size.h);
        };

        config_square = config["CONFIG_SQUARE"];
        if (chalk) {
            graphics_context_set_fill_color(ctx, config["CONFIG_COLOR_INNER_BACKGROUND"]);
            graphics_fill_circle(ctx, center, w / 2);
        } else {
            if (!config_square) {
                graphics_context_set_fill_color(ctx, config["CONFIG_COLOR_OUTER_BACKGROUND"]);
                graphics_fill_rect(ctx, GRect(0, 0, w, h));
                graphics_context_set_fill_color(ctx, config["CONFIG_COLOR_INNER_BACKGROUND"]);
                graphics_fill_circle(ctx, center, w / 2);
                graphics_context_set_stroke_color(ctx, config["CONFIG_COLOR_CIRCLE"]);
                var circleWidth = 4;
                ctx.lineWidth = circleWidth;
                ctx.beginPath();
                ctx.arc(center.x, center.y, w / 2 + circleWidth / 2, 0, 2 * Math.PI, false);
                ctx.stroke();
            } else {
                graphics_context_set_fill_color(ctx, config["CONFIG_COLOR_INNER_BACKGROUND"]);
                graphics_fill_rect(ctx, GRect(0, 0, w, h));
            }
        }

        ctx.strokeStyle = color(config["CONFIG_COLOR_TICKS"]);
        ctx.lineCap = "round";
        if (config["CONFIG_HOUR_TICKS"] != 3) {
            var tick_width;
            if (!config_square) {
                for (var i = 0; i < 12; ++i) {
                    if (config["CONFIG_HOUR_TICKS"] == 2 && (i % 3) != 0) continue;
                    var angle = i * Math.PI * 2 / 12;
                    var tick_length = PBL_IF_ROUND_ELSE(8, 6);
                    if (i % 3 == 0) {
                        tick_length = PBL_IF_ROUND_ELSE(12, 10);
                        tick_width = 4;
                    } else {
                        tick_width = 2;
                    }

                    graphics_draw_line_with_width(ctx, get_radial_point(radius, angle),
                        get_radial_point(radius - tick_length, angle),
                        tick_width);
                }
            } else {
                for (var i = 0; i < 12; ++i) {
                    if (config["CONFIG_HOUR_TICKS"] == 2 && (i % 3) != 0) continue;
                    var angle = i * TRIG_MAX_ANGLE / 12;
                    var tick_length = 8;
                    graphics_draw_line_with_width(ctx, get_radial_border_point(0, angle), get_radial_border_point(tick_length, angle), 4);
                }
            }
        }

        ctx.lineWidth = 1 + 1;
        var square_minute_tick = 4;
        if (config["CONFIG_MINUTE_TICKS"] == 2) {
            // only relevant minute ticks
            var start_min_tick = (t.tm_min / 5) * 5;
            for (i = start_min_tick; i < start_min_tick + 5; ++i) {
                angle = i * Math.PI * 2 / 60;
                if (!config_square) {
                    graphics_draw_line_with_width(ctx, get_radial_point(radius, angle),
                        get_radial_point(radius - PBL_IF_ROUND_ELSE(3, 3), angle), 0.5);
                } else {
                    graphics_draw_line_with_width(ctx, get_radial_border_point(0, angle),
                        get_radial_border_point(square_minute_tick, angle), 1);
                }
            }
        } else if (config["CONFIG_MINUTE_TICKS"] == 1) {
            // all minute ticks
            for (i = 0; i < 60; ++i) {
                angle = i * Math.PI * 2 / 60;
                if (!config_square) {
                    graphics_draw_line_with_width(ctx, get_radial_point(radius, angle),
                        get_radial_point(radius - PBL_IF_ROUND_ELSE(3, 3), angle), 0.5);
                } else {
                    graphics_draw_line_with_width(ctx, get_radial_border_point(0, angle),
                        get_radial_border_point(square_minute_tick, angle), 1);
                }
            }
        }

        // compute angles
        var minute_angle = t.tm_min * TRIG_MAX_ANGLE / 60;
        var minute_hand = get_radial_point(radius - PBL_IF_ROUND_ELSE(16, 10), minute_angle);
        var hour_tick = ((t.tm_hour % 12) * 6) + (t.tm_min / 10);
        var hour_angle = hour_tick * TRIG_MAX_ANGLE / (12 * 6);
        var hour_hand = get_radial_point(radius * 55 / 100, hour_angle);

        // minute hand
        var COLOR = function (x) {
            return x;
        };
        var config_color_minute_hand = config["CONFIG_COLOR_MINUTE_HAND"];
        var config_color_inner_minute_hand = config["CONFIG_COLOR_INNER_MINUTE_HAND"];
        var config_color_hour_hand = config["CONFIG_COLOR_HOUR_HAND"];
        var config_color_inner_hour_hand = config["CONFIG_COLOR_INNER_HOUR_HAND"];
        var config_color_inner_background = config["CONFIG_COLOR_INNER_BACKGROUND"];

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
        graphics_fill_circle(ctx, center, 5 + 0.5);
        graphics_context_set_fill_color(ctx, COLOR(config_color_inner_background));
        graphics_fill_circle(ctx, center, 2 + 0.5);

        ctx.font = PBL_IF_ROUND_ELSE("19px Verdana", "14px Verdana");
        ctx.textAlign = "center";
        graphics_context_set_fill_color(ctx, config["CONFIG_COLOR_DAY_OF_WEEK"]);
        ctx.fillText("Sat", w / 2, PBL_IF_ROUND_ELSE(130, 116));
        graphics_context_set_fill_color(ctx, config["CONFIG_COLOR_DATE"]);
        ctx.fillText("May 8", w / 2, PBL_IF_ROUND_ELSE(150, 130));

        if (config["CONFIG_WEATHER_LOCAL"] || (chalk && config["CONFIG_BLUETOOTH_LOGO"])) {
            ctx.font = PBL_IF_ROUND_ELSE("23px nupe2", "23px nupe2small");
            graphics_context_set_fill_color(ctx, config["CONFIG_COLOR_WEATHER"]);
            ctx.textAlign = "center";
            var weather = config["CONFIG_WEATHER_LOCAL"] ? (config["CONFIG_WEATHER_UNIT_LOCAL"] == 2 ? "a74°" : "a23°") : "";
            var weatherStr = (chalk && config["CONFIG_BLUETOOTH_LOGO"] ? "z" : "") + weather;
            ctx.fillText(weatherStr, w / 2, PBL_IF_ROUND_ELSE(60, 60));
        }

        var config_battery_logo = config["CONFIG_BATTERY_LOGO"];
        var battery_state = {
            charge_percent: 70
        };
        if (config_battery_logo == 1 || config_battery_logo == 2) {
            var battery = PBL_IF_ROUND_ELSE(GRect((w
                - 13) / 2, 21.5, 14, 8), GRect(125.5, 3.5, 14, 8));
            if (config_square) {
                battery.origin.x = 123.5;
                battery.origin.y = 7.5;
            }
            var battery_color = config["CONFIG_COLOR_BATTERY_LOGO"];
            ctx.lineWidth = 1.2;
            var GCornerNone = null;
            graphics_context_set_stroke_color(ctx, COLOR(battery_color));
            graphics_context_set_fill_color(ctx, COLOR(battery_color));
            graphics_draw_rect(ctx, battery);
            graphics_fill_rect(ctx, GRect(battery.origin.x + 1.5, battery.origin.y + 1.5, battery_state.charge_percent / 10, 5),
                0, GCornerNone);
            graphics_draw_line_with_width(ctx, GPoint(battery.origin.x + battery.size.w + 1, battery.origin.y + 2.5),
                GPoint(battery.origin.x + battery.size.w + 1, battery.origin.y + 5.5), 1);
        }

        if (!chalk && config["CONFIG_BLUETOOTH_LOGO"]) {
            var origin = GPoint(9.5, 9.5);
            var BLUETOOTH_LOGO_STEP = 3;
            var config_color_bluetooth_logo_2 = config["CONFIG_COLOR_BLUETOOTH_LOGO_2"];
            var config_color_bluetooth_logo = config["CONFIG_COLOR_BLUETOOTH_LOGO"];
            graphics_context_set_fill_color(ctx, COLOR(config_color_bluetooth_logo));
            //graphics_fill_rect(ctx, GRect(origin.x - 2.5, origin.y - 2.5, BLUETOOTH_LOGO_STEP * 2 + 5, BLUETOOTH_LOGO_STEP * 4 + 5), 2);
            roundRect(ctx, origin.x - 2.5, origin.y - 2.5, BLUETOOTH_LOGO_STEP * 2 + 5, BLUETOOTH_LOGO_STEP * 4 + 5, 2, true, false);

            // logo on the inside
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
        }
    }

    function roundRect(ctx, x, y, width, height, radius, fill, stroke) {
        if (typeof stroke == 'undefined') {
            stroke = true;
        }
        if (typeof radius === 'undefined') {
            radius = 5;
        }
        if (typeof radius === 'number') {
            radius = {tl: radius, tr: radius, br: radius, bl: radius};
        } else {
            var defaultRadius = {tl: 0, tr: 0, br: 0, bl: 0};
            for (var side in defaultRadius) {
                radius[side] = radius[side] || defaultRadius[side];
            }
        }
        ctx.beginPath();
        ctx.moveTo(x + radius.tl, y);
        ctx.lineTo(x + width - radius.tr, y);
        ctx.quadraticCurveTo(x + width, y, x + width, y + radius.tr);
        ctx.lineTo(x + width, y + height - radius.br);
        ctx.quadraticCurveTo(x + width, y + height, x + width - radius.br, y + height);
        ctx.lineTo(x + radius.bl, y + height);
        ctx.quadraticCurveTo(x, y + height, x, y + height - radius.bl);
        ctx.lineTo(x, y + radius.tl);
        ctx.quadraticCurveTo(x, y, x + radius.tl, y);
        ctx.closePath();
        if (fill) {
            ctx.fill();
        }
        if (stroke) {
            ctx.stroke();
        }

    }
    var lookKeys = [
        "CONFIG_COLOR_OUTER_BACKGROUND",
        "CONFIG_COLOR_INNER_BACKGROUND",
        "CONFIG_COLOR_MINUTE_HAND",
        "CONFIG_COLOR_INNER_MINUTE_HAND",
        "CONFIG_COLOR_HOUR_HAND",
        "CONFIG_COLOR_INNER_HOUR_HAND",
        "CONFIG_COLOR_CIRCLE",
        "CONFIG_COLOR_TICKS",
        "CONFIG_COLOR_DAY_OF_WEEK",
        "CONFIG_COLOR_DATE",
        "CONFIG_COLOR_BATTERY_LOGO",
        "CONFIG_COLOR_BLUETOOTH_LOGO",
        "CONFIG_COLOR_BLUETOOTH_LOGO_2",
        "CONFIG_COLOR_WEATHER"
    ];
    var chalkLookKeys = [
        "CONFIG_COLOR_INNER_BACKGROUND",
        "CONFIG_COLOR_MINUTE_HAND",
        "CONFIG_COLOR_INNER_MINUTE_HAND",
        "CONFIG_COLOR_HOUR_HAND",
        "CONFIG_COLOR_INNER_HOUR_HAND",
        "CONFIG_COLOR_CIRCLE",
        "CONFIG_COLOR_TICKS",
        "CONFIG_COLOR_DAY_OF_WEEK",
        "CONFIG_COLOR_DATE",
        "CONFIG_COLOR_BATTERY_LOGO",
        "CONFIG_COLOR_WEATHER"
    ];
    function getLookKeys(platform) {
        if (platform == "chalk") {
            return chalkLookKeys;
        }
        return lookKeys;
    }
    /** Return a string that identifies this look. */
    function lookSignature(platform, config) {
        var keys = getLookKeys(platform);
        var s = "";
        for (var i = 0; i < keys.length; i++) {
            var key = keys[i];
            s += key;
            s += config[key];
        }
        return s;
    }
    /** Return a default configuration, that has the look of config. */
    function filterLook(platform, config) {
        var res = defaultConfig(platform);
        var keys = getLookKeys(platform);
        for (var i = 0; i < keys.length; i++) {
            var key = keys[i];
            res[key] = config[key];
        }
        return res;
    }
    /** Is this configuration a default configuration? */
    function isDefaultLook(platform, config) {
        return sameLook(platform, defaultConfig(platform), b);
    }
    /** Do these two configurations look the same? */
    function sameLook(platform, a, b) {
        var keys = getLookKeys(platform);
        for (var i = 0; i < keys.length; i++) {
            var key = keys[i];
            if (a[key] != b[key]) return false;
        }
        return true;
    }
    function defaultConfig(platform) {
        var COLOR_FALLBACK = PebbleHelper.COLOR_FALLBACK(platform);
        var PBL_IF_ROUND_ELSE = PebbleHelper.PBL_IF_ROUND_ELSE(platform);
        // defaults are also in src/obsidian.c, src/js/pebble-js-app.js and config/js/preview.js
        var defaults = {
            CONFIG_COLOR_OUTER_BACKGROUND: COLOR_FALLBACK(GColor.DarkGray, GColor.Black),
            CONFIG_COLOR_INNER_BACKGROUND: COLOR_FALLBACK(GColor.White, GColor.White),
            CONFIG_COLOR_MINUTE_HAND: COLOR_FALLBACK(GColor.Black, GColor.Black),
            CONFIG_COLOR_INNER_MINUTE_HAND: COLOR_FALLBACK(GColor.LightGray, GColor.Black),
            CONFIG_COLOR_HOUR_HAND: COLOR_FALLBACK(GColor.JaegerGreen, GColor.Black),
            CONFIG_COLOR_INNER_HOUR_HAND: COLOR_FALLBACK(GColor.LightGray, GColor.Black),
            CONFIG_COLOR_CIRCLE: COLOR_FALLBACK(GColor.Black, GColor.Black),
            CONFIG_COLOR_TICKS: COLOR_FALLBACK(GColor.Black, GColor.Black),
            CONFIG_COLOR_DAY_OF_WEEK: COLOR_FALLBACK(GColor.JaegerGreen, GColor.Black),
            CONFIG_COLOR_DATE: COLOR_FALLBACK(GColor.Black, GColor.Black),
            CONFIG_BATTERY_LOGO: 1,
            CONFIG_COLOR_BATTERY_LOGO: COLOR_FALLBACK(PBL_IF_ROUND_ELSE(GColor.DarkGray, GColor.Black), GColor.White),
            CONFIG_COLOR_BATTERY_30: COLOR_FALLBACK(PBL_IF_ROUND_ELSE(GColor.Yellow, GColor.Black), GColor.White),
            CONFIG_COLOR_BATTERY_20: COLOR_FALLBACK(PBL_IF_ROUND_ELSE(GColor.Orange, GColor.Black), GColor.White),
            CONFIG_COLOR_BATTERY_10: COLOR_FALLBACK(PBL_IF_ROUND_ELSE(GColor.Red, GColor.Black), GColor.White),
            CONFIG_COLOR_BATTERY_BG_30: COLOR_FALLBACK(PBL_IF_ROUND_ELSE(GColor.White, GColor.Yellow), GColor.Black),
            CONFIG_COLOR_BATTERY_BG_20: COLOR_FALLBACK(PBL_IF_ROUND_ELSE(GColor.White, GColor.Orange), GColor.Black),
            CONFIG_COLOR_BATTERY_BG_10: COLOR_FALLBACK(PBL_IF_ROUND_ELSE(GColor.White, GColor.Red), GColor.Black),
            CONFIG_COLOR_BLUETOOTH_LOGO: COLOR_FALLBACK(GColor.White, GColor.White),
            CONFIG_COLOR_BLUETOOTH_LOGO_2: COLOR_FALLBACK(GColor.Black, GColor.Black),
            CONFIG_BLUETOOTH_LOGO: +true,
            CONFIG_VIBRATE_DISCONNECT: +true,
            CONFIG_VIBRATE_RECONNECT: +true,
            CONFIG_MESSAGE_DISCONNECT: +true,
            CONFIG_MESSAGE_RECONNECT: +true,
            CONFIG_MINUTE_TICKS: 1,
            CONFIG_HOUR_TICKS: 1,
            CONFIG_WEATHER_LOCAL: +true,
            CONFIG_COLOR_WEATHER: COLOR_FALLBACK(GColor.Black, GColor.Black),
            CONFIG_WEATHER_MODE_LOCAL: 1,
            CONFIG_WEATHER_UNIT_LOCAL: 2,
            CONFIG_WEATHER_SOURCE_LOCAL: 1,
            CONFIG_WEATHER_APIKEY_LOCAL: "",
            CONFIG_WEATHER_LOCATION_LOCAL: "",
            CONFIG_WEATHER_REFRESH: 10,
            CONFIG_WEATHER_EXPIRATION: 3 * 60,
            CONFIG_SQUARE: +false
        };
        return cloneConfig(defaults);
    }
    function cloneConfig(config) {
        var res = {};
        for (k in config) {
            res[k] = config[k];
        }
        return res;
    }

    return {
        filterLook: filterLook,
        sameLook: sameLook,
        isDefaultLook: isDefaultLook,
        lookSignature: lookSignature,
        defaultConfig: defaultConfig,
        drawPreview: function(config, canvasId, platform) {
            var first = !(canvasId in configurations);
            platforms[canvasId] = platform;
            configurations[canvasId] = config;
            if (first) {
                // schedule updates to redraw the configuration in case the fonts aren't loaded yet
                var fontUpdate = function (i) {
                    var timeout = 1000;
                    if (i < 5) {
                        timeout = 500;
                    } else if (i > 10) {
                        timeout = 10000;
                    }
                    if (i > 15) return;
                    setTimeout(function(){
                        drawConfig(canvasId);
                        fontUpdate(i + 1);
                    }, timeout);
                };
                fontUpdate(0);
            }
            drawConfig(canvasId);
        }
    }

}());