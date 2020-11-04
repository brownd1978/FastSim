//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsRecoCalo.cc 486 2010-01-13 16:36:34Z stroili $
//
// Description:
//	See AbsRecoCalo.hh
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Bob Jacobsen            LBNL
//	Stephen J. Gowdy        Univeristy of Edinburgh
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "AbsCalo/AbsRecoCalo.hh"
#include "AbsCalo/AbsRecoCaloErrorMatrix.hh"
#include "AbsCalo/AbsCaloTypes.hh"
//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEnv/AbsEnv.hh"
#include "AbsEnv/TwoCoordIndex.hh"
#include "BbrGeom/BbrError.hh"
#include "difAlgebra/DifNumber.hh"
#include "difAlgebra/DifArray.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "CLHEP/Utilities/CLHEP.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "PDT/PdtEntry.hh"
#include "EmcGeom/EmcXtal.hh"
#include "EmcGeom/EmcXtalType.hh"
#include "ErrLogger/ErrLog.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------


//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

unsigned int AbsRecoCalo::_nextId = 0;

//----------------
// Constructors --
//----------------
AbsRecoCalo::AbsRecoCalo()
{
  _id = _nextId;
  _nextId++;
}

//--------------
// Destructor --
//--------------
AbsRecoCalo::~AbsRecoCalo()
{
}

//-------------
// Operators --
//-------------
bool
AbsRecoCalo::operator==( const AbsRecoCalo& toCompare ) const
{
  bool answer = false;

  if ( fourMomentum() == toCompare.fourMomentum() ) answer = true;

  return answer;
}

bool
AbsRecoCalo::operator<( const AbsRecoCalo& toCompare ) const
{
  // double mag=fourMomentum().vect().mag();
  // double magCom=toCompare.fourMomentum().vect().mag();
  // double t = fourMomentum().t();
  // double tCom = toCompare.fourMomentum().t();
  
  // ErrMsg( warning ) << "mag,magCom:"<<mag<<" "<<magCom<<endmsg;
  
  // if ( mag < magCom ) {
  //  return true;
  // }
  // else 
  //  if ( mag == magCom && t < tCom) {
  //    return true;
  //  }
  //  else 
  //    return false;
  
  // ErrMsg( error ) << "AbsRecoCalo " << _id << " < " << toCompare.id() << " ?\n" << endmsg;
  return (id() < toCompare.id());
}

//-------------
// Selectors --
//-------------
Hep3Vector
AbsRecoCalo::direction( const HepPoint &from,
			const PdtEntry* particle ) const
{
  HepPoint position( this->position( from, particle ) );
  Hep3Vector direction( position - from );

  return direction.unit();
}

HepLorentzVector
AbsRecoCalo::fourMomentum( const HepPoint& from,
			   const PdtEntry* particle ) const
{
  double e = this->energy( from, particle );
  double p = e;
  if ( particle != 0 ) p = sqrt( sqr( e ) - sqr( particle->mass() ) );
  Hep3Vector momentum = p * this->direction( from, particle );

  HepLorentzVector fourVec( momentum, e);

  return fourVec;
}

HepPoint 
AbsRecoCalo::rawPosition( const HepPoint&,
			  const PdtEntry* ) const{

  ErrMsg(fatal) << "AbsRecoCalo::rawPosition not implemented will terminate"
		<< endmsg;

  return HepPoint(0.0,0.0,0.0);

}


double 
AbsRecoCalo::rawEnergy( const HepPoint&, 
			const PdtEntry* ) const {

  ErrMsg(fatal) << "AbsRecoCalo::rawEnergy not implemented will terminate"
		<< endmsg;

  return 0.0;

}


double
AbsRecoCalo::energyErr( const HepPoint &from, const PdtEntry* particle ) const
{
  const BbrError errors( this->errorMatrix( from, particle ) );

  return sqrt( errors(1,1) );
}

