project('simple-go', 'c')

CFLAGS = ['-std=c11', '-fstrict-aliasing', '-Wall', '-Wpedantic', '-Wextra']

cutils_sp = subproject('cutils')
dep = cutils_sp.get_variable('cutils_dep')

subdir('include')
subdir('src')

