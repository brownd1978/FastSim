
add_library(PacDetector SHARED 

PacCylDetector.cc
PacElemFactory.cc
#PacMeasurementFactory.cc
PacTypeFactory.cc
)
target_link_libraries(PacDetector ${Boost_LIBRARIES} ErrLogger PacGeom DetectorModel  )

# set top-level directory as include root
target_include_directories(PacDetector PRIVATE ${Boost_INCLUDE_DIRS} ${CLHEP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(PacDetector PROPERTIES VERSION ${PROJECT_VERSION})
