
executable('main', 'main/main.c',
  include_directories : gl_include_dirs,
  link_with : gl_lib)

configure_file(input : 'test.obj',
               output : 'test.obj',
               copy: true)

configure_file(input : 'test.bmp',
               output : 'test.bmp',
               copy: true)
