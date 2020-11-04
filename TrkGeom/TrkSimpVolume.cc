//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: TrkSimpVolume.cc 105 2010-01-15 12:14:11Z stroili $
//
// Description:
//	Class TrkSimpVolume
//
// Author List:
//	Gautier Hamel de Monchenault - CEN Saclay & Lawrence Berkeley Lab
//
//
//------------------------------------------------------------------------

//----------------
// BaBar Header --
//----------------
#include "BaBar/BaBar.hh"
#include "ErrLogger/ErrLog.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "TrkGeom/TrkSimpVolume.hh"

//---------------
// C++ Headers --
//---------------
#include <assert.h>
#include <algorithm>
#include <vector>
#include "BaBar/Constants.hh"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Utilities/CLHEP.h"
#include "CLHEP/Geometry/Transformation.h"
#include "CLHEP/Geometry/HepPoint.h"
#include "BbrGeom/Trajectory.hh"
#include "TrkBase/TrkErrCode.hh"
#include "DetectorModel/Intersection.hh"
#include "DetectorModel/DetCylinder.hh"
#include "DetectorModel/DetPlane.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------
TrkSimpVolume::TrkSimpVolume( double rmin, double rmax,
                              double zmin, double zmax )
  : _rmin(rmin), _rmax(rmax), _zmin(zmin), _zmax(zmax)
{
  HepTransformation nullTransf;
  _surf[0].reset( new DetCylinder( nullTransf, _rmin ) );
  _surf[1].reset( new DetCylinder( nullTransf, _rmax ) );
  _surf[2].reset( new DetPlane( HepTransformation( Hep3Vector(0.,0.,_zmin),
                                                   Hep3Vector(0,0,1.) ) ) );
  _surf[3].reset( new DetPlane( HepTransformation( Hep3Vector(0.,0.,_zmax),
                                                   Hep3Vector(0,0,1.) ) ) );
}

TrkSimpVolume::TrkSimpVolume( const char* name,
                              double rmin, double rmax,
                              double zmin, double zmax )
  : TrkVolume(name),
    _rmin(rmin), _rmax(rmax), _zmin(zmin), _zmax(zmax)
{
  HepTransformation nullTransf;
  _surf[0].reset( new DetCylinder( nullTransf, _rmin ) );
  _surf[1].reset( new DetCylinder( nullTransf, _rmax ) );
  _surf[2].reset( new DetPlane( HepTransformation( Hep3Vector(0.,0.,_zmin),
                                                   Hep3Vector(0,0,1.) ) ) );
  _surf[3].reset( new DetPlane( HepTransformation( Hep3Vector(0.,0.,_zmax),
                                                   Hep3Vector(0,0,1.) ) ) );
}

//--------------
// Destructor --
//--------------
TrkSimpVolume::~TrkSimpVolume()
{
}

//
// returns true if the onput point lies inside the tracking volume
//
bool
TrkSimpVolume::isInside( const HepPoint& point )  const 
{
  const double r ( sqrt( sqr(point.x()) + sqr(point.y()) ) );
  const double z ( point.z() );
  if( r<_rmin || r>_rmax || z<_zmin || z>_zmax ) return false;
  return true;
}

