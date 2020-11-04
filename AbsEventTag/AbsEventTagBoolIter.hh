//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AbsEventTagBoolIter.hh 488 2010-01-13 16:43:49Z stroili $
//
// Description:
//      Class AbsEventTagBoolIter. Interface definition file for 
//      the boolean iterator for the abstract event tag.
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

#ifndef ABSEVENTTAGBOOLITER_HH
#define ABSEVENTTAGBOOLITER_HH

//-----------------
// BaBar Headers --
//-----------------
#include "BaBar/BaBar.hh"

//              ---------------------
//              -- Class Interface --
//              ---------------------
 
/**
 ** The AbsEventTagIntIter class. This is an abstract interface for the iterator
 ** that retrives boolean tag elements from the event tag.
 **
 ** The event tag contains named integer, float and boolean elements. The
 ** name of each element must be unique, independent of the data type.
 **
 ** Copyright (C) 1998              Lawrence Berkeley National Laboratory
 **
 ** @version $Id: AbsEventTagBoolIter.hh 488 2010-01-13 16:43:49Z stroili $ 
 **
 ** @author (David R. Quarrie)		Originator
 **/

class AbsEventTagBoolIter {

//--------------------
// Instance Members --
//--------------------

public:

  // dtor must be declared or it won't be virtual
  virtual ~AbsEventTagBoolIter() {}

    /**
     ** Return the next boolean element from the current tag. The
     ** function value indicates whether a new element was located
     ** or whether the iterator was exhausted.
     **/
    virtual bool next( ) = 0;

    /**
     ** Return the name of the current boolean element within the tag.
     **/
    virtual const char* key( ) = 0;

    /**
     ** Return the value of the current boolean element within the tag.
     **/
    virtual bool value( ) = 0;

};

#endif
