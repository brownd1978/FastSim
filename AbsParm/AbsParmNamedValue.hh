#ifndef ABSPARM_NAMED_VALUE_HH
#define ABSPARM_NAMED_VALUE_HH
//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsParmNamedValue.hh 489 2010-01-13 16:45:39Z stroili $
//
// Description:
//	Class AbsParmNamedValue<T>
//
//      Templated command handler for setting module parameters with
//	symbolic names in TCL
//
//	parm set <name>		Set parm to object corresponding to name
//      parm list               Display current parm value
//
// AbsParmNamedValue provides a templated interface to simple objects that
// identifed by a symbolic (string) name.  After creating one of these, the
// module must register each valid object by value with an associated string
// using the function
//
//		addItem("name", value);
//
// The first name-value pair registered is the default setting.
//
// This class stores the value registered by copying it, and does not take
// ownership of the original object.  The using Framework module must delete
// them appropriately.
//
// For complex objects the AbsParmNamedObject class with a pointer interface
// should be used instead.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Michael Kelsey	<kelsey@slac.stanford.edu>
//
// Copyright Information:
//	Copyright (C) 1999	Princeton University
//
//------------------------------------------------------------------------

#include "Framework/AbsParm.hh"
#include "Framework/AppConfigRecorder.hh"

#include <string>
#include <map>

// Forward declarations

// Class Interface

template <class T>
class AbsParmNamedValue : public AbsParm {
public:
  // Constructors and destructor
  AbsParmNamedValue(const char* const theCommand, AppModule* theTarget);
  AbsParmNamedValue(const AbsParmNamedValue& theOther, AppModule* theTarget);

  virtual ~AbsParmNamedValue();

  // Register enumeration name and value
  virtual void addItem(const std::string& name, const T& value);

  // value and symbolic name accessors
  virtual const T& value() const { return _x; }
  virtual const std::string& symbol() const { return _xName; }

  // value (symbolic name) display
  virtual void print(std::ostream& o) const;

  virtual std::string classDescription() const {
    return "symbolic parameter: set or list.";
  }
  virtual std::string type() const { return "Symbol"; }

  void dumpState(AppConfigRecorder* r) { 
    r->store( const_cast<std::string&>( symbol() ), "set"); 
  }
  void restoreState(char* []);

  // set the value
  virtual void set(const T& v) ;

protected:
  virtual int setHandler();
  virtual int listHandler() const;
  virtual int getHandler();

private:

  // Not implemented.
  AbsParmNamedValue( const AbsParmNamedValue& );
  AbsParmNamedValue& operator=( const AbsParmNamedValue& );

  T _x;				// Cached value for quick return
  std::string _xName;

  std::map<std::string, T>* _dict;
};

// Output operator

template <class T>
std::ostream& operator<<(std::ostream& o, const AbsParmNamedValue<T>& anAbsPS);

#include "AbsParm/AbsParmNamedValue.icc"

#endif /* ABSPARM_NAMED_VALUE_HH */
