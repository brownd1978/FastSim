
add_library(AbsParm SHARED 

AbsParmIfdStrKey.cc
#AbsParmTestModule.cc
)

# set top-level directory as include root
target_include_directories(AbsParm PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(AbsParm AbsEvent ErrLogger ProxyDict Framework )

# set shared library version equal to project version
set_target_properties(AbsParm PROPERTIES VERSION ${PROJECT_VERSION})
