cutils_sp = subproject('cutils')
dep = cutils_sp.get_variable('cutils_dep')

src = ['simple-go.c', 'simple-gtp.c', 'simple-gtp-func.c']

executable('test_go', src + ['test.c'], include_directories : inc, dependencies : dep, c_args: CFLAGS )
executable('gtp-start', src + ['gtp-start.c'], include_directories : inc, dependencies : dep, c_args: CFLAGS )
