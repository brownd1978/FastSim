
add_library(BbrGeom SHARED 

BbrAngle.cc
BbrDoubleErr.cc
BbrErrProjector.cc
BbrError.cc
BbrLorentzVectorErr.cc
BbrPointErr.cc
BbrVectorErr.cc
Trajectory.cc
TrkGeomTraj.cc
TrkGeomTrajVisitor.cc
TrkPieceTraj.cc
)
target_link_libraries(BbrGeom  CLHEP::Vector CLHEP::Geometry CLHEP::Matrix ErrLogger difAlgebra CLHEP BaBar)

# set top-level directory as include root
target_include_directories(BbrGeom PRIVATE ${CLHEP_INCLUDE_DIRS}/g ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(BbrGeom PROPERTIES VERSION ${PROJECT_VERSION})