bool 
TrkSimpVolume::extendThrough( const Trajectory* traj, 
			      double& flightDistance, 
			      trkDirection trkDir,
			      double* startingValue     ) const 
{
  //  declaration of static variables
  static int depth = 0;  // depth of recursive calls
  static double flight[2] = {0.,0.};
  // 
  //  The array flight[2] is initialized using the starting value 
  //  of the flight distance (pointed at by 'startingValue'),  
  //  or  using the range of the trajectory if 'startingValue' is null.
  //
  //  i and j are the two indexes of the array, i always refers to the 
  //  value to be extended. 'sign' is - or +, when trkDir is In or Out.
  //  That is :
  //             i=1, j=0, sign=-   for  trkDir=trkIn
  //             i=0, j=1, sign=+   for  trkDir=trkOut
  //  i, j and sign used to be static and are now dynamic for clarity.
  //
  const int i( (trkDir==trkOut ) ? 1:0 ); // index of the value to be extended
  const int j( 1-i );                     // index to the other value
  const int sign( 2*i-1 );                // sign of the motion along the traj
  if( depth==0 ) {
    if( startingValue!=0 )  {
      // start from the povided starting value
      flight[i] = *startingValue;
    }  else if( trkDir==trkOut ) {
      // when extending outwards, start from the low range value
      flight[i] = traj->lowRange() ;
    } else {
      // when extending inwards, start from the high range value
      flight[i] = traj->hiRange() ;
    }
  } else {
    // initialize flight[i] to the current value of flightDistance 
    flight[i] = flightDistance;
  }
  flight[j] = flight[i];
  // f : value of flightrange to be extended
  double& f = flight[i];
  //
  // determine a reasonable range to extend the flight distance :
  // The range increment must be smaller than the length of a full loop,
  // assuming that the trajectory is described locally by an helix
  //
  HepPoint point( traj->position( f ) );
  Hep3Vector vect( traj->direction( f ) );
  double cosDip( vect.perp()/vect.mag() );
  Hep3Vector delDir( traj->delDirect( f ) );
  double curv( delDir.mag() );
  double maxDeltaF ( 500. );
  if( fabs(curv)>0. ) maxDeltaF = sign*2*Constants::pi*cosDip/fabs(curv);
  //
  // set the range for intersections
  //
  double frange[2];
  frange[j] = f;
  frange[i] = f+maxDeltaF;
  //
  // Create a vector for flight distances at intersections
  //
  std::vector<double> finter; finter.reserve(4);
  SurfacePoint uv;
  double fcur;
  // loop over the 4 surfaces and find intersections
  for ( int isurf=0; isurf<4; isurf++ ) {
    Intersection inter( *traj,  *_surf[isurf]  );
    fcur = f;
    TrkErrCode iflag = inter.intersect(fcur,uv,trkDir,frange);
    if( iflag.success() ) {
      if( isurf<2 ) {
	if( uv[1]<_zmin ||  uv[1]>_zmax ) continue;
      }	else {
	double r = sqrt( sqr( uv[0] ) + sqr( uv[1] ) );
	if( r<_rmin || r>_rmax ) continue;
      }
      finter.push_back(fcur);
    }
  }
  if( finter.empty() ) {
    // no intersection found
    depth = 0;
    return false;
  }
  // we only ever reference the 1st element,
  // after sorting, so we do a _partial_ sort
  // Also, we know that there is at least one entry,
  // so both [begin, begin+1) and [begin+1, end) are valid
  // ranges
  std::partial_sort(finter.begin(),finter.begin()+1,finter.end());
  // number of intersections found
  int n = finter.size();
  //
  // The following algorithm deals with the case where the entrance 
  // point and the exit point are on the same surface.
  //
  // Two cases are considered :
  // 1) the point was originally inside the tracking volume. 
  //    In that case, the exit point is the first intersection.
  // 2) the point was originally outside the tracking volume.
  //    In that case, the first intersection is the entrance point into
  //    the volume. The trajectory is then extended a little beyond the 
  //    entrance point (by 10 microns along the trajectory).  
  //    If the new point is inside the volume, we are back
  //    to the previous case (case 1).  Then we redo the intersection
  //    search from the new position (recursive call). 
  //    If on the contrary, after the tiny step beyond the entrance point,
  //    the new point is already outside the volume, 
  //    we consider that the trajectory was tangent to the entrance surface 
  //    and return the current value of the path length.  
  //
  // Warning :   
  //    In principle, there should never be more than two levels of
  //    recursion.  However, there is a protection against recursive traps.
  //    Any warning message is the hint that there is a bug 
  //    and should be reported.
  //  
  if( isInside(point) ) {
    if( n==1 ) {
      f = finter[0];
    } else {
      // the following includes the
      // case where the trajectory enters twice (or more) the volume.
      // For the moment, we only consider the first branch 
      // of the trajectory in the volume.
      f = finter[0];  
    }
  } else {
    // Extend the trajectory a little beyond the point of first intersection
    // (ie move a litle bit along the trajectory in the same direction)
    const double epsilon(0.01);
    fcur = finter[0]+sign*epsilon;
    if( isInside( traj->position( fcur ) ) ) {
      // If after the move the point is inside the volume (normal case),
      // redo the intersection search from the new position.
      if( ++depth==2 ) {  // increment the depth of recursive calls
	// protection against infinite loops - should never happen
	ErrMsg(warning) << "TrkSimpVolume - warning " 
			<< " caught in a trap - give up " << endmsg;
	depth = 0;
	return false;
      }
      f = fcur;
      bool nextinter = extendThrough( traj, f, trkDir );
      if(nextinter)flightDistance = f;
      return nextinter;
    }
    // If after the move the point is outside the volume, it's probably a
    // case where the trajectory is "tangent" to the surface. Do nothing
    // special then (ie return 'true' with the current flight).
    f = finter[0];
  }
  depth=0;
  flightDistance = f;
  return true;
}
