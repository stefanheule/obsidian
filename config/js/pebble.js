var PebbleHelper = (function () {
    return {
        COLOR_FALLBACK: function (platform) {
            return function(color, bw) {
                if (platform === "aplite" || platform === "diorite") return bw;
                return color;
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