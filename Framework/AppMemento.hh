//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppMemento.hh 631 2010-04-28 06:46:51Z stroili $
//
// Description:
//	Class AppMemento.
//      Do not use this for templated class (foo<T>).  use TemplateTemplate.hh
//      instead.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Akbar Mokhtarani>		originator
//	<Author2>		<originator/contributor etc.>
//
// Copyright Information:
//	Copyright (C) 1998	LBNL
//
//------------------------------------------------------------------------

#ifndef APPMEMENTO_HH
#define	APPMEMENTO_HH

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
#include "FrameUtil/APPList.hh"
#include "Framework/AppExecutable.hh"
//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class AbsModuleMemento;
class AppModule;
class AppSequence;
class AppFramework;
#include <string>
#include <iosfwd>
#include <iosfwd>

void parseLine(char*, const char*, char* [] );

//		---------------------
// 		-- Class Interface --
//		---------------------

class AppMemento  {

public:
  
  // Constructors
  AppMemento(AppFramework*);

  // Destructor
  virtual ~AppMemento( );
  
  // Operators
  
  void DumpConfig( std::ostream& );
  void DumpConfigTcl( std::ostream& );
  void restoreConfig( std::fstream& );

private:
  
  void restoreStates(std::fstream& , AppModule*);
  void restoreFramework(std::fstream&);
  void read_seq_nodes(AppSequence*, int, std::fstream& );
  void makeClone(char*, char*);
  void missingModules();  
  void load();  

  // Not implemented.
  AppMemento( const AppMemento& );
  AppMemento& operator=( const AppMemento& );
  
  // Data members
  const char* _fs ;
  AppFramework*   _framework;
  APPList<std::string*>  _modulesNotFound;
  APPList<std::string*>  _modules;
  APPList<AbsModuleMemento*>* _execList;
  bool _deleteAllPath;

};


#endif // APPMEMENTO_HH