BbrError
AbsRecoCalo::errorMatrixXYZ( const HepPoint &from, const PdtEntry* particle ) const
{
  const BbrError errors( errorMatrix( from, particle ) );
  const HepPoint point ( position( from, particle ) );
  DifNumber eDF(energy(from, particle),1,4), tDF(point.theta(),2,4), pDF(point.phi(),3,4), rDF(point.mag(),4,4);
  DifArray pars(4,4);
  pars[0]   =  rDF*sin(tDF)*cos(pDF);
  pars[1]   =  rDF*sin(tDF)*sin(pDF);
  pars[2]   =  rDF*cos(tDF);
  pars[3]   =  eDF;
  BbrError result(errors.similarity(pars.jacobian()));
  return result;
}

BbrError
AbsRecoCalo::directionErr( const HepPoint &from,
			   const PdtEntry* particle ) const
{
  // Do error transformation from (E, theta, phi, r) to (Nx, Ny, Nz)

  Hep3Vector dir( this->direction( from, particle ) );
  if(dir.mag()<1e-12)return  BbrError(3);
  
  double z = dir.z();
  double perp = dir.perp();
  
  double cos_theta = z;
  double sin_theta = perp;
  double sin_phi = dir.y() / perp;
  double cos_phi = dir.x() / perp;

  static HepMatrix toComp( 3, 4 );
  toComp(1,1) = 0;
  toComp(1,2) = cos_theta * cos_phi;
  toComp(1,3) = -sin_theta * sin_phi;
  toComp(1,4) = 0;
  toComp(2,1) = 0;
  toComp(2,2) = cos_theta * sin_phi;
  toComp(2,3) = sin_theta * cos_phi;
  toComp(2,4) = 0;
  toComp(3,1) = 0;
  toComp(3,2) = -sin_theta;
  toComp(3,3) = 0;
  toComp(3,4) = 0;

  BbrError theError( this->errorMatrix( from,
					particle ).similarity( toComp ) );

  return theError;
}

BbrError
AbsRecoCalo::positionErr( const HepPoint &from,
			  const PdtEntry* particle ) const
{
  // Do translation from (E, theta, phi, r) to (x, y, z)


  // distance-vector from "from" to "position"
  Hep3Vector offset = position(from,particle) - from ;
  if(offset.mag()<1e-12)return  BbrError(3);  
  double r = offset.mag();
  double z = offset.z();
  double perp = offset.perp();

  double cos_theta = z / r ;
  double sin_theta = perp / r ;
  double sin_phi = offset.y() / perp;
  double cos_phi = offset.x() / perp;

  // Create a matrix to transform the error matrix
  static HepMatrix toComp( 3, 4 );
  toComp(1,1) = 0;
  toComp(1,2) = r * cos_theta * cos_phi;
  toComp(1,3) = - r * sin_theta * sin_phi;
  toComp(1,4) = sin_theta * cos_phi;
  toComp(2,1) = 0;
  toComp(2,2) = r * cos_theta * sin_phi;
  toComp(2,3) = r * sin_theta * cos_phi;
  toComp(2,4) = sin_theta * sin_phi;
  toComp(3,1) = 0;
  toComp(3,2) = - r * sin_theta;
  toComp(3,3) = 0; 
  toComp(3,4) = cos_theta;

  BbrError theError( this->errorMatrix( from,
					particle ).similarity( toComp ) );

  return theError;
}

