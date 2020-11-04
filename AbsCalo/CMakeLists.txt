
add_library(AbsCalo SHARED 
#AbsRecoCalo.cc
#AbsRecoCaloErrorMatrix.cc
)

# set top-level directory as include root
target_include_directories(AbsCalo PRIVATE ${CLHEP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(AbsCalo PROPERTIES VERSION ${PROJECT_VERSION})
