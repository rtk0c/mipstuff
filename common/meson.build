common_sources = files([
  'buffer.c',
  'str.c',
])

common_headers = files([
  'buffer.h',
  'str.h',
])

common_inc = incdir

common_lib = library('common',
  sources: common_sources + common_headers)

common = declare_dependency(
  include_directories: common_inc,
  link_with: common_lib)
