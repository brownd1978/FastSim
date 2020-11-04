
add_library(TrkEnv SHARED 
KalContext.cc
TrkEnv.cc
TrkSimpleTimeAdjuster.cc
TrkTimeAdjuster.cc
TrkVolumeHandle.cc
)
target_link_libraries(TrkEnv BbrGeom TrajGeom ProxyDict AbsEnv BField)

# set top-level directory as include root
target_include_directories(TrkEnv PRIVATE ${CLHEP_INCLUDE_DIRS}/g ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(TrkEnv PROPERTIES VERSION ${PROJECT_VERSION})
