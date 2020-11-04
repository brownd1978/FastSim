//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppConfigRecorder.hh 617 2010-04-14 12:23:54Z stroili $
//
// Description:
//	Class AppConfigRecorder. Abstract base class for Memento classes
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

#ifndef APPCONFIGRECORDER_HH
#define APPCONFIGRECORDER_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <string>

//----------------------
// Base Class Headers --
//----------------------
//#include "AbsEvent/AbsEvtObj.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AbsCommandMemento.hh"
#include "FrameUtil/APPList.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
template < class T > class APPList;

#include <iosfwd>
#include <string>

class HepString;

//		---------------------
// 		-- Class Interface --
//		---------------------

class AppConfigRecorder //: public AbsEvtObj
{


public:
  
  // Constructors
  
  AppConfigRecorder();
  
  // Destructor
  ~AppConfigRecorder( );
  
  // Operators
  
  void store (int, const char*);
  void store (double, const char*);
  void store (float, const char*);
  void store (char*, const char*);
  void store (HepString, const char*);
  void store (bool, const char*);
  void store (long, const char*);
  void store (unsigned, const char*);
  void store (int, const char* []);
  void store (std::string, const char*);

  void Dump(std::ostream&, const char*)const ; 
  void Dump_tcl(std::ostream&, const char*)const ; 
  
  // Selectors (const)
  
  // Modifiers
  
  
  // Helper functions
private:
  APPList<AbsCommandMemento< bool >* >* _boolCommandList;
  APPList<AbsCommandMemento< int >* >* _intCommandList;
  APPList<AbsCommandMemento< double >* >* _doubleCommandList;
  APPList<AbsCommandMemento< char* >* >* _charCommandList;
  APPList<AbsCommandMemento< std::string >* >* _stdstringCommandList;
  APPList<AbsCommandMemento< float >* >* _floatCommandList;
  APPList<AbsCommandMemento< long >* >* _longCommandList;
  APPList<AbsCommandMemento< unsigned >* >* _unsignedCommandList;
  APPList<AbsCommandMemento< HepString >* >* _HepStringCommandList;
  APPList< std::string*  >  _argvCommandList;

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
  AppConfigRecorder( const AppConfigRecorder& );
  AppConfigRecorder& operator=( const AppConfigRecorder& );
  
  // Data members
protected:
  
};

#endif // APPCONFIGRECORDER_HH

