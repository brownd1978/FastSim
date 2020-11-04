//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsParmVector.hh 489 2010-01-13 16:45:39Z stroili $
//
// Description:
//	Class AbsParmVector.
//
//      Command handler for parameters which are vectors of values
//      or symbolic names.
//
//	In the case of values, the valid commands are
//	parm set <x>           Append <x> to the vector.
//      parm list              print parms
//
//      For symbolic names, the valid commands are
//      parm set <name>         Append, to vector, parm corresponding to name
//      parm list               Display current parm value
//      Note that the module must register each valid object by value with an 
//      associated string using the method
//        addItem("name", value);
//
//
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
// 	Bob Jacobsen 		Original Author of AbsParmXXXX class
//      Ed Frank                University of Pennsylvania.
//
// Bugs:
//	No way to reset the vector to nothing from Tcl.  Since these are
//      parms and parms are "set once, leave forever" this should be
//      OK.
//
//	The inline void set_value(xxxx *) stuff is not very nice:  see
//      further comments in AbsParmGeneral.hh
//
// 	Assumes cout <<  is defined for the type you are using. 
// 	(OK for intrinsics, not so for a general class.)
// 	
// 	
//------------------------------------------------------------------------

#ifndef ABSPARMVECTOR_HH
#define ABSPARMVECTOR_HH

#include "Framework/AbsParm.hh"
#include "Framework/AppConfigRecorder.hh"
#include <string>
#include <vector>
#include <map>


//		---------------------
// 		-- Class Interface --
//		---------------------
 
template <class T>
class AbsParmVector : public AbsParm {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors.
  
  AbsParmVector( const char* const theCommand, AppModule* theTarget );
  //   Initial vector is empty.  Future set()'s append to this.
  
  AbsParmVector( const AbsParmVector& theOther, AppModule* theTarget);
  //   Copy constructor. 
  
  // AbsParmVector( const char* const theCommand, AppModule* theTarget,
  //	 const std::vector<T>& defVector );
  //
  //   Initial vector is given by defVector.  Furture set()'s append to this.
  //   Upon reflection, I have removed this:  I can not see a way to have
  //   an RWTValOrdered vector already cooked up at contruction time of
  //   a module so that it could be passed in here.  You could take it in
  //   through the ctor of the module, which means it would live up in
  //   the appuserbuild.  I think that's too gross.  We can probably solve
  //   default vectors via Tcl and sequences.  I'll leave this comment here
  //   in case someone else has the same stupid bright-idea as I.
  
  
  // Destructor
  ~AbsParmVector( );
  
  // Handler
  virtual int handle( int argc, char* argv[] );
   
  // Register enumeration name and value
  virtual void addItem(const std::string& name, const T& value);
    
  // value accessor
  inline virtual const std::vector<T>&  value() const
  { return _theVector; };
  
  std::string classDescription () const; 
  std::string type ()            const;

  void  print( std::ostream& o) const;
  void dumpState(AppConfigRecorder* r);
  void restoreState(char* [] );
  
protected:
  
  virtual int setHandler    ( );
  virtual int listHandler   ( ) const;
  virtual int getHandler    ( );
  virtual int clearHandler  ( );
  
  
  // The following is overloading in an ugly way.  It would be preferable
  // to do something sensible with streams but it works for now.
  // These routines are only used by set_handler!!!!
  
  inline void set_value(double &);
  inline void set_value(float &);
  
  inline void set_value(long &);
  inline void set_value(int &);
  
  inline void set_value(std::string &);
  inline void set_value(bool &);
  
private:

  // Not implemented.
  AbsParmVector( const AbsParmVector& );
  AbsParmVector& operator=( const AbsParmVector& );
  
  std::vector<T> _theVector;

  // in case objects are identified by symbolic names
  std::vector<std::string>* _theSymbol;
  std::map<std::string, T>* _dict;

};


#include  "AbsParm/AbsParmVector.icc" 
 

#endif
