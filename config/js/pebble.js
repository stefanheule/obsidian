var PebbleHelper = (function () {
    var platform = "chalk";
    return {
        setPlatform: function(p) {
            platform = p;
        },
        isChalk: function() {
            return platform === "chalk";
        },
        isBasalt: function() {
            return platform === "basalt";
        },
        isAplite: function() {
            return platform === "chalk";
        },
        COLOR_FALLBACK: function(color, bw) {
            if (platform === "chalk" || platform === "basalt") return color;
            return bw;
        },
        PBL_IF_ROUND_ELSE: function(round, other) {
            if (platform === "chalk") return round;
            return other;
        }
    }

}());