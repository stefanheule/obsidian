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
    //url = 'file:///home/stefan/dev/projects/obsidian/config/index.html';
    url += '?platform=' + encodeURIComponent(getPlatform());
    url += '&watch=' + encodeURIComponent(getDetails());
    url += '&version=1.9';
    console.log('[ info/app ] Showing configuration page: ' + url);
    Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function (e) {
    var urlconfig = JSON.parse(decodeURIComponent(e.response));

    // decode config
    // config keys are also duplicated in src/obsidian.c, appinfo.json, src/js/pebble-js-app.js and config/index.html
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
        "CONFIG_COLOR_BATTERY_10": 27
    };
    var config = {};
    for (var k in keys) {
        config[k] = urlconfig[keys[k]];
    }

    console.log('[ info/app ] Configuration page returned: ' + JSON.stringify(config));

    Pebble.sendAppMessage(config, function () {
        console.log('[ info/app ] Send successful: ' + JSON.stringify(config));
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

function parseIcon(icon) {
    return ICONS[icon].charCodeAt(0);
}

function fetchWeatherForLocation(location){
    var query = "q=" + location;
    fetchWeather(query);
}

function fetchWeatherForCoordinates(latitude, longitude){
    var query = "lat=" + latitude + "&lon=" + longitude;
    fetchWeather(query);
}

function fetchWeather(query) {
    console.log('[ info/app ] requesting weather information...');
    var req = new XMLHttpRequest();
    query += "&cnt=1&appid=fa5280deac4b98572739388b55cd7591";
    req.open("GET", "http://api.openweathermap.org/data/2.5/weather?" + query, true);
    req.onload = function () {
        if (req.readyState === 4) {
            if (req.status === 200) {
                var response = JSON.parse(req.responseText);
                var temperature = Math.round(response.main.temp - 273.15);
                var icon = parseIcon(response.weather[0].icon);
                console.log('[ info/app ] weather information: ' + JSON.stringify(response));
                var data = {
                    "MSG_KEY_WEATHER_ICON": icon,
                    "MSG_KEY_WEATHER_TEMP": temperature
                };
                console.log('[ info/app ] weather send: temp=' + temperature + ", icon=" + String.fromCharCode(icon) + ".");
                Pebble.sendAppMessage(data);
            } else {
                console.log('[ info/app ] weather request failed');
            }
        }
    };
    req.send(null);
}

Pebble.addEventListener('appmessage',
    function (e) {
        console.log('[ info/app ] app message received: ' + JSON.stringify(e));
        var dict = e.payload;
        if (dict["MSG_KEY_FETCH_WEATHER"]) {
            var location = localStorage.getItem("CONFIG_WEATHER_LOCATION");
            if (location) {
                fetchWeatherForLocation(location);
            } else {
                navigator.geolocation.getCurrentPosition(
                    function (pos) {
                        var coordinates = pos.coords;
                        fetchWeatherForCoordinates(coordinates.latitude, coordinates.longitude);
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
