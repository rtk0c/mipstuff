project('mipstuff', 'c', version: '4.9.0', default_options : ['c_std=c11'])

incdir = include_directories('.')

subdir('common')
if get_option('build_as')
  subdir('as')
endif
if get_option('build_emu')
  subdir('emu')
endif
