
add_library(TrkFitter SHARED 

#TestPointHelix.cc
TrkBmSpotOnTrk.cc
TrkCircleRep.cc
TrkCircleTraj.cc
TrkContextCustom.cc
TrkContextEv.cc
TrkDeadMaker.cc
TrkDeadRep.cc
TrkDifLineTraj.cc
TrkDifPocaXY.cc
TrkHelix.cc
TrkHelixFitter.cc
TrkHelixMaker.cc
#TrkHelixRefiner.cc
TrkHelixRep.cc
TrkIdManDefault.cc
TrkIdManDumb.cc
TrkLineRep.cc
TrkMergeMap.cc
TrkMiniMaker.cc
TrkParVisitor.cc
TrkSimpleRep.cc
)

target_link_libraries(TrkFitter TrkBase  CLHEP::Vector CLHEP::Geometry CLHEP::Matrix ErrLogger difAlgebra CLHEP BaBar BbrGeom TrajGeom ProbTools CommonUtils BField Pdt TrkEnv ProxyDict )

# set top-level directory as include root
target_include_directories(TrkFitter PRIVATE ${CLHEP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(TrkFitter PROPERTIES VERSION ${PROJECT_VERSION})
