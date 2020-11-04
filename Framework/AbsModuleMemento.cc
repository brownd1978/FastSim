//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AbsModuleMemento.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AbsModuleMemento
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
#include "Framework/AbsModuleMemento.hh"

//-------------
// C Headers --
//-------------

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
AbsModuleMemento::AbsModuleMemento(AppExecutable* theExec)
  : _theExec(theExec)
{
}

//--------------
// Destructor --
//--------------
AbsModuleMemento::~AbsModuleMemento()
{
}
