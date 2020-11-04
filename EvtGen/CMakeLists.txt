add_library(EvtGen SHARED 
EvtCheckDecays.cc
EvtGen.cc
)
target_link_libraries(EvtGen EvtGenBase EvtGenModels )

# set top-level directory as include root
target_include_directories(EvtGen PRIVATE ${CLHEP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(EvtGen PROPERTIES VERSION ${PROJECT_VERSION})
