{
  "targets": [
    {
      "target_name": "R",
      "sources": [ "src/rl.cpp",
                   "src/rlink.cpp"],
      'conditions': [
        ['OS=="win"',{
          "variables": {
              'R_HOME%' : '<!(R RHOME | sed -e "s|\\\\|\/|g)',
              'R_ARCH%' : '<!(Rscript -e "cat(.Platform$r_arch)")',
              'RCPPFLAGS%' : '<!(<(R_HOME%)/bin/R <(R_ARCH%) CMD config --cppflags)',
              'RLDFLAGS%'  : '<!(<(R_HOME%)/bin/R <(R_ARCH%) CMD config --ldflags)',
              'RBLAS%'     : '<!(<(R_HOME%)/bin/R <(R_ARCH%) CMD config BLAS_LIBS)',
              'RLAPACK%'   : '<!(<(R_HOME%)/bin/R <(R_ARCH%) CMD config LAPACK_LIBS)',
              'RCPPINCL%'  : '<!(<(R_HOME%)/bin/<(R_ARCH%)/Rscript -e "Rcpp:::CxxFlags()")',
              'RCPPLIBS%'  : '<!(<(R_HOME%)/bin/<(R_ARCH%)/Rscript -e "Rcpp:::LdFlags()")',
              'RINSIDEINCL%' : '<!(<(R_HOME%)/bin/<(R_ARCH%)/Rscript -e "RInside:::CxxFlags()")',
              'RINSIDELIBS%' : '<!(<(R_HOME%)/bin/<(R_ARCH%)/Rscript -e "RInside:::LdFlags()")',
          }
        }],
        ['OS=="mac"',{
        }],
        ['OS=="linux"',{
          "variables": {
              'R_HOME%' : '<!(R RHOME)',
              'RCPPFLAGS%' : '<!(<(R_HOME%)/bin/R CMD config --cppflags | sed "s/^...//")',
              'RLDFLAGS%' : '<!(<(R_HOME%)/bin/R CMD config --ldflags)',
              'RBLAS%' : '<!(<(R_HOME%)/bin/R CMD config BLAS_LIBS)',
              'RLAPACK%' : '<!(<(R_HOME%)/bin/R CMD config LAPACK_LIBS)',
              'RINSIDEINCL%' : '<!(echo "RInside:::CxxFlags()" | <(R_HOME%)/bin/R --vanilla --slave | sed "s/^...//")',
              'RINSIDELIBS%' : '<!(echo "RInside:::LdFlags()" | <(R_HOME%)/bin/R --vanilla --slave)',
              'RCPPINCL%' : '<!(echo "Rcpp:::CxxFlags()" | <(R_HOME%)/bin/R --vanilla --slave | sed "s/^...//")',
              'RCPPLIBS%' : '<!(echo "Rcpp:::LdFlags()" | <(R_HOME%)/bin/R --vanilla --slave)',
              },

        }]
      ],
          'link_settings': 
            {
              'ldflags': ['<(RLDFLAGS)'],
              'libraries': ['<(RLDFLAGS)',
                            '<(RINSIDELIBS)',
                            '<(RCPPLIBS)',
                            '<(RBLAS)',
                            '<(RLAPACK)',
                            ]
            },
          'include_dirs': [
              '/<(RINSIDEINCL)',
              '/<(RCPPINCL)',
              '/<(RCPPFLAGS)',
                          ], 

          'cflags_cc!': ['-fno-rtti','-fno-exceptions'],
          'cflags_cc+': ['-frtti','-fno-exceptions'],
          }
  ]
}
