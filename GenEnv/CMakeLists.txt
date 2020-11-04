
add_library(GenEnv SHARED 

GenBuildEnv.cc
GenEnv.cc
)

# set top-level directory as include root
target_include_directories(GenEnv PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(GenEnv EidData ErrLogger ProxyDict Framework AbsEnv )

# set shared library version equal to project version
set_target_properties(GenEnv PROPERTIES VERSION ${PROJECT_VERSION})
