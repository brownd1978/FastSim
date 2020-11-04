//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BbrAngle.cc 635 2010-04-29 13:49:13Z stroili $
//
// Description:
//	Class BbrAngle
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Luca Lista	          	Original Author
//      Bob Jacobsen                    Added some implementation
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
//-----------------------
// This Class's Header --
//-----------------------
#include "BbrGeom/BbrAngle.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include <stdio.h>
#include "BaBar/Constants.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

const double BbrAngle::pi        = Constants::pi;
const double BbrAngle::twoPi     = Constants::twoPi;
const double BbrAngle::toDegrees = Constants::radToDegrees;

// The followings characters are used in DegString() 
// for printout in degrees
#ifdef HPUX
const std::string BbrAngle::degChar = "�";
#else
const std::string BbrAngle::degChar = "^";
#endif

const std::string BbrAngle::deg1Char =  "'";
const std::string BbrAngle::deg2Char = "\"";

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

std::string BbrAngle::degString() const
{
  float tmp;
  const int deglen=10;
  int deg, deg_, deg__;
  char cdeg[deglen], cdeg_[deglen], cdeg__[deglen];
  std::string sign = "";
  
  if ((tmp = this->deg()) < 0) 
  { sign = "-"; tmp = -tmp; };
  deg   = int(tmp);
  deg_  = int(tmp = 60*(tmp - deg));
  deg__ = int(60*(tmp - deg_));

  snprintf(cdeg,deglen,"%i",deg);
  snprintf(cdeg_,deglen,"%i",deg_);
  snprintf(cdeg__,deglen,"%i",deg__);
  return 
  (
    sign + 
    std::string(cdeg)+degChar+
    std::string(cdeg_)+deg1Char+
    std::string(cdeg__)+deg2Char
  );
}


