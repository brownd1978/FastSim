//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppPathMemento.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppPathMemento
//      Do not use this for Template class (foo<T>).  use TemplateTemplate.hh
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
#include "Framework/AppPathMemento.hh"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------

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
AppPathMemento::AppPathMemento(AppExecutable* theExec)
  : AppSequenceMemento(theExec)
{
}

//--------------
// Destructor --
//--------------
AppPathMemento::~AppPathMemento()
{
}

//-------------
// Methods   --
//-------------



