
add_library(difAlgebra SHARED 

DifArray.cc
DifComplex.cc
DifFourVector.cc
DifIndepPar.cc
DifNumber.cc
DifPoint.cc
DifRotation.cc
DifVector.cc
)
#message ( "CLHEP libs: " ${CLHEP_LIBRARIES} )
#target_link_libraries(difAlgebra ${CHLEP_LIBRARIES}) This is broken, missing separator!!
target_link_libraries(difAlgebra  CLHEP::Vector CLHEP::Geometry CLHEP::Matrix)

# set top-level directory as include root
target_include_directories(difAlgebra PRIVATE ${CLHEP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(difAlgebra PROPERTIES VERSION ${PROJECT_VERSION})
