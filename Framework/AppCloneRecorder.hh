//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppCloneRecorder.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppCloneRecorder. A simple class to hold the name of clone and
//      its parent. This is for storing and restoring clones 
//      Do not use this for Template class (foo<T>).  use TemplateTemplate.hh
//      instead.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Akbar Mokhtarani	originator
//	<Author2>		<originator/contributor etc.>
//
// Copyright Information:
//	Copyright (C) 1998	LBNL
//
//------------------------------------------------------------------------

#ifndef APPCLONERECORDER_HH
#define APPCLONERECORDER_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------

//----------------------
// Base Class Headers --
//----------------------
#include "AbsEvent/AbsEvtObj.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------

class AppCloneRecorder //: public AbsEvtObj
{
  
  
public:
  
  // Constructors
  
  AppCloneRecorder(const char* , const char*);  
  AppCloneRecorder(const AppCloneRecorder& R );
  
  // Destructor
  ~AppCloneRecorder( );
  
  // Operators
  
  const char* targetname() const {return _theTargetName;}
  const char* clonename() const {return _theNewName;}
  
  // Selectors (const)
  
  // Modifiers
  
  
  // Helper functions
private:

  // Not implemented.
  AppCloneRecorder& operator=( const AppCloneRecorder& );

  //  const char* _theTargetName;
  //  const char* _theNewName;
  char* _theTargetName;
  char* _theNewName;
  
  
  // Data members
  
  //------------------
  // Static Members --
  //------------------
  
public:
  
  // Selectors (const)
  
  // Modifiers
  
private:
  
  // Data members
protected:
  
};

#endif // APPCLONERECORDER_HH

