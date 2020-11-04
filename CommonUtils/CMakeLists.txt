
add_library(CommonUtils SHARED 
freeunit.F
BbrBitVector.cc
BbrRefBar.cc
BbrRefCountedPointerBase.cc
BbrRefFoo.cc
Com3State.cc
ComCovAccum.cc
ComCrc.cc
ComExtendedArgValues.cc
ComExtendedSyntaxParser.cc
ComFloatIOManip.cc
ComIOManip.cc
ComMatrix.cc
ComPackBase.cc
ComPackBool.cc
ComPackBoolsIntoOctet.cc
ComPackEngine.cc
ComPackExpFloat.cc
ComPackFlatFloat.cc
ComPackFlatFloatBroken.cc
ComPackFloat.cc
ComPackFloatBroken.cc
ComPackInt.cc
ComPackNonFlatFloat.cc
ComPackNonFlatFloatBroken.cc
ComPackRat.cc
ComPackSignedExpFloat.cc
ComPackStrings.cc
ComPackUFloat.cc
ComPackUFloatBroken.cc
ComPackedConsistency.cc
ComPackedConsistencyBroken.cc
ComPackedCovariance.cc
ComPackedDEDX.cc
ComPackedDEDXBroken.cc
ComPackedData.cc
ComPathNameSearch.cc
ComReadUtils.cc
ComTimeStamp.cc
ComTimer.cc
#searchFile.cc
#terminationDecoder.cc
#terminator.cc
)

# set top-level directory as include root
target_include_directories(CommonUtils PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(CommonUtils  CLHEP::Vector CLHEP::Geometry CLHEP::Matrix CLHEP ErrLogger ProbTools BaBar)

# set shared library version equal to project version
set_target_properties(CommonUtils PROPERTIES VERSION ${PROJECT_VERSION})
