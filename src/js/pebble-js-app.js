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

function getWToken() {
    if (Pebble.getWatchToken) {
        return Pebble.getWatchToken();
    } else {
        return 'null';
    }
}

function getUToken() {
    if (Pebble.getAccountToken) {
        return Pebble.getAccountToken();
    } else {
        return 'null';
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
    var url = 'https://stefanheule.com/obsidian/config-8/config/index.html';
    url = 'https://local.com/obsidian/config/0/index.html';
    // url = 'https://rawgit.com/stefanheule/obsidian/master/config/index.html';
    url += '?platform=' + encodeURIComponent(getPlatform());
    url += '&watch=' + encodeURIComponent(getDetails());
    url += '&wtoken=' + encodeURIComponent(getWToken());
    url += '&utoken=' + encodeURIComponent(getUToken());
    url += '&watch=' + encodeURIComponent(getDetails());
    url += '&version=2.1';
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
        if (k.indexOf("_LOCAL") == -1) {
            // we can only deal with integers, but let's make sure they are all ints
            config[k] = +urlconfig[keys[k]];
        }
        localStorage.setItem(k, urlconfig[keys[k]]);
    }
    // don't allow really small values for refresh rate
    if (config["CONFIG_WEATHER_REFRESH"] < 10) {
        config["CONFIG_WEATHER_REFRESH"] = 10;
    }
    // set refresh to 0 to indicate that weather information is off
    if (fullconfig["CONFIG_WEATHER_LOCAL"] == false) {
        config["CONFIG_WEATHER_REFRESH"] = 0;
    }


    console.log('[ info/app ] Configuration page returned: ' + JSON.stringify(fullconfig));

    Pebble.sendAppMessage(config, function () {
        console.log('[ info/app ] Send successful: ' + JSON.stringify(config));
    }, function(e) {
        console.log(JSON.stringify(config));
        console.log('Message failed: ' + JSON.stringify(e));
    });
});

/** Read a configuration element (handles defaults) */
function readConfig(key) {
    var res = localStorage.getItem(key);
    // defaults are also in src/obsidian.c, src/js/pebble-js-app.js and config/index.html
    if (res === null) {
        if (key == "CONFIG_WEATHER_UNIT_LOCAL") {
            return 2;
        } else if (key == "CONFIG_WEATHER_MODE_LOCAL") {
            return 1;
        } else if (key == "CONFIG_WEATHER_LOCATION_LOCAL") {
            return "";
        } else if (key == "CONFIG_WEATHER_SOURCE_LOCAL") {
            return 1;
        }
    }
    return res;
}


