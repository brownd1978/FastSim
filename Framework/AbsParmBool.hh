//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsParmBool.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AbsParmBool
//
//      Command handler for the "parm" command for
//	the module.  Provides a "bool" value. Valid commands are
//
//	parm set <x>                            set variable to x
//      parm list                               print parms
//
//
// AbsParmBool provides a 'bool' that can be set and examined via
// some interface (the Framework command line, etc. No MicroFrame 
// support yet, and
// none envisioned for a while)  A module should create one of these
// for each parameter.  See FrameExample/ParmExample for a sample.
//
//
// Eventually, the AbsEnv class should provide a factory for
// these so they can be subtyped as needed to various database
// classes, etc.  
//
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Bob Jacobsen		Original Author
//      Marc Turcotte           Merged in R.Kapur's TK interface 5/98
//
// Copyright Information:
//	Copyright (C) 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1998      University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef ABSPARMBOOL_HH
#define ABSPARMBOOL_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AbsParm.hh"
#include "Framework/AppConfigRecorder.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AbsParmBool : public AbsParm {

  //--------------------
  // Instance Members --
  //--------------------

public:
  // Constructors
  AbsParmBool( const char* const theCommand, AppModule* theTarget, 
	       bool def = false );
  AbsParmBool( const AbsParmBool& theOther, AppModule* theTarget);
  
  // Destructor
  virtual ~AbsParmBool( );
  
  // Handler is inherited
  
  // value accessor
  virtual const bool& value() const       { return _x; }
  std::string type() const           {return "Boolean";}
  
  std::string classDescription () const { 
    return "bool parameter: set or list."; 
  }

  void dumpState(AppConfigRecorder* r){r->store(value(), "set");};
  void restoreState(char* [] );
  
  // set the value
  void set(const bool v) { _x = v; }
  
protected:
  
  virtual int setHandler    ( );
  virtual int listHandler   ( ) const;
  virtual int getHandler    ( );
  
private:

  // Copy and assignment not implemented.
  AbsParmBool( const AbsParmBool& );
  AbsParmBool& operator=( const AbsParmBool& );

  bool _x;

};

#endif
