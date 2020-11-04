//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsParmGeneral.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AbsParmGeneral
//
//      Command handler for the "parm" command for
//	the module.  Provides a value of specified type. Valid commands are
//
//	parm set <n> <x>                        set variable n to x
//      parm list                               print parms
//
//
//
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
// 	Bob Jacobsen 		Original Author of AbsParmXXXX class
//	Phil Strother		Imperial College  Templating  21/03/96
//      Luca Lista              HepString implementation      07/05/96
//	Michael Kelsey		Add set_value(long)	      99/03/30
//      Rainer Bartoldus        Decl. copy ctor and assgn op  01/06/11
//
// Bugs:
//	
//	The inline void set_value(xxxx *) stuff is not very nice. 
//	inline void set_vlaue(DefType *) causes instantiation problems.
//	Help anyone? 
//
// 	Assumes cout <<  is defined for the type you are using. 
// 	(OK for intrinsics, not so for a general class.)
// 	
// 	It would be nice if the set_Handler code could drop its 
//	dependance on type altogether but this may not be so easy.
// 	
//------------------------------------------------------------------------



#ifndef ABSPARMGENERAL_HH
#define ABSPARMGENERAL_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AbsParm.hh"
#include "Framework/AppConfigRecorder.hh"
#include <string>

class HepString;


//		---------------------
// 		-- Class Interface --
//		---------------------
 
template <class DefType> class AbsParmGeneral : public AbsParm {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  inline AbsParmGeneral( const char* const theCommand, AppModule* theTarget, 
			 DefType def = 0 );
  inline AbsParmGeneral( const AbsParmGeneral& theOther, 
			   AppModule* theTarget);

  // Destructor
  inline virtual ~AbsParmGeneral( );
  
  // Handler is inherited
  
  // value accessor
  inline virtual const DefType& value() const { return _x; }
  inline std::string   type () const { return "General";}

  std::string classDescription () const { 
    return "general parameter: set or list."; 
  }
 
  void dumpState(AppConfigRecorder* r){r->store(value(), "set");}
  inline void restoreState(char* var[] );
  
  // set methods
  inline  void set(const int v);
  inline  void set(const double v);
  inline  void set(const float v);
  inline  void set(const std::string v);

protected:

  inline virtual int setHandler    ( );
  inline virtual int listHandler   ( ) const;
  inline virtual int getHandler    ( );
  virtual void listValue( AppModule* theInput ) const;

  // The following is overloading in an ugly way.  It would be preferable
  // to do something sensible with streams but it works for now.
  // These routines are only used by set_handler!!!!
  
  inline void set_value(double *);
  inline void set_value(int *);
  inline void set_value(long *);
  inline void set_value(unsigned *);
  inline void set_value(short *);
  inline void set_value(unsigned short *);
  inline void set_value(bool *);
  inline void set_value(HepString *);
  inline void set_value(float *);
  inline void set_value(std::string *);
    
private:

  // Copy and assignment not implemented.
  AbsParmGeneral( const AbsParmGeneral& );
  AbsParmGeneral& operator=( const AbsParmGeneral& );

  DefType _x;
  
};

#include "Framework/AbsParmGeneral.icc"

#endif
