
add_library(AbsEventTag SHARED 

dummyAbsEventTag.cc
)

# set top-level directory as include root
target_include_directories(AbsEventTag PRIVATE ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(AbsEventTag PROPERTIES VERSION ${PROJECT_VERSION})