var OWM_ICONS = {
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

var WU_ICONS = {
    // see https://www.wunderground.com/weather/api/d/docs?d=resources/icon-sets for details
    "chanceflurries": "h",
    "chancerain": "e",
    "chancesleet": "h",
    "chancesnow": "h",
    "chancetstorms": "f",
    "clear": "a",
    "cloudy": "c",
    "flurries": "h",
    "fog": "i",
    "hazy": "i",
    "mostlycloudy": "c",
    "mostlysunny": "b",
    "partlycloudy": "d",
    "partlysunny": "b",
    "rain": "f",
    "sleet": "h",
    "snow": "h",
    "sunny": "a",
    "tstorms": "g"
};

function parseIconWU(icon) {
    return WU_ICONS[icon];
}

function parseIconOpenWeatherMap(icon) {
    return OWM_ICONS[icon];
}

function parseIconForecastIO(icon) {
    return FORECAST_ICONS[icon];
}

/** Returns true iff a and b represent the same day (ignoring time). */
function sameDate(a, b) {
    return a.getDay() == b.getDay() && a.getFullYear() == b.getFullYear() && a.getMonth() && b.getMonth();
}

/** Callback if determining weather conditions failed. */
function failedWeatherCheck(err) {
    console.log('[ info/app ] weather request failed: ' + err);
    var data = {
        "MSG_KEY_WEATHER_FAILED": 1
    };
    Pebble.sendAppMessage(data);
}

function fetchWeather(latitude, longitude) {

    var now = new Date();

    var daily;
    var mode = +readConfig("CONFIG_WEATHER_MODE_LOCAL");
    if (mode == 3) {
        // use current weather information after 2pm, until 4am
        daily = !(now.getHours() >= 14 || now.getHours() <= 3);
    } else {
        daily = mode == 2; // daily mode
    }

    /** Callback on successful determination of weather conditions. */
    var success = function(temp, icon) {
        if (+readConfig("CONFIG_WEATHER_UNIT_LOCAL") == 2) {
            temp = temp * 9.0/5.0 + 32.0;
        }
        temp = Math.round(temp);
        if (!icon) {
            icon = "a";
        }
        if (daily) {
            icon = icon.toLowerCase();
        } else if (now.getHours() >= 20) {
            icon = icon.toUpperCase();
        }
        icon = icon.charCodeAt(0);
        var data = {
            "MSG_KEY_WEATHER_ICON": icon,
            "MSG_KEY_WEATHER_TEMP": temp
        };
        console.log('[ info/app ] weather send: temp=' + temp + ", icon=" + String.fromCharCode(icon) + ".");
        Pebble.sendAppMessage(data);
    };

    var runRequest = function (url, parse) {
        var req = new XMLHttpRequest();
        // try for 30 seconds to get weather, then time out
        var myTimeout = setTimeout(function(){
            failedWeatherCheck("timeout");
            req.abort();
        }, 30000);
        req.open("GET", url, true);
        req.onload = function () {
            if (req.readyState === 4) {
                clearTimeout(myTimeout);
                if (req.status === 200) {
                    var response = JSON.parse(req.responseText);
                    try {
                        parse(response);
                    } catch (e) {
                        failedWeatherCheck("exception: " + e)
                    }
                } else {
                    failedWeatherCheck("non-200 status: " + req.status + " / " + req.statusText)
                }
            }
        };
        console.log("[ info/app ] loading from " + url);
        req.send(null);
    };

    var source = +readConfig("CONFIG_WEATHER_SOURCE_LOCAL");
    var apikey = readConfig("CONFIG_WEATHER_APIKEY_LOCAL");
    console.log('[ info/app ] requesting weather information (' + (daily ? "daily" : "currently") + ')...');
    if (source == 1) {
        var query = "lat=" + latitude + "&lon=" + longitude;
        query += "&cnt=1&appid=fa5280deac4b98572739388b55cd7591";
        query = "http://api.openweathermap.org/data/2.5/weather?" + query;
        runRequest(query, function (response) {
            var temp = response.main.temp - 273.15;
            if (daily) temp = response.main.temp_max - 273.15;
            var icon = parseIconOpenWeatherMap(response.weather[0].icon);
            console.log('[ info/app ] weather information: ' + JSON.stringify(response));
            success(temp, icon);
        });
    } else if (source == 3) {
        var q = "conditions";
        if (daily) q = "forecast";
        var url = "http://api.wunderground.com/api/" + apikey + "/" + q + "/q/" + latitude + "," + longitude + ".json";
        runRequest(url, function (response) {
            var temp, icon;
            console.log('[ info/app ] weather information: ' + JSON.stringify(response));
            if (daily) {
                for (var i in response.forecast.simpleforecast.forecastday) {
                    var data = response.forecast.simpleforecast.forecastday[i];
                    var date = new Date(data.date.epoch*1000);
                    if (sameDate(now, date)) {
                        console.log('[ info/app ] using this information: ' + JSON.stringify(data));
                        temp = +data.high.celsius;
                        icon = parseIconWU(data.icon);
                        break;
                    }
                }
            } else {
                temp = response.current_observation.temp_c;
                icon = parseIconWU(response.current_observation.icon);
            }
            success(temp, icon);
        });
    } else {
        // source == 2
        var baseurl = "https://api.forecast.io/forecast/" + apikey + "/" + latitude + "," + longitude + "?units=si&";
        var exclude = "exclude=minutely,hourly,alerts,flags,";
        if (daily) {
            exclude += "currently"
        } else {
            exclude += "daily"
        }
        runRequest(baseurl + exclude, function(response) {
            var temp;
            var icon;
            console.log('[ info/app ] weather information: ' + JSON.stringify(response));
            if (daily) {
                for (var i in response.daily.data) {
                    var data = response.daily.data[i];
                    var date = new Date(data.time*1000);
                    if (sameDate(now, date)) {
                        temp = data.apparentTemperatureMax;
                        icon = data.icon;
                        console.log('[ info/app ] using this information: ' + JSON.stringify(data));
                        break;
                    }
                }
            } else {
                temp = response.currently.apparentTemperature;
                icon = response.currently.icon;
            }
            temp = Math.round(temp);
            icon = parseIconForecastIO(icon);
            success(temp, icon);
        });
    }
}

Pebble.addEventListener('appmessage',
    function (e) {
        console.log('[ info/app ] app message received: ' + JSON.stringify(e));
        var dict = e.payload;
        if (dict["MSG_KEY_FETCH_WEATHER"]) {
            var location = readConfig("CONFIG_WEATHER_LOCATION_LOCAL");
            if (location) {
                var loc = location.split(",");
                fetchWeather(loc[0], loc[1]);
            } else {
                navigator.geolocation.getCurrentPosition(
                    function (pos) {
                        var coordinates = pos.coords;
                        fetchWeather(coordinates.latitude, coordinates.longitude);
                    },
                    function (err) {
                        failedWeatherCheck("location not found");
                    },
                    {timeout: 15000, maximumAge: 60000}
                );
            }
        }
    }
);
