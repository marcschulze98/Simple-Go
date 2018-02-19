project('simple-go', 'c')

CFLAGS = ['-std=c11', '-fstrict-aliasing', '-Wall', '-Wpedantic', '-Wextra', '-Wno-unused-parameter']

cutils_sp = subproject('cutils')
dep = dependency('cutils', fallback : ['cutils', 'cutils_dep'])

subdir('include')
subdir('src')

