
add_library(BaBar SHARED 

Constants.cc
clock_gettime.cc
)

# set top-level directory as include root
target_include_directories(BaBar PRIVATE ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(BaBar PROPERTIES VERSION ${PROJECT_VERSION})
