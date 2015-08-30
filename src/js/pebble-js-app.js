
Pebble.addEventListener('ready', function() {
  console.log('[ info/app ] PebbleKit JS ready!');
});

Pebble.addEventListener('showConfiguration', function() {
  var url = 'file:///home/stefan/dev/projects/obsidian/config/index.html';
  console.log('[ info/app ] Showing configuration page: ' + url);
  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  var config = JSON.parse(decodeURIComponent(e.response));
  console.log('[ info/app ] Configuration page returned: ' + JSON.stringify(config));

  Pebble.sendAppMessage(config, function() {
    console.log('[ info/app ] Send successful: ' + JSON.stringify(config));
  }, function() {
    console.log('[ info/app ] Send failed!');
  });
});
