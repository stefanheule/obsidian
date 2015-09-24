var system = require('system');
var page = require('webpage').create();
page.open(system.args[2], function() {
  page.render('tmp.png');
  phantom.exit();
});
