#!/bin/sh
cd test
mkdir -p ~/Rlib
echo 'R_LIBS=~/Rlib' > .Renviron
echo 'options(repos = "http://cran.rstudio.com")' > .Rprofile
echo '.travis.yml' > .Rbuildignore
Rscript -e 'if(!"RJSONIO" %in% rownames(installed.packages())) { install.packages("RJSONIO", dependencies = TRUE) }'
Rscript -e 'if(!"RInside" %in% rownames(installed.packages())) { install.packages("RInside", dependencies = TRUE) }'
Rscript -e 'update.packages(ask = FALSE, instlib = "~/Rlib")'
