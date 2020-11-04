
add_library(G3Data SHARED 
GEvent.cc
GTrack.cc
GTrackList.cc
GTrimMarker.cc
GVertex.cc
)
target_link_libraries(G3Data  CLHEP::Vector CLHEP::Geometry BbrGeom Pdt AbsEvent)

# set top-level directory as include root
target_include_directories(G3Data PRIVATE ${CLHEP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(G3Data PROPERTIES VERSION ${PROJECT_VERSION})
