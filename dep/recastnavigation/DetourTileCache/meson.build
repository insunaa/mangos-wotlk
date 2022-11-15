detourtilecache_dir = include_directories('Include')
detourtilecache_sys = include_directories(is_system : true)
detourtilecache_inc = [detourtilecache_dir, detourtilecache_sys]
detourtilecache_src = files(
  'Source/DetourTileCacheBuilder.cpp',
  'Source/DetourTileCache.cpp')
detourtilecache = static_library(
  'detourtilecache',
  detourtilecache_src,
  build_by_default : false,
  link_args : [],
  link_with : [],
  include_directories : detourtilecache_inc,
  install : false,
  override_options : [],
  objects : [],
  cpp_args : [],
  c_args : [],
  pic : false
)
detourtilecache_dep = declare_dependency(
  link_args : [],
  link_with : detourtilecache,
  compile_args : [],
  include_directories : detourtilecache_inc
)
