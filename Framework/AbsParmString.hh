//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsParmString.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AbsParmString
//
//      Command handler for the "parm" command for
//	the module.  Provides a "filename" value. Valid commands are
//
//	parm set <x>                            set variable to x
//      parm list                               print parms
//
//      An example is given in FrameExample/ParmExample.cc.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Ed Frank                University of Pennsylvania
//      Marc Turcotte           University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef ABSPARMSTRING_HH
#define ABSPARMSTRING_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AbsParm.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppConfigRecorder.hh"

#include <string>

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AbsParmString : public AbsParm {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AbsParmString( const char* const theCommand,
		 AppModule* theTarget,
		 const std::string& def );
  AbsParmString( const AbsParmString& theOther, AppModule* theTarget);
  
  // Destructor
  virtual ~AbsParmString();
  
  // Handler is inherited
  
  // Operators
  
  virtual int operator==( const AbsParmString & ) const;  

  // value accessor
  virtual const std::string& value() const;
  std::string  type () const {return "String";}

  std::string  classDescription() const {
    return "string parameter: set or list.";
  }

  void dumpState(AppConfigRecorder* r){r->store(value(), "set");}
  void restoreState(char* [] );
  
  // set the value
  void set(const std::string& v) { _x = v; }

protected:
  
  virtual int setHandler    ();
  virtual int listHandler   () const;
  virtual int getHandler    ( ); 
   
private:

  // Copy and assignment not implemented.
  AbsParmString( const AbsParmString& );
  AbsParmString& operator=( const AbsParmString& );

  std::string _x;
  
};

#endif
