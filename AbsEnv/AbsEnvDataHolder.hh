//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsEnvDataHolder.hh 483 2010-01-13 14:03:08Z stroili $
//
// Description:
//      Abstract environmental dataholder designed to be contained 
//      by objects inheriting from AbsDetEnv
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Phil Strother           Imperial College
//
//------------------------------------------------------------------------

#ifndef ABSENVDATAHOLDER_HH
#define ABSENVDATAHOLDER_HH

//---------------
// C++ Headers --
//---------------

//----------------------
// Base Class Headers --
//----------------------

#include "AbsEnv/AbsEnvObj.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------

class AbsEnvDataHolder : public AbsEnvObj {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AbsEnvDataHolder(){}
  AbsEnvDataHolder( const AbsEnvDataHolder& ){};

  // Destructor
  virtual ~AbsEnvDataHolder();


protected:

  // Selectors (const)
  virtual const AbsEnvObj* getIndividualDatum(long ) const=0;


};

#endif
