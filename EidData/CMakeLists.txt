
add_library(EidData SHARED 
EidCondKeyTriplet.cc
EidEventContext.cc
EidEventContextXTC.cc
EidEventTriplet.cc
EidEventTripletSet.cc
EidInfo.cc
EidPartitionDoublet.cc
)

# set top-level directory as include root
target_include_directories(EidData PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(EidData AbsEvent ErrLogger ProxyDict BdbTime BbrStdUtils OdfCommon )
# set shared library version equal to project version
set_target_properties(EidData PROPERTIES VERSION ${PROJECT_VERSION})
