//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppHash.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppHash. Hash Table class for BaBar Application Framework.
//	Keys are character strings and are case-sensitive.
//
// NOTE: This is an extremely fragile template class.  It must have NO
//       virutal members to avoid problems with the AIX linker when using 
//       the g++ 2.7.2 compiler. It should be recoded using Tools.h++ ASAP.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------

#ifndef APPHASH_HH
#define APPHASH_HH

//-----------
// Headers --
//-----------
extern "C" {
#include "tcl.h"
}

//		---------------------
// 		-- Class Interface --
//		---------------------
 
template<class Item> 
class AppHash {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AppHash( );
  
  // Destructor
  ~AppHash( );
  
  // Selectors (const)
  
  Item* fetch( const char* const aKey ) const;
  bool  has  ( const char* const aKey ) const;
  
  // Modifiers
  
  bool add   ( const char* const aKey, const Item* const anItem );
  void force ( const char* const aKey, const Item* const anItem );
  void remove( const char* const aKey );
  
private:

  // Not implemented.
  AppHash( const AppHash& );
  AppHash& operator=( const AppHash& );
  
  // Data members
  Tcl_HashTable* _tclHashTable;
  
};

// Template implementations
#include "Framework/AppHash.icc"
 
#endif
