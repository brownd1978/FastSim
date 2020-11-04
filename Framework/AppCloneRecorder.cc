//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppCloneRecorder.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppCloneRecorder
//      Do not use this for templated class (foo<T>).  use TemplateTemplate.hh
//      instead.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Akbar Mokhtarani		originator
//	<Author2>		<originator/contributor etc.>
//
// Copyright Information:
//	Copyright (C) 1998	LBNL
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppCloneRecorder.hh"

//-------------
// C Headers --
//-------------
extern "C" {
  //#include <assert.h>

}

//---------------
// C++ Headers --
//---------------
//#include <strings.h>
#include <string.h>
//-------------------------------
// Collaborating Class Headers --
//-------------------------------


//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
AppCloneRecorder::AppCloneRecorder(const char* target, const char* newname)
{
  _theTargetName = new char[strlen(target)+1];
  strcpy(_theTargetName, target);
  _theNewName = new char[strlen(newname)+1];
  strcpy(_theNewName, newname);
}
AppCloneRecorder::AppCloneRecorder(const AppCloneRecorder& R)
{
  _theTargetName = new char[strlen(R.targetname())+1];
  strcpy(_theTargetName, R.targetname());
  _theNewName = new char[strlen(R.clonename())+1];
  strcpy(_theNewName, R.clonename());
}


//--------------
// Destructor --
//--------------
AppCloneRecorder::~AppCloneRecorder()
{
  delete [] _theTargetName;
  delete [] _theNewName;

}










