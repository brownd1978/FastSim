
add_library(BdbTime SHARED 

BdbDuration.cc
BdbIntervalBase.cc
BdbMinimumInterval.cc
BdbTime.cc
BdbTimeConst.cc
BdbTimeInput.cc
)

# set top-level directory as include root
target_include_directories(BdbTime PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(BdbTime BbrStdUtils BaBar)

# set shared library version equal to project version
set_target_properties(BdbTime PROPERTIES VERSION ${PROJECT_VERSION})
