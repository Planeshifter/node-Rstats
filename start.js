var R = require('./build/Release/R');

var h = new R.session(); // 'world'

h.parseEvalQ("cat('\n Hello World \n')");

h.assign('X', 2483)

h.parseEvalQ("x = rnorm(10000)")

h.parseEvalQ("print(X)")

// h.parseEvalQ("print(x[1:100])")

