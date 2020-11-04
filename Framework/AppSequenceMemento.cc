//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppSequenceMemento.cc 631 2010-04-28 06:46:51Z stroili $
//
// Description:
//	Class AppSequenceMemento
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
#include "Framework/AppSequenceMemento.hh"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <fstream>
#include <string>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "FrameUtil/APPList.hh"
#include "FrameUtil/APPListIterator.hh"
#include "Framework/AppSequence.hh"
#include "Framework/AppExecNode.hh"
#include "Framework/AppExecutable.hh"
using std::endl;
using std::fstream;
using std::ostream;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
AppSequenceMemento::AppSequenceMemento(AppExecutable *theExec)
  : AbsModuleMemento(theExec)
  , _theExecList (new APPList<AppExecutable*>)
{
  AppSequence* seq = (AppSequence*) theExec;
  AppExecNode** node;
  APPListIterator<AppExecNode*> theIterator( *seq->nodes() );
  while ( ( node = theIterator( ) ) ) {
    AppExecutable* exec = (*node)->target();
    _theExecList->append(exec);
  }
  
}

//--------------
// Destructor --
//--------------

AppSequenceMemento::~AppSequenceMemento()
{
  delete _theExecList;
}

//-------------
// Methods   --
//-------------

void  AppSequenceMemento::Dump(ostream& o) const
{
  const char* fs = "#";
  o << endl;
  if (exectype() == AppExecutable::APP_sequence) {
    o << "S";
  }
  else {
    o << "P";
  }
  o  << fs << name() << fs;
  o <<( isEnabled() ? "Y" : "N") << fs;
  o << exectype() << fs << _theExecList->length();
  o << endl;
    
  AppExecutable** exec;
  APPListIterator<AppExecutable*> iter ( *_theExecList );
  while ( ( exec = iter () ) ){
    switch ((*exec)->execType()) {
    case AppExecutable::APP_sequence:
      o << "S";
      break;
    default:
      o << "M";
      break;
    }
    o << fs << (*exec)->name() << endl;
  }
  
}

void  AppSequenceMemento::Dump_tcl(ostream& o) const
{
  std::string thisExec;
  if (exectype() == AppExecutable::APP_sequence) {
    thisExec = "sequence";
  }
  else {
    thisExec = "path";
  }

  o << thisExec << " " << ( isEnabled() ? "enable " : "disable ") 
    << name() << endl;
    
  AppExecutable** exec;
  APPListIterator<AppExecutable*> iter ( *_theExecList );
  while ( ( exec = iter () ) ){
    o << thisExec << " append " << name() << " " << (*exec)->name() << endl;
  }

}








