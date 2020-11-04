//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AbsDetStructure.cc 483 2010-01-13 14:03:08Z stroili $
//
// Description:
//      Class AbsDetStructure header file
//
//      Abstract class to define the structure of a non-determined 
//      detector system in terms of AbsDetIndex classes which hold 
//      co-ordinate and index information. Each detector system will 
//      define its own structure (there may be more than one if 
//      there is more than one representation of the detector). 
//      Collections of AbsDetIndeces are implemented as std::vector.

//      A particular system's Structure, implemented as derived classes
//      from this one, will be posessed (via an inherited data member)
//      by that system's derived version of AbsDetEnv.  
//      See EmcEnv/EmcEnv.cc/hh for an example.
//    
// Direct dependencies:
//      Inherits from AbsEnvObj.
//
// Implementation dependencies:
//      General:
//            AbsDetIndex
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//  Author List:
//
//         Phil Strother                         Imperial College
//         Stephen J. Gowdy                      University of Edinburgh
// 
//  Bugs, questions, etc. to:    pd.strother@ic.ac.uk, S.Gowdy@ed.ac.uk
//
// Copyright Information:
//	Copyright (C) 1996	Imperial College
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "AbsEnv/AbsDetStructure.hh"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <stdlib.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEnv/AbsDetIndexFactory.hh"
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

AbsDetStructure::AbsDetStructure()
  : cardinality( 0 ),
    theFactory( 0 )
{
}

AbsDetStructure::AbsDetStructure(long index, AbsDetCoord *coords)
  : AbsDetIndex(index,coords),
    cardinality( 0 ),
    itsChannels( 0 ),
    theFactory( 0 )
{
}

//AbsDetStructure::AbsDetStructure(const AbsDetStructure &copy)
//  : AbsDetIndex(copy)
//    cardinality( copy.cardinality ),
//    theFactory( copy.theFactory )
//{
//}

//--------------
// Destructor --
//--------------

AbsDetStructure::~AbsDetStructure()
{
}


//-------------
// Operators --
//-------------
bool 
AbsDetStructure::operator==( const AbsDetStructure &compare) const
{
  bool answer = AbsDetIndex::operator==(compare);
  return answer;
}

bool
AbsDetStructure::operator!=( const AbsDetStructure &compare ) const
{
  bool answer = AbsDetIndex::operator!=(compare);
  return answer;
}

    
//-------------
// Selectors --
//-------------

unsigned
AbsDetStructure::hash() const
{
  return index%101;
}

void
AbsDetStructure::print(ostream& o) const
{
  o << index << endl;
}

  // 'printAll(cout)', is allowed to take multiple lines and should
  // dump entire contents
void
AbsDetStructure::printAll(ostream& o) const
{
  o << index << endl;
}


//-------------
// Modifiers --  
//-------------

void
AbsDetStructure::makeDirectNeighbours()
{
  ErrMsg( fatal ) << "Call to AbsDetStructure's makeDirectNeighbours() "
		  << endl
		  << " Member function not redefined on your derived class. "
		  << endmsg;
  ::abort();
}

void
AbsDetStructure::makeDirectNeighbours(const AbsDetStructure *const)
{
  ErrMsg( fatal ) << "Call to AbsDetStructure's makeDirectNeighbours(const AbsDetStructure *const) " 
		  << endl
		  << " Member function not redefined on your derived class. "
		  << endmsg;
  ::abort();
}




