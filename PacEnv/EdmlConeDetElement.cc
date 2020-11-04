/* An implementation of class EdmlConeDetElement
 */
#include "BaBar/BaBar.hh"

#include "PacEnv/EdmlConeDetElement.hh"

#include <sstream>
#include <math.h>

EdmlConeDetElement::EdmlConeDetElement( const std::string& theType,
  const std::string& theName,
  const std::string& theMaterial,
  const std::string& theDevice,
  const int          theId,
  const float        theThick,
  const float        theGap,
  const float        theOverlap,
  const float        theRmin,
  const float        theRmax,
  const float        theTanTheta,
  const float        theZVert):
EdmlDetElement( theType,
  theName,
  theMaterial,
  theDevice,
  theId,
  theThick,
  theGap,
  theOverlap ),
  _rmin   (theRmin),
  _rmax   (theRmax),
  _tanth  (theTanTheta),
  _zvert  (theZVert)
  { }
  
EdmlConeDetElement::EdmlConeDetElement( const std::string& theType,
  const std::string& theName,
  const std::string& theMaterial,
  const std::string& theDevice,
  const int          theId,
  const float        theThick,
  const float        theGap,
  const float        theOverlap,
  const std::pair<float,float>& cylpt1,
  const std::pair<float,float>& cylpt2) :
EdmlDetElement( theType,
  theName,
  theMaterial,
  theDevice,
  theId,
  theThick,
  theGap,
  theOverlap )
{
  double theRho1 = cylpt1.first;
  double theRho2 = cylpt2.first;
  double theZ1 = cylpt1.second;
  double theZ2 = cylpt2.second;
  double theta = atan2(theRho2-theRho1,theZ2-theZ1);
  _tanth = tan(theta);
  _zvert = theZ1 - theRho1/_tanth;
  double costh = cos(theta);
  double r1 = fabs((theZ1-_zvert)/costh);
  double r2 = fabs((theZ2-_zvert)/costh);
  if(r1>r2){
    _rmin = r2;
    _rmax = r1;
  } else {
    _rmin = r1;
    _rmax = r2;
  }
}  

EdmlConeDetElement::~EdmlConeDetElement( )
  { }

std::string
  EdmlConeDetElement::toString( ) const
{
  std::ostringstream str;
  str << EdmlDetElement::toString( )
    << ", rmin: "    << _rmin
    << ", rmax: "    << _rmax
    << ", tantheta: "   << _tanth
    << ", zvert: "   << _zvert;
  return str.str( );
}
