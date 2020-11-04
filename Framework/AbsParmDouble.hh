//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsParmDouble.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AbsParmDouble
//
//      Command handler for the "parm" command for
//	the module.  Provides a "double" value. Valid commands are
//
//	parm set <n> <x>                        set variable n to x
//      parm list                               print parms
//
//
// AbsParmDouble provides a 'double' that can be set and examined via
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
//      Marc Turcotte           Merged R.Kapur's TK interface 5/98
//
// Copyright Information:
//	Copyright (C) 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1998      University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef ABSPARMDOUBLE_HH
#define ABSPARMDOUBLE_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AbsParm.hh"
#include "Framework/AppConfigRecorder.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AbsParmDouble : public AbsParm {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AbsParmDouble( const char* const theCommand, AppModule* theTarget, 
		 double def = 0 );
  AbsParmDouble( const AbsParmDouble& theOther, AppModule* theTarget);
  
  // Destructor
  virtual ~AbsParmDouble( );
  
  // Handler is inherited

  // value accessor
  virtual const double& value() const       { return _x; }  
  std::string     type  () const       {return "Double";}

  std::string classDescription () const { 
    return "double parameter: set or list."; 
  }

  void dumpState(AppConfigRecorder* r){r->store(value(), "set");};
  void restoreState(char* [] );

  // set method
  void set(const double v) { _x = v; };
 
protected:

  virtual int setHandler    ( );
  virtual int listHandler   ( ) const;
  virtual int getHandler    ( );
  
  
private:

  // Copy and assignment not implemented.
  AbsParmDouble( const AbsParmDouble& );
  AbsParmDouble& operator=( const AbsParmDouble& );

  double _x;
  
};

#endif
