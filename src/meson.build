src = ['simple-go.c', 'simple-gtp.c', 'simple-gtp-func.c']

executable('test_go', src + ['test.c'], include_directories : inc, dependencies : cutils_dep, c_args: CFLAGS )
executable('gtp-start', src + ['gtp-start.c'], include_directories : inc, dependencies : cutils_dep, c_args: CFLAGS )
