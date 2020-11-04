
add_library(TrajGeom SHARED 

BbrHelixTraj.cc
TrkLineTraj.cc
TrkPieceLineTraj.cc
TrkTransformVisitor.cc
)
target_link_libraries(TrajGeom  CLHEP::Vector CLHEP::Geometry CLHEP::Matrix ErrLogger difAlgebra CLHEP BaBar BbrGeom )

# set top-level directory as include root
target_include_directories(TrajGeom PRIVATE ${CLHEP_INCLUDE_DIRS}/g ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(TrajGeom PROPERTIES VERSION ${PROJECT_VERSION})
