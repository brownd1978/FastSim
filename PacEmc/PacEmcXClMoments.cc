//-----------------------------------------------------------------------
// File and Version Information:
//    PacEmcClusterEnergySums
//      Modified from Babar's EmcClusterEnergySums class.
//
// Author List:
//     Chih-hsiang Cheng       Caltech   (initial version 2008/06/13)
//
//-----------------------------------------------------------------------
#include "BaBar/BaBar.hh"

#include "PacEmc/PacEmcXClMoments.hh"

//-------------
// C Headers --
//-------------

extern "C" {
#include <math.h>
}

#include "BaBar/Constants.hh"
#include "AbsEnv/TwoCoordIndex.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Vector/ThreeVector.h"

#include "PacEmc/PacEmcCluster.hh"
#include "PacEmc/PacEmcDigi.hh"
using namespace std;
using std::ostream;

//----------------
// Constructors --
//----------------

PacEmcXClMoments::PacEmcXClMoments( const PacEmcCluster& toUse )
  : PacEmcAbsClusterProperty( toUse )
{
  init();
}

PacEmcXClMoments::PacEmcXClMoments( const PacEmcXClMoments &other)
  : PacEmcAbsClusterProperty( other )
{
  init();
}

//--------------
// Destructor --
//--------------

PacEmcXClMoments::~PacEmcXClMoments()
{
  _energyDistribution->clear();
  delete _energyDistribution;
  _energyDistribution = 0;
}


//-------------
// Methods   --
//-------------

//-------------
// Methods   --
//-------------

double
PacEmcXClMoments::secondMoment() const
{
  double r,sum=0;
  for (int i=0; i<_clusterSize; i++) 
  {
    r=(*_energyDistribution)[i].r;
    sum+=(*_energyDistribution)[i].deposited_energy*r*r;
  }
  sum/=myCluster().energy();

  return sum;
}

double
PacEmcXClMoments::secondMomentTP() const
{

  Hep3Vector cluster = myCluster().where() - HepPoint(0);

  double sum=0;

  const std::vector< PacEmcDigi* > *digilist= myCluster().pacEmcDigis();
  PacEmcDigiConstIterator iter= digilist->begin();
  while ( (iter!= digilist->end()) ) {
    double da = cluster.angle( (*iter)->where()-HepPoint(0) );
    sum+= (*iter)->energy() *da*da;
    iter++;
  }
  sum/=myCluster().energy();
  
  return sum;
}

double 
PacEmcXClMoments::secondMomentPhi() const
{
  double r,sum=0;
  for (int i=0; i<_clusterSize; i++) 
  {
    r=(*_energyDistribution)[i].r*cos((*_energyDistribution)[i].phi); // Babar code uses sin(), which I believe is wrong.  chcheng 20090304
    sum+=(*_energyDistribution)[i].deposited_energy*r*r;
  }
  sum/=myCluster().energy();

  return sum;
}

double 
PacEmcXClMoments::secondMomentTheta() const
{
  double r,sum=0;
  for (int i=0; i<_clusterSize; i++) 
  {
    r=(*_energyDistribution)[i].r*sin((*_energyDistribution)[i].phi); // Babar code uses cos(), which I believe is wrong.  chcheng 20090304
    sum+=(*_energyDistribution)[i].deposited_energy*r*r;
  }
  sum/=myCluster().energy();

  return sum;
}  

double 
PacEmcXClMoments::absZernikeMoment(int n, int m, double R0) const
{
  // 1. Check if n,m are correctly
  if ((m>n) || ((n-m)%2 != 0) || (n<0) || (m<0)) 
  { 
    return -1;
  }

  // 2. Check if n,R0 are within validity Range :
  // n>20 or R0<5cm  just makes so sense !
  if ((n>20) || (R0<=5)) 
  {
    return -1;
  }
  if (n<=5) 
  {
    return fast_AbsZernikeMoment(n,m,R0);
  } else {
    return calc_AbsZernikeMoment(n,m,R0);
  }
}

