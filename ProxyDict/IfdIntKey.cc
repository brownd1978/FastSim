//--------------------------------------------------------------------------
// File and Version Information:
//    $Id: IfdIntKey.cc 520 2010-01-15 11:40:48Z stroili $
//
// Description:
//    Implementation of IfdIntKey.  See .hh for details.
//
// Author List:
//    Ed Frank                        University of Pennsylvania
//
// History:
//    Ed Frank      19 Jan 00         Creation of first version by moving
//                                    old code from .hh to here.
//
// Copyright Information:
//      Copyright (C) 1997
//
// Bugs:
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
#include "ProxyDict/IfdIntKey.hh"


IfdIntKey::IfdIntKey( const int i ) 
  : IfdKey( intKey )
{
   intVal   =  i; 
   _hashVal = i % _nHashBuckets;
}

