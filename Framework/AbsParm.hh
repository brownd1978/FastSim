//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsParm.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AbsParm
//
//      Abstract base class for the
//      command handler for the "parm" command for
//	the module.  Valid commands are
//
//	parm set <n> <x>           set variable n to x
//      parm list                  write parms, target into cout
//      parm get                   return value 
//
// Desired future extensions:
//      Work with MicroFrame somehow
//      More complicated things than doubles (via similar subclasses
//         like AbsIntParm, etc?)
//
// AbsParm provides an interface to parameters.  The AbsParm
// base class defines basic behavior. Its subclassed
// to AbsParmDouble, etc, for particular data types.
// These abstract classes are then implemented by TclParmDouble, etc.
// The AbsEnv object acts as a factorya.
//
// A module should create one of these
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
//      Marc Turcotte           Merged in R.Kapur's TK interface 5/18
//
// Copyright Information:
//	Copyright (C) 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1998      University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef ABSPARM_HH
#define ABSPARM_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppCommand.hh"

#include <iosfwd>
#include <string>
#include "FrameUtil/APPList.hh"
#include "FrameUtil/APPListIterator.hh"

class AppConfigRecorder;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AbsParm : public AppCommand {

  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AbsParm( const char* const theCommand, AppModule* theTarget );
  AbsParm( const AbsParm& theOther, AppModule* theTarget );

  // Destructor
  virtual ~AbsParm( );
  
  // Handler
  virtual int handle( int argc, char* argv[] );

  void addDescription( const std::string& userString );
  
  // for tcl
  virtual std::string type() const {return "Unknown";};

  // derived classes should _not_ override description().  
  // See classDescription() below.
  std::string description ( ) const;  

  virtual void dumpState( AppConfigRecorder* ) =0;
  virtual void restoreState(char* [] )=0;

    
protected:

  void      show          ( ) const;
  bool      isShowable    ( ) const;

  // Optional description of a parameter, used in help if not blank.
  std::string _userDescription;
  
  virtual int  setHandler    ( ) =0;
  virtual int  listHandler   ( ) const =0;
  virtual int  getHandler    ( ) =0;  // sets result to actual value
 
  // Derived classes expected to write classDescription()  
  virtual std::string classDescription() const = 0;

private:

  // Copy and assignement not implemented.
  AbsParm( const AbsParm& );
  AbsParm& operator=( const AbsParm& );

};

#endif