double 
PacEmcXClMoments::lat() const
{
  double r,redmoment=0;
  int n,n1,n2,tmp;
  if (_clusterSize<3) 
  { 
    return 0; 
  }

  n1=0; n2=1;
  if ((*_energyDistribution)[1].deposited_energy > (*_energyDistribution)[0].deposited_energy) 
  {
    tmp=n2; n2=n1; n1=tmp;
  }
  for (int i=2; i<_clusterSize; i++) 
  {
    n=i;
    if ((*_energyDistribution)[i].deposited_energy > (*_energyDistribution)[n1].deposited_energy) 
    {
      tmp = n2;
      n2 = n1; n1 = i; n=tmp;
    } else {
      if ((*_energyDistribution)[i].deposited_energy > (*_energyDistribution)[n2].deposited_energy) 
      {
	tmp=n2; n2=i; n=tmp;
      }
    }

    r = (*_energyDistribution)[n].r;
    redmoment+= r*r* (*_energyDistribution)[n].deposited_energy;

  }
  double e1 = (*_energyDistribution)[n1].deposited_energy;
  double e2 = (*_energyDistribution)[n2].deposited_energy;
  double lat = redmoment/(redmoment+25.0*(e1+e2));

  return lat;
}

void
PacEmcXClMoments::print( ostream& o ) 
{
  HepPoint cl( myCluster().where() );
  Hep3Vector ClusDirection(cl.x(),cl.y(),cl.z());
  ClusDirection *= 1.0/ClusDirection.mag();
  Hep3Vector theta_axis(ClusDirection.y(),-ClusDirection.x(),0.0);
  theta_axis *= 1.0/theta_axis.mag();
  Hep3Vector phi_axis = theta_axis.cross(ClusDirection);

  const std::vector< PacEmcDigi* > *digilist= myCluster().pacEmcDigis();
  PacEmcDigiConstIterator iter= digilist->begin();

  PacEmcClEnergyDeposition clEdep;
  o << "Dump of EmcCluster with CoG at (" << cl.x() << "," << cl.y()
    << "," << cl.z() << ")" << endl;
  o << "------------------------------------------------------------------"
    << endl;
  o << " theta - axis : (" << theta_axis.x() << "," << theta_axis.y() << ","
    << theta_axis.z() << ")" << endl;
  o << " phi - axis : (" << phi_axis.x() << "," << phi_axis.y() << ","
    << phi_axis.z() << ")" << endl;
  int i=0;

  while ( (iter!= digilist->end()) )  {
    clEdep = (*_energyDistribution)[i];
    (*iter)->print( o );
    o << "Polar coords : (" << clEdep.r << "," << clEdep.phi << ")" << endl;
    iter++;
  }  
  
  for (int j=0; j<=8; j++)
    {
      o << "f[" << j << "](5) = " << (this->*_fcn[j])(5.0) << endl;
    }
  
  o << "------------------------------------------------------------------"
    << endl;
}

//-------------
// Selectors --
//-------------
    
//-------------
// Modifiers --
//-------------

//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------


//		-----------------------------------------
// 		-- Private Function Member Definitions --
//		-----------------------------------------


double 
PacEmcXClMoments::f00(double r) const { return 1; }

double 
PacEmcXClMoments::f11(double r) const { return r; }

double 
PacEmcXClMoments::f20(double r) const { return 2.0*r*r-1.0; }

double 
PacEmcXClMoments::f22(double r) const { return r*r; }

double 
PacEmcXClMoments::f31(double r) const { return 3.0*r*r*r - 2.0*r; }

double 
PacEmcXClMoments::f33(double r) const { return r*r*r; }

double 
PacEmcXClMoments::f40(double r) const { return 6.0*r*r*r*r-6.0*r*r+1.0; }

double 
PacEmcXClMoments::f42(double r) const { return 4.0*r*r*r*r-3.0*r*r; }

double 
PacEmcXClMoments::f44(double r) const { return r*r*r*r; }

double 
PacEmcXClMoments::f51(double r) const { return 10.0*pow(r,5)-12.0*pow(r,3)+3.0*r; }

double 
PacEmcXClMoments::f53(double r) const { return 5.0*pow(r,5) - 4.0*pow(r,3); }

double 
PacEmcXClMoments::f55(double r) const { return pow(r,5); }

double
PacEmcXClMoments::fast_AbsZernikeMoment(int n, int m, double R0)  const
{
  double r,ph,e,Re=0,Im=0,result;
  double TotalEnergy = myCluster().energy();
  int index = (n/2)*(n/2)+(n/2)+m; 
  for (int i=0; i<_clusterSize; i++)
  {
    r = (*_energyDistribution)[i].r / R0;
    if (r<1) 
    { 
      ph = ((*_energyDistribution)[i]).phi;
      e = (*_energyDistribution)[i].deposited_energy;
      Re = Re + e/TotalEnergy * (this->*_fcn[index])(r) * cos( (double) m * ph);
      Im = Im - e/TotalEnergy * (this->*_fcn[index])(r) * sin( (double) m * ph);
    }
  }
  result = sqrt(Re*Re+Im*Im);

  return result;
}

