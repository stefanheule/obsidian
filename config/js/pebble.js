var PebbleHelper = (function () {
    return {
        COLOR_FALLBACK: function (platform) {
            return function(color, bw) {
                if (platform === "chalk" || platform === "basalt") return color;
                return bw;
            };
        },
        PBL_IF_ROUND_ELSE: function (platform) {
            return function(round, other) {
                if (platform === "chalk") return round;
                return other;
            };
        }
    }

}());