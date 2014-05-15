var R = require('./build/Release/R');
var util = require('util');
var h = new R.session(); // 'world'

h.parseEvalQ("cat('\n Hello World \n')");

h.assign('X', 2483)

h.parseEvalQ('print(X)')

h.assign('Y',{firstName:"John", lastName:"Doe", age:50, eyeColor:"blue"})

h.parseEvalQ('print(paste(Y$firstName, Y$lastName,sep=" "))')

var res = h.get('Y')

h.parseEvalQ('mat = matrix(1:4,ncol=2,nrow=2)')
h.parseEvalQ('vec = c(3,2,4.2)')
res2 = h.get('vec')

h.parseEvalQ('x = rnorm(100); y = 4*x + rnorm(100); lm_fit = lm(y~x);');
var lm_fit = h.get('lm_fit')
var coefs = lm_fit.coefficients
var residuals = lm_fit.residuals

console.log(util.inspect(residuals))
/*
h.parseEvalQ("x = rnorm(10000)")

h.parseEvalQ("print(X)")

h.assign("Name","Philipp")

h.parseEvalQ("sen = paste('My name is',Name,'!\n',sep=''); cat(sen);")
// h.parseEvalQ("print(x[1:100])")

a = [5,3]

h.assign("array",a)

h.parseEvalQ("print(array)")

h.parseEvalQ("cat(doesNotExist))")
*/