//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsRecoCaloErrorMatrix.cc 486 2010-01-13 16:36:34Z stroili $
//
// Description:
//	See AbsRecoCaloErrorMatrix.hh
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Stephane Willocq        Univeristy of Massachusetts
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "AbsCalo/AbsRecoCaloErrorMatrix.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEnv/AbsEnv.hh"
#include "CLHEP/Vector/ThreeVector.h"
#include "EmcEnv/EmcEnv.hh"
#include "EmcGeom/EmcXtal.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------


//----------------
// Constructors --
//----------------
AbsRecoCaloErrorMatrix::AbsRecoCaloErrorMatrix()
{
}

//--------------
// Destructor --
//--------------
AbsRecoCaloErrorMatrix::~AbsRecoCaloErrorMatrix()
{
}

//-------------
// Selectors --
//-------------


Hep3Vector AbsRecoCaloErrorMatrix::vecFromXtalCtr( const Hep3Vector* aVector ) const
{
  float distCtr = -1.0;
  Hep3Vector posFromCtr(0.0, 0.0, 0.0);
  const TwoCoordIndex *theTCI
    = gblEnv->getEmc()->locateIndex(aVector->theta(), aVector->phi());

  if (0 != theTCI)  // require the vector to point inside the EMC
  {
    const EmcXtal *theGeom = gblEnv->getEmc()->getXtal(*theTCI);

    // compute the distance between the center of the crystal and the
    // entrance point of the vector on the face of the crystal

    const Hep3Vector *normal = theGeom->normalToFrontFace();
    Hep3Vector centre(theGeom->frontCentre() - HepPoint(0.0,0.0,0.0));
    double distanceOfPlane = normal->dot(centre);
    Hep3Vector tmpVector( aVector->x(), aVector->y(), aVector->z() );
    double amplitude = distanceOfPlane / normal->dot(aVector->unit());
    tmpVector.setMag(amplitude);
    Hep3Vector fromCtr = tmpVector - centre;
    distCtr = fromCtr.mag();
    // determine new x' and z' axes parallel to the plane of the crystal
    // front surface and pointing in the direction of increasing phi and
    // theta, respectively
    Hep3Vector xPtmp( normal->cross(Hep3Vector(0.0, 0.0, 1.0)) );
    Hep3Vector xPrime( xPtmp.unit() );
    Hep3Vector zPtmp( xPrime.cross(*normal) );
    Hep3Vector zPrime( zPtmp.unit() );
    // project distance from center vector onto new axes
    posFromCtr.setX( fromCtr.dot(xPrime) );
    posFromCtr.setZ( fromCtr.dot(zPrime) );
  }
  return posFromCtr;
}

double AbsRecoCaloErrorMatrix::sigmaVsE( const double* paramVector, const double energy ) const
{
  double sigma = -999.0;

  if ( energy > 0.0 )
  {
     double arg = paramVector[1] / pow(energy,paramVector[2]);
     sigma = (paramVector[0] + arg);
  }
  return sigma;
}

