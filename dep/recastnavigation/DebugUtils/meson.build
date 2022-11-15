debugutils_dir = include_directories('Include')
debugutils_sys = include_directories(is_system : true)
debugutils_inc = [debugutils_dir, debugutils_sys]
debugutils_src = files(
  'Source/RecastDebugDraw.cpp',
  'Source/RecastDump.cpp',
  'Source/DebugDraw.cpp',
  'Source/DetourDebugDraw.cpp')
debugutils = static_library(
  'debugutils',
  debugutils_src,
  build_by_default : false,
  link_args : [],
  link_with : [],
  include_directories : debugutils_inc,
  install : false,
  override_options : [],
  objects : [],
  cpp_args : [],
  c_args : [],
  pic : false
)
debugutils_dep = declare_dependency(
  link_args : [],
  link_with : debugutils,
  compile_args : [],
  include_directories : debugutils_inc
)
