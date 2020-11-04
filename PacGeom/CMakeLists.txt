
add_library(PacGeom SHARED 

PacConeDetElem.cc
PacConeDetType.cc
PacCylBField.cc
PacCylDetElem.cc
PacCylDetType.cc
PacCylVoxel.cc
PacCylVoxelSet.cc
PacDetElem.cc
PacDetSet.cc
PacDetector.cc
PacGradBField.cc
PacHelix.cc
PacMeasurement.cc
PacPieceTraj.cc
PacPlaneDetElem.cc
PacPlaneDetType.cc
PacRectDetType.cc
PacRingDetType.cc
PacVoxel.cc
PacVoxelSet.cc
)
target_link_libraries(PacGeom  ErrLogger DetectorModel PacEmc )

# set top-level directory as include root
target_include_directories(PacGeom PRIVATE ${CLHEP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(PacGeom PROPERTIES VERSION ${PROJECT_VERSION})
