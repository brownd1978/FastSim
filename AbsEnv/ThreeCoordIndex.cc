//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: ThreeCoordIndex.cc 483 2010-01-13 14:03:08Z stroili $
//
// Description:
// Class ThreeCoordIndex
//
//    Implementation of AbsDetIndex for system in which three co-ordinates 
//    specifies a channel uniquely.
//
//  Direct dependencies:
//      Inherits from AbsDetIndex.
//
//  Implementation dependencies:
//      iostream
//
//  Author List:
//
//         Phil Strother                         Imperial College
//         Stephen J. Gowdy                      University of Edinburgh
// 
//  Bugs, questions, etc. to:    pd.strother@ic.ac.uk, S.Gowdy@ed.ac.uk
// 
// 
// Copyright Information:
//	Copyright (C) 1996	Imperial College
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "AbsEnv/ThreeCoordIndex.hh"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <iostream>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "ErrLogger/ErrLog.hh"
using std::endl;
using std::ostream;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------


//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
ThreeCoordIndex::ThreeCoordIndex(long x, long y, long z, long ind)
{
  itsCoords= new AbsDetCoord(3);
  // This is deleted by AbsDetIndex.
  (*itsCoords)[0]=x;
  (*itsCoords)[1]=y;
  (*itsCoords)[2]=z;
  index=ind;
  theNeighbours=new NeighbourStore;
  // This is deleted by AbsDetIndex.
}

ThreeCoordIndex::ThreeCoordIndex( long theIndex, AbsDetCoord *init )
  : AbsDetIndex( theIndex, init )
{
  if (itsCoords->size()!=3)
    {
      ErrMsg( fatal ) << "Constructor called with initial vector of length " 
		      << init->size() << endmsg;
      ::abort();
    }
}

ThreeCoordIndex::ThreeCoordIndex( const ThreeCoordIndex &copy )
  : AbsDetIndex(copy)
{
}

//--------------
// Destructor --
//--------------
ThreeCoordIndex::~ThreeCoordIndex()
{
}

//-------------
// Methods   --
//-------------

void
ThreeCoordIndex::print(ostream& o) const
{
  o << index << " " << (*itsCoords)[0] << " " << (*itsCoords)[1] 
    << " " << (*itsCoords)[2] << endl;
}

void
ThreeCoordIndex::printAll(ostream& o) const
{
  print(o);
}

//-------------
// Operators --
//-------------

//-------------
// Selectors --
//-------------
unsigned 
ThreeCoordIndex::hash() const
{
  return AbsDetIndex::hash();
}

//-------------
// Modifiers --
//-------------

//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------

unsigned
ThreeCoordIndex::hash(const ThreeCoordIndex &theIndex)
{
  return ((AbsDetIndex &)theIndex).hash();
}

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

//		-----------------------------------------
// 		-- Private Function Member Definitions --
//		-----------------------------------------

//		-----------------------------------
// 		-- Internal Function Definitions --
//		-----------------------------------




