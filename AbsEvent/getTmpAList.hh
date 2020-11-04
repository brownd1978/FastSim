#ifndef GETTMPALIST_HH
#define GETTMPALIST_HH

//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: getTmpAList.hh 487 2010-01-13 16:40:34Z stroili $
//
// Description:
//	A function template for getting a temporary HepAList from 
//      the event.  "Temporary" means a list that will persist throughout
//      the lifetime of processing the current event, but will not
//      not be written out to disk (i.e., it will not be put in the DB).
//
//      If a list of the kind you request is already present in the event,
//      then the list from the event will be returned to you.  Otherwise,
//      a list of the kind you rqqested will be created and added to
//      the event.  Note that several lists of the same kind may be
//      put into the event if they are distinguished via a secondary
//      key  (see the second form of getTmpAList(), below).
//
//      In all cases, the list returned to you belongs to the event.
//
//      A typical usage eample would be when a number of modules wish
//      to communicate with each other via a list in the event and where
//      any one of them might add data to the list.  Each one of them
//      would obtain the list via a call to getTmpAList(), and whichever
//      one happens to call first will actually cause the list to be
//      created.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Ed Frank		University of Pennsylvania
//
// Copyright Information:
//	Copyright (C) 1997
//
//------------------------------------------------------------------------


//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class AbsEvent;
class IfdKey;
template <class T> class HepAList;

template <class T>
void
getTmpAList( AbsEvent* ev, HepAList<T>* &list );

template <class T>
void
getTmpAList( AbsEvent* ev, HepAList<T>* &list, const IfdKey& k );

#ifdef    BABAR_COMP_INST
#include  "AbsEvent/getTmpAList.icc"
#endif // BABAR_COMP_INST

#endif  // GETTMPALIST_HH
