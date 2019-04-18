
gl_src = [
  'gl.c',
  'vector.c',
  'matrix.c'
]


gl_lib = library('gl', gl_src,
                 include_directories: gl_include_dirs,
                 dependencies: m_dep,
                 link_with: [bmp_lib, models_lib],
                 )

