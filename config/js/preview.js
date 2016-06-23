var ObsidianPreview = (function () {

    function drawConfig(config, canvasId) {
        var GPoint = function (x, y) {
            return {x: x, y: y};
        };
        var GRect = function (x, y, w, h) {
            return {origin: {x: x, y: y}, size: {w: w, h: h}};
        };
        var canvas = document.getElementById(canvasId);
        var ctx = canvas.getContext('2d');
        var chalk = isChalk();
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

        var TRIG_MAX_ANGLE = Math.PI * 2;
        var get_radial_point = function (radius, angle) {
            return {
                x: Math.sin(angle) * radius + center.x,
                y: -Math.cos(angle) * radius + center.y
            };
        };
        var graphics_draw_line_with_width = function (ctx, p0, p1, w) {
            ctx.lineWidth = w + 0.5;
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

        if (chalk) {
            graphics_context_set_fill_color(ctx, config["CONFIG_COLOR_INNER_BACKGROUND"]);
            graphics_fill_circle(ctx, center, w / 2);
        } else {
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
        }

        if (config["CONFIG_HOUR_TICKS"] != 3) {
            ctx.strokeStyle = color(config["CONFIG_COLOR_TICKS"]);
            ctx.lineCap = "round";
            var tick_width;
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
        }

        ctx.lineWidth = 1 + 1;
        if (config["CONFIG_MINUTE_TICKS"] == 2) {
            // only relevant minute ticks
            var start_min_tick = (t.tm_min / 5) * 5;
            for (i = start_min_tick; i < start_min_tick + 5; ++i) {
                angle = i * Math.PI * 2 / 60;
                graphics_draw_line_with_width(ctx, get_radial_point(radius, angle),
                    get_radial_point(radius - PBL_IF_ROUND_ELSE(3, 3), angle), 0.5);
            }
        } else if (config["CONFIG_MINUTE_TICKS"] == 1) {
            // all minute ticks
            for (i = 0; i < 60; ++i) {
                angle = i * Math.PI * 2 / 60;
                graphics_draw_line_with_width(ctx, get_radial_point(radius, angle),
                    get_radial_point(radius - PBL_IF_ROUND_ELSE(3, 3), angle), 0.5);
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

        if (fontsAvailable && config["CONFIG_WEATHER_LOCAL"] == true) {
            ctx.font = PBL_IF_ROUND_ELSE("23px nupe2", "23px nupe2small");
            graphics_context_set_fill_color(ctx, config["CONFIG_COLOR_WEATHER"]);
            ctx.textAlign = "center";
            ctx.fillText((config["CONFIG_WEATHER_UNIT_LOCAL"] == 2 ? "a74°" : "a23°"), w / 2, PBL_IF_ROUND_ELSE(60, 60));
        }

        var config_battery_logo = config["CONFIG_BATTERY_LOGO"];
        var battery_state = {
            charge_percent: 70
        };
        if (config_battery_logo == 1 || config_battery_logo == 2) {
            var battery = PBL_IF_ROUND_ELSE(GRect((w
                - 14) / 2, 21, 14, 8), GRect(125, 3, 14, 8));
            var battery_color = config["CONFIG_COLOR_BATTERY_LOGO"];
            ctx.lineWidth = 1.2;
            var GCornerNone = null;
            graphics_context_set_stroke_color(ctx, COLOR(battery_color));
            graphics_context_set_fill_color(ctx, COLOR(battery_color));
            graphics_draw_rect(ctx, battery);
            graphics_fill_rect(ctx, GRect(battery.origin.x + 2, battery.origin.y + 2, battery_state.charge_percent / 10, 4),
                0, GCornerNone);
            graphics_draw_line_with_width(ctx, GPoint(battery.origin.x + battery.size.w, battery.origin.y + 2),
                GPoint(battery.origin.x + battery.size.w, battery.origin.y + 5), 1);
        }
    }

    return {
        drawPreview: drawConfig
    }

}());