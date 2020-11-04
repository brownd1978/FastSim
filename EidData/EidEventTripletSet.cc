//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: EidEventTripletSet.cc 451 2010-01-14 13:03:45Z stroili $
//
// Description:
//	EidEventTripletSet implementation
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Gregory Dubois-Felsmann  (originator)
//
// Copyright Information:
//	Copyright (C) 2002   California Institute of Technology
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "EidData/EidEventTripletSet.hh"

//-----------------
// C/C++ Headers --
//-----------------
#include <utility>
#include <vector>
#include <algorithm>

// For commented-out debug output:
// #include <iostream>
// using std::cout;
// using std::endl;

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

//--------------
// Destructor --
//--------------
EidEventTripletSet::~EidEventTripletSet( )
{ }

//-------------
// Modifiers --
//-------------


void
EidEventTripletSet::add( const EidEventTriplet& t )
{
  _tripletSet.insert( t );
  // cout << "Inserted " << t << ", entries: " << _tripletSet.size() << endl;
}


void
EidEventTripletSet::clear()
{
  _tripletSet.clear();
}


bool
EidEventTripletSet::find( const EidEventTriplet& t, bool remove )
{
  // cout << "Entries: " << _tripletSet.size() 
  //      << ", count: " << _tripletSet.count( t ) << endl;

  bool found = false;

  // Find any triplets in the set that match the one supplied.
  std::pair<EetSet::iterator,EetSet::iterator> 
    er( _tripletSet.equal_range( t ) );

  // Anything?
  if ( er.first != er.second ) {
    // Since we ordered the set by WildCardOrdering, which respects
    // only the time stamp part of the triplets, the resulting range
    // can include triplets with non-matching EidPartitionDoublets.
    // So we verify that we really have a match:
    if ( remove ) {
      EetSet::iterator i = er.first;
      while ( ( i = std::find( i, er.second, t ) ) != er.second ) {
	// OK, got a match.
	found = true;
	// Get rid of it.
	// cout << "Removing " << *i << endl;
	// NB: _tripletSet.remove( i++ ) may not be safe -- I am not sure 
	// that it is guaranteed that the postincrement will be completed
	// before the function call.  So force the ordering by hand:
	EetSet::iterator ri(i);
	++i;
	_tripletSet.erase( ri );
      }
    }
    else {
      found = ( std::find( er.first, er.second, t ) != er.second );
    }
  }

  return found;
}


//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

//		-----------------------------------------
// 		-- Private Function Member Definitions --
//		-----------------------------------------

//		-----------------------------------
// 		-- Internal Function Definitions --
//		-----------------------------------
