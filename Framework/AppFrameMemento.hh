//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppFrameMemento.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppFrameMemento.
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

#ifndef APPFRAMEMEMENTO_HH
#define APPFRAMEMEMENTO_HH

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
#include "Framework/AppModuleMemento.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "Framework/AppExecutable.hh"

template < class T > class APPList;
#include <iosfwd>
//		---------------------
// 		-- Class Interface --
//		---------------------

class AppFrameMemento : public AppModuleMemento {

public:
  
  // Constructors
  AppFrameMemento(AppExecutable* theExec);
  // Destructor
  virtual ~AppFrameMemento( );
  
  // Operators
  
  virtual void Dump(std::ostream&) const; 
  virtual void Dump_tcl(std::ostream&) const; 
  
private:

  // Not implemented.
  AppFrameMemento( const AppFrameMemento& );
  AppFrameMemento& operator=( const AppFrameMemento& );
  
};

#endif // APPFRAMEMENTO_HH



