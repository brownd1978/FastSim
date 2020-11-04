
add_library(RandControl SHARED 

RacAbsEnginePtr.cc
RacEidConfig.cc
RacModules.cc
RacRandomControl.cc
RacRandomPicker.cc
RacRanecuEngine.cc
RacRanecuEnginePtr.cc
RacRunConfig.cc
RacTestInput.cc
)

# set top-level directory as include root
target_include_directories(RandControl PRIVATE ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(RandControl PROPERTIES VERSION ${PROJECT_VERSION})