double AbsRecoCaloErrorMatrix::sigmaPhi( double energy, double costh, double xphi ) const
{
  double sigma = -999.0;

  static double xphiGridmean[10]={-0.965,-0.8331,-0.5432,-0.2268,-0.04662,0.04747,0.2267,0.5433,0.8331,0.9649};
  static double cosGridmean[12]={-0.751,-0.624,-0.362,-0.103,0.102,0.361,0.611,0.750,0.842,0.908,0.935,0.954};

  static double STAR2[12][10][3];


//Phi backward
//////////////////////////////////////////////
//cos(theta) bin: 0 xphi bin: 0
  STAR2[0][0][0] = -0.00581361;
  STAR2[0][0][1] = 0.00927319;
  STAR2[0][0][2] = 0.178208;
//cos(theta) bin: 0 xphi bin: 1
  STAR2[0][1][0] = -0.00241975;
  STAR2[0][1][1] = 0.00598712;
  STAR2[0][1][2] = 0.27567;
//cos(theta) bin: 0 xphi bin: 2
  STAR2[0][2][0] = -0.00258202;
  STAR2[0][2][1] = 0.00779404;
  STAR2[0][2][2] = 0.286612;
//cos(theta) bin: 0 xphi bin: 3
  STAR2[0][3][0] = -0.00378198;
  STAR2[0][3][1] = 0.0100563;
  STAR2[0][3][2] = 0.234441;
//cos(theta) bin: 0 xphi bin: 4
  STAR2[0][4][0] = -0.00293019;
  STAR2[0][4][1] = 0.00924084;
  STAR2[0][4][2] = 0.252398;
//cos(theta) bin: 0 xphi bin: 5
  STAR2[0][5][0] = -0.0051953;
  STAR2[0][5][1] = 0.0115774;
  STAR2[0][5][2] = 0.202731;
//cos(theta) bin: 0 xphi bin: 6
  STAR2[0][6][0] = -0.00277569;
  STAR2[0][6][1] = 0.00913509;
  STAR2[0][6][2] = 0.269836;
//cos(theta) bin: 0 xphi bin: 7
  STAR2[0][7][0] = -0.00172759;
  STAR2[0][7][1] = 0.00681875;
  STAR2[0][7][2] = 0.324305;
//cos(theta) bin: 0 xphi bin: 8
  STAR2[0][8][0] = -0.00286964;
  STAR2[0][8][1] = 0.00650969;
  STAR2[0][8][2] = 0.266947;
//cos(theta) bin: 0 xphi bin: 9
  STAR2[0][9][0] = -0.00496538;
  STAR2[0][9][1] = 0.00842634;
  STAR2[0][9][2] = 0.19616;

///////////////////////////////////////////////

//Phi barrel
//////////////////////////////////////////////
//cos(theta) bin: 0 xphi bin: 0
  STAR2[1][0][0] = -0.00655641;
  STAR2[1][0][1] = 0.0101954;
  STAR2[1][0][2] = 0.169738;
//cos(theta) bin: 0 xphi bin: 1
  STAR2[1][1][0] = -0.00160367;
  STAR2[1][1][1] = 0.00530258;
  STAR2[1][1][2] = 0.339073;
//cos(theta) bin: 0 xphi bin: 2
  STAR2[1][2][0] = -0.00257483;
  STAR2[1][2][1] = 0.00785228;
  STAR2[1][2][2] = 0.289245;
//cos(theta) bin: 0 xphi bin: 3
  STAR2[1][3][0] = -0.00348615;
  STAR2[1][3][1] = 0.0098413;
  STAR2[1][3][2] = 0.252582;
//cos(theta) bin: 0 xphi bin: 4
  STAR2[1][4][0] = -0.00517729;
  STAR2[1][4][1] = 0.011606;
  STAR2[1][4][2] = 0.194317;
//cos(theta) bin: 0 xphi bin: 5
  STAR2[1][5][0] = -0.0125568;
  STAR2[1][5][1] = 0.0191352;
  STAR2[1][5][2] = 0.119513;
//cos(theta) bin: 0 xphi bin: 6
  STAR2[1][6][0] = -0.00297291;
  STAR2[1][6][1] = 0.00944455;
  STAR2[1][6][2] = 0.270585;
//cos(theta) bin: 0 xphi bin: 7
  STAR2[1][7][0] = -0.00112882;
  STAR2[1][7][1] = 0.00640196;
  STAR2[1][7][2] = 0.367369;
//cos(theta) bin: 0 xphi bin: 8
  STAR2[1][8][0] = -0.0016201;
  STAR2[1][8][1] = 0.00535852;
  STAR2[1][8][2] = 0.347834;
//cos(theta) bin: 0 xphi bin: 9
  STAR2[1][9][0] = -0.0040974;
  STAR2[1][9][1] = 0.00769239;
  STAR2[1][9][2] = 0.225888;
//cos(theta) bin: 1 xphi bin: 0
  STAR2[2][0][0] = -0.00444927;
  STAR2[2][0][1] = 0.00812161;
  STAR2[2][0][2] = 0.221068;
//cos(theta) bin: 1 xphi bin: 1
  STAR2[2][1][0] = -0.00212148;
  STAR2[2][1][1] = 0.00587182;
  STAR2[2][1][2] = 0.308068;
//cos(theta) bin: 1 xphi bin: 2
  STAR2[2][2][0] = -0.00145313;
  STAR2[2][2][1] = 0.00675156;
  STAR2[2][2][2] = 0.348269;
//cos(theta) bin: 1 xphi bin: 3
  STAR2[2][3][0] = -0.00282679;
  STAR2[2][3][1] = 0.00922964;
  STAR2[2][3][2] = 0.273353;
//cos(theta) bin: 1 xphi bin: 4
  STAR2[2][4][0] = -0.006266;
  STAR2[2][4][1] = 0.0127979;
  STAR2[2][4][2] = 0.188439;
//cos(theta) bin: 1 xphi bin: 5
  STAR2[2][5][0] = -0.00431928;
  STAR2[2][5][1] = 0.0107528;
  STAR2[2][5][2] = 0.22108;
//cos(theta) bin: 1 xphi bin: 6
  STAR2[2][6][0] = -0.0035824;
  STAR2[2][6][1] = 0.00999645;
  STAR2[2][6][2] = 0.248448;
//cos(theta) bin: 1 xphi bin: 7
  STAR2[2][7][0] = -0.00140161;
  STAR2[2][7][1] = 0.00673938;
  STAR2[2][7][2] = 0.347069;
//cos(theta) bin: 1 xphi bin: 8
  STAR2[2][8][0] = -0.00591444;
  STAR2[2][8][1] = 0.00976064;
  STAR2[2][8][2] = 0.17488;
//cos(theta) bin: 1 xphi bin: 9
  STAR2[2][9][0] = -0.00640897;
  STAR2[2][9][1] = 0.0101157;
  STAR2[2][9][2] = 0.17304;
//cos(theta) bin: 2 xphi bin: 0
  STAR2[3][0][0] = -0.00303679;
  STAR2[3][0][1] = 0.00655627;
  STAR2[3][0][2] = 0.271314;
//cos(theta) bin: 2 xphi bin: 1
  STAR2[3][1][0] = -0.00148084;
  STAR2[3][1][1] = 0.00522954;
  STAR2[3][1][2] = 0.368358;
//cos(theta) bin: 2 xphi bin: 2
  STAR2[3][2][0] = -0.00142437;
  STAR2[3][2][1] = 0.00664341;
  STAR2[3][2][2] = 0.346856;
//cos(theta) bin: 2 xphi bin: 3
  STAR2[3][3][0] = -0.00389669;
  STAR2[3][3][1] = 0.0102324;
  STAR2[3][3][2] = 0.239061;
//cos(theta) bin: 2 xphi bin: 4
  STAR2[3][4][0] = -0.0112484;
  STAR2[3][4][1] = 0.0177348;
  STAR2[3][4][2] = 0.131526;
//cos(theta) bin: 2 xphi bin: 5
  STAR2[3][5][0] = -0.0060746;
  STAR2[3][5][1] = 0.012548;
  STAR2[3][5][2] = 0.192396;
//cos(theta) bin: 2 xphi bin: 6
  STAR2[3][6][0] = -0.00490482;
  STAR2[3][6][1] = 0.0113067;
  STAR2[3][6][2] = 0.217761;
//cos(theta) bin: 2 xphi bin: 7
  STAR2[3][7][0] = -0.00180145;
  STAR2[3][7][1] = 0.00714406;
  STAR2[3][7][2] = 0.329994;
//cos(theta) bin: 2 xphi bin: 8
  STAR2[3][8][0] = -0.00262119;
  STAR2[3][8][1] = 0.00644213;
  STAR2[3][8][2] = 0.293049;
//cos(theta) bin: 2 xphi bin: 9
  STAR2[3][9][0] = -0.0040051;
  STAR2[3][9][1] = 0.00760505;
  STAR2[3][9][2] = 0.233693;
//cos(theta) bin: 3 xphi bin: 0
  STAR2[4][0][0] = -0.0029198;
  STAR2[4][0][1] = 0.00645791;
  STAR2[4][0][2] = 0.269848;
//cos(theta) bin: 3 xphi bin: 1
  STAR2[4][1][0] = -0.00115854;
  STAR2[4][1][1] = 0.00489272;
  STAR2[4][1][2] = 0.40924;
//cos(theta) bin: 3 xphi bin: 2
  STAR2[4][2][0] = -0.000970154;
  STAR2[4][2][1] = 0.00611444;
  STAR2[4][2][2] = 0.377622;
//cos(theta) bin: 3 xphi bin: 3
  STAR2[4][3][0] = -0.00249836;
  STAR2[4][3][1] = 0.00882047;
  STAR2[4][3][2] = 0.285101;
//cos(theta) bin: 3 xphi bin: 4
  STAR2[4][4][0] = -0.00981014;
  STAR2[4][4][1] = 0.0162193;
  STAR2[4][4][2] = 0.143697;
//cos(theta) bin: 3 xphi bin: 5
  STAR2[4][5][0] = -0.00737856;
  STAR2[4][5][1] = 0.0138711;
  STAR2[4][5][2] = 0.171532;
//cos(theta) bin: 3 xphi bin: 6
  STAR2[4][6][0] = -0.00354351;
  STAR2[4][6][1] = 0.00999141;
  STAR2[4][6][2] = 0.258321;
//cos(theta) bin: 3 xphi bin: 7
  STAR2[4][7][0] = -0.00137752;
  STAR2[4][7][1] = 0.00661843;
  STAR2[4][7][2] = 0.351692;
//cos(theta) bin: 3 xphi bin: 8
  STAR2[4][8][0] = -0.00237415;
  STAR2[4][8][1] = 0.00604383;
  STAR2[4][8][2] = 0.293727;
//cos(theta) bin: 3 xphi bin: 9
  STAR2[4][9][0] = -0.00296257;
  STAR2[4][9][1] = 0.00655185;
  STAR2[4][9][2] = 0.270444;
//cos(theta) bin: 4 xphi bin: 0
  STAR2[5][0][0] = -0.0042005;
  STAR2[5][0][1] = 0.00775826;
  STAR2[5][0][2] = 0.224293;
//cos(theta) bin: 4 xphi bin: 1
  STAR2[5][1][0] = -0.0027655;
  STAR2[5][1][1] = 0.00650627;
  STAR2[5][1][2] = 0.275651;
//cos(theta) bin: 4 xphi bin: 2
  STAR2[5][2][0] = -0.00144282;
  STAR2[5][2][1] = 0.00668767;
  STAR2[5][2][2] = 0.345443;
//cos(theta) bin: 4 xphi bin: 3
  STAR2[5][3][0] = -0.00256411;
  STAR2[5][3][1] = 0.00890217;
  STAR2[5][3][2] = 0.280039;
//cos(theta) bin: 4 xphi bin: 4
  STAR2[5][4][0] = -0.00958295;
  STAR2[5][4][1] = 0.0160577;
  STAR2[5][4][2] = 0.146004;
//cos(theta) bin: 4 xphi bin: 5
  STAR2[5][5][0] = -0.0124589;
  STAR2[5][5][1] = 0.0189949;
  STAR2[5][5][2] = 0.122721;
//cos(theta) bin: 4 xphi bin: 6
  STAR2[5][6][0] = -0.00233154;
  STAR2[5][6][1] = 0.00871402;
  STAR2[5][6][2] = 0.291104;
//cos(theta) bin: 4 xphi bin: 7
  STAR2[5][7][0] = -0.00140989;
  STAR2[5][7][1] = 0.00667894;
  STAR2[5][7][2] = 0.348506;
//cos(theta) bin: 4 xphi bin: 8
  STAR2[5][8][0] = -0.00247169;
  STAR2[5][8][1] = 0.00631035;
  STAR2[5][8][2] = 0.291116;
//cos(theta) bin: 4 xphi bin: 9
  STAR2[5][9][0] = -0.00703219;
  STAR2[5][9][1] = 0.0106321;
  STAR2[5][9][2] = 0.158539;
//cos(theta) bin: 5 xphi bin: 0
  STAR2[6][0][0] = -0.00414709;
  STAR2[6][0][1] = 0.00788252;
  STAR2[6][0][2] = 0.234356;
//cos(theta) bin: 5 xphi bin: 1
  STAR2[6][1][0] = -0.000848612;
  STAR2[6][1][1] = 0.00456155;
  STAR2[6][1][2] = 0.412274;
//cos(theta) bin: 5 xphi bin: 2
  STAR2[6][2][0] = -0.00115858;
  STAR2[6][2][1] = 0.00642632;
  STAR2[6][2][2] = 0.364451;
//cos(theta) bin: 5 xphi bin: 3
  STAR2[6][3][0] = -0.00448748;
  STAR2[6][3][1] = 0.0108103;
  STAR2[6][3][2] = 0.222927;
//cos(theta) bin: 5 xphi bin: 4
  STAR2[6][4][0] = -0.00813018;
  STAR2[6][4][1] = 0.0145609;
  STAR2[6][4][2] = 0.156923;
//cos(theta) bin: 5 xphi bin: 5
  STAR2[6][5][0] = -0.00843526;
  STAR2[6][5][1] = 0.0150231;
  STAR2[6][5][2] = 0.164077;
//cos(theta) bin: 5 xphi bin: 6
  STAR2[6][6][0] = -0.00333496;
  STAR2[6][6][1] = 0.00971361;
  STAR2[6][6][2] = 0.253104;
//cos(theta) bin: 5 xphi bin: 7
  STAR2[6][7][0] = -0.0016752;
  STAR2[6][7][1] = 0.00706139;
  STAR2[6][7][2] = 0.339568;
//cos(theta) bin: 5 xphi bin: 8
  STAR2[6][8][0] = -0.00628024;
  STAR2[6][8][1] = 0.0102109;
  STAR2[6][8][2] = 0.174924;
//cos(theta) bin: 5 xphi bin: 9
  STAR2[6][9][0] = -0.0066912;
  STAR2[6][9][1] = 0.0104062;
  STAR2[6][9][2] = 0.169069;
///////////////////////////////////////////////

//Phi forward
//////////////////////////////////////////////
//cos(theta) bin: 0 xphi bin: 0
  STAR2[7][0][0] = -0.00507342;
  STAR2[7][0][1] = 0.008819;
  STAR2[7][0][2] = 0.205846;
//cos(theta) bin: 0 xphi bin: 1
  STAR2[7][1][0] = -0.00599006;
  STAR2[7][1][1] = 0.00980365;
  STAR2[7][1][2] = 0.167568;
//cos(theta) bin: 0 xphi bin: 2
  STAR2[7][2][0] = -0.00153255;
  STAR2[7][2][1] = 0.00688368;
  STAR2[7][2][2] = 0.343304;
//cos(theta) bin: 0 xphi bin: 3
  STAR2[7][3][0] = -0.00365959;
  STAR2[7][3][1] = 0.0101446;
  STAR2[7][3][2] = 0.253477;
//cos(theta) bin: 0 xphi bin: 4
  STAR2[7][4][0] = -0.0137463;
  STAR2[7][4][1] = 0.0203705;
  STAR2[7][4][2] = 0.117182;
//cos(theta) bin: 0 xphi bin: 5
  STAR2[7][5][0] = -0.00913366;
  STAR2[7][5][1] = 0.0157458;
  STAR2[7][5][2] = 0.15345;
//cos(theta) bin: 0 xphi bin: 6
  STAR2[7][6][0] = -0.00366617;
  STAR2[7][6][1] = 0.0101601;
  STAR2[7][6][2] = 0.250979;
//cos(theta) bin: 0 xphi bin: 7
  STAR2[7][7][0] = -0.00171862;
  STAR2[7][7][1] = 0.00714104;
  STAR2[7][7][2] = 0.335373;
//cos(theta) bin: 0 xphi bin: 8
  STAR2[7][8][0] = -0.00632586;
  STAR2[7][8][1] = 0.0103643;
  STAR2[7][8][2] = 0.177329;
//cos(theta) bin: 0 xphi bin: 9
  STAR2[7][9][0] = -0.00331375;
  STAR2[7][9][1] = 0.00703517;
  STAR2[7][9][2] = 0.26745;
//cos(theta) bin: 1 xphi bin: 0
  STAR2[8][0][0] = -0.00498725;
  STAR2[8][0][1] = 0.00878606;
  STAR2[8][0][2] = 0.209761;
//cos(theta) bin: 1 xphi bin: 1
  STAR2[8][1][0] = -0.00128835;
  STAR2[8][1][1] = 0.00492466;
  STAR2[8][1][2] = 0.369995;
//cos(theta) bin: 1 xphi bin: 2
  STAR2[8][2][0] = -0.00129023;
  STAR2[8][2][1] = 0.00666337;
  STAR2[8][2][2] = 0.360532;
//cos(theta) bin: 1 xphi bin: 3
  STAR2[8][3][0] = -0.00344503;
  STAR2[8][3][1] = 0.00998576;
  STAR2[8][3][2] = 0.260931;
//cos(theta) bin: 1 xphi bin: 4
  STAR2[8][4][0] = -0.00793196;
  STAR2[8][4][1] = 0.0145152;
  STAR2[8][4][2] = 0.166158;
//cos(theta) bin: 1 xphi bin: 5
  STAR2[8][5][0] = -0.0085569;
  STAR2[8][5][1] = 0.0151654;
  STAR2[8][5][2] = 0.159566;
//cos(theta) bin: 1 xphi bin: 6
  STAR2[8][6][0] = -0.00132594;
  STAR2[8][6][1] = 0.00775664;
  STAR2[8][6][2] = 0.338035;
//cos(theta) bin: 1 xphi bin: 7
  STAR2[8][7][0] = -0.00287482;
  STAR2[8][7][1] = 0.00832532;
  STAR2[8][7][2] = 0.285597;
//cos(theta) bin: 1 xphi bin: 8
  STAR2[8][8][0] = -0.00501492;
  STAR2[8][8][1] = 0.00882988;
  STAR2[8][8][2] = 0.190563;
//cos(theta) bin: 1 xphi bin: 9
  STAR2[8][9][0] = -0.0113609;
  STAR2[8][9][1] = 0.0151834;
  STAR2[8][9][2] = 0.114453;
///////////////////////////////////////////////

//Phi endcap
//////////////////////////////////////////////
//cos(theta) bin: 0 xphi bin: 0
  STAR2[9][0][0] = 0.000392283;
  STAR2[9][0][1] = 0.00263644;
  STAR2[9][0][2] = 0.719103;
//cos(theta) bin: 0 xphi bin: 1
  STAR2[9][1][0] = 0.000189131;
  STAR2[9][1][1] = 0.00326141;
  STAR2[9][1][2] = 0.644669;
//cos(theta) bin: 0 xphi bin: 2
  STAR2[9][2][0] = 0.000250494;
  STAR2[9][2][1] = 0.00470471;
  STAR2[9][2][2] = 0.505603;
//cos(theta) bin: 0 xphi bin: 3
  STAR2[9][3][0] = -0.00104441;
  STAR2[9][3][1] = 0.00740321;
  STAR2[9][3][2] = 0.356126;
//cos(theta) bin: 0 xphi bin: 4
  STAR2[9][4][0] = -0.00215607;
  STAR2[9][4][1] = 0.00860813;
  STAR2[9][4][2] = 0.296266;
//cos(theta) bin: 0 xphi bin: 5
  STAR2[9][5][0] = -0.00256135;
  STAR2[9][5][1] = 0.00913023;
  STAR2[9][5][2] = 0.282801;
//cos(theta) bin: 0 xphi bin: 6
  STAR2[9][6][0] = -0.000651233;
  STAR2[9][6][1] = 0.00693151;
  STAR2[9][6][2] = 0.374455;
//cos(theta) bin: 0 xphi bin: 7
  STAR2[9][7][0] = 0.000187593;
  STAR2[9][7][1] = 0.00476975;
  STAR2[9][7][2] = 0.492987;
//cos(theta) bin: 0 xphi bin: 8
  STAR2[9][8][0] = -0.00010581;
  STAR2[9][8][1] = 0.00363094;
  STAR2[9][8][2] = 0.575452;
//cos(theta) bin: 0 xphi bin: 9
  STAR2[9][9][0] = -0.000279968;
  STAR2[9][9][1] = 0.00371618;
  STAR2[9][9][2] = 0.534953;
//cos(theta) bin: 1 xphi bin: 0
  STAR2[10][0][0] = 0.000473194;
  STAR2[10][0][1] = 0.00296457;
  STAR2[10][0][2] = 0.720309;
//cos(theta) bin: 1 xphi bin: 1
  STAR2[10][1][0] = -0.000145119;
  STAR2[10][1][1] = 0.00468493;
  STAR2[10][1][2] = 0.637088;
//cos(theta) bin: 1 xphi bin: 2
  STAR2[10][2][0] = 0.000157657;
  STAR2[10][2][1] = 0.00624118;
  STAR2[10][2][2] = 0.54583;
//cos(theta) bin: 1 xphi bin: 3
  STAR2[10][3][0] = -0.00133799;
  STAR2[10][3][1] = 0.00952725;
  STAR2[10][3][2] = 0.377118;
//cos(theta) bin: 1 xphi bin: 4
  STAR2[10][4][0] = -0.00231509;
  STAR2[10][4][1] = 0.0107614;
  STAR2[10][4][2] = 0.338828;
//cos(theta) bin: 1 xphi bin: 5
  STAR2[10][5][0] = -0.00135818;
  STAR2[10][5][1] = 0.00954605;
  STAR2[10][5][2] = 0.371559;
//cos(theta) bin: 1 xphi bin: 6
  STAR2[10][6][0] = -0.00522571;
  STAR2[10][6][1] = 0.0135564;
  STAR2[10][6][2] = 0.250473;
//cos(theta) bin: 1 xphi bin: 7
  STAR2[10][7][0] = 0.000345287;
  STAR2[10][7][1] = 0.00602325;
  STAR2[10][7][2] = 0.564669;
//cos(theta) bin: 1 xphi bin: 8
  STAR2[10][8][0] = 0.000336061;
  STAR2[10][8][1] = 0.00405707;
  STAR2[10][8][2] = 0.716202;
//cos(theta) bin: 1 xphi bin: 9
  STAR2[10][9][0] = 0.000280086;
  STAR2[10][9][1] = 0.00344681;
  STAR2[10][9][2] = 0.678903;
//cos(theta) bin: 2 xphi bin: 0
  STAR2[11][0][0] = -190.949;
  STAR2[11][0][1] = 190.963;
  STAR2[11][0][2] = 3.68471e-05;
//cos(theta) bin: 2 xphi bin: 1
  STAR2[11][1][0] = -190.95;
  STAR2[11][1][1] = 190.961;
  STAR2[11][1][2] = 3.16769e-05;
//cos(theta) bin: 2 xphi bin: 2
  STAR2[11][2][0] = -190.95;
  STAR2[11][2][1] = 190.962;
  STAR2[11][2][2] = 2.70725e-05;
//cos(theta) bin: 2 xphi bin: 3
  STAR2[11][3][0] = -190.949;
  STAR2[11][3][1] = 190.962;
  STAR2[11][3][2] = 2.65323e-05;
//cos(theta) bin: 2 xphi bin: 4
  STAR2[11][4][0] = -190.948;
  STAR2[11][4][1] = 190.963;
  STAR2[11][4][2] = 3.07513e-05;
//cos(theta) bin: 2 xphi bin: 5
  STAR2[11][5][0] = -190.949;
  STAR2[11][5][1] = 190.962;
  STAR2[11][5][2] = 2.48929e-05;
//cos(theta) bin: 2 xphi bin: 6
  STAR2[11][6][0] = -190.949;
  STAR2[11][6][1] = 190.962;
  STAR2[11][6][2] = 2.69609e-05;
//cos(theta) bin: 2 xphi bin: 7
  STAR2[11][7][0] = -190.95;
  STAR2[11][7][1] = 190.962;
  STAR2[11][7][2] = 2.61585e-05;
//cos(theta) bin: 2 xphi bin: 8
  STAR2[11][8][0] = -190.951;
  STAR2[11][8][1] = 190.961;
  STAR2[11][8][2] = 2.55676e-05;
//cos(theta) bin: 2 xphi bin: 9
  STAR2[11][9][0] = -190.998;
  STAR2[11][9][1] = 190.914;
  STAR2[11][9][2] = -0.000292091;
///////////////////////////////////////////////


  //PHI
  double parijt[3];
  double paripjt[3];
  double parijpt[3];
  double parimjt[3];
  double parijmt[3];

  int costhbin=0, xphibin=0;
    
  // cos(theta) bins  
  if      (costh < -0.731) { costhbin = 0;}
  else if (costh < -0.517) { costhbin = 1;}
  else if (costh < -0.206) { costhbin = 2;}
  else if (costh <  0.000) { costhbin = 3;}
  else if (costh <  0.206) { costhbin = 4;}
  else if (costh <  0.517) { costhbin = 5;}
  else if (costh <  0.707) { costhbin = 6;}
  else if (costh <  0.795) { costhbin = 7;}
  else if (costh <  0.892) { costhbin = 8;}
  else if (costh <  0.923) { costhbin = 9;}
  else if (costh <  0.948) { costhbin = 10;}
  else if (costh <  0.975) { costhbin = 11;}

  // xphi bins
  if      (xphi<-0.9)   { xphibin=0;}
  else if (xphi<-0.77)  { xphibin=1;}
  else if (xphi<-0.325) { xphibin=2;}
  else if (xphi<-0.125) { xphibin=3;}
  else if (xphi<-0.)    { xphibin=4;}
  else if (xphi<0.125)  { xphibin=5;}
  else if (xphi<0.325)  { xphibin=6;}
  else if (xphi<0.77)   { xphibin=7;}
  else if (xphi<0.9)    { xphibin=8;}
  else if (xphi>=0.9)   { xphibin=9;}

  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  //INTERPOLATION PHI
  if (costh <= 0.923 && costh > -0.775)
  {
    if (xphi < 0.9)
    {
      parijt[0]=STAR2[costhbin][xphibin][0];
      parijt[1]=STAR2[costhbin][xphibin][1];
      parijt[2]=STAR2[costhbin][xphibin][2];
	      
      paripjt[0]=STAR2[costhbin+1][xphibin][0];
      paripjt[1]=STAR2[costhbin+1][xphibin][1];
      paripjt[2]=STAR2[costhbin+1][xphibin][2];
	      
      parijpt[0]=STAR2[costhbin][xphibin+1][0];
      parijpt[1]=STAR2[costhbin][xphibin+1][1];
      parijpt[2]=STAR2[costhbin][xphibin+1][2];
	      
      if (energy!=0.0)
      {
        sigma = sigmaVsE(parijt,energy)
        +      (sigmaVsE(paripjt,energy) - sigmaVsE(parijt,energy))/(cosGridmean[costhbin+1]-cosGridmean[costhbin])*(costh-cosGridmean[costhbin])
        +      (sigmaVsE(parijpt,energy) - sigmaVsE(parijt,energy))/(xphiGridmean[xphibin+1]-xphiGridmean[xphibin])*(xphi-xphiGridmean[xphibin]);
      }
    }
    else if (xphi >= 0.9)
    { 
      parijt[0]=STAR2[costhbin][xphibin][0];
      parijt[1]=STAR2[costhbin][xphibin][1];
      parijt[2]=STAR2[costhbin][xphibin][2];
	      
      paripjt[0]=STAR2[costhbin+1][xphibin][0];
      paripjt[1]=STAR2[costhbin+1][xphibin][1];
      paripjt[2]=STAR2[costhbin+1][xphibin][2];
	      
      parijmt[0]=STAR2[costhbin][xphibin-1][0];
      parijmt[1]=STAR2[costhbin][xphibin-1][1];
      parijmt[2]=STAR2[costhbin][xphibin-1][2];
	      
      if (energy!=0.0)
      {
        sigma = sigmaVsE(parijt,energy)
        +      (sigmaVsE(paripjt,energy) - sigmaVsE(parijt,energy))/(cosGridmean[costhbin+1]-cosGridmean[costhbin])*(costh-cosGridmean[costhbin])
        +      (sigmaVsE(parijt,energy) - sigmaVsE(parijmt,energy))/(xphiGridmean[xphibin]-xphiGridmean[xphibin-1])*(xphi-xphiGridmean[xphibin]);
      }
    }
  }
  else if (costh > 0.923 && costh <= 0.975)
  {
    if (xphi < 0.9)
    {
      parijt[0]=STAR2[costhbin][xphibin][0];
      parijt[1]=STAR2[costhbin][xphibin][1];
      parijt[2]=STAR2[costhbin][xphibin][2];
	      
      parimjt[0]=STAR2[costhbin-1][xphibin][0];
      parimjt[1]=STAR2[costhbin-1][xphibin][1];
      parimjt[2]=STAR2[costhbin-1][xphibin][2];
	      
      parijpt[0]=STAR2[costhbin][xphibin+1][0];
      parijpt[1]=STAR2[costhbin][xphibin+1][1];
      parijpt[2]=STAR2[costhbin][xphibin+1][2];
	      
      if (energy!=0.0)
      {
        sigma = sigmaVsE(parijt,energy)
        +      (sigmaVsE(parijt,energy) - sigmaVsE(parimjt,energy))/(cosGridmean[costhbin]-cosGridmean[costhbin-1])*(costh-cosGridmean[costhbin]) 
        +      (sigmaVsE(parijpt,energy) - sigmaVsE(parijt,energy))/(xphiGridmean[xphibin+1]-xphiGridmean[xphibin])*(xphi-xphiGridmean[xphibin]);
      }
    }
    else if (xphi >=0.9)
    { 
      parijt[0]=STAR2[costhbin][xphibin][0];
      parijt[1]=STAR2[costhbin][xphibin][1];
      parijt[2]=STAR2[costhbin][xphibin][2];
	      
      parimjt[0]=STAR2[costhbin-1][xphibin][0];
      parimjt[1]=STAR2[costhbin-1][xphibin][1];
      parimjt[2]=STAR2[costhbin-1][xphibin][2];
	      
      parijmt[0]=STAR2[costhbin][xphibin-1][0];
      parijmt[1]=STAR2[costhbin][xphibin-1][1];
      parijmt[2]=STAR2[costhbin][xphibin-1][2];

      if (energy!=0.0)
      {
        sigma = sigmaVsE(parijt,energy)
        +      (sigmaVsE(parijt,energy) - sigmaVsE(parimjt,energy))/(cosGridmean[costhbin]-cosGridmean[costhbin-1])*(costh-cosGridmean[costhbin])     
        +      (sigmaVsE(parijt,energy) - sigmaVsE(parijmt,energy))/(xphiGridmean[xphibin]-xphiGridmean[xphibin-1])*(xphi-xphiGridmean[xphibin]);
      }
    }
  }
  return sigma;
}

