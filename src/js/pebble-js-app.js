function getPlatform() {
    if (Pebble.getActiveWatchInfo) {
        var watch = Pebble.getActiveWatchInfo();
        return watch.platform;
    } else {
        return 'aplite';
    }
}

function getDetails() {
    if (Pebble.getActiveWatchInfo) {
        var watch = Pebble.getActiveWatchInfo();
        return JSON.stringify(watch);
    } else {
        return '{}';
    }
}

Pebble.addEventListener('ready', function () {
    console.log('[ info/app ] PebbleKit JS ready!');
    var data = {
        "MSG_KEY_JS_READY": 1
    };
    Pebble.sendAppMessage(data);
});

Pebble.addEventListener('showConfiguration', function () {
    var url = 'https://rawgit.com/stefanheule/obsidian/config-6/config/index.html';
    url = 'file:///home/stefan/dev/projects/obsidian/config/index.html';
    url += '?platform=' + encodeURIComponent(getPlatform());
    url += '&watch=' + encodeURIComponent(getDetails());
    url += '&version=1.10';
    console.log('[ info/app ] Showing configuration page: ' + url);
    Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function (e) {
    var urlconfig = JSON.parse(decodeURIComponent(e.response));

    // decode config
    // config keys are also duplicated in src/obsidian.h, appinfo.json, src/js/pebble-js-app.js and config/index.html
    var keys = {
        "CONFIG_COLOR_OUTER_BACKGROUND": 1,
        "CONFIG_COLOR_INNER_BACKGROUND": 2,
        "CONFIG_COLOR_MINUTE_HAND": 3,
        "CONFIG_COLOR_INNER_MINUTE_HAND": 4,
        "CONFIG_COLOR_HOUR_HAND": 5,
        "CONFIG_COLOR_INNER_HOUR_HAND": 6,
        "CONFIG_COLOR_CIRCLE": 7,
        "CONFIG_COLOR_TICKS": 8,
        "CONFIG_COLOR_DAY_OF_WEEK": 9,
        "CONFIG_COLOR_DATE": 10,
        "CONFIG_BATTERY_LOGO": 11,
        "CONFIG_COLOR_BATTERY_LOGO": 12,
        "CONFIG_COLOR_BATTERY_BG_30": 13,
        "CONFIG_COLOR_BATTERY_BG_20": 14,
        "CONFIG_COLOR_BATTERY_BG_10": 15,
        "CONFIG_COLOR_BLUETOOTH_LOGO": 16,
        "CONFIG_COLOR_BLUETOOTH_LOGO_2": 17,
        "CONFIG_BLUETOOTH_LOGO": 18,
        "CONFIG_VIBRATE_DISCONNECT": 19,
        "CONFIG_VIBRATE_RECONNECT": 20,
        "CONFIG_MESSAGE_DISCONNECT": 21,
        "CONFIG_MESSAGE_RECONNECT": 22,
        "CONFIG_MINUTE_TICKS": 23,
        "CONFIG_HOUR_TICKS": 24,
        "CONFIG_COLOR_BATTERY_30": 25,
        "CONFIG_COLOR_BATTERY_20": 26,
        "CONFIG_COLOR_BATTERY_10": 27,
        "CONFIG_WEATHER_LOCAL": 28,
        "CONFIG_COLOR_WEATHER": 29,
        "CONFIG_WEATHER_MODE_LOCAL": 30,
        "CONFIG_WEATHER_UNIT_LOCAL": 31,
        "CONFIG_WEATHER_SOURCE_LOCAL": 32,
        "CONFIG_WEATHER_APIKEY_LOCAL": 33,
        "CONFIG_WEATHER_LOCATION_LOCAL": 34,
        "CONFIG_WEATHER_REFRESH": 35,
        "CONFIG_WEATHER_EXPIRATION": 36
    };
    var config = {};
    var fullconfig = {};
    for (var k in keys) {
        fullconfig[k] = urlconfig[keys[k]];
        if (!(k.indexOf("_LOCAL") > -1)) {
            config[k] = urlconfig[keys[k]];
        }
        localStorage.setItem(k, urlconfig[keys[k]]);
    }

    console.log('[ info/app ] Configuration page returned: ' + JSON.stringify(fullconfig));

    Pebble.sendAppMessage(config, function () {
        console.log('[ info/app ] Send successful: ' + JSON.stringify(fullconfig));
    }, function () {
        console.log('[ info/app ] Send failed!');
    });
});


var ICONS = {
    // see http://openweathermap.org/weather-conditions for details
    // day icons
    "01d": "a", // sun
    "02d": "b", // cloud and sun
    "03d": "c", // cloud
    "04d": "d", // clouds
    "09d": "e", // rain drops
    "10d": "f", // rain drops
    "11d": "g", // lightning
    "13d": "h", // snow flake
    "50d": "i", // mist
    // night icons
    "01n": "A",
    "02n": "B",
    "03n": "C",
    "04n": "D",
    "09n": "E",
    "10n": "F",
    "11n": "G",
    "13n": "H",
    "50n": "I"
};

var FORECAST_ICONS = {
    "clear-day": "a",
    "clear-night": "A",
    "rain": "f",
    "snow": "h",
    "sleet": "h",
    "wind": "j",
    "fog": "i",
    "cloudy": "d",
    "partly-cloudy-day": "b",
    "partly-cloudy-night": "B",
    "hail": "h",
    "thunderstorm": "g",
    "tornado": "j"
};

/**
 *https://www.wunderground.com/weather/api/d/docs?d=resources/icon-sets
 <img src="http://icons.wxug.com/i/c/i/chanceflurries.gif" alt="http://icons.wxug.com/i/c/i/chanceflurries.gif">
 <img src="http://icons.wxug.com/i/c/i/chancerain.gif" alt="http://icons.wxug.com/i/c/i/chancerain.gif">
 <img src="http://icons.wxug.com/i/c/i/chancesleet.gif" alt="http://icons.wxug.com/i/c/i/chancesleet.gif">
 <img src="http://icons.wxug.com/i/c/i/chancesleet.gif" alt="http://icons.wxug.com/i/c/i/chancesleet.gif">
 <img src="http://icons.wxug.com/i/c/i/chancesnow.gif" alt="http://icons.wxug.com/i/c/i/chancesnow.gif">
 <img src="http://icons.wxug.com/i/c/i/chancetstorms.gif" alt="http://icons.wxug.com/i/c/i/chancetstorms.gif">
 <img src="http://icons.wxug.com/i/c/i/chancetstorms.gif" alt="http://icons.wxug.com/i/c/i/chancetstorms.gif">
 <img src="http://icons.wxug.com/i/c/i/clear.gif" alt="http://icons.wxug.com/i/c/i/clear.gif">
 <img src="http://icons.wxug.com/i/c/i/cloudy.gif" alt="http://icons.wxug.com/i/c/i/clear.gif">
 <img src="http://icons.wxug.com/i/c/i/flurries.gif" alt="http://icons.wxug.com/i/c/i/flurries.gif">
 <img src="http://icons.wxug.com/i/c/i/fog.gif" alt="http://icons.wxug.com/i/c/i/fog.gif">
 <img src="http://icons.wxug.com/i/c/i/hazy.gif" alt="http://icons.wxug.com/i/c/i/hazy.gif">
 <img src="http://icons.wxug.com/i/c/i/mostlycloudy.gif" alt="http://icons.wxug.com/i/c/i/mostlycloudy.gif">
 <img src="http://icons.wxug.com/i/c/i/mostlysunny.gif" alt="http://icons.wxug.com/i/c/i/mostlysunny.gif">
 <img src="http://icons.wxug.com/i/c/i/partlycloudy.gif" alt="http://icons.wxug.com/i/c/i/partlycloudy.gif">
 <img src="http://icons.wxug.com/i/c/i/partlysunny.gif" alt="http://icons.wxug.com/i/c/i/partlysunny.gif">
 <img src="http://icons.wxug.com/i/c/i/sleet.gif" alt="http://icons.wxug.com/i/c/i/sleet.gif">
 <img src="http://icons.wxug.com/i/c/i/rain.gif" alt="http://icons.wxug.com/i/c/i/rain.gif">
 <img src="http://icons.wxug.com/i/c/i/sleet.gif" alt="http://icons.wxug.com/i/c/i/sleet.gif">
 <img src="http://icons.wxug.com/i/c/i/snow.gif" alt="http://icons.wxug.com/i/c/i/snow.gif">
 <img src="http://icons.wxug.com/i/c/i/sunny.gif" alt="http://icons.wxug.com/i/c/i/sunny.gif">
 <img src="http://icons.wxug.com/i/c/i/tstorms.gif" alt="http://icons.wxug.com/i/c/i/tstorms.gif">
 <img src="http://icons.wxug.com/i/c/i/tstorms.gif" alt="http://icons.wxug.com/i/c/i/tstorms.gif">
 <img src="http://icons.wxug.com/i/c/i/tstorms.gif" alt="http://icons.wxug.com/i/c/i/tstorms.gif">
 <img src="http://icons.wxug.com/i/c/i/cloudy.gif" alt="http://icons.wxug.com/i/c/i/cloudy.gif">
 <img src="http://icons.wxug.com/i/c/i/partlycloudy.gif" alt="http://icons.wxug.com/i/c/i/partlycloudy.gif">
 */

function parseIcon(icon) {
    return ICONS[icon].charCodeAt(0);
}

function parseIconForecastIO(icon) {
    if (icon in FORECAST_ICONS) {
        return FORECAST_ICONS[icon].charCodeAt(0);
    }
    return "a".charCodeAt(0);
}


/** Returns true iff a and b represent the same day (ignoring time). */
function sameDate(a, b) {
    return a.getDay() == b.getDay() && a.getFullYear() == b.getFullYear() && a.getMonth() && b.getMonth();
}

function fetchWeather(latitude, longitude) {

    /** Callback on successful determination of weather conditions. */
    var success = function(temp, icon) {
        if (+localStorage.getItem("CONFIG_WEATHER_UNIT_LOCAL") == 2) {
            temp = temp * 9.0/5.0 + 32.0;
        }
        temp = Math.round(temp);
        var data = {
            "MSG_KEY_WEATHER_ICON": icon,
            "MSG_KEY_WEATHER_TEMP": temp
        };
        console.log('[ info/app ] weather send: temp=' + temp + ", icon=" + String.fromCharCode(icon) + ".");
        Pebble.sendAppMessage(data);
    };

    /** Callback if determining weather conditions failed. */
    var failure = function() {
        console.log('[ info/app ] weather request failed');
    };

    var now = new Date();

    var daily;
    var mode = +localStorage.getItem("CONFIG_WEATHER_MODE_LOCAL");
    if (mode == 3) {
        // use current weather information after 2pm, until 4am
        daily = !(now.getHours() >= 14 || now.getHours() <= 3);
    } else {
        daily = mode == 2; // daily mode
    }

    console.log('[ info/app ] requesting weather information (' + (daily ? "daily" : "currently") + ')...');
    if (false) { // TODO
        var query = "lat=" + latitude + "&lon=" + longitude;
        var req = new XMLHttpRequest();
        query += "&cnt=1&appid=fa5280deac4b98572739388b55cd7591";
        req.open("GET", "http://api.openweathermap.org/data/2.5/weather?" + query, true);
        req.onload = function () {
            if (req.readyState === 4) {
                if (req.status === 200) {
                    var response = JSON.parse(req.responseText);
                    var temp = response.main.temp - 273.15;
                    var icon = parseIcon(response.weather[0].icon);
                    console.log('[ info/app ] weather information: ' + JSON.stringify(response));
                    success(temp, icon);
                } else {
                    failure()
                }
            }
        };
        req.send(null);
    } else {
        var req = new XMLHttpRequest();
        var baseurl = "https://api.forecast.io/forecast/4b98aa266403560e3082149a247072fb/" + latitude + "," + longitude + "?units=si&";
        var exclude = "exclude=minutely,hourly,alerts,flags,";
        if (daily) {
            exclude += "currently"
        } else {
            exclude += "daily"
        }
        req.open("GET", baseurl + exclude, true);
        req.onload = function () {
            if (req.readyState === 4) {
                if (req.status === 200) {
                    var response = JSON.parse(req.responseText);
                    var temp;
                    var icon;
                    if (daily) {
                        for (var i in response.daily.data) {
                            var data = response.daily.data[i];
                            var date = new Date(data.time*1000);
                            if (sameDate(now, date)) {
                                temp = data.apparentTemperatureMax;
                                icon = data.icon;
                            }
                        }
                    } else {
                        temp = response.currently.apparentTemperature;
                        icon = response.currently.icon;
                    }
                    temp = Math.round(temp);
                    icon = parseIconForecastIO(icon);
                    console.log('[ info/app ] weather information: ' + JSON.stringify(response));
                    success(temp, icon);
                } else {
                    console.log('[ info/app ] weather request failed');
                }
            }
        };
        req.send(null);
    }
}

Pebble.addEventListener('appmessage',
    function (e) {
        console.log('[ info/app ] app message received: ' + JSON.stringify(e));
        var dict = e.payload;
        if (dict["MSG_KEY_FETCH_WEATHER"]) {
            var location = localStorage.getItem("CONFIG_WEATHER_LOCATION");
            if (location) {
                fetchWeather(location); // TODO
            } else {
                navigator.geolocation.getCurrentPosition(
                    function (pos) {
                        var coordinates = pos.coords;
                        fetchWeather(coordinates.latitude, coordinates.longitude);
                    },
                    function (err) {
                        console.log('[ info/app ] error requesting location: ' + err + '!');
                    },
                    {timeout: 15000, maximumAge: 60000}
                );
            }
        }
    }
);
