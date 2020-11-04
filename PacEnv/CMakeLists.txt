
add_library(PacEnv SHARED 

EdmlConeDetElement.cc
EdmlCylDetElement.cc
EdmlDetElement.cc
EdmlDetVolume.cc
EdmlDetector.cc
EdmlMeasuringDevice.cc
EdmlParser.cc
EdmlRectDetElement.cc
EdmlRingDetElement.cc
EdmlXMLEngine.cc
PacBuildEnv.cc
PacBuildEnvModule.cc
PacConfig.cc
pstring.cc
)

# set top-level directory as include root
target_include_directories(PacEnv PRIVATE ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(PacEnv PROPERTIES VERSION ${PROJECT_VERSION})
