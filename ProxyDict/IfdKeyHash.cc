//***************************************************************************
//*  For use by things like roguewave where a function taking an X
//*  returning an hash value for X is needed rather than X.hash() or
//*  unsigned int (X::*)(const X&)
//***************************************************************************
#include "BaBar/BaBar.hh"
#include "ProxyDict/IfdKeyHash.hh"
#include "ProxyDict/IfdKey.hh"

//****************************************************************************
unsigned int
ifdKeyHash( const IfdKey& k ) {
//****************************************************************************

  return k._hashVal;
}