double
PacEmcXClMoments::calc_AbsZernikeMoment(int n, int m, double R0)  const
{
  double r,ph,e,Re=0,Im=0,f_nm,result;
  double TotalEnergy = myCluster().energy();
  for (int i=0; i<_clusterSize; i++) 
  {
    r = (*_energyDistribution)[i].r / R0;
    if (r<1) 
    { 
      ph = ((*_energyDistribution)[i]).phi;
      e = (*_energyDistribution)[i].deposited_energy;  
      f_nm=0;
      for (int s=0; s<=(n-m)/2; s++) 
      {
	if (s%2==0) {
	  f_nm = f_nm + fak(n-s)*pow(r,(double) (n-2*s))/(fak(s)*fak((n+m)/2-s)*fak((n-m)/2-s));
	}else {
	  f_nm = f_nm - fak(n-s)*pow(r,(double) (n-2*s))/(fak(s)*fak((n+m)/2-s)*fak((n-m)/2-s));
	}
      }
      Re = Re + e/TotalEnergy * f_nm * cos( (double) m*ph);
      Im = Im - e/TotalEnergy * f_nm * sin( (double) m*ph);
    }
  }
  result = sqrt(Re*Re+Im*Im);

  return result;
}

double
PacEmcXClMoments::fak(int n) const 
{
  double res=1.0;
  for (int i=2; i<=n; i++) 
  {
    res=res*(double) i;
  }

  return res;
}

void
PacEmcXClMoments::init()
{

  // need to get one digi to ask what kind of digi position method is used
  // or to be more precise what scaling factor has do be used
  double rescaleFactor = 1.;
  const PacEmcDigi* pDigi = myCluster().maxima();
  if ( pDigi != 0 ) rescaleFactor = pDigi->getRescaleFactor();

  // make sure we always use gravWhere for the cluster moments
  //  HepPoint cl( myCluster().where() );
  HepPoint cl( myCluster().gravWhere(&myCluster()) );
  Hep3Vector ClusDirection(cl.x(),cl.y(),cl.z());
  ClusDirection *= 1.0/ClusDirection.mag();
  Hep3Vector theta_axis(ClusDirection.y(),-ClusDirection.x(),0.0);
  theta_axis *= 1.0/theta_axis.mag();
  Hep3Vector phi_axis = theta_axis.cross(ClusDirection);


  const std::vector< PacEmcDigi* > *digilist= myCluster().pacEmcDigis();
  PacEmcDigiConstIterator iter= digilist->begin();
  _clusterSize = digilist->size();
  _energyDistribution = new vector<PacEmcClEnergyDeposition> (_clusterSize);
  
  int i=0;
  PacEmcClEnergyDeposition clEdep;

  while ( (iter!= digilist->end()) ) {

    clEdep.deposited_energy = (*iter)->energy();
    
    //     Hep3Vector diff = digi->where() - myCluster().where();
    Hep3Vector diff = (*iter)->where() - cl;
    Hep3Vector DigiVect = diff - diff.dot(ClusDirection) * ClusDirection;
    clEdep.r = DigiVect.mag() / rescaleFactor;
    clEdep.phi = DigiVect.angle(theta_axis);
    if (DigiVect.dot(phi_axis)<0)  { 
      clEdep.phi = 2*M_PI - clEdep.phi; 
    }

    (*_energyDistribution)[i] = clEdep;
    i++;
    iter++;
  }
  // gg 09/10/98

  _fcn[0]  = &PacEmcXClMoments::f00 ;
  _fcn[1]  = &PacEmcXClMoments::f11 ;
  _fcn[2]  = &PacEmcXClMoments::f20 ;
  _fcn[3]  = &PacEmcXClMoments::f31 ;
  _fcn[4]  = &PacEmcXClMoments::f22 ;
  _fcn[5]  = &PacEmcXClMoments::f33 ;
  _fcn[6]  = &PacEmcXClMoments::f40 ;
  _fcn[7]  = &PacEmcXClMoments::f51 ;
  _fcn[8]  = &PacEmcXClMoments::f42 ;
  _fcn[9]  = &PacEmcXClMoments::f53 ;
  _fcn[10] = &PacEmcXClMoments::f44 ;
  _fcn[11] = &PacEmcXClMoments::f55 ;
}

//		-----------------------------------
// 		-- Internal Function Definitions --
//		-----------------------------------

