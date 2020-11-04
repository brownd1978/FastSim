
add_library(AbsEvent SHARED 

AbsDigi.cc
AbsEvent.cc
AbsEventID.cc
AbsEvtObj.cc
AbsGHit.cc
)

# set top-level directory as include root
target_include_directories(AbsEvent PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(AbsEvent ErrLogger ProxyDict )

# set shared library version equal to project version
set_target_properties(AbsEvent PROPERTIES VERSION ${PROJECT_VERSION})
