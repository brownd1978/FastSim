
add_library(PacTrk SHARED 

PacHit.cc
PacHitOnTrk.cc
PacReconstructTrk.cc
PacTrkHitMeas.cc
PacTrkHitView.cc
PacTrkHitViewDch.cc
PacTrkHitViewSi.cc
PacTrkQual.cc
PacTrkResModelSi.cc
PacTrkSimHotMap.cc
PacTrkdEdxMeas.cc
PacTruncMean.cc
)

# set top-level directory as include root
target_include_directories(PacTrk PRIVATE ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(PacTrk PROPERTIES VERSION ${PROJECT_VERSION})
