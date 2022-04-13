project('Vi er veldig kule (og har fet bart)', 'c',
  version : '1.0',
  default_options : ['warning_level=3'])

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)

glu_dep = dependency('glu')
glfw_dep = dependency('glfw3')
freeimage_dep = cc.find_library('freeimage')

bass_dep = cc.find_library('bass', required: false)
if not bass_dep.found()
  bass_sdk = get_option('bass-sdk')
  if bass_sdk == ''
    bass_dep = dependency('bass')
  else
    bass_inc = include_directories(join_paths(bass_sdk))
    bass_lib = cc.find_library('bass', dirs : [ join_paths(bass_sdk, 'x64'), bass_sdk ] )
    bass_dep = declare_dependency(dependencies : bass_lib,
                                  include_directories : bass_inc)
  endif
endif

files = files(
  'glad/gl.c',
  'file.c',
  'image-freeimage.c',
  'particles.c',
  'matrix.c',
  'blur.c',
  'main.c',
  'rtt.c',
  'object.c'
)

executable(
  'carlb',
  files,
  include_directories: include_directories('include'),
  dependencies : [ m_dep, glu_dep, glfw_dep, freeimage_dep, bass_dep ]
)
