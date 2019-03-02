project('sr1', 'c')

gl_include_dirs = include_directories('include')
add_project_arguments('-Wpedantic', language : 'c')

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)

subdir('ds')
subdir('bmp')
subdir('models')
subdir('gl')
subdir('labs')
subdir('tests')
