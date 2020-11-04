//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: TwoCoordIndex.cc 483 2010-01-13 14:03:08Z stroili $
//
// Description:
// Class TwoCoordIndex
//
//    Implementation of AbsDetIndex for system in which two co-ordinates 
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
//======================================================================
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "AbsEnv/TwoCoordIndex.hh"
#include "AbsEnv/AbsEnvTypes.hh"

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


//              ----------------------------------------
//              -- Public Function Member Definitions --
//              ----------------------------------------

//----------------
// Constructors --
//----------------
TwoCoordIndex::TwoCoordIndex( long x, long y, long ind )
{
  itsCoords = new AbsDetCoord(2);
  // This is deleted by AbsDetIndex.
  (*itsCoords)[0]=x;
  (*itsCoords)[1]=y;
  index=ind;
  theNeighbours=new NeighbourStore;
  // This is deleted by AbsDetIndex.
}

TwoCoordIndex::TwoCoordIndex(long theIndex, AbsDetCoord *init)
  : AbsDetIndex( theIndex, init )
{
  if (itsCoords->size()!=2)
    {
      ErrMsg( fatal ) << "Constructor called with initial vector of length " 
		      << init->size() << endmsg;
      ::abort();
    }
}

TwoCoordIndex::TwoCoordIndex( const TwoCoordIndex &copy )
  : AbsDetIndex( copy )
{
}  

//--------------
// Destructor --
//--------------
TwoCoordIndex::~TwoCoordIndex()
{
}

//-------------
// Methods   --
//-------------
void
TwoCoordIndex::print(ostream& o) const
{
  o << index << " " << (*itsCoords)[0] << " " << (*itsCoords)[1] << endl;
}

void
TwoCoordIndex::printAll(ostream& o) const
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
TwoCoordIndex::hash() const
{
  return AbsDetIndex::hash();
}
    
//-------------
// Modifiers --
//-------------

//              -----------------------------------------------
//              -- Static Data & Function Member Definitions --
//              -----------------------------------------------

unsigned 
TwoCoordIndex::hashFun(const TwoCoordIndex &theIndex)
{
  return theIndex.hash();
}

//              -------------------------------------------
//              -- Protected Function Member Definitions --
//              -------------------------------------------

//              -----------------------------------------
//              -- Private Function Member Definitions --
//              -----------------------------------------

//              -----------------------------------
//              -- Internal Function Definitions --
//              -----------------------------------
