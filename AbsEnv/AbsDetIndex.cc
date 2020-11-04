//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AbsDetIndex.cc 659 2010-09-03 07:39:54Z stroili $
//
// Description:
//      Class AbsDetIndex.  Abstract class to hold co-ordinate and
//      index information. A given concrete class derived from
//      AbsDetIndex may need to redefined one or all of the member
//      functions
//
//           virtual int compareTo(const AbsDetIndex *) const;
//           virtual bool isEqual(const AbsDetIndex *) const;
//           virtual unsigned hash() const {return 0;}
//
//      For details, see below.  N.B. If the derived class is to be
//      stored in a hash table, or equivalent then the hash() function
//      MUST be redefined.  The actual co-ordinates are held as long
//      ints.
//
//  Direct dependencies:
//      Inherits from AbsEnvObj.
//
//  Implementation dependencies:
//
//  Author List:
//
//         Phil Strother                         Imperial College
//         Stephen J. Gowdy                      University of Edinburgh
// 
//  Bugs, questions, etc. to:    pd.strother@ic.ac.uk, S.Gowdy@ed.ac.uk
// 
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//----------------------
// This class' header --
//----------------------
#include "AbsEnv/AbsDetIndex.hh"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------

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
AbsDetIndex::AbsDetIndex()
  : index(0),
    itsCoords( 0 ),
    previousIndex(0),
    nextIndex(0),
    theNeighbours( 0 )
{
}

AbsDetIndex::AbsDetIndex( long theIndex, AbsDetCoord *initial )
  : index( theIndex ),
    itsCoords( new AbsDetCoord( *initial ) ),
    previousIndex( 0 ),
    nextIndex( 0 ),
    theNeighbours( new NeighbourStore )
{
  if (initial->size()>4)
    {
      ErrMsg( warning ) << "Creating AbsDetIndex with " 
	   << initial->size() << " co-ordinates." << endmsg;
    }
}

AbsDetIndex::AbsDetIndex( const AbsDetIndex &copy )
  : index( copy.index ),
    itsCoords( new AbsDetCoord( *copy.itsCoords ) ),
    previousIndex( copy.previousIndex ),
    nextIndex( copy.nextIndex ),
    theNeighbours( new NeighbourStore( *copy.theNeighbours ) )
{
}

//--------------
// Destructor --
//--------------
AbsDetIndex::~AbsDetIndex()
{
  // The contents are held by value, so we don't have to worry about them
  delete itsCoords;
  itsCoords=0;

  // The contents are the other AbsDetIndices which will be deleted elsewhere
  delete theNeighbours;
  theNeighbours=0;
}

//-------------
// Operators --
//-------------
AbsDetIndex&
AbsDetIndex::operator=(const AbsDetIndex &assign)
{
  if ( &assign == this ) return *this;

  delete itsCoords;
  itsCoords = new AbsDetCoord( *assign.itsCoords );

  index=assign.index;

  delete theNeighbours;
  theNeighbours = new NeighbourStore( *assign.theNeighbours );

  nextIndex = assign.nextIndex;
  previousIndex = assign.previousIndex;

  return *this;
}

bool
AbsDetIndex::operator==( const AbsDetIndex& compare) const
{
  bool answer=(index==compare.index);
  return answer;
}

bool
AbsDetIndex::operator!=( const AbsDetIndex&compare ) const
{
  bool answer=(index!=compare.index);
  return answer;
}
    
bool
AbsDetIndex::operator<( const AbsDetIndex& compare ) const
{
  bool answer = (index < compare.index);
  return answer;
}

//-------------
// Selectors --
//-------------

//-------------
// Methods   --
//-------------

const NeighbourStore*
AbsDetIndex::itsNeighbours() const
{
  return theNeighbours;
}

bool AbsDetIndex::isNeighbour(const AbsDetIndex* c) const
{
  if ( itsNeighbours()->find( const_cast<AbsDetIndex*> (c) ) != itsNeighbours()->end() ) return true;
  return false;

}

// These functions assume that the index member variable is unique to
// each instance.  If this is not the case with your implementation of
// AbsDetIndex, then override it.

int
AbsDetIndex::compareTo( const AbsDetIndex *compare ) const
{
  if (index==compare->itsIndex()) return 0;
  return index > compare->itsIndex() ? 1 : -1;
}

bool
AbsDetIndex::isEqual( const AbsDetIndex *compare ) const
{
  return compare->itsIndex()==index;
}

unsigned
AbsDetIndex::hash() const
{
  return index%1009;
}

void
AbsDetIndex::print( ostream& o ) const
{
  o << "Printing AbsDetIndex.   Error!" << endl;
}

void
AbsDetIndex::printAll( ostream& o ) const
{
  o << "Printing AbsDetIndex.   Error!" << endl;
}

//-------------
// Modifiers --
//-------------

void
AbsDetIndex::setPreviousIndex( AbsDetIndex *indx )
{
  previousIndex=indx;
}

void
AbsDetIndex::setNextIndex( AbsDetIndex *indx )
{
  nextIndex=indx;
}

void
AbsDetIndex::addToNeighbourList( const AbsDetIndex *addition )
{
  theNeighbours->insert( (AbsDetIndex*)addition );
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


