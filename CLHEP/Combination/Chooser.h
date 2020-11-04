// -*- C++ -*-
// CLASSDOC OFF
// $Id: Chooser.h 478 2010-01-22 08:54:39Z stroili $
// CLASSDOC ON
//
// Chooser.h - given lists of objects, successively generate a list of
//             objects which form a unique combination.
//
// Copyright (C)  1993  The Board of Trustees of The Leland Stanford
//                      Junior University.  All Rights Reserved.
//
// Author: Paul Rensing (rensing@slac.stanford.edu)
//
// Usage:
//  HepChooser is a combinatoric engine which uses locks (see Lockable.h). A
//  HepChooser can take between 1 and 6 lists of objects. Then, the next()
//  method or () operator will return the next set of objects, or 0 if done.
//  The set is returned as a HepAList.
//
//  On each successive call to next(), HepChooser tries to find a new unique
//  combination of objects. It uses locks to make certain that the same
//  (fundamental) object is not used more than once anywhere in the list. For
//  example, if A, B, C, and D are fundamental "track" objects, and E = A +
//  B, then E and B or A cannot be in the same combination.
//  
//  HepChooser respects locks, and therefore, any objects locked by a HepLock
//  (which is turned on!) will not be used in a combination. 
//
//  If a HepChooser is given the same list more than once, it will take the
//  appropriate action so that the same combination (in a different) order
//  does not occur more than once. The method orderMatters() will turn off
//  this feature.
//
// Bugs and Gotcha's:
//  If you turn on a Lock in the middle of a HepChooser loop, you may not get
//  the results you expect. For example, suppose you have two tracks, both of
//  which id as both pion and kaon. If you ask for all combinations of a pi
//  and kaon, you expect to get 2 combinations. However, if you do the
//  following: 
//       HepAList<Track> pions, kaons;
//       HepChooser<Track> piKs(&pions, &kaons);
//       HepAList<Track> *p;
//       HepLock piKLock;
//
//       piKLock.lock();
//       while ( p = piKs() ) {
//           piKLock.add((*p)[0]);
//           piKLock.add((*p)[1]);
//       }
//  you will only get 1 combination, because both tracks will be locked as
//  soon as they are added to piKLock.
//
//  HepChooser does NOT correctly handle the case where an object is in more
//  than one (different) list. It will not use it twice in one combination,
//  but it will return the same combination more than once (with the order
//  permuted). For example, suppose there are three pions, two are
//  "goldplated" pions (pi1, pi2) and the other is an OK pion (pi3). Then,
//       HepAList<Track> pions;         // = {pi1, pi2, pi3}
//       HepAList<Track> goldPions;     // = {pi1, pi2}
//       HepChooser<Track> twoPis(&pions, &goldPions);
//       HepAList<Track> *p;
//
//       while ( p = twoPis() ) {
//           ..
//       }
//  will produce the following combinations:
//    (pi1,pi2), (pi2,pi1), (pi3,pi1), (pi3,pi2)
//  so you get pi1+pi2 twice.
//
// See Also:
//  Lockable.h, Lock.h, Combiner.h

#ifndef _CHOOSER_H_
#define _CHOOSER_H_

#include "CLHEP/config/CLHEP.h"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"

#ifdef HEP_NO_INLINE_IN_TEMPLATE_DECLARATION
#define inline
#endif

template <class TYPE>
class HepChooser {

public:
     
  inline HepChooser(HepAList<TYPE> *l1, HepAList<TYPE> *l2=0, 
	     HepAList<TYPE> *l3=0, HepAList<TYPE> *l4=0, 
	     HepAList<TYPE> *l5=0, HepAList<TYPE> *l6=0 );
  // Constructor taking up to six pointers to lists as argument

  inline HepChooser(HepAList< HepAList<TYPE> > &ll);
   // constructor from a list of lists.
 
  inline ~HepChooser();
  // The destructor
     
  inline HepAList<TYPE>* next();
  inline HepAList<TYPE>* operator () ();
  // Returns a pointer to a list with the next combination of objects or
  // zero if all combinations are done.
	  
  inline void orderMatters();

protected:

  inline void init(HepAList< HepAList<TYPE> > &ll);
   
  inline void nextComb();

  inline int incrIndex(int i);

  // Data members:
  int nlists;
  int *startList;
  HepAListIterator<TYPE> **iter;
  int done;
  HepAList<TYPE> currList;
};

#ifdef HEP_NO_INLINE_IN_TEMPLATE_DECLARATION
#undef inline
#endif

#ifdef HEP_SHORT_NAMES
#define Chooser HepChooser
#endif

#include "CLHEP/Combination/Chooser.icc"

#endif
