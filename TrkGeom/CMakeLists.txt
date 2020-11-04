
add_library(TrkGeom SHARED 

TrkBPLayoutT.cc
TrkCentralVolume.cc
TrkPassive.cc
TrkPassiveTubeType.cc
TrkSTLayoutT.cc
TrkSimpVolume.cc
TrkSvtSLayoutT.cc
TrkType.cc
)

# set top-level directory as include root
target_include_directories(TrkGeom PRIVATE ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(TrkGeom PROPERTIES VERSION ${PROJECT_VERSION})
