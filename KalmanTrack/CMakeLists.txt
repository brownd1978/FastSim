
add_library(KalmanTrack SHARED 

KalBend.cc
#KalBetaCons.cc
#KalBrems.cc
KalConstraint.cc
KalEndSite.cc
KalFit.cc
KalHit.cc
KalHitCache.cc
KalInterface.cc
KalMaker.cc
KalMaterial.cc
#KalMiniInterface.cc
#KalMiniListPromoter.cc
#KalMiniPromoter.cc
#KalMiniRX.cc
#KalMiniRep.cc
#KalMiniTrk.cc
#KalMiniTrk_001.cc
#KalPairConduit.cc
#KalPairFit.cc
#KalPairInterface.cc
#KalPairMaker.cc
#KalPairRep.cc
#KalPairSite.cc
#KalPairVisitor.cc
KalParams.cc
#KalRX.cc
KalRep.cc
KalScatter.cc
KalSite.cc
KalSmear.cc
KalStub.cc
KalUtils.cc
KalWeight.cc
)
target_link_libraries(KalmanTrack TrkBase  CLHEP::Vector CLHEP::Geometry CLHEP::Matrix ErrLogger difAlgebra CLHEP BaBar BbrGeom TrajGeom ProbTools CommonUtils BField Pdt TrkEnv ProxyDict DetectorModel Framework AbsParm TrkFitter RecoUtils )

# set top-level directory as include root
target_include_directories(KalmanTrack PRIVATE ${CLHEP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(KalmanTrack PROPERTIES VERSION ${PROJECT_VERSION})
