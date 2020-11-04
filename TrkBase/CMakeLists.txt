
add_library(TrkBase SHARED 

HelixTraj.cc
NeutParams.cc
NeutTraj.cc
TrkAbsFit.cc
TrkActiveHotSelector.cc
#TrkCompTrk.cc
TrkContext.cc
TrkDchHotSelector.cc
TrkDetElemId.cc
TrkDifPieceTraj.cc
TrkDifPoca.cc
TrkDifTraj.cc
TrkDirection.cc
TrkErrCode.cc
TrkExchangeData.cc
TrkExchangePar.cc
#TrkExpectedHot.cc
#TrkExpectedHotFactory.cc
#TrkExpectedHotSet.cc
#TrkExpectedMap.cc
#TrkExpectedTrk.cc
TrkExtInterface.cc
TrkFit.cc
TrkFitMaker.cc
TrkFitStatus.cc
TrkFitSummary.cc
TrkFitTypeKey.cc
TrkFundHit.cc
#TrkGammaVertex.cc
TrkGauAvgTimeCalculator.cc
TrkHelixData.cc
TrkHelixData_001.cc
TrkHelixUtils.cc
TrkHistory.cc
TrkHitList.cc
TrkHitOnTrk.cc
TrkHitOnTrkUpdater.cc
TrkHitUse.cc
TrkHotList.cc
TrkHotListData.cc
TrkHotListEmpty.cc
TrkHotListFull.cc
TrkHotListUnowned.cc
TrkId.cc
TrkIdManager.cc
TrkKalComposite.cc
TrkKalTrk.cc
TrkModuleId.cc
TrkMomCalculator.cc
TrkMomVisitor.cc
TrkOutAvgTimeCalculator.cc
TrkParams.cc
#TrkPidInfo.cc
TrkPoca.cc
TrkPocaBase.cc
TrkPocaVertex.cc
TrkPocaXY.cc
TrkPrimaryVertex.cc
TrkRecoTrk.cc
TrkRep.cc
TrkSimpTraj.cc
TrkStoreHypo.cc
TrkSvtHotSelector.cc
TrkTypeUtil.cc
TrkVertex.cc
TrkView.cc
TrkVisitor.cc
TrkVolume.cc
)
target_link_libraries(TrkBase  CLHEP::Vector CLHEP::Geometry CLHEP::Matrix ErrLogger difAlgebra CLHEP BaBar BbrGeom TrajGeom ProbTools CommonUtils BField Pdt )

# set top-level directory as include root
target_include_directories(TrkBase PRIVATE ${Boost_INCLUDE_DIRS}/g ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(TrkBase PROPERTIES VERSION ${PROJECT_VERSION})
