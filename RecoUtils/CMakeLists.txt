
add_library(RecoUtils SHARED 
RecCopyParms.cc
)
#message ( "CLHEP libs: " ${CLHEP_LIBRARIES} )
#target_link_libraries(RecoUtils ${CHLEP_LIBRARIES}) This is broken, missing separator!!
target_link_libraries(RecoUtils  CLHEP::Vector CLHEP::Geometry CLHEP::Matrix CLHEP Framework AbsParm  )

# set top-level directory as include root
target_include_directories(RecoUtils PRIVATE ${CLHEP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(RecoUtils PROPERTIES VERSION ${PROJECT_VERSION})
