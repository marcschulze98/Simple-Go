project('simple-go', 'c')

CFLAGS = ['-std=c11', '-fstrict-aliasing', '-Wall', '-Wpedantic', '-Wextra', '-Wno-unused-parameter', '-Wconversion', '-Wstrict-aliasing', '-Wstrict-overflow=5']

cutils_dep = dependency('cutils', fallback : ['cutils', 'cutils_dep'])

subdir('include')
subdir('src')

