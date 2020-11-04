//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppModuleMemento.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppModuleMemento.
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

#ifndef APPMODULEMEMENTO_HH
#define APPMODULEMEMENTO_HH

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
#include "Framework/AbsModuleMemento.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "Framework/AppExecutable.hh"

template < class T > class APPList;
class AppConfigRecorder;
#include <iosfwd>

//		---------------------
// 		-- Class Interface --
//		---------------------

class AppModuleMemento : public AbsModuleMemento {

public:
  
  // Constructors
  AppModuleMemento(AppExecutable* theExec);

  // Destructor
  virtual ~AppModuleMemento( );
  
  // Operators
  virtual void Dump(std::ostream&) const; 
  virtual void Dump_tcl(std::ostream&) const; 
  
private:
  
  // Not implemented.
  AppModuleMemento( const AppModuleMemento& );
  AppModuleMemento& operator=( const AppModuleMemento& );
  
protected:

  typedef struct {
    const char* itsCommand;
    AppConfigRecorder* itsRecorder;
  } Command;
  
  APPList<Command*>* _commandList;

};

#endif // APPMODULEMEMENTO_HH



