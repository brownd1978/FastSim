#ifndef ABSPARM_NAMED_OBJECT_HH
#define ABSPARM_NAMED_OBJECT_HH
//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsParmNamedObject.hh 489 2010-01-13 16:45:39Z stroili $
//
// Description:
//	Class AbsParmNamedObject<T>
//
//      Templated command handler for setting module parameters with
//	symbolic names in TCL
//
//	parm set <name>		Set parm to object corresponding to name
//      parm list               Display current parm value
//
// AbsParmNamedObject provides a templated interface to arbitrary objects
// that can be set and examined via some interface (the Framework command
// line, etc.).   After creating one of these, the module must register
// each valid object by pointer with an associated string using the function
//
//		addItem("name", pointer);
//
// The first name-value pair registered is the default setting.
//
// This class does NOT take ownership of the registered objects; the
// Framework module must delete them in its dtor.
//
// For simple "objects" (enums, special floats, etc.), the AbsParmNamedValue
// class should be used instead.
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

#include "AbsParm/AbsParmNamedValue.hh"


template <class T>
class AbsParmNamedObject : public AbsParmNamedValue<T*> {
public:
  // Constructors and destructor
  AbsParmNamedObject(const char* const theCommand, AppModule* theTarget);
  AbsParmNamedObject(const AbsParmNamedObject& theOther, AppModule* theTarget);

  virtual ~AbsParmNamedObject();

  // NOTE:  All of the interface functions are inherited

private:

  // Not implemented.
  AbsParmNamedObject( const AbsParmNamedObject& );
  AbsParmNamedObject& operator=( const AbsParmNamedObject& );

};

#include "AbsParm/AbsParmNamedObject.icc"

#endif /* ABSPARM_NAMED_OBJECT_HH */
