
add_library(Pdt SHARED 
DecayMode.cc
Pdt.cc
PdtEntry.cc
PdtGeant.cc
PdtPid.cc
)
target_link_libraries(Pdt ErrLogger ProxyDict AbsEvent )

# set top-level directory as include root
target_include_directories(Pdt PRIVATE ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(Pdt PROPERTIES VERSION ${PROJECT_VERSION})
