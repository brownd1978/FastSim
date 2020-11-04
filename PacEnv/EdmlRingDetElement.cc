/* An implementation of class EdmlRingDetElement
 */
#include "BaBar/BaBar.hh"

#include "PacEnv/EdmlRingDetElement.hh"

#include <sstream>

EdmlRingDetElement::EdmlRingDetElement( const std::string& theType,
  const std::string& theName,
  const std::string& theMaterial,
  const std::string& theDevice,
  const int          theId,
  const float        theThick,
  const float        theGap,
  const float        theOverlap,
  const float        theZ,
  const float        theLowRadius,
  const float        theHiRadius) :
EdmlDetElement(theType,
  theName,
  theMaterial,
  theDevice,
  theId,
  theThick,
  theGap,
  theOverlap ),
  _z        (theZ),
  _lowradius(theLowRadius),
  _hiradius (theHiRadius)
  { }

EdmlRingDetElement::~EdmlRingDetElement( )
  { }

std::string
  EdmlRingDetElement::toString( ) const
{
  std::ostringstream str;
  str << EdmlDetElement::toString( )
    << ", z: "         << _z
    << ", lowradius: " << _lowradius
    << ", hiradius: "  << _hiradius;
  return str.str( );
}
