// -*- C++ -*- 
// CLASSDOC OFF
// $Id: Combiner.h 478 2010-01-22 08:54:39Z stroili $
// CLASSDOC ON
//
// Combiner.h - handle combinatorics and combine the result into one object
//
// Copyright (C)  1993  The Board of Trustees of The Leland Stanford
//                      Junior University.  All Rights Reserved.
//
// Author: Paul Rensing (rensing@slac.stanford.edu)
//
// Usage:
//  HepCombiner is essentially the same as HepChooser, except the results of
//  next() or operator() is one object. The += operator are used to combine
//  a the objects returned from HepChooser into one. Also, the child list of
//  the combined object is built using the method addChild().
// 
// Bugs and Gotcha's:
//  The object returned (by pointer) by the method next() is maintained by
//  HepCombiner. Therefore, if you want to keep it around for later use, you
//  must remember to make a copy of it.
// 
// See also: ** Chooser.h **

#ifndef _Combiner_H_
#define _Combiner_H_

#include "CLHEP/config/CLHEP.h"
#include "CLHEP/Combination/Chooser.h"

#ifdef HEP_NO_INLINE_IN_TEMPLATE_DECLARATION
#define inline
#endif

template <class TYPE>
class HepCombiner : public HepChooser<TYPE> {

public:

   inline HepCombiner(HepAList<TYPE> *l1,   HepAList<TYPE> *l2=0, 
		      HepAList<TYPE> *l3=0, HepAList<TYPE> *l4=0, 
		      HepAList<TYPE> *l5=0, HepAList<TYPE> *l6=0);
   // constructor from 1->6 pointers to lists. 
 
   inline HepCombiner(HepAList< HepAList<TYPE> > &ll);
   // constructor from list of lists.

   inline ~HepCombiner();
   
   inline TYPE *next();
   inline TYPE * operator() ();
   
protected:
   
   inline TYPE *combine(const HepAList<TYPE> *l);
   
   TYPE *currObj;
};

#ifdef HEP_NO_INLINE_IN_TEMPLATE_DECLARATION
#undef inline
#endif

#ifdef HEP_SHORT_NAMES
#define Combiner HepCombiner
#endif

#include "CLHEP/Combination/Combiner.icc"

#endif
