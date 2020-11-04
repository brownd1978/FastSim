//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AbsGHit.hh 487 2010-01-13 16:40:34Z stroili $
//
// Description:
//      Class AbsGHit header file
//
//      An AbsGHit is an abstract base class for holding common
//      method definitions for all GHit objects
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Bob Jacobsen                 UC Berkeley
//      Ed Frank                     University of Pennsylvania
//
// History:
//      00 XXX 00   Bob Jacobsen     Create
//      23 Jan 96   Ed Frank         Add default implementation too.  This
//                                   may be a bad idea, or it may mean that
//                                   "Abs" is a bad name.
//      25 Jan 96   Ed Frank         print is no longer pure virtual.
//
//      12 Feb 96   Bob J            add printAll
//
//      28 Feb 96   Bob J            add x,y,z members as superclass elements
//
//      28 Mar 96   Ed Frank         inherit from AbsEvtObj
//
// Copyright Information:
//      Copyright (C) 1995      LBNL
//
//------------------------------------------------------------------------
#ifndef ABSGHIT_HH
#define ABSGHIT_HH

#include "BaBar/BaBar.hh"          // for boolean
#include "AbsEvent/AbsEvtObj.hh"

#include <iosfwd>
#include <assert.h>

//
// Class Definition
//

class AbsGHit : public AbsEvtObj {

private:

public:

  virtual ~AbsGHit();

  // the x,y,z  members are actually defined in lower classes
  // via dbio.
  // They are not pure virtual because the SVT is missing the standard
  // names (so far)
  virtual float x() const { assert(false); return 0; }
  virtual float y() const { assert(false); return 0; }
  virtual float z() const { assert(false); return 0; }

  // There are three different print forms:
  // 'print(cout)', used by and equivalent to <<, occupies exactly one line
  virtual void print(std::ostream& o) const;

  // 'printAll(cout)', is allowed to take multiple lines and should
  // dump entire contents
  virtual void printAll(std::ostream& o) const;

  // This time is to be in uniform units- nanoseconds.
  virtual float time() const=0; 
  virtual void setTime(const float)=0;

  // common method for time-shifting, used to mix GHits
  virtual void shiftTime( float shift );    // shift the hit time by shift.

};

std::ostream&  operator << (std::ostream& o, const AbsGHit& );

#endif
