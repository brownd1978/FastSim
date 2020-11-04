//--------------------------------------------------------------------------
// File and Version Information:
//
// Description:
//      Class AbsDigi header file
//
//      An AbsDigi is an abstract base class for holding common
//      method definitions for all Digi objects
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Bob Jacobsen                 UC Berkeley
//
// Copyright Information:
//      Copyright (C) 1998      LBNL
//
//------------------------------------------------------------------------
#ifndef ABSDIGI_HH
#define ABSDIGI_HH

#include "BaBar/BaBar.hh"          // for boolean
#include "AbsEvent/AbsEvtObj.hh"

#include <iosfwd>
#include <assert.h>

//
// Class Definition
//

class AbsDigi : public AbsEvtObj {

private:

public:

  virtual ~AbsDigi();

  // There are three different print forms:
  // 'print(cout)', used by and equivalent to <<, occupies exactly one line
  virtual void print(std::ostream& o) const;

  // 'printAll(cout)', is allowed to take multiple lines and should
  // dump entire contents
  virtual void printAll(std::ostream& o) const;

};

std::ostream&  operator << (std::ostream& o, const AbsDigi& );

#endif
