node-Rstats
===========

An interface for node.js to statistical programming language R based on the fabulous Rcpp package

## Installation

## Getting Started

Once the package is loaded, we can create an R session by the command 

```javascript
var R  = new Rstats.session(); 
```
## Important Functions

### parseEvalQ

Evaluating R expressions is easy. We can use the *parseEvalQ* function as follows:

```javascript
R.parseEvalQ("cat('\n Hello World \n')");
```

If 

### assign

Numeric values can be easily assigned to variables in the current R session:

```javascript
R.assign('x', 17)
R.assign('y', 3)

// calculate the sum of x+y
R.parseEvalQ("res = x + y; print(res);")
```

### get

To retrieve an object from the R session, we use the *get* command. For example, let us create a 2x2 matrix in R and retrieve it in JavaScript as a nested array:

```javascript
R.parseEvalQ("mat = matrix(1:4,ncol=2,nrow=2)")
var mat = R.get('mat')
```

Internally, the *get* function uses JSON in order to convert the R data types to JavaScript data types.

We can also run much more complicated calculations and expose the R objects to JavaScript. Consider a linear regression example:

```javascript
R.parseEvalQ('x = rnorm(100); y = 4x + rnorm(100); lm_fit = lm(y~x); 
var lm_fit = R.get('lm_fit')
var coefs = lm_fit.coefficients
var residuals = lm_fit.residuals
```
