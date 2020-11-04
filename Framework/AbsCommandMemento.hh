//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AbsCommandMemento.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AbsCommandMemento. Abstract base class for Memento classes
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

#ifndef ABSCOMMANDMEMENTO_HH
#define ABSCOMMANDMEMENTO_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <iostream>
#include <string>

//----------------------
// Base Class Headers --
//----------------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------
template < class T >
class AbsCommandMemento
{
  
  
public:
  
  // Constructors
  
  //  AbsCommandMemento();
  AbsCommandMemento(T value , std::string type, std::string action="set"); 
  
  // Destructor
  virtual  ~AbsCommandMemento( );
  
  // Operators
  
  // Selectors (const)
  
  T value() const {return _itsValue;}
  std::string type() const { return _itsType;}
  std::string action() const { return _itsAction;}
  // Modifiers
  virtual void Dump(std::ostream& o) const;
  
  // Helper functions
private:
  
  // Friends
  
  // Data members
  
  //------------------
  // Static Members --
  //------------------
  
public:
  
  // Selectors (const)
  
  // Modifiers
  
private:

  // Not implemented.
  AbsCommandMemento( const AbsCommandMemento& );
  AbsCommandMemento& operator=( const AbsCommandMemento& );
  
  // Data members
  
  std::string _itsAction;
  std::string _itsType;
  T  _itsValue;
};

// Inline implementations
#include "Framework/AbsCommandMemento.icc"


#endif // ABSCOMMANDMEMENTO_HH
