var page = require('webpage').create();
page.open('config/index.html', function() {
  page.render('tmp.png');
  phantom.exit();
});
