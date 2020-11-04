
add_library(PacTest SHARED 

Dummy.cc
)

# set top-level directory as include root
target_include_directories(PacTest PRIVATE ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(PacTest PROPERTIES VERSION ${PROJECT_VERSION})
