detourcrowd_dir = include_directories('Include')
detourcrowd_sys = include_directories(is_system : true)
detourcrowd_inc = [detourcrowd_dir, detourcrowd_sys]
detourcrowd_src = files(
  'Source/DetourObstacleAvoidance.cpp',
  'Source/DetourPathQueue.cpp',
  'Source/DetourCrowd.cpp',
  'Source/DetourLocalBoundary.cpp',
  'Source/DetourPathCorridor.cpp',
  'Source/DetourProximityGrid.cpp'
)
detourcrowd = static_library(
  'detourcrowd',
  detourcrowd_src,
  build_by_default : true,
  link_args : [],
  link_with : [],
  include_directories : detourcrowd_inc,
  install : false,
  override_options : [],
  objects : [],
  cpp_args : [],
  c_args : [],
  pic : false
)
detourcrowd_dep = declare_dependency(
  link_args : [],
  link_with : detourcrowd,
  compile_args : [],
  include_directories : detourcrowd_inc
)
