//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsDetEnv.hh 483 2010-01-13 14:03:08Z stroili $
//
// Description:
//	Class AbsDetEnv
//      
//      Abstract class for detector envirnoments.  Individual
//      detectors' environment classes will inherit from this
//      class and define their own environment variables.  Access
//      to these variables is via an asbtract structure, which 
//      also contains neighbour information.  See EmcEnv.cc/hh for
//      an example. 
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Bob Jacobsen		        Original Author
//      Stephen J. Gowdy                University Of Edinburgh
//      Phil Strother                   Imperial College
//
// Copyright Information:
//	Copyright (C) 1996		Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------

#ifndef ABSDETENV_HH
#define ABSDETENV_HH

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class AbsDetStructure;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AbsDetEnv
{
public:
  // Constructors
  AbsDetEnv() {}
  
  // Destructor
  virtual ~AbsDetEnv( );

  // member functions
  virtual const AbsDetStructure* getDetStructure() const;

  virtual void setDetStructure(const AbsDetStructure* detStructure);

protected:
  AbsDetStructure* _detStructure;

};

#endif // ABSDETENV


