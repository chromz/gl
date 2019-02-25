project('sr1', 'c')

gl_include_dirs = include_directories('include')
add_project_arguments('-Wpedantic', language : 'c')

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)
glib_dep = dependency('glib-2.0',
  method : 'pkg-config',
  version : '>=2.58',
  required : true)

subdir('bmp')
subdir('models')
subdir('gl')
subdir('labs')
subdir('tests')
