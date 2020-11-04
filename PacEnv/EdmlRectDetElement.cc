/* An implementation of class EdmlRectDetElement
 */
#include "BaBar/BaBar.hh"

#include "PacEnv/EdmlRectDetElement.hh"

#include <sstream>

EdmlRectDetElement::EdmlRectDetElement( const std::string& theType,
  const std::string& theName,
  const std::string& theMaterial,
  const std::string& theDevice,
  const int          theId,
  const float        theThick,
  const float        theGap,
  const float        theOverlap,
  const porient      theOrientation,
  const float        theRho,
  const float        theZ,
  const float        thePhi,
  const float        theEta,
  const float        theUSize,
  const float        theVSize) :
EdmlDetElement(theType,
  theName,
  theMaterial,
  theDevice,
  theId,
  theThick,
  theGap,
  theOverlap ),
  _orient(theOrientation),
  _rho(theRho),
  _z(theZ),
  _phi(thePhi),
  _eta(theEta),
  _usize(theUSize),
  _vsize(theVSize)
  { }

EdmlRectDetElement::~EdmlRectDetElement( )
  { }

std::string
  EdmlRectDetElement::toString( ) const
{
  std::ostringstream str;
  str << EdmlDetElement::toString( );
    if(_orient == zparallel)
      str << " normal parallel to Z ";
    else
      str << " normal perp to Z ";
    str << ", transverse radius to center: "  << _rho 
    << ", center z: "  << _z
    << ", center azimuth: "         << _phi
    << ", axis phi : " << _eta
    << ", U size: "  << _usize
    << ", V size: "  << _vsize;
  return str.str( );
}
