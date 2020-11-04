
add_library(PacSim SHARED 
PacBrems.cc
PacDKChainGenerator.cc
PacDKChainInfo.cc
PacDKEvtGen.cc
PacDKPointGenerator.cc
PacDKPointInfo.cc
PacDecayTable.cc
PacDetEffect.cc
PacEMShower.cc
PacHadShower.cc
PacMatInt.cc
PacShowerModel.cc
PacSimHit.cc
PacSimHitMergeInfo.cc
PacSimTrack.cc
PacSimulate.cc
PacWriteParticles.cc
)
target_link_libraries(PacSim  ErrLogger G3Data EvtGenBase EvtGen EvtGenModels )

# set top-level directory as include root
target_include_directories(PacSim PRIVATE ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(PacSim PROPERTIES VERSION ${PROJECT_VERSION})
