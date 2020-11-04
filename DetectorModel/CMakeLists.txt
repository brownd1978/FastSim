
add_library(DetectorModel SHARED 

DetAEAdder.cc
DetAlignElem.cc
#DetAlignElemTester.cc
DetCone.cc
#DetConeTester.cc
DetCylinder.cc
#DetCylinderTester.cc
DetElem.cc
DetElemPointIterator.cc
DetElemPrinter.cc
DetElemSet.cc
DetExternalSet.cc
DetIntersection.cc
DetMaterial.cc
DetNullElem.cc
DetPlane.cc
#DetPlaneTester.cc
DetSCone.cc
DetSet.cc
DetSetHash.cc
DetSimpleVolumeType.cc
DetSurface.cc
DetSurfaceElem.cc
DetSurfaceSet.cc
DetSurfaceType.cc
DetType.cc
DetVolSideIntersection.cc
DetVolumeElem.cc
DetVolumeType.cc
Intersection.cc
)
target_link_libraries(DetectorModel CLHEP::Vector CLHEP::Geometry CLHEP::Matrix ErrLogger CLHEP BaBar TrkBase Pdt ErrLogger Framework)

# set top-level directory as include root
target_include_directories(DetectorModel PRIVATE ${CLHEP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(DetectorModel PROPERTIES VERSION ${PROJECT_VERSION})
