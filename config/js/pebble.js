var PebbleHelper = (function () {

    return {
        COLOR_FALLBACK: function(color, bw) {
            if (isBasalt() || isChalk()) return color;
            return bw;
        },
        PBL_IF_ROUND_ELSE: function(round, other) {
            if (isChalk()) return round;
            return other;
        }
    }

}());