BbrError
AbsRecoCalo::fourMomentumErr( const HepPoint &from,
			      const PdtEntry* particle ) const
{
  // Do translation from (E, theta, phi, r) to ( px, py, pz, E )

  Hep3Vector dir( this->direction( from, particle ) );

  double z = dir.z();
  double perp = dir.perp();
  if(dir.mag()<1e-12)return  BbrError(4);

  double cos_theta = z;
  double sin_theta = perp;
  double sin_phi = dir.y() / perp;
  double cos_phi = dir.x() / perp;

  double e( this->energy( from, particle ) );
  double p = e;

  // If particle is defined use this to work out the momentum
  if ( particle != 0 )
    p = sqrt( sqr( e ) - sqr( particle->mass() ) );

  // Create a matrix to transform the error matrix
  static HepMatrix toComp( 4, 4 );
  toComp(1,1) = sin_theta * cos_phi * e/p;
  toComp(1,2) = p * cos_theta * cos_phi;
  toComp(1,3) = -p * sin_theta * sin_phi;
  toComp(1,4) = 0;
  toComp(2,1) = sin_theta * sin_phi * e/p;
  toComp(2,2) = p * cos_theta * sin_phi;
  toComp(2,3) = p * sin_theta * cos_phi;
  toComp(2,4) = 0;
  toComp(3,1) = cos_theta * e/p;
  toComp(3,2) = -p * sin_theta;
  toComp(3,3) = 0;
  toComp(3,4) = 0;
  toComp(4,1) = 1;
  toComp(4,2) = 0;
  toComp(4,3) = 0;
  toComp(4,4) = 0;

  BbrError theError( this->errorMatrix( from,
					particle ).similarity( toComp ) );

  return theError;
}

AbsRecoCaloPVec 
AbsRecoCalo::madeFrom() const
{
  static AbsRecoCaloPVec list(0);
  return list;
}

const EmcBump*
AbsRecoCalo::dynamic_cast_EmcBump() const
{
  return 0;
}

const EmcCand*
AbsRecoCalo::dynamic_cast_EmcCand() const
{
  return 0;
}

const EmcCluster*
AbsRecoCalo::dynamic_cast_EmcCluster() const
{
  return 0;
}

const IfrAbs3D*
AbsRecoCalo::dynamic_cast_IfrAbs3D() const
{
  return 0;
}

const NeutralHad*
AbsRecoCalo::dynamic_cast_NeutralHad() const
{
  return 0;
}

EmcBump*
AbsRecoCalo::dynamic_cast_EmcBump()
{
  return 0;
}

EmcCand*
AbsRecoCalo::dynamic_cast_EmcCand()
{
  return 0;
}

EmcCluster*
AbsRecoCalo::dynamic_cast_EmcCluster()
{
  return 0;
}

IfrAbs3D*
AbsRecoCalo::dynamic_cast_IfrAbs3D()
{
  return 0;
}

NeutralHad*
AbsRecoCalo::dynamic_cast_NeutralHad()
{
  return 0;
}

