re = require('./lib/index');

var mysess = re.session();

mysess.parseEvalQ("cat('Hello World')");