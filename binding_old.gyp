{
  "targets": [
    {
      "target_name": "R",
      "sources": [ "hello.cc" ],
      "variables": {
      'R_HOME%' : '/usr/lib/R',
      'RCPPFLAGS%' : '-I/usr/share/R/include',
      'RLDFLAGS%' : '-L/usr/lib/R/lib -lR',
      'RBLAS%' : '-lblas',
      'RLAPACK%' : '-llapack',
      },
      "include_dirs": [
    '/usr/local/lib/R/site-library/RInside/include',
    '/usr/local/lib/R/site-library/Rcpp/include',
    '/usr/share/R/include',
	],
    'conditions': [
      ['OS=="linux"', {
          'link_settings': 
            {
              'ldflags': ['<(RLDFLAGS)'],
              'libraries': ['-L<(R_HOME)/lib -lR',
'-L/home/philipp/R/RInside/lib -lRInside -Wl,-rpath,/home/philipp/R/RInside/lib'
               ]
            },
      }],
    ],
'cflags_cc!': ['-fno-rtti','-fno-exceptions'],
'cflags_cc+': ['-frtti','-fno-exceptions']
}
  ]
}
