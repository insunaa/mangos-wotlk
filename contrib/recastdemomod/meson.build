fastlz_dir = include_directories('dep/fastlz')
fastlz_sys = include_directories(is_system : true)
fastlz_inc = [fastlz_dir, fastlz_sys]
fastlz_src = files('dep/fastlz/fastlz.c')
fastlz = static_library(
  'fastlz',
  fastlz_src,
  link_args : [],
  link_with : [],
  include_directories : fastlz_inc,
  install : false,
  override_options : [],
  objects : [],
  c_args : ['-DNO_CORE_FUNCS'],
  pic : false
)
fastlz_dep = declare_dependency(
  link_args : [],
  link_with : fastlz,
  compile_args : [],
  include_directories : fastlz_inc
)
libs = ['dep/SDL/lib/x64/SDL.lib', 'dep/SDL/lib/x64/SDLmain.lib']
RecastDemoMod_dir = include_directories(['Include', 'dep/SDL/include', 'dep/stb_image', '../../src/game', '../../src/game/Vmap', '../../src/framework', '../../dep/recastnavigation/Recast/Include', '../../dep/recastnavigation/Detour/Include', '../../dep/recastnavigation/DebugUtils/Include', '../../dep/recastnavigation/DetourCrowd/Include', '../../dep/recastnavigation/DetourTileCache/Include', '../mmap/src', '../../dep/g3dlite/G3D'])
RecastDemoMod_sys = include_directories(is_system : true)
RecastDemoMod_inc = [RecastDemoMod_dir, RecastDemoMod_sys, global_includes]
RecastDemoMod_src = files(
  'Source/MMapData.cpp',
  'Source/GeomData.cpp',
  'Source/SampleInterfaces.cpp',
  'Source/MapData.cpp',
  'Source/ValueHistory.cpp',
  'Source/imgui.cpp',
  'Source/Tools.cpp',
  'Source/PerfTimer.cpp',
  'Source/imguiRenderGL.cpp',
  'Source/ChunkyTriMesh.cpp',
  'Source/NavMeshTesterTool.cpp',
  'Source/Filelist.cpp',
  'Source/main.cpp',
  'Source/CMaNGOS_Map.cpp')
RecastDemoMod = executable(
  'RecastDemoMod',
  RecastDemoMod_src,
  build_by_default : true,
  link_args : [
    '-rdynamic',
    '-lfastlz',
    '-ldetour',
    '-lrecast',
    '-lg3dlite',
    '-ldetourcrowd',
    '-ldetourtilecache',
    '-ldebugutils',
    '-lvmaplib',
    '-lmmaplib'
  ],
  link_with : [fastlz, sharedlib, gamelib, gsoaplib],
  include_directories : RecastDemoMod_inc,
  install : true,
  override_options : [],
  objects : [],
  cpp_args : ['-DDT_POLYREF64', '-DNO_CORE_FUNCS'],
  pie : false
)
