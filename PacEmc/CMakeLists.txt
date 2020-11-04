
add_library(PacEmc SHARED 

#PacCalQual.cc
PacCaloSplitMerge.cc
PacEmcAbsCalibrator.cc
PacEmcAbsClusterProperty.cc
PacEmcCalibration.cc
PacEmcCalibrator.cc
PacEmcCalibratorShift.cc
PacEmcCluster.cc
PacEmcClusterEnergySums.cc
PacEmcClusterMeas.cc
PacEmcClusterSplitter.cc
PacEmcCylType.cc
PacEmcDigi.cc
PacEmcListSplit.cc
PacEmcMakeIntersections.cc
PacEmcModel.cc
PacEmcShowerLib.cc
PacEmcTMInfo.cc
PacEmcXClMoments.cc
PacReconstructEmc.cc
PacTrkClusterMatch.cc
)
target_link_libraries(PacEmc  ErrLogger PacSim G3Data )

# set top-level directory as include root
target_include_directories(PacEmc PRIVATE ${CLHEP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(PacEmc PROPERTIES VERSION ${PROJECT_VERSION})
