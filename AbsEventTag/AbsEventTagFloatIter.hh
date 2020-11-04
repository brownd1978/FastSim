//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AbsEventTagFloatIter.hh 488 2010-01-13 16:43:49Z stroili $
//
// Description:
//      Class AbsEventTagFloatIter. Interface definition file for 
//      the float iterator for the abstract event tag.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      David R. Quarrie                Original Author
//
// Copyright Information:
//      Copyright (C) 1998              Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------

#ifndef ABSEVENTTAGFLOATITER_HH
#define ABSEVENTTAGFLOATITER_HH

//-----------------
// BaBar Headers --
//-----------------
#include "BaBar/BaBar.hh"

//              ---------------------
//              -- Class Interface --
//              ---------------------
 
/**
 ** The AbsEventTagFloatIter class. This is an abstract interface for the iterator
 ** that retrives float tag elements from the event tag.
 **
 ** The event tag contains named integer, float and boolean elements. The
 ** name of each element must be unique, independent of the data type.
 **
 ** Copyright (C) 1998              Lawrence Berkeley National Laboratory
 **
 ** @version $Id: AbsEventTagFloatIter.hh 488 2010-01-13 16:43:49Z stroili $ 
 **
 ** @author (David R. Quarrie)		Originator
 **/

class AbsEventTagFloatIter {

//--------------------
// Instance Members --
//--------------------

public:

  // dtor must be declared or it won't be virtual
  virtual ~AbsEventTagFloatIter() {}

    /**
     ** Return the next float element from the current tag. The
     ** function value indicates whether a new element was located
     ** or whether the iterator was exhausted.
     **/
    virtual bool next( ) = 0;

    /**
     ** Return the name of the current float element within the tag.
     **/
    virtual const char* key( ) = 0;

    /**
     ** Return the value of the current float element within the tag.
     **/
    virtual float value( ) = 0;

};

#endif
