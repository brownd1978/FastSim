//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: GenEnvData.hh 510 2010-01-14 15:28:09Z stroili $
//
// Description:
//	Class GenEnvData.  Private data member of GenEnv
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Bob Jacobsen		        Original Author
//      Stephen J. Gowdy                University Of Edinburgh
//      Dave Brown                      LBL
//
// Copyright Information:
//	Copyright (C) 1996		Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------

#ifndef GENENVDATA_HH
#define GENENVDATA_HH

class HepTupleManager;
class HepScalerManager;
class BdbTime;
class EidCondKeyTriplet;
#include <map>
#include <string>
class GenEnvData
{
public:
  GenEnvData()
    : _scalerManager(0),
      _eventTime(0),
      _bkgKey(0),
      _primaryKey(0)
    {
    }

  ~GenEnvData()
    {
    }

  std::map<std::string,HepTupleManager*> _ntupleManager;
  HepScalerManager* _scalerManager;
  // time is owned OUTSIDE GenEnv, by InputModule
  const BdbTime* _eventTime;
  // these keys are owned OUTSIDE GenEnv
  const EidCondKeyTriplet* _bkgKey;
  const EidCondKeyTriplet* _primaryKey;
};

#endif