BbrError
AbsRecoCalo::errorMatrixOfEmcClusters(double clEnergy,double clTheta,double clPhi) const{
  
  //  Jan 11 - 2000, Per reinertsen
  //  Okey, I'll hardwire the results from our study in Bergen. A Babar note will 
  //  be ready pretty soon. 
  //  The energy resolution is the relative resolution (Delta E/E)
  //  The correlation  entries are found to be very small and are thus set equal to zero.   

  //  July 27, 2004, Stephane Willocq
  //  The angular resolution parameterization is re-evaluated with release 14 single
  //  photon MC, taking into account the dependence on energy, cos(theta) and local
  //  crystal coordinates xtheta and xphi.
  //  This is documented in various Emc meeting presentations by Josef-Stefan
  //  Wenzler (2003-2004).

  // Seven regions are probed 
  double thetaMean[7]={0.1700636,0.3557819,0.7937707,0.8437179,0.8925519,0.9041466,0.9334562};

  //Each resolution parameter for each region is represented as a function of energy
  //The energy resolution is represented by a three parameter function  
  double fit1Energy[7]={2.220309,2.345498,2.551324,2.555908,2.304174,2.186032,2.118749};
  double fit2Energy[7]={0.6664312,0.2305636,1.7593733E-04,1.8059720E-04,0.9898133,0.1151342,2.5358578E-04};
  double fit3Energy[7]={0.3464189,0.3311598,0.2569567,0.1612448,0.2685481,0.3284982,0.3220451}; 
  double clCosTheta = cos(clTheta);
           

  // First, determine the energy resolution           
  // --------------------------------------

  // Create a position parameter in theta (maps) that will be the 
  // effective position from where the resolution is found. 

  double maps  = clCosTheta ; 
  
  // In the straight uppward region use the first measured point

  if ( (clCosTheta < 0.1700636 ) && (clCosTheta >  -0.1700636 ) )  {
    maps =  0.1700636 ;
  }

  // In the backward region use the corresponding region in the forward region 
  if (clCosTheta < - 0.1700636 ) {
    maps = - clCosTheta ;
  }
  // If in the very forward region, use the last measured point. 
 
  if ( maps >= 0.9334562 ) {
    maps = 0.9334562 ;
  } 

  //Do a linear interpolation between current position and the measured positions

  double temp1Energy=0.,temp2Energy=0.,delta=0.,distance=0.;

  for (int i = 0; i < 6 ; i++) {
    if ( ( maps <= thetaMean[i+1] ) && ( maps >= thetaMean[i] ) ) {
      double en1  = fit1Energy[i]/( sqrt(sqrt(clEnergy)) ) ;
      double en2  = fit2Energy[i] ;
      double en3  = fit3Energy[i]*clEnergy ;
      temp1Energy = sqrt( en1*en1 + en2*en2 + en3*en3 ) ;
             en1  = fit1Energy[i+1]/( pow(clEnergy,0.4) ) ;
             en2  = fit2Energy[i+1] ;
             en3  = fit3Energy[i+1]*clEnergy ;
      temp2Energy = sqrt( en1*en1 + en2*en2 + en3*en3 ) ;

      delta    = thetaMean[i+1]-thetaMean[i] ;
      distance = maps - thetaMean[i] ;
    }   
  }

  double errEnergy = ( temp2Energy - temp1Energy )*distance/delta + temp1Energy ;
         errEnergy /= 100. ;
  //HM: turn Delta[E]/E into Delta[E]
         errEnergy *= clEnergy;

  // Second, determine the angular resolution           
  // ----------------------------------------

  AbsRecoCaloErrorMatrix errMatrixUtil;

  // Determine local coordinates

  HepPoint Location(this->position());
  Hep3Vector vecLoc(Location.x(), Location.y(), Location.z());
  Hep3Vector vecCtr = errMatrixUtil.vecFromXtalCtr( &vecLoc );

  // Determine dimensions of peak Xtal front face
  double Adim = 4.746;
  double Bdim = 4.696;
  double Cdim = 4.650;
  const TwoCoordIndex *theTCI = gblEnv->getEmc()->locateIndex(clTheta, clPhi);
  if (0 != theTCI)  // require the vector to point inside the EMC
  {
    const EmcXtalType *theXtalType = gblEnv->getEmc()->getXtal(*theTCI)->dimensions();
    Adim = theXtalType->getA();
    Bdim = theXtalType->getB();
    Cdim = theXtalType->getC();
  }
  //  else { ErrMsg( warning ) << " TCI not found" << endmsg;}

  // Local coordinates are normalized by the Xtal size
  double xTheta = 4.*vecCtr.z()/(Adim+Cdim);
  double xPhi   = 2.*vecCtr.x()/Bdim;

  // Extract parameterized angular resolution
  double errTheta = errMatrixUtil.sigmaTheta( clEnergy, clCosTheta, xTheta );
  double errPhi = errMatrixUtil.sigmaPhi( clEnergy, clCosTheta, xPhi );


  // Third, set arbitrary radial error
  // ---------------------------------
  double errR = 10;  

  BbrError theError(4);

  theError[0][0] = errEnergy*errEnergy ; 
  theError[1][1] = errTheta*errTheta ; 
  theError[2][2] = errPhi*errPhi ; 
  theError[3][3] = errR*errR ; 
                       
  return theError ;
}

bool
AbsRecoCalo::overlaps(const AbsRecoCalo& other) const {
// default implementation is to use pointer identity (yuch!)
  return this == &other;
}