double AbsRecoCaloErrorMatrix::sigmaTheta( double energy, double costh, double xtheta ) const
{
  double sigma = -999.0;

  static double xthetaGridmean[10]={-0.9857,-0.8337,-0.5370,-0.2185,-0.06969,0.06991,0.2135,0.5338,0.8332,0.9882};
  static double cosGridmean[12]={-0.751,-0.624,-0.362,-0.103,0.102,0.361,0.611,0.750,0.842,0.908,0.935,0.954};

  static double STAR1[12][10][3];

//Theta Backward Region//////////////
/////////////////////////////////////
//cos(theta) bin: 0 xtheta bin: 0
  STAR1[0][0][0] = -0.00276855;
  STAR1[0][0][1] = 0.00443974;
  STAR1[0][0][2] = 0.178107;
//cos(theta) bin: 0 xtheta bin: 1
  STAR1[0][1][0] = -0.000109048;
  STAR1[0][1][1] = 0.00168479;
  STAR1[0][1][2] = 0.489369;
//cos(theta) bin: 0 xtheta bin: 2
  STAR1[0][2][0] = -0.000397;
  STAR1[0][2][1] = 0.00299388;
  STAR1[0][2][2] = 0.366084;
//cos(theta) bin: 0 xtheta bin: 3
  STAR1[0][3][0] = -0.00141967;
  STAR1[0][3][1] = 0.00517509;
  STAR1[0][3][2] = 0.279178;
//cos(theta) bin: 0 xtheta bin: 4
  STAR1[0][4][0] = -0.0018506;
  STAR1[0][4][1] = 0.00606399;
  STAR1[0][4][2] = 0.252889;
//cos(theta) bin: 0 xtheta bin: 5
  STAR1[0][5][0] = -0.00463714;
  STAR1[0][5][1] = 0.00910873;
  STAR1[0][5][2] = 0.171168;
//cos(theta) bin: 0 xtheta bin: 6
  STAR1[0][6][0] = -0.00384677;
  STAR1[0][6][1] = 0.0082165;
  STAR1[0][6][2] = 0.168048;
//cos(theta) bin: 0 xtheta bin: 7
  STAR1[0][7][0] = -0.000756158;
  STAR1[0][7][1] = 0.00412127;
  STAR1[0][7][2] = 0.313484;
//cos(theta) bin: 0 xtheta bin: 8
  STAR1[0][8][0] = -0.000271305;
  STAR1[0][8][1] = 0.00246943;
  STAR1[0][8][2] = 0.372531;
//cos(theta) bin: 0 xtheta bin: 9
  STAR1[0][9][0] = 0.000156433;
  STAR1[0][9][1] = 0.00157107;
  STAR1[0][9][2] = 0.515277;
/////////////////////////////////////////////////


//theta barrel///////////////////////////////////
/////////////////////////////////////////////////
//cos(theta) bin: 0 xtheta bin: 0
  STAR1[1][0][0] = -0.000748598;
  STAR1[1][0][1] = 0.00277876;
  STAR1[1][0][2] = 0.390008;
//cos(theta) bin: 0 xtheta bin: 1
  STAR1[1][1][0] = 0.000178602;
  STAR1[1][1][1] = 0.00171591;
  STAR1[1][1][2] = 0.58284;
//cos(theta) bin: 0 xtheta bin: 2
  STAR1[1][2][0] = -0.000487302;
  STAR1[1][2][1] = 0.00357461;
  STAR1[1][2][2] = 0.370931;
//cos(theta) bin: 0 xtheta bin: 3
  STAR1[1][3][0] = -0.00265583;
  STAR1[1][3][1] = 0.00703413;
  STAR1[1][3][2] = 0.234316;
//cos(theta) bin: 0 xtheta bin: 4
  STAR1[1][4][0] = -0.00346331;
  STAR1[1][4][1] = 0.00839643;
  STAR1[1][4][2] = 0.216798;
//cos(theta) bin: 0 xtheta bin: 5
  STAR1[1][5][0] = -0.0120134;
  STAR1[1][5][1] = 0.0172004;
  STAR1[1][5][2] = 0.0978599;
//cos(theta) bin: 0 xtheta bin: 6
  STAR1[1][6][0] = -0.00658962;
  STAR1[1][6][1] = 0.0115616;
  STAR1[1][6][2] = 0.141489;
//cos(theta) bin: 0 xtheta bin: 7
  STAR1[1][7][0] = -0.00155609;
  STAR1[1][7][1] = 0.00545831;
  STAR1[1][7][2] = 0.27815;
//cos(theta) bin: 0 xtheta bin: 8
  STAR1[1][8][0] = -0.00184127;
  STAR1[1][8][1] = 0.00440773;
  STAR1[1][8][2] = 0.213365;
//cos(theta) bin: 0 xtheta bin: 9
  STAR1[1][9][0] = 8.00809e-05;
  STAR1[1][9][1] = 0.00196288;
  STAR1[1][9][2] = 0.467999;
//cos(theta) bin: 1 xtheta bin: 0
  STAR1[2][0][0] = -0.000999074;
  STAR1[2][0][1] = 0.00346809;
  STAR1[2][0][2] = 0.36381;
//cos(theta) bin: 1 xtheta bin: 1
  STAR1[2][1][0] = -0.000213575;
  STAR1[2][1][1] = 0.00260126;
  STAR1[2][1][2] = 0.470634;
//cos(theta) bin: 1 xtheta bin: 2
  STAR1[2][2][0] = -0.000833809;
  STAR1[2][2][1] = 0.00455089;
  STAR1[2][2][2] = 0.35015;
//cos(theta) bin: 1 xtheta bin: 3
  STAR1[2][3][0] = -0.00353891;
  STAR1[2][3][1] = 0.00878889;
  STAR1[2][3][2] = 0.235515;
//cos(theta) bin: 1 xtheta bin: 4
  STAR1[2][4][0] = -0.00689879;
  STAR1[2][4][1] = 0.0127684;
  STAR1[2][4][2] = 0.163324;
//cos(theta) bin: 1 xtheta bin: 5
  STAR1[2][5][0] = -0.0097623;
  STAR1[2][5][1] = 0.0156848;
  STAR1[2][5][2] = 0.117179;
//cos(theta) bin: 1 xtheta bin: 6
  STAR1[2][6][0] = -0.00729836;
  STAR1[2][6][1] = 0.0131841;
  STAR1[2][6][2] = 0.149917;
//cos(theta) bin: 1 xtheta bin: 7
  STAR1[2][7][0] = -0.0018965;
  STAR1[2][7][1] = 0.00647815;
  STAR1[2][7][2] = 0.279908;
//cos(theta) bin: 1 xtheta bin: 8
  STAR1[2][8][0] = -0.00336279;
  STAR1[2][8][1] = 0.00648454;
  STAR1[2][8][2] = 0.175084;
//cos(theta) bin: 1 xtheta bin: 9
  STAR1[2][9][0] = -0.000205305;
  STAR1[2][9][1] = 0.00281345;
  STAR1[2][9][2] = 0.42448;
//cos(theta) bin: 2 xtheta bin: 0
  STAR1[3][0][0] = -0.00070593;
  STAR1[3][0][1] = 0.00329985;
  STAR1[3][0][2] = 0.384413;
//cos(theta) bin: 2 xtheta bin: 1
  STAR1[3][1][0] = -0.0003806;
  STAR1[3][1][1] = 0.0028726;
  STAR1[3][1][2] = 0.434218;
//cos(theta) bin: 2 xtheta bin: 2
  STAR1[3][2][0] = -0.000960209;
  STAR1[3][2][1] = 0.00494908;
  STAR1[3][2][2] = 0.348867;
//cos(theta) bin: 2 xtheta bin: 3
  STAR1[3][3][0] = -0.00586059;
  STAR1[3][3][1] = 0.0113579;
  STAR1[3][3][2] = 0.181426;
//cos(theta) bin: 2 xtheta bin: 4
  STAR1[3][4][0] = -0.00371471;
  STAR1[3][4][1] = 0.00991148;
  STAR1[3][4][2] = 0.231174;
//cos(theta) bin: 2 xtheta bin: 5
  STAR1[3][5][0] = -0.00775092;
  STAR1[3][5][1] = 0.0140391;
  STAR1[3][5][2] = 0.141326;
//cos(theta) bin: 2 xtheta bin: 6
  STAR1[3][6][0] = -0.00375826;
  STAR1[3][6][1] = 0.00989237;
  STAR1[3][6][2] = 0.215907;
//cos(theta) bin: 2 xtheta bin: 7
  STAR1[3][7][0] = -0.00213594;
  STAR1[3][7][1] = 0.00696476;
  STAR1[3][7][2] = 0.282357;
//cos(theta) bin: 2 xtheta bin: 8
  STAR1[3][8][0] = -0.0056015;
  STAR1[3][8][1] = 0.00889709;
  STAR1[3][8][2] = 0.13564;
//cos(theta) bin: 2 xtheta bin: 9
  STAR1[3][9][0] = -0.000202955;
  STAR1[3][9][1] = 0.00298617;
  STAR1[3][9][2] = 0.411127;
//cos(theta) bin: 3 xtheta bin: 0
  STAR1[4][0][0] = -0.000707892;
  STAR1[4][0][1] = 0.00346943;
  STAR1[4][0][2] = 0.323225;
//cos(theta) bin: 3 xtheta bin: 1
  STAR1[4][1][0] = -0.00348928;
  STAR1[4][1][1] = 0.00678966;
  STAR1[4][1][2] = 0.183834;
//cos(theta) bin: 3 xtheta bin: 2
  STAR1[4][2][0] = -0.00194184;
  STAR1[4][2][1] = 0.00676693;
  STAR1[4][2][2] = 0.289129;
//cos(theta) bin: 3 xtheta bin: 3
  STAR1[4][3][0] = -0.00372079;
  STAR1[4][3][1] = 0.00989275;
  STAR1[4][3][2] = 0.223737;
//cos(theta) bin: 3 xtheta bin: 4
  STAR1[4][4][0] = -0.00988975;
  STAR1[4][4][1] = 0.016131;
  STAR1[4][4][2] = 0.118597;
//cos(theta) bin: 3 xtheta bin: 5
  STAR1[4][5][0] = -0.00595664;
  STAR1[4][5][1] = 0.0121435;
  STAR1[4][5][2] = 0.180121;
//cos(theta) bin: 3 xtheta bin: 6
  STAR1[4][6][0] = -0.00290853;
  STAR1[4][6][1] = 0.0084029;
  STAR1[4][6][2] = 0.254079;
//cos(theta) bin: 3 xtheta bin: 7
  STAR1[4][7][0] = -0.000852397;
  STAR1[4][7][1] = 0.00479887;
  STAR1[4][7][2] = 0.357311;
//cos(theta) bin: 3 xtheta bin: 8
  STAR1[4][8][0] = -0.000837044;
  STAR1[4][8][1] = 0.00340337;
  STAR1[4][8][2] = 0.356338;
//cos(theta) bin: 3 xtheta bin: 9
  STAR1[4][9][0] = -0.00163182;
  STAR1[4][9][1] = 0.00418094;
  STAR1[4][9][2] = 0.284591;
//cos(theta) bin: 4 xtheta bin: 0
  STAR1[5][0][0] = -2.7859e-05;
  STAR1[5][0][1] = 0.00256961;
  STAR1[5][0][2] = 0.421948;
//cos(theta) bin: 4 xtheta bin: 1
  STAR1[5][1][0] = -0.00283549;
  STAR1[5][1][1] = 0.00596878;
  STAR1[5][1][2] = 0.190111;
//cos(theta) bin: 4 xtheta bin: 2
  STAR1[5][2][0] = -0.00180119;
  STAR1[5][2][1] = 0.00640441;
  STAR1[5][2][2] = 0.286313;
//cos(theta) bin: 4 xtheta bin: 3
  STAR1[5][3][0] = -0.0031812;
  STAR1[5][3][1] = 0.00896512;
  STAR1[5][3][2] = 0.222934;
//cos(theta) bin: 4 xtheta bin: 4
  STAR1[5][4][0] = -0.00728458;
  STAR1[5][4][1] = 0.0131427;
  STAR1[5][4][2] = 0.140162;
//cos(theta) bin: 4 xtheta bin: 5
  STAR1[5][5][0] = -0.0053898;
  STAR1[5][5][1] = 0.0112202;
  STAR1[5][5][2] = 0.181898;
//cos(theta) bin: 4 xtheta bin: 6
  STAR1[5][6][0] = -0.0043077;
  STAR1[5][6][1] = 0.00963235;
  STAR1[5][6][2] = 0.216086;
//cos(theta) bin: 4 xtheta bin: 7
  STAR1[5][7][0] = -0.000641677;
  STAR1[5][7][1] = 0.00435285;
  STAR1[5][7][2] = 0.37084;
//cos(theta) bin: 4 xtheta bin: 8
  STAR1[5][8][0] = -0.000121267;
  STAR1[5][8][1] = 0.00246863;
  STAR1[5][8][2] = 0.480378;
//cos(theta) bin: 4 xtheta bin: 9
  STAR1[5][9][0] = -0.000851782;
  STAR1[5][9][1] = 0.00324658;
  STAR1[5][9][2] = 0.368425;
//cos(theta) bin: 5 xtheta bin: 0
  STAR1[6][0][0] = 0.00015778;
  STAR1[6][0][1] = 0.00196866;
  STAR1[6][0][2] = 0.493088;
//cos(theta) bin: 5 xtheta bin: 1
  STAR1[6][1][0] = 9.34158e-05;
  STAR1[6][1][1] = 0.00242725;
  STAR1[6][1][2] = 0.407713;
//cos(theta) bin: 5 xtheta bin: 2
  STAR1[6][2][0] = -0.00115629;
  STAR1[6][2][1] = 0.0051351;
  STAR1[6][2][2] = 0.300826;
//cos(theta) bin: 5 xtheta bin: 3
  STAR1[6][3][0] = -0.00519583;
  STAR1[6][3][1] = 0.0102371;
  STAR1[6][3][2] = 0.164501;
//cos(theta) bin: 5 xtheta bin: 4
  STAR1[6][4][0] = -0.0121703;
  STAR1[6][4][1] = 0.017314;
  STAR1[6][4][2] = 0.0938925;
//cos(theta) bin: 5 xtheta bin: 5
  STAR1[6][5][0] = -0.00634359;
  STAR1[6][5][1] = 0.0113791;
  STAR1[6][5][2] = 0.153955;
//cos(theta) bin: 5 xtheta bin: 6
  STAR1[6][6][0] = -0.00244778;
  STAR1[6][6][1] = 0.0069687;
  STAR1[6][6][2] = 0.25218;
//cos(theta) bin: 5 xtheta bin: 7
  STAR1[6][7][0] = -0.000633853;
  STAR1[6][7][1] = 0.00378017;
  STAR1[6][7][2] = 0.358053;
//cos(theta) bin: 5 xtheta bin: 8
  STAR1[6][8][0] = 3.53969e-05;
  STAR1[6][8][1] = 0.00196776;
  STAR1[6][8][2] = 0.555179;
//cos(theta) bin: 5 xtheta bin: 9
  STAR1[6][9][0] = -0.000686107;
  STAR1[6][9][1] = 0.00270239;
  STAR1[6][9][2] = 0.382191;
////////////////////////////////////////////////

//Theta forward////////////////////////////////
////////////////////////////////////////////////
//cos(theta) bin: 0 xtheta bin: 0
  STAR1[7][0][0] = 0.000253318;
  STAR1[7][0][1] = 0.00152019;
  STAR1[7][0][2] = 0.560074;
//cos(theta) bin: 0 xtheta bin: 1
  STAR1[7][1][0] = -0.000213981;
  STAR1[7][1][1] = 0.00240282;
  STAR1[7][1][2] = 0.362039;
//cos(theta) bin: 0 xtheta bin: 2
  STAR1[7][2][0] = -0.000882443;
  STAR1[7][2][1] = 0.00431655;
  STAR1[7][2][2] = 0.310321;
//cos(theta) bin: 0 xtheta bin: 3
  STAR1[7][3][0] = -0.00875262;
  STAR1[7][3][1] = 0.0131026;
  STAR1[7][3][2] = 0.106488;
//cos(theta) bin: 0 xtheta bin: 4
  STAR1[7][4][0] = -0.0137428;
  STAR1[7][4][1] = 0.0183339;
  STAR1[7][4][2] = 0.0828611;
//cos(theta) bin: 0 xtheta bin: 5
  STAR1[7][5][0] = -0.00227192;
  STAR1[7][5][1] = 0.0065355;
  STAR1[7][5][2] = 0.229739;
//cos(theta) bin: 0 xtheta bin: 6
  STAR1[7][6][0] = -0.00233822;
  STAR1[7][6][1] = 0.00614176;
  STAR1[7][6][2] = 0.232601;
//cos(theta) bin: 0 xtheta bin: 7
  STAR1[7][7][0] = -0.000258464;
  STAR1[7][7][1] = 0.00291832;
  STAR1[7][7][2] = 0.3798;
//cos(theta) bin: 0 xtheta bin: 8
  STAR1[7][8][0] = -0.000232729;
  STAR1[7][8][1] = 0.00200005;
  STAR1[7][8][2] = 0.443938;
//cos(theta) bin: 0 xtheta bin: 9
  STAR1[7][9][0] = -1.64164e-05;
  STAR1[7][9][1] = 0.00175295;
  STAR1[7][9][2] = 0.474741;
//cos(theta) bin: 1 xtheta bin: 0
  STAR1[8][0][0] = 0.000304091;
  STAR1[8][0][1] = 0.00121931;
  STAR1[8][0][2] = 0.588426;
//cos(theta) bin: 1 xtheta bin: 1
  STAR1[8][1][0] = 0.000311142;
  STAR1[8][1][1] = 0.00151249;
  STAR1[8][1][2] = 0.476439;
//cos(theta) bin: 1 xtheta bin: 2
  STAR1[8][2][0] = -0.000314702;
  STAR1[8][2][1] = 0.0032066;
  STAR1[8][2][2] = 0.338972;
//cos(theta) bin: 1 xtheta bin: 3
  STAR1[8][3][0] = -0.00567966;
  STAR1[8][3][1] = 0.00935754;
  STAR1[8][3][2] = 0.118665;
//cos(theta) bin: 1 xtheta bin: 4
  STAR1[8][4][0] = -0.00319952;
  STAR1[8][4][1] = 0.00695082;
  STAR1[8][4][2] = 0.176957;
//cos(theta) bin: 1 xtheta bin: 5
  STAR1[8][5][0] = -0.00475187;
  STAR1[8][5][1] = 0.00844697;
  STAR1[8][5][2] = 0.145564;
//cos(theta) bin: 1 xtheta bin: 6
  STAR1[8][6][0] = -0.0014855;
  STAR1[8][6][1] = 0.00471829;
  STAR1[8][6][2] = 0.247988;
//cos(theta) bin: 1 xtheta bin: 7
  STAR1[8][7][0] = 0.000265738;
  STAR1[8][7][1] = 0.00201044;
  STAR1[8][7][2] = 0.436151;
//cos(theta) bin: 1 xtheta bin: 8
  STAR1[8][8][0] = 0.000661898;
  STAR1[8][8][1] = 0.000729845;
  STAR1[8][8][2] = 0.782661;
//cos(theta) bin: 1 xtheta bin: 9
  STAR1[8][9][0] = -0.000373297;
  STAR1[8][9][1] = 0.00210272;
  STAR1[8][9][2] = 0.376699;
//////////////////////////////////////////////


//Theta endcap
//////////////////////////////////////////////
//cos(theta) bin: 0 xtheta bin: 0
  STAR1[9][0][0] = 0.000121494;
  STAR1[9][0][1] = 0.00192888;
  STAR1[9][0][2] = 0.375791;
//cos(theta) bin: 0 xtheta bin: 1
  STAR1[9][1][0] = 0.000251508;
  STAR1[9][1][1] = 0.00221591;
  STAR1[9][1][2] = 0.39253;
//cos(theta) bin: 0 xtheta bin: 2
  STAR1[9][2][0] = 0.000199352;
  STAR1[9][2][1] = 0.00276573;
  STAR1[9][2][2] = 0.414717;
//cos(theta) bin: 0 xtheta bin: 3
  STAR1[9][3][0] = 1.29023e-05;
  STAR1[9][3][1] = 0.00316059;
  STAR1[9][3][2] = 0.336546;
//cos(theta) bin: 0 xtheta bin: 4
  STAR1[9][4][0] = -0.000189537;
  STAR1[9][4][1] = 0.00327777;
  STAR1[9][4][2] = 0.338492;
//cos(theta) bin: 0 xtheta bin: 5
  STAR1[9][5][0] = -0.000718007;
  STAR1[9][5][1] = 0.00367936;
  STAR1[9][5][2] = 0.285455;
//cos(theta) bin: 0 xtheta bin: 6
  STAR1[9][6][0] = 0.000119939;
  STAR1[9][6][1] = 0.00252683;
  STAR1[9][6][2] = 0.405586;
//cos(theta) bin: 0 xtheta bin: 7
  STAR1[9][7][0] = 0.000659369;
  STAR1[9][7][1] = 0.00135187;
  STAR1[9][7][2] = 0.592356;
//cos(theta) bin: 0 xtheta bin: 8
  STAR1[9][8][0] = 0.000457174;
  STAR1[9][8][1] = 0.00109279;
  STAR1[9][8][2] = 0.615712;
//cos(theta) bin: 0 xtheta bin: 9
  STAR1[9][9][0] = 0.000650743;
  STAR1[9][9][1] = 0.00095036;
  STAR1[9][9][2] = 0.652594;
//cos(theta) bin: 1 xtheta bin: 0
  STAR1[10][0][0] = 0.000562567;
  STAR1[10][0][1] = 0.001332;
  STAR1[10][0][2] = 0.715787;
//cos(theta) bin: 1 xtheta bin: 1
  STAR1[10][1][0] = 0.000474547;
  STAR1[10][1][1] = 0.00172085;
  STAR1[10][1][2] = 0.609533;
//cos(theta) bin: 1 xtheta bin: 2
  STAR1[10][2][0] = 7.82881e-05;
  STAR1[10][2][1] = 0.00275937;
  STAR1[10][2][2] = 0.445574;
//cos(theta) bin: 1 xtheta bin: 3
  STAR1[10][3][0] = -0.000368547;
  STAR1[10][3][1] = 0.00351031;
  STAR1[10][3][2] = 0.359053;
//cos(theta) bin: 1 xtheta bin: 4
  STAR1[10][4][0] = -0.000719713;
  STAR1[10][4][1] = 0.00394881;
  STAR1[10][4][2] = 0.339149;
//cos(theta) bin: 1 xtheta bin: 5
  STAR1[10][5][0] = -0.000818592;
  STAR1[10][5][1] = 0.00386879;
  STAR1[10][5][2] = 0.351838;
//cos(theta) bin: 1 xtheta bin: 6
  STAR1[10][6][0] = -0.000137078;
  STAR1[10][6][1] = 0.00284118;
  STAR1[10][6][2] = 0.442941;
//cos(theta) bin: 1 xtheta bin: 7
  STAR1[10][7][0] = 0.000387985;
  STAR1[10][7][1] = 0.00167188;
  STAR1[10][7][2] = 0.683338;
//cos(theta) bin: 1 xtheta bin: 8
  STAR1[10][8][0] = 0.000163667;
  STAR1[10][8][1] = 0.00173857;
  STAR1[10][8][2] = 0.742128;
//cos(theta) bin: 1 xtheta bin: 9
  STAR1[10][9][0] = 0.00039623;
  STAR1[10][9][1] = 0.00129768;
  STAR1[10][9][2] = 0.813506;
//cos(theta) bin: 2 xtheta bin: 0
  STAR1[11][0][0] = -0.00160942;
  STAR1[11][0][1] = 0.00455251;
  STAR1[11][0][2] = 0.370311;
//cos(theta) bin: 2 xtheta bin: 1
  STAR1[11][1][0] = -0.00183615;
  STAR1[11][1][1] = 0.0052243;
  STAR1[11][1][2] = 0.327457;
//cos(theta) bin: 2 xtheta bin: 2
  STAR1[11][2][0] = -0.0113643;
  STAR1[11][2][1] = 0.0155191;
  STAR1[11][2][2] = 0.117467;
//cos(theta) bin: 2 xtheta bin: 3
  STAR1[11][3][0] = -0.00162222;
  STAR1[11][3][1] = 0.00548193;
  STAR1[11][3][2] = 0.276867;
//cos(theta) bin: 2 xtheta bin: 4
  STAR1[11][4][0] = -0.0064386;
  STAR1[11][4][1] = 0.0102783;
  STAR1[11][4][2] = 0.174508;
//cos(theta) bin: 2 xtheta bin: 5
  STAR1[11][5][0] = -0.00148774;
  STAR1[11][5][1] = 0.0048444;
  STAR1[11][5][2] = 0.349206;
//cos(theta) bin: 2 xtheta bin: 6
  STAR1[11][6][0] = -0.000988449;
  STAR1[11][6][1] = 0.00418842;
  STAR1[11][6][2] = 0.391287;
//cos(theta) bin: 2 xtheta bin: 7
  STAR1[11][7][0] = -0.00245574;
  STAR1[11][7][1] = 0.00564538;
  STAR1[11][7][2] = 0.33706;
//cos(theta) bin: 2 xtheta bin: 8
  STAR1[11][8][0] = -0.00860121;
  STAR1[11][8][1] = 0.0121583;
  STAR1[11][8][2] = 0.145668;
//cos(theta) bin: 2 xtheta bin: 9
  STAR1[11][9][0] = -0.262157;
  STAR1[11][9][1] = 0.266233;
  STAR1[11][9][2] = 0.00789633;
//////////////////////////////////////////////


  //THETA
  double parijt[3];
  double paripjt[3];
  double parijpt[3];
  double parimmjt[3];
  double parimjmt[3];
  double parimjt[3];
  double parijmt[3];
  double parimjpt[3];

  int costhbin=0, xthetabin=0;
    
  // cos(theta) bins  
  if      (costh < -0.731) { costhbin = 0;}
  else if (costh < -0.517) { costhbin = 1;}
  else if (costh < -0.206) { costhbin = 2;}
  else if (costh <  0.000) { costhbin = 3;}
  else if (costh <  0.206) { costhbin = 4;}
  else if (costh <  0.517) { costhbin = 5;}
  else if (costh <  0.707) { costhbin = 6;}
  else if (costh <  0.795) { costhbin = 7;}
  else if (costh <  0.892) { costhbin = 8;}
  else if (costh <  0.923) { costhbin = 9;}
  else if (costh <  0.948) { costhbin = 10;}
  else if (costh <  0.975) { costhbin = 11;}

  // xtheta bins
  if      (xtheta<-0.9)   { xthetabin=0;}
  else if (xtheta<-0.77)  { xthetabin=1;}
  else if (xtheta<-0.325) { xthetabin=2;}
  else if (xtheta<-0.125) { xthetabin=3;}
  else if (xtheta<-0.)    { xthetabin=4;}
  else if (xtheta<0.125)  { xthetabin=5;}
  else if (xtheta<0.325)  { xthetabin=6;}
  else if (xtheta<0.77)   { xthetabin=7;}
  else if (xtheta<0.9)    { xthetabin=8;}
  else if (xtheta>=0.9)   { xthetabin=9;}

  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  //INTERPOLATION THETA
  if (costh <= 0.923 && costh > -0.775)
  {
    if (xtheta < 0.9)
    {
      parijt[0]=STAR1[costhbin][xthetabin][0];
      parijt[1]=STAR1[costhbin][xthetabin][1];
      parijt[2]=STAR1[costhbin][xthetabin][2];
	      
      paripjt[0]=STAR1[costhbin+1][xthetabin][0];
      paripjt[1]=STAR1[costhbin+1][xthetabin][1];
      paripjt[2]=STAR1[costhbin+1][xthetabin][2];
	      
      parijpt[0]=STAR1[costhbin][xthetabin+1][0];
      parijpt[1]=STAR1[costhbin][xthetabin+1][1];
      parijpt[2]=STAR1[costhbin][xthetabin+1][2];
	      
      if (energy!=0.0)
      {
        sigma = sigmaVsE(parijt,energy)
        +      (sigmaVsE(paripjt,energy) - sigmaVsE(parijt,energy))/(cosGridmean[costhbin+1]-cosGridmean[costhbin])*(costh-cosGridmean[costhbin])
        +      (sigmaVsE(parijpt,energy) - sigmaVsE(parijt,energy))/(xthetaGridmean[xthetabin+1]-xthetaGridmean[xthetabin])*(xtheta-xthetaGridmean[xthetabin]);
      }
    }
    else if (xtheta >=0.9)
    { 
      parijt[0]=STAR1[costhbin][xthetabin][0];
      parijt[1]=STAR1[costhbin][xthetabin][1];
      parijt[2]=STAR1[costhbin][xthetabin][2];
	      
      paripjt[0]=STAR1[costhbin+1][xthetabin][0];
      paripjt[1]=STAR1[costhbin+1][xthetabin][1];
      paripjt[2]=STAR1[costhbin+1][xthetabin][2];
	      
      parijmt[0]=STAR1[costhbin][xthetabin-1][0];
      parijmt[1]=STAR1[costhbin][xthetabin-1][1];
      parijmt[2]=STAR1[costhbin][xthetabin-1][2];
	      
      if (energy!=0.0)
      {
        sigma = sigmaVsE(parijt,energy)
        +      (sigmaVsE(paripjt,energy) - sigmaVsE(parijt,energy))/(cosGridmean[costhbin+1]-cosGridmean[costhbin])*(costh-cosGridmean[costhbin])
        +      (sigmaVsE(parijt,energy) - sigmaVsE(parijmt,energy))/(xthetaGridmean[xthetabin]-xthetaGridmean[xthetabin-1])*(xtheta-xthetaGridmean[xthetabin]);
      }
    }
  }
  else if (costh>0.923 && costh<=0.948)
  {
    if (xtheta < 0.9)
    {
      parijt[0]=STAR1[costhbin][xthetabin][0];
      parijt[1]=STAR1[costhbin][xthetabin][1];
      parijt[2]=STAR1[costhbin][xthetabin][2];
	      
      parimjt[0]=STAR1[costhbin-1][xthetabin][0];
      parimjt[1]=STAR1[costhbin-1][xthetabin][1];
      parimjt[2]=STAR1[costhbin-1][xthetabin][2];
	      
      parijpt[0]=STAR1[costhbin][xthetabin+1][0];
      parijpt[1]=STAR1[costhbin][xthetabin+1][1];
      parijpt[2]=STAR1[costhbin][xthetabin+1][2];
	      
      if (energy!=0.0)
      {
        sigma = sigmaVsE(parijt,energy)
        +      (sigmaVsE(parijt,energy) - sigmaVsE(parimjt,energy))/(cosGridmean[costhbin]-cosGridmean[costhbin-1])*(costh-cosGridmean[costhbin]) 
        +      (sigmaVsE(parijpt,energy) - sigmaVsE(parijt,energy))/(xthetaGridmean[xthetabin+1]-xthetaGridmean[xthetabin])*(xtheta-xthetaGridmean[xthetabin]);
      }
    }
    else if (xtheta >=0.9)
    { 
      parijt[0]=STAR1[costhbin][xthetabin][0];
      parijt[1]=STAR1[costhbin][xthetabin][1];
      parijt[2]=STAR1[costhbin][xthetabin][2];
	      
      parimjt[0]=STAR1[costhbin-1][xthetabin][0];
      parimjt[1]=STAR1[costhbin-1][xthetabin][1];
      parimjt[2]=STAR1[costhbin-1][xthetabin][2];
	      
      parijmt[0]=STAR1[costhbin][xthetabin-1][0];
      parijmt[1]=STAR1[costhbin][xthetabin-1][1];
      parijmt[2]=STAR1[costhbin][xthetabin-1][2];

      if (energy!=0.0)
      {
        sigma = sigmaVsE(parijt,energy)
        +      (sigmaVsE(parijt,energy) - sigmaVsE(parimjt,energy))/(cosGridmean[costhbin]-cosGridmean[costhbin-1])*(costh-cosGridmean[costhbin])     
        +      (sigmaVsE(parijt,energy) - sigmaVsE(parijmt,energy))/(xthetaGridmean[xthetabin]-xthetaGridmean[xthetabin-1])*(xtheta-xthetaGridmean[xthetabin]);
      }
    }
  }
  else if (costh > 0.948 && costh <= 0.975)
  {
    if (xtheta < 0.9)
    {
      parimmjt[0]=STAR1[costhbin-2][xthetabin][0];
      parimmjt[1]=STAR1[costhbin-2][xthetabin][1];
      parimmjt[2]=STAR1[costhbin-2][xthetabin][2];
	      
      parimjt[0]=STAR1[costhbin-1][xthetabin][0];
      parimjt[1]=STAR1[costhbin-1][xthetabin][1];
      parimjt[2]=STAR1[costhbin-1][xthetabin][2];
	      
      parimjpt[0]=STAR1[costhbin-1][xthetabin+1][0];
      parimjpt[1]=STAR1[costhbin-1][xthetabin+1][1];
      parimjpt[2]=STAR1[costhbin-1][xthetabin+1][2];
	      
      if (energy!=0.0)
      {
	sigma = sigmaVsE(parimjt,energy)
        +      (sigmaVsE(parimjt,energy) - sigmaVsE(parimmjt,energy))/(cosGridmean[costhbin-1]-cosGridmean[costhbin-2])*(costh-cosGridmean[costhbin-1])
        +      (sigmaVsE(parimjpt,energy) - sigmaVsE(parimjt,energy))/(xthetaGridmean[xthetabin+1]-xthetaGridmean[xthetabin])*(xtheta-xthetaGridmean[xthetabin]);
      }
    }
    else if (xtheta >=0.9)
    { 
      parimmjt[0]=STAR1[costhbin-2][xthetabin][0];
      parimmjt[1]=STAR1[costhbin-2][xthetabin][1];
      parimmjt[2]=STAR1[costhbin-2][xthetabin][2];
	      
      parimjt[0]=STAR1[costhbin-1][xthetabin][0];
      parimjt[1]=STAR1[costhbin-1][xthetabin][1];
      parimjt[2]=STAR1[costhbin-1][xthetabin][2];
	      
      parimjmt[0]=STAR1[costhbin-1][xthetabin-1][0];
      parimjmt[1]=STAR1[costhbin-1][xthetabin-1][1];
      parimjmt[2]=STAR1[costhbin-1][xthetabin-1][2];

      if (energy!=0.0)
      {
        sigma = sigmaVsE(parimjt,energy)
        + (sigmaVsE(parimjt,energy) - sigmaVsE(parimmjt,energy))/(cosGridmean[costhbin-1]-cosGridmean[costhbin-2])*(costh-cosGridmean[costhbin-1])
        + (sigmaVsE(parimjt,energy) - sigmaVsE(parimjmt,energy))/(xthetaGridmean[xthetabin]-xthetaGridmean[xthetabin-1])*(xtheta-xthetaGridmean[xthetabin]);
      }
    }
  }
  return sigma;
}

