//
//  Test voxels
//
//
#include <cmath>
#include <iostream>
#include <string>
#include <assert.h>

#include "PacEnv/PacConfig.hh"
#include "PacGeom/PacCylVoxelSet.hh"
#include "PacGeom/PacHelix.hh"
#include "PacGeom/PacCylDetType.hh"
#include "PacGeom/PacCylDetElem.hh"
#include "PacGeom/PacConeDetType.hh"
#include "PacGeom/PacConeDetElem.hh"
#include "PacGeom/PacRingDetType.hh"
#include "PacGeom/PacRectDetType.hh"
#include "PacGeom/PacPlaneDetElem.hh"
#include "Framework/AppFileName.hh"
#include "BaBar/Constants.hh"
#include "BField/BField.hh"
#include <TRandom3.h>

using namespace std;

int main(int argc, char* argv[]) {
  if(argc <= 1){
    cout << "reading standard config file" << endl;
    gconfig.parsefile(AppFileName("PacTest/testVoxels.xml").pathname().c_str());
  } else {
    cout << "reading config from arguments ";
    for(unsigned iarg=1;iarg<argc;iarg++){
      cout << argv[iarg] << " ";
    }
    cout << endl;  
    gconfig.parseargs(argc, argv);
	}
	
// get the voxel boundaries
  
  std::vector<double> zdef;
  zdef.push_back(-50.0);
  zdef.push_back(50.0);
  std::vector<double> rbounds = gconfig.getvector("rbounds");
  std::vector<double> zbounds = gconfig.getvector("zbounds",zdef);
  std::vector<double> rnfvoxels = gconfig.getvector("nphivoxels");
  std::vector<unsigned> nfvoxels;
  for(unsigned ifv=0;ifv<rnfvoxels.size();ifv++)
    nfvoxels.push_back((unsigned)rint(rnfvoxels[ifv]));

  unsigned nrvoxels = gconfig.getint("nrvoxels",50);
  unsigned nzvoxels = gconfig.getint("nzvoxels",50);  
  double minfsize = gconfig.getdouble("minphisize",5.0);
  
  PacCylVoxelSet* cvset;
    
// build voxel set
  if(rbounds.size() > 2 || zbounds.size() > 2 || nfvoxels.size() > 0)
    cvset = new PacCylVoxelSet(rbounds,zbounds,nfvoxels);
  else
    cvset = new PacCylVoxelSet(&rbounds[0],nrvoxels,&zbounds[0],nzvoxels,minfsize);
  cout << "Created " << *cvset << endl;
// test a point
	HepPoint point = gconfig.getHepPoint("point");
  PacCylVoxelSet::VoxelIter viter = cvset->findVoxel(point);
  PacCylVoxelSet::VoxelIter olditer;
  if(viter != cvset->end()){
    
    cout << "Found voxel " << *viter  << "for point " << point <<  " rho, z, phi = " 
      << point.perp() << "," << point.z() << "," << point.phi() << endl;
    if(viter->isInside(point))
      cout << "Point is inside voxel" << endl;
    else
      cout << "Point is not inside voxel !! " << endl;
      
  } else {
    cout << "Didn't find voxel for point " << point << " rho, z, phi = " 
      << point.perp() << "," << point.z() << "," << point.phi() << endl;
  }
// create a trajectory
  std::vector<double> helixpars = gconfig.getvector("helixpars");
  assert(helixpars.size() == 5);
  HepVector hpars(5,0);
  for(int ipar=0;ipar<5;ipar++)
    hpars[ipar] = helixpars[ipar];  
  PacHelix helix(hpars,0,1000);
// trace the trajectory through the voxels and print
  point = helix.position(0.0);
  viter = cvset->findVoxel(point);
  if(viter != cvset->end()) {
    int ivoxel(0);
    double pathlen = 0.0;
    while(viter != cvset->end()){
      cout << "Found Voxel " << ivoxel << *viter << " for rho, z, phi = " 
        << point.perp() << "," << point.z() << "," << point.phi() << endl;
      olditer = viter;
      if(viter->findExit(&helix,pathlen,pathlen)){
        viter = cvset->findNextVoxel(&helix,pathlen);
        point = helix.position(pathlen);
        ivoxel++;
      } else
        viter = cvset->end();
    }
// test for exit
    if(cvset->exit(*olditer))
      cout << "Trajectory exits voxel set" << endl;
    else {
      cout << "Trajectory doesn't exit voxel set" << endl;
      return -1;
    }
  } else {
    cout << " Helix start isn't in voxel set " << endl;
  }

// create some simple elements and insert them
  double zends[2] = {cvset->zbounds().front()+1.0,cvset->zbounds().back()-1.0};
  PacCylDetType ctype(0.0,zends,0,0);
  std::vector<PacDetElem*> elems;
  unsigned nr = cvset->rbounds().size();
  double dr = cvset->rbounds().back()/nr;
  PacCylDetElem* celem(0);
  std::vector<const PacMeasurement*> nomeas;
  for(unsigned ir=0;ir<nr;ir++){
    double radius = dr*(ir+0.5);
    celem = new PacCylDetElem(&ctype,radius,"Cylinder",ir,nomeas,0.0,0.0);
    if(cvset->insert(celem)){
      cout << "Inserted cylinder element at radius " << radius << endl;
    } else
      cout << "Failed to Insert element at radius " << radius << endl;
  }
// check which voxels contain one of these
  std::vector<PacCylVoxel> voxels;
  cvset->findVoxels(celem,voxels);
  cout << "Cylinder with radius " << celem->radius() << " and z ends " 
    << celem->cylType()->lowZ() << ":" << celem->cylType()->hiZ()
    << " is in the following voxels:" << endl;
  for(std::vector<PacCylVoxel>::iterator ivox= voxels.begin();ivox!=voxels.end();ivox++){
    cout << *ivox << endl;
  }

// build a cone to fit inside the range
  double rsize = cvset->rbounds().back()-cvset->rbounds().front();
  double theRho1 = cvset->rbounds().front()+rsize/10.0;
  double theRho2 = cvset->rbounds().back()-rsize/10.0;
  double zsize = cvset->zbounds().back() - cvset->zbounds().front();
  double theZ1 = cvset->zbounds().front()+zsize/9.9;
  double theZ2 = cvset->zbounds().back()-zsize/9.9;
  double theta = atan2(theRho2-theRho1,theZ2-theZ1);
  double tanth = tan(theta);
  double zvert = theZ1 - theRho1/tanth;
  double costh = cos(theta);
  double r1 = fabs((theZ1-zvert)/costh);
  double r2 = fabs((theZ2-zvert)/costh);
  double rrange[2];
  if(r1>r2){
    rrange[0] = r2;
    rrange[1]= r1;
  } else {
    rrange[0] = r1;
    rrange[1] = r2;
  }
  PacConeDetType cntype(0.0,rrange,0,0);
  PacConeDetElem* cone = new PacConeDetElem(&cntype,tanth,zvert,"Cone",0,nomeas,0.0,0.0);
  if(cvset->insert(cone)){
    cout << "Inserted Cone" << endl;
  } else {
    cout << "Failed to insert cone" << endl;
  }
// printout voxels
  cvset->findVoxels(cone,voxels);
  cout << "Cone with radii " << theRho1 << ":" << theRho2
    <<  " at z  " << theZ1 << ":" << theZ2
    << " is in the following voxels:" << endl;
  for(std::vector<PacCylVoxel>::iterator ivox= voxels.begin();ivox!=voxels.end();ivox++){
    cout << *ivox << endl;
  }

// rings
  unsigned nz = cvset->zbounds().size()-1;
  double zstep = (cvset->zbounds().back()-cvset->zbounds().front())/nz;
  PacRingDetType rtype(0.0,theRho1,theRho2,0,0);
  PacPlaneDetElem* relem(0);
  for(unsigned iz=0;iz<nz;iz++){
    double z = cvset->zbounds().front()+(iz+0.5)*zstep;
    Hep3Vector center(0.0,0.0,z);
    relem = new PacPlaneDetElem(&rtype,PacPlaneDetElem::zparallel,center,0.0,
      "Ring",iz,nomeas,0.0,0.0);
    if(cvset->insert(relem)){
      cout << "Inserted Ring at z "<< z << endl;
    } else {
      cout << "Failed to insert ring at z " << z << endl;
    }
  }
// printout voxels
  cvset->findVoxels(relem,voxels);
  cout << "Ring at Z " << relem->midpoint().z() << " and radii " 
    << theRho1 << ":" << theRho2
    << " is in the following voxels:" << endl;
  for(std::vector<PacCylVoxel>::iterator ivox= voxels.begin();ivox!=voxels.end();ivox++){
    cout << *ivox << endl;
  }
  
//  rectangles
  unsigned nphi = cvset->fbounds().size()-1;
  double phistep = (cvset->fbounds().back()-cvset->fbounds().front())/nphi;
  double zmid = (cvset->zbounds().back() + cvset->zbounds().front())/2.0;
  
  PacRectDetType rectt(0.0,0.9*rsize,0.6*zsize,0,0);
  PacPlaneDetElem* recte(0);
  for(unsigned iphi=0;iphi<nphi;iphi++){
    double phi = (iphi+0.5)*phistep;
    double uphi = phi + phistep;
    double rho = rsize/2.0;
    Hep3Vector center(rho*cos(phi),rho*sin(phi),zmid);
    recte = new PacPlaneDetElem(&rectt,PacPlaneDetElem::zperp,center,uphi,
      "PerpRect",iphi,nomeas,0.0,0.0);
    if(cvset->insert(recte)){
      cout << "Inserted Perp Rectangle at phi "<< phi << endl;
    } else {
      cout << "Failed to insert Perp Rectangle at phi " << phi << endl;
    }
  }

  // printout voxels
  cvset->findVoxels(recte,voxels);
  cout << "Perp rectangle at phi " << recte->udir().phi() << " and usize " 
    << rectt.uSize() << " and v size " << rectt.vSize()
    << " is in the following voxels:" << endl;
  for(std::vector<PacCylVoxel>::iterator ivox= voxels.begin();ivox!=voxels.end();ivox++){
    cout << *ivox << endl;
  }

/*  
  unsigned nz = cvset->zbounds().size()-1;
  double rsize = cvset->rbounds().back()-cvset->rbounds().front();
  double zstep = (cvset->zbounds().back()-cvset->zbounds().front())/nz;
  PacPlaneDetElem* recte;
  std::vector<PacCylVoxel> voxels;
  double zmid = (cvset->zbounds().back() + cvset->zbounds().front())/2.0;
  double zsize = cvset->zbounds().back() - cvset->zbounds().front();
*/  
  
// parallel rectangle
// rotate them too
   double phistep2 = Constants::twoPi/nz;
   PacRectDetType rectt2(0.0,0.4*rsize,0.4*rsize,0,0);
   for(unsigned iz=0;iz<nz;iz++){
     double z = cvset->zbounds().front()+(iz+0.25)*zstep;
     double phi = iz*phistep2;
     Hep3Vector center(cos(phi)*0.5*rsize,sin(phi)*0.5*rsize,z);
     recte = new PacPlaneDetElem(&rectt2,PacPlaneDetElem::zparallel,center,phi/4.0,
       "ParallelRect",iz,nomeas,0.0,0.0);
     if(cvset->insert(recte)){
       cout << "Inserted Rectangle at center "<< center << endl;
     } else {
       cout << "Failed to insert Rectangle at center " << center << endl;
     }
   }
  // printout voxels
   cvset->findVoxels(recte,voxels);
   cout << "Parallel rectangle with center " << recte->midpoint() << " and usize " 
     << rectt2.uSize() << " and v size " << rectt2.vSize() << " and u phi " << recte->udir().phi()
     << " is in the following voxels:" << endl;
   for(std::vector<PacCylVoxel>::iterator ivox= voxels.begin();ivox!=voxels.end();ivox++){
     cout << *ivox << endl;
   }
// now, trace through the set finding all the detector elements
  DetIntersection dinter;
  dinter.pathlen = 0.0;
  dinter.trajet = &helix;
  while(cvset->findNextInter(&helix,dinter)){
    cout << "Intersected detector element " 
    << dinter.delem->elementName() << dinter.delem->elementNumber() 
    << " at point " << helix.position(dinter.pathlen) 
    << " flightlen " << dinter.pathlen << endl;
  }
// now, spray the detector and test that the helices come out

  const int rndseed = gconfig.getint("rndseed", 324237);
  TRandom3 rng(rndseed);

  unsigned nhelix = gconfig.getint("nhelix",1000);
  double d0range[2] = {-0.9*rsize,0.9*rsize};
  double phi0range[2] = {-Constants::pi,Constants::pi};
  double ptrange[2];
  ptrange[0] = gconfig.getdouble("min_pt",0.05);
  ptrange[1] = gconfig.getdouble("max_pt",5.0);
  double z0range[2] = {zmid-0.45*zsize,zmid+0.45*zsize};
  double tdrange[2];
  tdrange[0] = gconfig.getdouble("min_tandip",-1.5);
  tdrange[1] = gconfig.getdouble("max_tandip",1.5);
  for (int ih=0;ih<nhelix;ih++){
// throw randoms to get helix parameters
    hpars[0] = rng.Uniform(d0range[0],d0range[1]);
    hpars[1] = rng.Uniform(phi0range[0],phi0range[1]);
    double pt	= fabs(rng.Uniform(ptrange[0], ptrange[1]));
    hpars[2] = BField::cmTeslaToGeVc/pt; // assumes a 1T field
    if(ih%2 == 0) hpars[2] *= -1.0;
    hpars[3] = rng.Uniform(z0range[0],z0range[1]);
    hpars[4] = rng.Uniform(tdrange[0],tdrange[1]);
    PacHelix helix(hpars,0,10000);
    // trace the trajectory through the voxels and print
    double pathlen = 0.0;
    PacCylVoxelSet::VoxelIter viter,olditer;
    viter = cvset->findVoxel(helix.position(pathlen));
    while(viter != cvset->end()){
      olditer = viter;
      if(viter->findExit(&helix,pathlen,pathlen))
        viter = cvset->findNextVoxel(&helix,pathlen);
      else
        viter = cvset->end();
    }    
    if(!cvset->exit(*olditer)){
      cerr << "Voxel navigation ends before exiting volume " << endl;
    }
// now elements
    DetIntersection dinter,dold;
    dinter.pathlen = 0.0;
    dinter.trajet = &helix;
    while(cvset->findNextInter(&helix,dinter)){
      if(dinter.delem == dold.delem && dinter.pathlen - dold.pathlen < 1e-5 )
        cerr << "double element " << endl;
      dold = dinter;
    }
  } 
	return 0;
}
