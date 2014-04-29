var R = require('./build/Release/R');

var h = new R.session(); // 'world'

h.parseEvalQ("cat('\n Hello World \n')");

h.assign('X', 2483)

h.parseEvalQ("x = rnorm(10000)")

h.parseEvalQ("print(X)")

h.assign("Name","Philipp")

h.parseEvalQ("sen = paste('My name is',Name,'!\n',sep=''); cat(sen);")
// h.parseEvalQ("print(x[1:100])")

a = [5,3]

h.assign("array",a)

h.parseEvalQ("print(array)")