/* An implementation of class EdmlCylDetElement
 */
#include "BaBar/BaBar.hh"

#include "PacEnv/EdmlCylDetElement.hh"

#include <sstream>

EdmlCylDetElement::EdmlCylDetElement( const std::string& theType,
  const std::string& theName,
  const std::string& theMaterial,
  const std::string& theDevice,
  const int          theId,
  const float        theThick,
  const float        theGap,
  const float        theOverlap,
  const float        theZmin,
  const float        theZmax,
  const float        theRadius) :
EdmlDetElement( theType,
  theName,
  theMaterial,
  theDevice,
  theId,
  theThick,
  theGap,
  theOverlap ),
  _zmin   (theZmin),
  _zmax   (theZmax),
  _radius (theRadius)
  { }

EdmlCylDetElement::~EdmlCylDetElement( )
  { }

std::string
  EdmlCylDetElement::toString( ) const
{
  std::ostringstream str;
  str << EdmlDetElement::toString( )
    << ", zmin: "    << _zmin
    << ", zmax: "    << _zmax
    << ", radius: "  << _radius;
  return str.str( );
}
