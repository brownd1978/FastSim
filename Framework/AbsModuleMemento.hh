//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AbsModuleMemento.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AbsModuleMemento. Abstract base class for Memento classes
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

#ifndef ABSMODULEMEMENTO_HH
#define ABSMODULEMEMENTO_HH

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

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppExecutable.hh"
//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include <iosfwd>

//		---------------------
// 		-- Class Interface --
//		---------------------

class AbsModuleMemento {
    
public:
  
  // Constructors
  AbsModuleMemento(AppExecutable* theExec);

  // Destructor
  virtual ~AbsModuleMemento( );
  
  // Operators
    
  virtual void Dump(std::ostream&)const = 0; 
  virtual void Dump_tcl(std::ostream&)const = 0; 

  const char* name() const    { return _theExec->name();}
  AppExecutable::ExecType exectype() const { return _theExec->execType();}
  bool isEnabled() const {return _theExec->isEnabled();}
  AppExecutable* exec() const {return _theExec;}

private:

  // Not implemented.
  AbsModuleMemento( const AbsModuleMemento& );
  AbsModuleMemento& operator=( const AbsModuleMemento& );
  
  AppExecutable* _theExec;

};

#endif // ABSMODULEMEMENTO_HH


