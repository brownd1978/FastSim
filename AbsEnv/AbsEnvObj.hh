//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AbsEnvObj.hh 483 2010-01-13 14:03:08Z stroili $
//
// Description:
//      Class AbsEnvObj header file
//
//      An AbsEnvObj is an abstract base class for holding common
//      method definitions for all objects in the AbsEnv environment.
//
//      The most important of these virtual methods is the destructor,
//      used to break dependencies on teh various class definitions
//
//      Three forms of print are also declared.  print() and the stream <<
//      operator are to produce an output string (no endl); printAll() should
//      output the entire contents of the object on one or more lines
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Bob Jacobsen
//
// Copyright Information:
//      Copyright (C) 1995      LBNL
//
//------------------------------------------------------------------------
#ifndef ABSENVOBJ_HH
#define ABSENVOBJ_HH

#include <assert.h>

#include <iosfwd>

//
// Class Definition
//

class AbsEnvObj
{

public:

  virtual ~AbsEnvObj();

  // There are three different print forms:
  // 'print(cout)', used by and equivalent to <<, occupies exactly one line
  virtual void print(std::ostream& o) const;

  // 'printAll(cout)', is allowed to take multiple lines and should
  // dump entire contents
  virtual void printAll(std::ostream& o) const;

protected:
  // default constructor is protected to make it harder to 
  // instantiate by accident
  AbsEnvObj();

};

// The third print form is the stand alone:
std::ostream&  operator << (std::ostream& o, const AbsEnvObj&);

#endif // ABSENVOBJ_HH
