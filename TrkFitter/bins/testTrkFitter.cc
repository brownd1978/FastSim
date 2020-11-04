//  This is a crude test-bench for testing TrkBase and TrkFitter classes and
//    functions.  It is not pretty.
//
//  some more extensive tests added.. 09/26/97
//
//
#include "BaBar/BaBar.hh"

#include "TrkBase/TrkExchangePar.hh"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Geometry/HepPoint.h"
#include "TrkBase/TrkHelixUtils.hh"
#include "BField/BFieldFixed.hh"
#include "TrkBase/TrkPoca.hh"
#include "TrkBase/TrkDifPoca.hh"
#include "TrkBase/TrkPocaBase.hh"
#include "TrkBase/TrkDifPieceTraj.hh"
#include "TrkBase/TrkRep.hh"
#include "TrkBase/TrkRecoTrk.hh"
#include "TrkFitter/TrkIdManDefault.hh"
#include "TrajGeom/TrkPieceLineTraj.hh"
#include "TrajGeom/TrkLineTraj.hh"
#include "TrkBase/NeutTraj.hh"
#include "TrkBase/HelixTraj.hh"
#include "TrkFitter/TrkCircleTraj.hh"
#include "TrkBase/TrkMomCalculator.hh"
#include "TrkFitter/TrkDifLineTraj.hh"
#include "TrajGeom/TrkTransformVisitor.hh"
#include "TrkFitter/TrkMiniMaker.hh"
#include "TrkFitter/TrkHelixRep.hh"
#include "TrkFitter/TrkContextCustom.hh"
#include "CLHEP/Geometry/Transformation.h"
#include "CLHEP/Geometry/Translation.h"
#include "CLHEP/Vector/Rotation.h"
#include "BbrGeom/BbrVectorErr.hh"
#include <iostream>
#include "TrajGeom/BbrHelixTraj.hh"
using std::cin;
using std::cout;
using std::endl;

#define PI 3.141592635

double
routine1(TrkLineTraj t1, double a1, double a2, char axe, int direction)
{
  int n;
  double x, y, z, angle = 0.;
  if (axe == 'x') {
    angle = a1;
    a1 -= .01 * direction;
  }
  if (axe == 'y') {
    angle = a2;
    a2 -= .01 * direction;
  }
  HepPoint pos2(-1.0, 1.0, 1.0);
  int c = 0;
  do {
    c++;
    if (axe == 'x') a1 += .001 * direction;
    if (axe == 'y') a2 += .001 * direction;
    x = cos(a2) * cos(a1);
    y = sin(a2);
    z = cos(a2) * sin(a1);
    Hep3Vector dir2(x, y, z);
    TrkLineTraj testTraj2(pos2, dir2, 10.);
    TrkPoca poca1(t1, 0., testTraj2, 10.);
    n = poca1.status().success();
  } while (n != 3 && c < 12);
  do {
    //   cout << a1 << "   " << a2 << endl;
    if (axe == 'x') a1 -= .00001 * direction;
    if (axe == 'y') a2 -= .00001 * direction;
    x = cos(a2) * cos(a1);
    y = sin(a2);
    z = cos(a2) * sin(a1);
    Hep3Vector dir2(x, y, z);
    TrkLineTraj testTraj2(pos2, dir2, 10.);
    TrkPoca poca1(t1, 0., testTraj2, 10.);
    n = poca1.status().success();
  } while (n == 3);
  if (axe == 'x') return a1 - angle;
  return a2 - angle;
}

// prints out Traj's position and direction with the step 1.0 along the flight path
void
lookAtTraj(Trajectory& tr, int begin_at, int end_at)
{
  int i;
  HepPoint curpos;
  Hep3Vector curdir;
  if (begin_at >= end_at) {
    cout << "Can't do backward ! :(" << endl;
    return;
  }
  for (i = begin_at; i <= end_at; i++) {
    tr.getInfo((double) i, curpos, curdir);
    cout << "flt=" << i << " pos=" << curpos << " dir=" << curdir << endl;
  }
  cout << endl;
}

