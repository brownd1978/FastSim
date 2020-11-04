//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: GenEnv.hh 510 2010-01-14 15:28:09Z stroili $
//
// Description:
//	Class GenEnv
//
//      This class will contain general environmental information.
//      Note that material information is only available if
//      MatBuildEnv is included in AppUserBuild (-AFTER- GenBuildEnv).
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

#ifndef GENENV_HH
#define GENENV_HH

//-------------
// C Headers --
//-------------

//---------------
// C++ Headers --
//---------------

//----------------------
// Base Class Headers --
//----------------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class BdbTime;
class DetMaterial;
class EidCondKeyTriplet;
class GenEnvData;
class MaterialInfo;
class HepTupleManager;
class HepScalerManager;
#include <vector>
#include <string>

//		---------------------
// 		-- Class Interface --
//		---------------------

class GenEnv
{
//--------------------
// Declarations     --
//--------------------

  // Typedefs, consts, and enums

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  GenEnv();

  // Copy Constructor

  // Destructor
  virtual ~GenEnv();

  // Operators

  // Selectors (const)

  //  access to event time - now being deprecated
  const BdbTime* eventTime() const;

  // access to event conditions keys
  const EidCondKeyTriplet* mixedBkgCondKey() const;
  const EidCondKeyTriplet* primaryCondKey() const;

  // Modifiers
  HepTupleManager*& ntupleManager();  // should be made the default of the function below
  HepTupleManager*& ntupleManager(const std::string& managername);  // Can be used to set also
  HepScalerManager*& scalerManager();  // Can be used to set also
  MaterialInfo*& materialInfo();  // Can be used to set also
  // Set time function: this should ONLY be called by input modules
  // after they have read in a new event.  Note that the time object
  // is owned OUTSIDE of GenEnv itself
  void setEventTime(const BdbTime* newtime);
  // set conditions keys function
  void setConditionsKeys(const BdbTime*,
			 const EidCondKeyTriplet* primary, 
			 const EidCondKeyTriplet* background);

//  Find the material, given the name
  const DetMaterial* findDetMaterial( const std::string& matName ) const;
// names
  const std::vector< std::string >& materialNames() const;

protected:

  // Helper functions

private:
  // Don't allow these
  GenEnv( const GenEnv& otherEnv );
  GenEnv& operator=( const GenEnv& otherEnv );
  // Data members
  GenEnvData* _privateData;

  double _test;

  MaterialInfo* _matinfo;
// empty list; ugly!!
  std::vector< std::string >* _empty;

//------------------
// Static Members --
//------------------

public:

  // Selectors (const)

  // Modifiers

private:

  // Data members

};

#endif // GENENV_HH
