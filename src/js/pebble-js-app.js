
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

Pebble.addEventListener('ready', function() {
  console.log('[ info/app ] PebbleKit JS ready!');
});

Pebble.addEventListener('showConfiguration', function() {
  var url = 'https://rawgit.com/stefanheule/obsidian/config-5/config/index.html';
  //url = 'file:///home/stefan/dev/projects/obsidian/config/index.html';
  url += '?platform=' + encodeURIComponent(getPlatform());
  url += '&watch=' + encodeURIComponent(getDetails());
  url += '&version=1.6';
  console.log('[ info/app ] Showing configuration page: ' + url);
  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
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

  Pebble.sendAppMessage(config, function() {
    console.log('[ info/app ] Send successful: ' + JSON.stringify(config));
  }, function() {
    console.log('[ info/app ] Send failed!');
  });
});