void
routine7()
{
  double d0 = 5.0, phi0 = PI / 4, z0 = 0, tanDip = 0;
  cout << PI / 4 << " " << M_PI / 4 << "  " << PI / 4 - M_PI / 4 << endl;

  // I decided to create them from the 'bottom line'
  // int i below for the test loop - not shown here
  int i = 0;
  TrkExchangePar par1(d0, phi0, 1.0, z0, tanDip);
  TrkExchangePar par2(2 * d0, phi0, 1.0, z0, tanDip);
  TrkDifLineTraj traj1(par1, 0.0, 10.0);
  TrkDifLineTraj traj2(par2, (double) i, 15.0);

  //lookAtTraj(traj2,0,15);
  //lookAtTraj(traj1,0,15);

  TrkDifPoca poca(traj1, 0., traj2, 0.);

  HepVector der = poca.derivs();
  cout << poca.doca() << " derivs: " << der[0] << " " << der[1] << " "
      << der[2] << " " << der[3] << " " << endl;
  cout << "f1: " << poca.flt1() << "  f2: " << poca.flt2() << endl;
}

int
main()
{
  int n;
  double x, y, z;
  double angle1, angle2;
  double register tmp;

  cout << "What test would you like to perform ?" << endl;
  cout << "1- minimum detectable angle between linear Trk's" << endl;
  cout << "2- precision test with rotating linear Trk's" << endl;
  cout << "3- test Poka.doca on piecewise Traj's" << endl;
  cout << "4- TrkDifPoca precision test with rotating TrkDifLineTraj" << endl;
  cout << "5- test DifPoka.doca on piecewise DifTraj & LinTraj" << endl;
  cout << "6,7- test DifPoka.derivs on parallel Traj's" << endl;
  cout << "8- TrkCompTrk, TrkDifPoca, NeutTraj, ..." << endl;
  cout << "9- test transformation Visitor " << endl;
  cout << "10- test momentum visitor " << endl;
  cout << "11- Old test" << endl;
  cout << "12- BbrHelixTraj" << endl << endl;
  cin >> n;

  switch (n) {
  case 1: {
    double xmaxdiff = 0., ymaxdiff = 0., ymaxangle = 0., xmaxangle = 0.;
    HepPoint pos1(1.0, 1.0, 1.0);

    // next commented lines are for future upgrade
    cout << "Please wait..." << endl;
    // both Traj's gradually rotating in the octant.
    // it's the reason for long execution, sorry... 
    for (angle1 = 0.; angle1 <= 1.56; angle1 += .02)
      for (angle2 = 0.; angle2 <= 1.56; angle2 += .02) {
        x = cos(angle2) * cos(angle1);
        y = sin(angle2);
        z = cos(angle2) * sin(angle1);
        Hep3Vector dir1(x, y, z);
        TrkLineTraj testTraj1(pos1, dir1, 10.);
        tmp = routine1(testTraj1, angle1, angle2, 'x', -1) * cos(angle2);
        if (tmp > xmaxdiff) {
          xmaxdiff = tmp;
          xmaxangle = angle1;
          ymaxangle = angle2;
        }
        tmp = routine1(testTraj1, angle1, angle2, 'y', -1);
        if (tmp > ymaxdiff) {
          ymaxdiff = tmp;
          xmaxangle = angle1;
          ymaxangle = angle2;
        }
        //cout << xmaxdiff << endl;
      }
    cout << "Maximinimal angle difference along X : " << xmaxdiff
        << " ,at angles : " << xmaxangle << " , " << ymaxangle << endl;
    cout << "Maximinimal angle difference along Y : " << ymaxdiff
        << " ,at angles : " << xmaxangle << " , " << ymaxangle << endl;
    break;
  }

  case 2: {// precision doca()
    double dist = 2.5, maxdiff = 0, maxdiffangle = 0.;
    Hep3Vector dir2, dir1;
    HepPoint pos2;
    angle1 = 0.;//pi/8;
    Hep3Vector e1(cos(angle1), -1 * sin(angle1), 0.);
    Hep3Vector e2(sin(angle1), cos(angle1), 0.);
    Hep3Vector e3(0., 0., 1.);
    HepPoint pos1(0., 0., 0.);
    pos2 = pos1 + dist * e2;
    dir1 = e1;
    TrkLineTraj testTraj1(pos1, dir1, 10.);
    //  testTraj2 is gradually rotating, angle is in the most
    //  hard range near zero.
    for (angle2 = 0.; angle2 < .01; angle2 += .001) {
      dir2 = cos(angle2) * e1 + sin(angle2) * e3;
      cout << dir1 << dir2;
      TrkLineTraj testTraj2(pos2, dir2, 10.);
      TrkPoca poca(testTraj1, 0., testTraj2, 10.);
      tmp = fabs(poca.doca());
      cout << tmp << endl;
      tmp -= dist;
      if (fabs(tmp) > maxdiff) {
        maxdiff = fabs(tmp);
        maxdiffangle = angle2;
      }
    }
    cout << "Maximal inaccuracy in doca determination is " << maxdiff
        << " at angle " << maxdiffangle << endl;
    dir2 = 2 * dir1;
    TrkLineTraj testTraj2(pos2, dir2, 10.);
    cout << "It is one of hard cases :" << endl;
    cout << "Tr1 : " << pos1 << dir1 << endl;
    cout << "Tr2 : " << pos2 << dir2 << endl;
    TrkPoca poca(testTraj1, 5., testTraj2, 5.);
    cout << "Real distance is : " << dist << " while doca()=" << poca.doca()
        << endl;
    break;
  }

  case 3: { // Piecewise Traj && (Linear Traj || HepPoint).
    cout << "Piecewise Traj & LineTraj." << endl;
    Hep3Vector e1(1., 0., 0.);
    Hep3Vector e2(0., 1., 0.);
    Hep3Vector e3(0., 0., 1.);
    Hep3Vector dir1, curdir;
    HepPoint curpos, pos1, pos22, pos21, pos23, pos0(0., 0., 0.), point;
    int i, n_steps = 10;
    double len = 20., dist = 5 * sqrt(2.), pathoffset, piecepath;
    pos1 = pos0 + (len / 2) * (e1 - e2).unit();
    dir1 = (e2 - e1).unit();
    TrkLineTraj testTraj1(pos1, dir1, len);
    pos22 = pos0 + dist * (e1 + e2).unit();
    pos21 = pos22 + len * (e2).unit();
    pos23 = pos22 + len * (e1).unit();
    cout << "First point: " << pos21 << endl;
    cout << "Second point: " << pos22 << endl;
    cout << "Third point: " << pos23 << endl;
    TrkPieceLineTraj pieceTr(pos21, pos22);
    pieceTr.append(pos23);
    TrkPoca poca(testTraj1, 0., pieceTr, 20.);
    cout << "testTraj1 : " << pos1 << dir1 << endl;
    cout << "pieceTr : " << pos21 << pos22 << pos23 << endl;
    cout << poca.doca() << " , Real: " << dist << endl << endl;
    cout << "Piecewise Traj & HepPoint (with variable coords)" << endl;
    cout
        << "Output description: below are doca() return values for input sets,"
        << endl;
    cout
        << "which represent a problem. piecepath is the path along the Piecewise Traj,"
        << endl;
    cout << "where iterations start. Joint point is set to piecepath=20.0."
        << endl;
    cout
        << "Next entry=point at the given piecepath. Then 4 HepPoints of interest go:"
        << endl;
    cout << pos1 << pos1 + 2 * dir1 << pos1 + 18 * dir1 << pos1 + 20 * dir1
        << endl;
    for (piecepath = 0.; piecepath <= 2 * len; piecepath += 1) {
      cout << "piecepath" << piecepath << " -> ";
      pieceTr.getInfo(piecepath, curpos, curdir);
      cout << curpos << " -> ";
      pathoffset = 0;
      for (i = 0; i <= n_steps; i++, pathoffset += len / n_steps) {
        point = pos1 + pathoffset * dir1;
        if (fabs(point.x()) < 5) continue;
        TrkPoca poca(pieceTr, piecepath, point);
        //cout << point ;
        cout << poca.doca() << "  ";// << endl;
      }
      cout << endl;
    }

    break;
  }

  case 4: {
    BFieldFixed field(0.0, 0.0, 1.5);
    double dist = 2.5, maxdiff = 0., maxdiffangle = 0.;
    Hep3Vector dir2, dir1, curdir;
    HepPoint pos2, curpos;
    angle1 = 0.;//pi/8;
    Hep3Vector e1(cos(angle1), -1 * sin(angle1), 0.);
    Hep3Vector e2(sin(angle1), cos(angle1), 0.);
    Hep3Vector e3(0., 0., 1.);
    HepPoint pos1(0., 0., 0.);
    pos2 = pos1 + dist * e2;
    dir1 = e1;
    TrkLineTraj testTraj1(pos1, dir1, 10.);
    //  testTraj2 is gradually rotating, angle is in the most
    //  hard range near zero.
    for (angle2 = 0.; angle2 < .01; angle2 += .001) {
      dir2 = cos(angle2) * e1 + sin(angle2) * e3;
      cout << dir1 << dir2;
      TrkExchangePar par1 = TrkHelixUtils::helixFromMom(pos2, dir2, 1., field);
      TrkDifLineTraj testTraj2(par1, 0.0, 10.);
      TrkDifPoca poca(testTraj2, 0., testTraj1, 10.);
      tmp = fabs(poca.doca());
      cout << tmp << endl;
      tmp -= dist;
      if (fabs(tmp) > maxdiff) {
        maxdiff = fabs(tmp);
        maxdiffangle = angle2;
      }
    }
    cout << "Maximal inaccuracy in doca determination is " << maxdiff
        << " at angle " << maxdiffangle << endl;
    dir2 = 2 * dir1;
    TrkExchangePar par1 = TrkHelixUtils::helixFromMom(pos2, dir2, 1., field);
    TrkDifLineTraj testTraj2(par1, 0.0, 10.);
    TrkDifPoca poca(testTraj2, 5., testTraj1, 10.);
    cout << "It is one of hard cases :" << endl;
    cout << "Tr1 : " << pos1 << dir1 << endl;
    cout << "Tr2 : " << pos2 << dir2 << endl;
    cout << "Real distance is : " << dist << " while doca()=" << poca.doca()
        << endl;

    break;
  }

  case 5: { // Piecewise Traj && (Linear Traj || HepPoint).
    cout << "TrkDifPieceTraj & LineTraj." << endl;
    BFieldFixed field(0.0, 0.0, 1.5);
    Hep3Vector e1(1., 0., 0.);
    Hep3Vector e2(0., 1., 0.);
    Hep3Vector e3(0., 0., 1.);
    Hep3Vector dir1, dir2, dir3, curdir;
    HepPoint curpos, pos1, pos22, pos21, pos23, pos0(0., 0., 0.), point;
    double len = 20., dist = 5 * sqrt(2.);
    pos1 = pos0 + (len / 2) * (e1 - e2).unit();
    dir1 = (e2 - e1).unit();
    TrkLineTraj testTraj1(pos1, dir1, len);
    dir2 = (-1 * e2).unit();
    cout << dir2;
    dir3 = (e1).unit();
    pos22 = pos0 + dist * (e1 + e2).unit();
    pos21 = pos22 - len * dir2;
    pos23 = pos22 + len * dir3;
    cout << "First point: " << pos21 << endl;
    cout << "Second point: " << pos22 << endl;
    cout << "Third point: " << pos23 << endl;

    TrkExchangePar par1 = TrkHelixUtils::helixFromMom(pos21, dir2,
        .000000000001, field);
    TrkDifLineTraj testTraj21(par1, 0.0, len);
    TrkExchangePar par2 = TrkHelixUtils::helixFromMom(pos22, dir3,
        .000000000001, field);
    TrkDifLineTraj testTraj22(par2, 0.0, len);
    TrkDifPieceTraj pieceDFTraj(testTraj21, -25.0, -5);
    double gap;
    pieceDFTraj.append(-5, testTraj22, gap);

    // lookAtTraj(pieceDFTraj,-25,15);

    TrkDifPoca poca(pieceDFTraj, 0., testTraj1, len);
    cout << "testTraj1 : " << pos1 << dir1 << endl;
    cout << "pieceTr : " << pos21 << pos22 << pos23 << endl;
    cout << poca.doca() << " , Real: " << dist << endl << endl;

    break;
  }

  case 6: {
    int j = 0;
    HepPoint curpos, zero(0., 0., 0.);
    Hep3Vector curdir;
    double d0 = 5., phi0, z0 = 0, tanDip = 0., doca, z;
    phi0 = PI / 4;
    Hep3Vector e1(1., 0., 0.);
    Hep3Vector e2(0., 1., 0.);
    Hep3Vector e3(0., 0., 1.);
    HepPoint pos;
    Hep3Vector dir;
    dir = (e1 + e2).unit();
    pos = zero + (e2 - e1).unit() * 2 * d0 + dir * 15.;

    TrkLineTraj testTraj1(pos, dir, 10.0);
    TrkExchangePar par2(2 * d0, phi0, 1.0, z0, tanDip);
    TrkDifLineTraj traj2(par2, 5.0, 15.0);

    // for(j=0;j<10;j++) {
    z = z0 + .1 * j;
    TrkExchangePar par1(d0, phi0, 1.0, z, tanDip);
    TrkDifLineTraj traj1(par1, 0.0, 10.0);

    lookAtTraj(traj2, 0, 15);
    lookAtTraj(traj1, 0, 15);

    // this gives nan's !
    cout << "This is the case of two TrkDifTraj`s passed to TrkDifPoca..."
        << endl;
    TrkDifPoca poca(traj1, 0., traj2, 0.);
    HepVector der = poca.derivs();
    cout << poca.doca() << " derivs: " << der[0] << " " << der[1] << " "
        << der[2] << " " << der[3] << endl;
    cout << "And here is the case of one DifTraj & LineTraj " << endl;
    TrkDifPoca poca1(traj1, 0., testTraj1, 0.);
    doca = poca1.doca();
    HepVector der1 = poca1.derivs();
    cout << doca << " derivs: " << der1[0] << " " << der1[1] << " " << der1[2]
        << " " << der1[3] << endl;
    cout << "Should be: " << d0 / doca << " " << z / doca << endl;
    //  }

    break;
  }

  case 7: { // TrkDifPoca with two parallel trajs

    routine7();
    break;
  }

  case 8: {
    cout << "Trajectories form newly born TrkCompTrk class." << endl;
    BFieldFixed field(0.0, 0.0, 1.5);
    double dist = 2.5, maxdiff = 0., maxdiffangle = 0.;
    Hep3Vector dir2, dir1, curdir;
    HepPoint pos2, curpos;
    angle1 = 0.;//pi/8;
    Hep3Vector e1(cos(angle1), -1 * sin(angle1), 0.);
    Hep3Vector e2(sin(angle1), cos(angle1), 0.);
    Hep3Vector e3(0., 0., 1.);
    HepPoint pos1(0., 0., 0.);
    pos2 = pos1 + dist * e2;
    dir1 = e1;
    double mom_mag;
    cout << "Momentum magnitude : ";
    cin >> mom_mag;
    dir1 = dir1 * mom_mag;

    //  TrkCompTrk testTrk(pos1,dir1,1,0.1,3);
    //  testTrk.print(cout);
    //  lookAtTraj((Trajectory&)testTrk.traj(),0.,10.);

    //  testTraj2 is gradually rotating, angle is in the most
    //  hard range near zero.
    for (angle2 = 0.; angle2 < .1; angle2 += .01) {
      dir2 = cos(angle2) * e1 + sin(angle2) * e3;
      cout << "pos=" << pos2 << " dir=" << dir2;
      TrkExchangePar par1 = TrkHelixUtils::helixFromMom(pos2, dir2, 1., field);
      TrkDifLineTraj testTraj2(par1, 0.0, 10.);
      //      TrkDifPoca poca(testTraj2,0.,testTrk.traj(),10.);
      //      tmp=fabs(poca.doca());
      //      cout << " doca=" << tmp << " success=" << poca.status().success() << endl;
      //      tmp-=dist;
      //      if(fabs(tmp)>maxdiff) {maxdiff=fabs(tmp);maxdiffangle=angle2;}
    }
    //  cout << "Maximal inaccuracy in doca determination is " << maxdiff << 
    //    " at angle " << maxdiffangle << endl;
    //  cout << endl;

    maxdiff = 0;
    //  TrkCompTrk testTrk1(pos1,dir1,0,0.1,3);
    //  testTrk1.print(cout);
    //  lookAtTraj((Trajectory&)testTrk1.traj(),0.,10.);

    //  testTraj2 is gradually rotating, angle is in the most
    //  hard range near zero.
    for (angle2 = 0.; angle2 < .1; angle2 += .01) {
      dir2 = cos(angle2) * e1 + sin(angle2) * e3;
      cout << "pos=" << pos2 << " dir=" << dir2;
      TrkExchangePar par1 = TrkHelixUtils::helixFromMom(pos2, dir2, 1., field);
      TrkDifLineTraj testTraj2(par1, 0.0, 9.);
      //      TrkDifPoca poca(testTraj2,0.,testTrk1.traj(),9.);
      //      tmp=fabs(poca.doca());
      //      cout << " doca=" << tmp << " success=" << poca.status().success() << endl;
      //      tmp-=dist;
      //      if(fabs(tmp)>maxdiff) {maxdiff=fabs(tmp);maxdiffangle=angle2;}
    }
    cout << "Maximal inaccuracy in doca determination is " << maxdiff
        << " at angle " << maxdiffangle << endl;

    break;
  }

  case 9: {
    // test transformation visitor on TrkGeomTrajs

    HepPoint start(1., 2., 3.);
    HepPoint end(3., 2., 3.);
    Hep3Vector move(0., 4.5, 1.);
    TrkLineTraj line(start, end);
    HepRotation aRot;
    HepTranslation aTran(move);
    HepTransformation tran(aRot, aTran);
    TrkTransformVisitor visitor(tran);
    line.accept(visitor);
    TrkLineTraj* newTraj = (TrkLineTraj*) visitor.newTraj();
    cout << "New endpoints: " << endl;
    cout << line.position(line.lowRange()) << "  " << line.position(
        line.hiRange()) << endl;
    cout << "Translate by: " << move << endl;

    cout << "New endpoints: " << endl;
    cout << newTraj->position(newTraj->lowRange()) << "  "
        << newTraj->position(newTraj->hiRange()) << endl;
    delete newTraj;

    // PieceLineTraj:
    TrkPieceLineTraj pieceTraj(line);
    HepPoint addOn(5., 10., 3.);
    pieceTraj.append(addOn);
    pieceTraj.accept(visitor);
    TrkPieceLineTraj *newTraj2 = (TrkPieceLineTraj*) visitor.newTraj();
    pieceTraj.printAll(cout);
    newTraj2->printAll(cout);

    break;
  }

  case 10: {
    // test momentum visitor on HelixTraj, NeutTraj, TrkCircleTraj,
    // and (of course futilely) on TrkDifLineTraj.

    double xMom, yMom, zMom;
    cout << "Gimme a momentum vector:" << endl;
    cout << "  Mom. in x-direction: ";
    cin >> xMom;
    cout << "  Mom. in y-direction: ";
    cin >> yMom;
    cout << "  Mom. in z-direction: ";
    cin >> zMom;
    Hep3Vector theMom(xMom, yMom, zMom);
    cout << endl << "The magnitude is: " << theMom.mag() << endl;

    // is field initialized? yes--to default
    // is there a NeutUtils? no--steal code in TrkCompTrk.cc 
    // will helixpars work for circle? yes

    BFieldFixed theField(0.0, 0.0, 1.5);
    HepPoint thePos(0., 0., 0.);
    TrkExchangePar thePars = TrkHelixUtils::helixFromMom(thePos, theMom, 1.,
        theField);
    double theOmega = thePars.omega();
    thePars.setOmega(theMom.mag());
    //    NeutParams neutPars(thePars);
    thePars.setOmega(theOmega);

    HelixTraj theHelix(thePars, 0., 10.);
    TrkCircleTraj theCircle(thePars, 0., 10.);
    TrkDifLineTraj theLine(thePars, 0., 10.);
    //    NeutTraj theNeut(neutPars, 0., 10.);


    double fltlen;
    cout << "Gimme a flight length: ";
    cin >> fltlen;

    double helixPt = TrkMomCalculator::ptMom(theHelix, theField, fltlen);
    double circlePt = TrkMomCalculator::ptMom(theCircle, theField, fltlen);
    double linePt = TrkMomCalculator::ptMom(theLine, theField, fltlen);
    //    double neutPt = TrkMomCalculator::ptMom(theNeut, theField, fltlen);

    cout << "the Helix's Pt is: " << helixPt << endl;
    cout << "the Circle's Pt is: " << circlePt << endl;
    cout << "the Line's Pt is: " << linePt << endl;
    //    cout << "the Neut's Pt is: " << neutPt << endl;

    Hep3Vector helixVecMom = TrkMomCalculator::vecMom(theHelix, theField,
        fltlen);
    Hep3Vector circleVecMom = TrkMomCalculator::vecMom(theCircle, theField,
        fltlen);
    Hep3Vector lineVecMom = TrkMomCalculator::vecMom(theLine, theField, fltlen);
    //    Hep3Vector neutVecMom = TrkMomCalculator::vecMom(theNeut, theField,
    //						     fltlen);

    cout << "the Helix's vector momentum is: " << helixVecMom << endl;
    cout << "the Circle's vector momentum is: " << circleVecMom << endl;
    cout << "the Line's vector momentum is: " << lineVecMom << endl;
    //    cout << "the Neut's vector momentum is: " << neutVecMom << endl;

    BbrVectorErr helixErrMom = TrkMomCalculator::errMom(theHelix, theField,
        fltlen);
    BbrVectorErr circleErrMom = TrkMomCalculator::errMom(theCircle, theField,
        fltlen);
    BbrVectorErr lineErrMom = TrkMomCalculator::errMom(theLine, theField,
        fltlen);
    //    BbrVectorErr neutErrMom = TrkMomCalculator::errMom(theNeut, theField,
    //						       fltlen);

    cout << "the Helix's momentum w// error is: " << helixErrMom << endl;
    cout << "the Circle's momentum w// error is: " << circleErrMom << endl;
    cout << "the Line's momentum w// error is: " << lineErrMom << endl;
    //    cout << "the Neut's momentum w// error is: " << neutErrMom << endl;

    int helixCharge = TrkMomCalculator::charge(theHelix, theField, fltlen);
    int circleCharge = TrkMomCalculator::charge(theCircle, theField, fltlen);
    int lineCharge = TrkMomCalculator::charge(theLine, theField, fltlen);
    //    int neutCharge = TrkMomCalculator::charge(theNeut, theField,
    //					      fltlen);

    cout << "the Helix's charge is: " << helixCharge << endl;
    cout << "the Circle's charge is: " << circleCharge << endl;
    cout << "the Line's charge is: " << lineCharge << endl;
    //    cout << "the Neut's charge is: " << neutCharge << endl << endl;

    cout << "Now time to create a helix track and rep" << endl;
    cout << "and find the momenta ..." << endl << endl;

    TrkMiniMaker theMaker;
    TrkIdManDefault theManager(0);
    TrkId theId(theManager.nextId(), &theManager);
    TrkContextCustom theContext(&theField, theId);
    TrkRecoTrk* helixTrack = theMaker.makeTrack(thePars, 0, 0, 0, 0., 0., 10.,
        PdtPid::pion, theContext, 0.);

    //   TrkHelixRep helixRep(thePars, helixTrack, PdtPid::pion, 0, 0, 0, 0.,
    //                        0., 10.);

    cout << "Vector Mom: " << helixTrack->fitResult()->momentum(fltlen) << endl
        << endl;
    cout << "VecMom w/ err: " << helixTrack->fitResult()->momentumErr(fltlen)
        << endl;

    break;
  }

  case 11: {

    // Test helixFromMom
    BFieldFixed field(0.0, 0.0, 1.5);
    Hep3Vector mom1(125., 125., 125.);
    HepPoint pos1(10., 0., 0.);
    HepPoint pos0(0., 5., 0.);
    TrkExchangePar par1 = TrkHelixUtils::helixFromMom(pos0, mom1, 1., field);

    TrkDifLineTraj dltr(par1, 0.0, 10.0);
    //TrkDifPoca poca(dltr,0.0,


    //  Hep3Vector mom2(.5, 0., 0.);
    //  HepPoint pos2(0.,3.3,0.);
    Hep3Vector mom2(0.0134517, 0.125158, 0.0387616);
    HepPoint pos2(-4.94628, 13.7167, 5.40326);
    TrkExchangePar par2 = TrkHelixUtils::helixFromMom(pos2, mom2, 1., field);

    cout << "1  d0: " << par1.d0() << " phi0: " << par1.phi0() << " omega: "
        << par1.omega() << " ct: " << par1.tanDip() << " z0: " << par1.z0()
        << endl;
    cout << "2  d0: " << par2.d0() << " phi0: " << par2.phi0() << " omega: "
        << par2.omega() << " ct: " << par2.tanDip() << " z0: " << par2.z0()
        << endl;

    // Test TrkPoca(traj, pt)
    Hep3Vector dir(0.3, .2, 1.);
    TrkLineTraj testTraj(pos1, dir, 10.);
    HepPoint pos3(1., 1., 1.);
    TrkPoca poca1(testTraj, 0., pos2);
    poca1.status().print(cout);
    cout << "Poca 1: " << poca1.doca() << "  " << poca1.flt1() << endl;
    TrkPoca poca2(testTraj, 0., pos3);
    poca2.status().print(cout);
    cout << "Poca 2: " << poca2.doca() << "  " << poca2.flt1() << endl;

    // Test TrkDifLineTraj
    TrkDifLineTraj linTraj1(par1);
    TrkPoca pocaA(linTraj1, 10., testTraj, -10.);
    TrkDifPoca pocaB(linTraj1, 125., testTraj, 30.);
    cout << "LineTraj: " << pocaA.status().success() << " " << pocaA.doca()
        << " " << pocaA.flt1() << " " << pocaA.flt2() << endl;
    cout << "LineTraj: " << pocaB.status().success() << " " << pocaB.doca()
        << " " << pocaB.flt1() << " " << pocaB.flt2() << endl;

    // Test TrkPoca(traj, traj)
    TrkLineTraj traj2(pos2, dir, 10.);
    TrkPoca poca3(testTraj, 0., traj2, 10.);
    poca3.status().print(cout);
    cout << poca3.status().success();
    cout << "Poca 3: " << poca3.doca() << "  " << poca3.flt1() << "  "
        << poca3.flt2() << endl;

    Hep3Vector dir2(0., 1., 1.);
    TrkLineTraj traj3(pos2, dir2, 10.);
    TrkPoca poca4(testTraj, 0., traj3, 10.);
    poca4.status().print(cout);
    cout << "Poca 4: " << poca4.doca() << "  " << poca4.flt1() << "  "
        << poca4.flt2() << endl;

    // Test derivs of traj/pt poca
    HelixTraj htraj(par2);
    TrkDifPoca poca5(htraj, 0., pos3);
    poca5.status().print(cout);
    HepVector derivs = poca5.derivs();
    cout << "Poca 5: " << poca5.doca() << endl;
    cout << "Derivs: " << derivs[0] << " " << derivs[1] << " " << derivs[2]
        << " " << derivs[3] << " " << derivs[4] << " " << endl;

    double docaCentral = poca5.doca();
    double increment = 0.001;
    int i;
    for (i = 0; i < 5; i++) {
      par2.params()[i] += increment;
      HelixTraj htraj1(par2);
      TrkDifPoca poca6(htraj1, 0., pos3);
      double docaOff = poca6.doca();
      cout << docaCentral << "  " << docaOff << "  " << (docaOff - docaCentral)
          / derivs[i] / increment << endl;
      cout << derivs[i] << "  " << (docaOff - docaCentral) / increment << endl;
      cout << endl;

      par2.params()[i] -= increment;
    }

    // Test derivs of traj/traj, with TrkDifLineTraj
    TrkDifLineTraj linTraj2(par2);
    TrkDifPoca pocaC(linTraj2, 125., testTraj, 30.);
    HepVector derivsLin = pocaC.derivs();
    cout << "Test LineTraj derivs: doca=" << pocaC.doca() << endl;
    cout << "Derivs: " << derivsLin[0] << " " << derivsLin[1] << " "
        << derivsLin[2] << " " << derivsLin[3] << " " << " " << endl;
    pocaC.status().print(cout);
    docaCentral = pocaC.doca();
    for (i = 0; i < 4; i++) {
      int index = i;
      if (i > 1) index = i + 1;
      par2.params()[index] += increment;
      TrkDifLineTraj linTrajTest(par2);
      TrkDifPoca pocaD(linTrajTest, 125., testTraj, 30);
      double docaOff = pocaD.doca();
      cout << docaCentral << "  " << docaOff << "  " << (docaOff - docaCentral)
          / derivsLin[i] / increment << endl;
      cout << derivsLin[i] << "  " << (docaOff - docaCentral) / increment
          << endl;
      cout << endl;

      par2.params()[index] -= increment;
    }
  }

  case 12: {
    HepPoint o(1., 1., 1.);
    double radius = 2.;
    double phiStart = 0.;
    int whichDir = 1;
    double tanDip = 0.;
    double startRange = -4.;
    double endRange = 4.;
    BbrHelixTraj bht(o, radius, phiStart, whichDir, tanDip, startRange,
        endRange);

    int nStep = 50;
    double stepSize = 2 * radius * PI / nStep;
    double flt = -nStep / 2 * stepSize;
    for (int i = 0; i <= nStep; i++) {
      cout << flt << " pos: " << bht.position(flt) << " dir: "
          << bht.direction(flt) << endl;
      flt += stepSize;
    }
    break;
  }
  }

  return 0;
}

/*  dir2=2*dir1;
 TrkExchangePar par1 = TrkHelixUtils::helixFromMom(pos2, dir2, 1., field);
 TrkDifLineTraj testTraj2(par1,0.0,10.);
 TrkDifPoca poca(testTraj2,5.,testTraj1,10.);
 cout << "It is one of hard cases :" << endl;
 cout << "Tr1 : " << pos1 << dir1 << endl;
 cout << "Tr2 : " << pos2 << dir2 << endl;
 cout << "Real distance is : " << dist << " while doca()="
 << poca.doca() << endl;
 */

