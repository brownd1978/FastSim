//--------------------------------------------------------------------------
// File and Version Information:
//
// Description:
//	Class AbsParmIfdStrKey
//
//      Command handler for the "parm" command for
//	the module.  Provides a "filename" value. Valid commands are
//
//	parm set <x>                            set variable to x
//      parm list                               print parms
//
//
// AbsParmIfdStrKey is the standard way to put a secondary key parameter in a 
// module.  Recall that a secondary key is needed whenever two data
// items of the same type are stored in the event.  For example, there
// are several HepAList<TrkRecoTrk> lists in the event:  a default list,
// a Dch list, and an Svt list.  Secondary keys distinguish them.
//
// An AbsParmIfdStrKey is a parameter that allows a string provied by
// a user in a Tcl script to be converted into a key for usage in accessing
// the event.  Continuing with the example above, a track processing
// module might have an AbsParmIfdStrKey that has a default value
// of "DefaultTrack," but which the user may override in Tcl to change
// which list of tracks is used by the module.  If we called the parm
//    AbsParmIfdStrKey  key;
// we would fetch the data from the event with
//    HepAList<TrkRecoTrk> *tl = 
//       Ifd< HepAList<TrkRecoTrk> >::get( ev, key.value() );
//
// An example is given in FrameExample/ParmExample.cc.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Ed Frank                University of Pennsylvania
//      Marc Turcotte           Univeristy of Texas at Dallas 5/98
//
//------------------------------------------------------------------------

#ifndef ABSPARMIFDSTRKEY_HH
#define ABSPARMIFDSTRKEY_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AbsParm.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "ProxyDict/IfdStrKey.hh"
#include "Framework/AppConfigRecorder.hh"
#include "CLHEP/String/Strings.h" 

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AbsParmIfdStrKey : public AbsParm {

//--------------------
// Instance Members --
//--------------------

public:


  // Constructors
  AbsParmIfdStrKey( const char* const theCommand,
		    AppModule* theTarget,
		    HepString def );
  AbsParmIfdStrKey( const AbsParmIfdStrKey& theOther, AppModule* theTarget); 
  
  // Destructor
  virtual ~AbsParmIfdStrKey( );
  
  // Handler is inherited
  
  // Operators
  
  virtual int operator==( const AbsParmIfdStrKey & ) const;  
  
  // value accessor
  virtual const IfdKey& value() const { return *_x; }
  std::string  type () const {return "StringKey";}
  std::string  valueString() const { return (*_x).asString(); }

  std::string classDescription () const {
    return "string key parameter: set or list.";
  }
    
  void dumpState(AppConfigRecorder* r){r->store((char*)(_x->asString()), 
						"set");}
  void restoreState(char* [] );
  
protected:
  
  virtual int setHandler    ( );
  virtual int listHandler   ( ) const;
  virtual int getHandler    ( );
  
  
private:

  // Copy and assignment not implemented.
  AbsParmIfdStrKey( const AbsParmIfdStrKey& );
  AbsParmIfdStrKey& operator=( const AbsParmIfdStrKey& );

  IfdStrKey* _x;
  
};

#endif
