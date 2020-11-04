
#include "PacDisplayMacro.hh"
#ifdef __MAKECINT__
#pragma link C++ class PacDispMat;
#pragma link C++ class PacDispCyl;
#pragma link C++ class PacDispRing;
#pragma link C++ class PacDispRect;
#pragma link C++ class PacDispCone;
#pragma link C++ class PacDispTraj;
#pragma link C++ class PacDispTrackTraj;
#pragma link C++ class PacDispPoint;
#pragma link C++ class PacDispSimHit;
#pragma link C++ class PacDispCluster;
#pragma link C++ class vector<PacDispTrackTraj>;
#pragma link C++ class vector<PacDispPoint>;
#pragma link C++ class vector<PacDispSimHit>;
#pragma link C++ class vector<PacDispCluster>; 
#pragma link C++ class PacDisplayMacro;
#endif

#include <iostream>
#include <math.h>
#include "TGeoVolume.h"
#include "TGeoMaterial.h"
#include "TGeoManager.h"
#include "TGeoMedium.h"
#include "TGeoMatrix.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TPolyLine3D.h"
#include "TPolyMarker3D.h"
#include "TGLViewer.h"
#include "TColor.h"
#include "TTree.h"
#include "TStyle.h"
#include "TDirectory.h"
#include "TSystem.h"
#include "TControlBar.h"
#include <TROOT.h>

#include <TGButton.h>
#include <TGButtonGroup.h>
#include <TGLabel.h>
#include <TGNumberEntry.h>
#include <TG3DLine.h>
#include <TApplication.h>
#include <TString.h>
#include <TObjString.h>

#include "TKey.h"

#include <string>
#include <algorithm>

using namespace std;

#define STRINGLEN 100
#define NTRKTYPES 3

#define r2d 57.29577951

PacDisplayMacro::~PacDisplayMacro()
{}

void
PacDisplayMacro::resetEvent() {
  _top->RemoveNode(_top->GetNode("Event_1"));
//  delete _event; 
  _event = _geom->MakeBox("Event", _Vacuum, 500.,500.,500.);
  _event->SetVisibility(kFALSE);
  _top->AddNode(_event,1, new TGeoTranslation(0,0,0));
// clear out the trajectories and markers
  for(unsigned itraj=0;itraj<_trajs.size();itraj++)
    delete _trajs[itraj];
  _trajs.clear();
  for(unsigned imark=0;imark<_hmarks.size();imark++)
    delete _hmarks[imark];
  _hmarks.clear();
}

void PacDisplayMacro::DrawEvent(){
  std::cout << "plotting event: " << _currentEv << std::endl;
  loadTrees();
  if(_type.find("e") != string::npos)DrawClusters();
  _top->Draw();
  if(_type.find("h") != string::npos)drawSimHits();
  DrawTracks();
  _can->Update();
}

void
PacDisplayMacro::Next() {
	if(_currentEv < _maxEv) {
		_currentEv++;
		resetEvent();
    DrawEvent();
	} else {
		cout << "Current Event is at MAX " << _currentEv << endl;
	}
}

void
PacDisplayMacro::Prev() {
	if(_currentEv > 0) {
		_currentEv--;
		resetEvent();
    DrawEvent();
	} else {
		cout << "Current Event is at MIN: event " << _currentEv << endl;
	}
}

void
PacDisplayMacro::DisplayChargePid(int charge,int pid) {
	cout << "q: " << charge << endl;
	if(charge == 0) {
		if(pid == -1) {cout << "pid: none" << endl;}
		else if (pid == 0) {cout << "pid: gamma" << endl;}
		else if (pid == 1) {cout << "pid: pi0" << endl;}
		else if (pid == 2) {cout << "pid: K0L" << endl;}
		else if (pid == 3) {cout << "pid: neutron" << endl;}
		else if (pid == 4) {cout << "pid: anti_neutron" << endl;}
	} else {
		if(pid == -1) {cout << "pid: null" << endl;}
		else if (pid == 0) {cout << "pid: electron" << endl;}
		else if (pid == 1) {cout << "pid: muon" << endl;}
		else if (pid == 2) {cout << "pid: pion" << endl;}
		else if (pid == 3) {cout << "pid: kaon" << endl;}
		else if (pid == 4) {cout << "pid: proton" << endl;}
	}
}

const char*
PacDisplayMacro::effectName(int effect) {
  // See PacSimHit.hh     enum effect {none ...}
  static const char* effname[]= {"none","creation","mark","brems","compton","normal","stop","interact","convert","decay","shower","hadshower","neffects"};
  return effname[effect];
}

void PacDisplayMacro::OnClicked(TObject *obj) {	
	// Handle click events in GL viewer
	if (obj) {
    cout << "Clicked on " << obj->GetName() << endl;
		return;
	}
  return;
}

void
PacDisplayMacro::DrawDetector() {
// use gradient colors
  Int_t MyPalette[100];
  Double_t Red[3]   = { 0.5, 0.5, 1.0 };
  Double_t Green[3] = { 0.5, 1.0, 0.5 };
  Double_t Blue[3]  = { 1.0, 0.5, 0.5 };
  Double_t Stops[] = {0., .25, .50, .75, 1.0};
  Int_t FI = TColor::CreateGradientColorTable(5,Stops,Red,Green,Blue,100);
  for (int i=0;i<100;i++) MyPalette[i] = FI+i;
  gStyle->SetPalette(100, MyPalette);
  static const double mindensity(0.0);
// setup material (names)
//  cout << "Found " << _materials->GetEntries() << " materials " << endl;
// draw cylinders
//  cout << "Found " << _cylinders->GetEntries() << " cylinders" << endl;
	for(unsigned icyl = 0;icyl<_cylinders->GetEntries();icyl++) {
		_cylinders->GetEntry(icyl);
		if(_dcyl.imat > -1 && _dcyl.radius< _rmax && _dcyl.lowZ > _zmin && _dcyl.hiZ< _zmax) {
      _materials->GetEntry(_dcyl.imat);
      std::string matname;
      TGeoMaterial* matmed;
      TGeoMedium* medium;
      int dcolor(0);
      fillMaterial(_dcyl.imat,matname,matmed,medium,dcolor);
      if(matmed->GetDensity() > mindensity){
        char cylname[STRINGLEN];
        snprintf(cylname,STRINGLEN,"cylinder_%02d",icyl);   
        TGeoVolume *cylinder = _geom->MakeTube(cylname,medium,_dcyl.radius-(_dcyl.thickness/2.0),_dcyl.radius+(_dcyl.thickness/2.0),(_dcyl.hiZ-_dcyl.lowZ)/2.0);
        cylinder->SetUniqueID(_objectID++);
        cylinder->SetLineColor(dcolor);
        cylinder->SetTransparency(_transparency);
//      cout << "Drawing cylinder " << icyl << " radius " << _dcyl.radius << " material " << matname << endl;
        _detector->AddNode(cylinder, icyl+1, new TGeoTranslation(0,0,(_dcyl.hiZ+_dcyl.lowZ)/2.0));
      }
		}
	}
// draw rings
//  cout << "Found " << _rings->GetEntries() << " rings" << endl;
	for(unsigned iring = 0;iring<_rings->GetEntries();iring++) {
		_rings->GetEntry(iring);
		if(_dring.imat > -1 && _dring.radhigh<_rmax && _dring.z > _zmin && _dring.z < _zmax ) {
      _materials->GetEntry(_dring.imat);
      std::string matname;
      TGeoMaterial* matmed;
      TGeoMedium* medium;
      int dcolor(0);
      fillMaterial(_dring.imat,matname,matmed,medium,dcolor);
      if(matmed->GetDensity() > mindensity){
        static char ringname[STRINGLEN];
        snprintf(ringname,STRINGLEN,"ring_%02d",iring);
        TGeoVolume *ring = _geom->MakeTube(ringname,medium,_dring.radlow,_dring.radhigh,_dring.thickness/2.0);
        ring->SetUniqueID(_objectID++);
        ring->SetTransparency(_transparency);
        ring->SetLineColor(dcolor);
        _detector->AddNode(ring,iring+1, new TGeoTranslation(0,0,_dring.z));
      }
    }
	}
// draw rectangles
//  cout << "Found " << _rects->GetEntries() << " rectangles" << endl;
	for(unsigned irect = 0;irect<_rects->GetEntries();irect++) {
		_rects->GetEntry(irect);
		if(_drect.imat > -1 && sqrt(pow(_drect.cx,2)+pow(_drect.cy,2))<_rmax && _drect.cz > _zmin && _drect.cz < _zmax ) 
		 {
      _materials->GetEntry(_drect.imat);
      std::string matname;
      TGeoMaterial* matmed;
      TGeoMedium* medium;
      int dcolor(0);
      fillMaterial(_drect.imat,matname,matmed,medium,dcolor);
      if(matmed->GetDensity() > mindensity){
        static char rectname[STRINGLEN];
        snprintf(rectname,STRINGLEN,"rect_%02d",irect);
        TGeoVolume *rect = _geom->MakeBox(rectname,medium,_drect.usize/2.0,_drect.vsize/2.0,_drect.thickness/2.0);
//        cout << "built rectangle with u size=" << _drect.usize << "v size=" << _drect.vsize 
//          << " thickness =" << _drect.thickness << endl; 
        rect->SetUniqueID(_objectID++);
        rect->SetTransparency(_transparency);
//      if(!_showendcap) rect->SetTransparency(85);
        rect->SetLineColor(dcolor);
// setup the rotation
        TGeoRotation *rotation(0);
        if(_drect.nz > 0.99){
          double phi = atan2(_drect.uy,_drect.ux);
//          cout << "rectangle normal along z, rotation = " << phi << endl;
          rotation = new TGeoRotation("rotation",r2d*phi,0.0,0.0);          
        } else if(abs(_drect.nz) < 0.001){
//          cout << "rectangle normal perp to z " << endl;
          rotation = new TGeoRotation("rotation",90+r2d*atan2(_drect.ny,_drect.nx),90.0,0.0);
        } else {
          cout << "error making rotation" << endl;
        }
//        cout << "rectangle center at x =" << _drect.cx << "y =" << _drect.cy << "z =" << _drect.cz << endl;
        TGeoCombiTrans *transform = new TGeoCombiTrans(_drect.cx,_drect.cy,_drect.cz,rotation);
        _detector->AddNode(rect,irect+1, transform);
      }
    }
	}

// draw cones
//  cout << "Found " << _cones->GetEntries() << " cones" << endl;
	for(unsigned icone = 0;icone<_cones->GetEntries();icone++) {
    _cones->GetEntry(icone);
    double cost = sqrt(1/(1+pow(_dcone.tanTheta,2)));
    double sint = cost*_dcone.tanTheta;
    double rmax = std::max(_dcone.hiR*sint,_dcone.lowR*sint);
    double zmax = std::max(_dcone.zVertex + _dcone.hiR*cost, _dcone.zVertex + _dcone.lowR*cost);
    double zmin = std::min(_dcone.zVertex + _dcone.hiR*cost, _dcone.zVertex + _dcone.lowR*cost);
		if(_dcone.imat > -1 && rmax < _rmax && zmax< _zmax && zmin > _zmin) {
      _materials->GetEntry(_dcone.imat);
      std::string matname;
      TGeoMaterial* matmed;
      TGeoMedium* medium;
      int dcolor(0);
      fillMaterial(_dcone.imat,matname,matmed,medium,dcolor);
      if(matmed->GetDensity() > mindensity){
        static char conename[STRINGLEN];
        snprintf(conename,STRINGLEN,"cone_%02d",icone);
        double conelowZ = 0.0;
        double conelowR = 0.0;
        double conehiZ = 0.0;
        double conehiR = 0.0;
        double coneSin = 0.0;
        double coneCos = 0.0;

        if(_dcone.tanTheta >= 0) {
          coneSin = sin(atan(_dcone.tanTheta));
          coneCos = cos(atan(_dcone.tanTheta));				
          conehiZ = _dcone.hiR*coneCos;
          conehiR = _dcone.hiR*coneSin;				
          conelowZ = _dcone.lowR*coneCos;										
          conelowR = _dcone.lowR*coneSin;	
        } else {
          coneSin = sin(fabs(atan(_dcone.tanTheta)));
          coneCos = cos(fabs(atan(_dcone.tanTheta)));				
          conehiZ = -1*_dcone.lowR*coneCos;
          conehiR = _dcone.lowR*coneSin;				
          conelowZ = -1*_dcone.hiR*coneCos;										
          conelowR = _dcone.hiR*coneSin;	
        }

        TGeoVolume *cone = _geom->MakeCone(conename,medium,
          fabs(fabs(conehiZ)-fabs(conelowZ))/2.0, 
          fabs(conelowR) - _dcone.thick/2.0,
          fabs(conelowR) + _dcone.thick/2.0,
          fabs(conehiR) - _dcone.thick/2.0,
          fabs(conehiR) + _dcone.thick/2.0);

        cone->SetUniqueID(_objectID++);
        cone->SetLineColor(dcolor);
        cone->SetTransparency(_transparency);
        _detector->AddNode(cone, icone+1, new TGeoTranslation(0,0,_dcone.zVertex + conelowZ +(conehiZ - conelowZ)/2.0));
      }
    }
	}
}

void
PacDisplayMacro::DrawVoxels() {
  static const double thick(0.1);
	for(unsigned icyl = 0;icyl<_vcylinders->GetEntries();icyl++) {
		_vcylinders->GetEntry(icyl);
		if(_vcyl.radius< _rmax && _vcyl.lowZ > _zmin && _vcyl.hiZ< _zmax) {
      char cylname[STRINGLEN];
      snprintf(cylname,STRINGLEN,"vcylinder_%02d",icyl);   
      TGeoVolume *cylinder = _geom->MakeTube(cylname,_Vacuum,_vcyl.radius-thick,
        _vcyl.radius+thick,(_vcyl.hiZ-_vcyl.lowZ)/2.0);
      cylinder->SetUniqueID(_objectID++);
      cylinder->SetLineColor(kOrange);
      cylinder->SetTransparency(_transparency);
      _detector->AddNode(cylinder, icyl+1, new TGeoTranslation(0,0,(_vcyl.hiZ+_vcyl.lowZ)/2.0));
		}
	}
// draw rings
	for(unsigned iring = 0;iring<_vrings->GetEntries();iring++) {
		_vrings->GetEntry(iring);
		if(_vring.imat > -1 && _vring.radhigh<_rmax && _vring.z > _zmin && _vring.z < _zmax ) {
      static char ringname[STRINGLEN];
      snprintf(ringname,STRINGLEN,"vring_%02d",iring);
      TGeoVolume *ring = _geom->MakeTube(ringname,_Vacuum,_vring.radlow,_vring.radhigh,thick);
      ring->SetUniqueID(_objectID++);
      ring->SetTransparency(_transparency);
      ring->SetLineColor(kCyan);
      _detector->AddNode(ring,iring+1, new TGeoTranslation(0,0,_vring.z));
    }
	}
// draw rectangles
	for(unsigned irect = 0;irect<_vrects->GetEntries();irect++) {
		_vrects->GetEntry(irect);
		if(_vrect.imat > -1 && sqrt(pow(_vrect.cx,2)+pow(_vrect.cy,2))<_rmax && _vrect.cz > _zmin && _vrect.cz < _zmax ){
      static char rectname[STRINGLEN];
      snprintf(rectname,STRINGLEN,"vrect_%02d",irect);
      TGeoVolume *rect = _geom->MakeBox(rectname,_Vacuum,_vrect.usize/2.0,_vrect.vsize/2.0,thick);
      rect->SetUniqueID(_objectID++);
      rect->SetTransparency(_transparency);
      rect->SetLineColor(kYellow);
// setup the rotation
      TGeoRotation *rotation(0);
      if(_vrect.nz > 0.99){
        double phi = atan2(_vrect.uy,_vrect.ux);
//          cout << "rectangle normal along z, rotation = " << phi << endl;
        rotation = new TGeoRotation("rotation",r2d*phi,0.0,0.0);          
      } else if(abs(_vrect.nz) < 0.001){
//          cout << "rectangle normal perp to z " << endl;
        rotation = new TGeoRotation("rotation",90+r2d*atan2(_vrect.ny,_vrect.nx),90.0,0.0);
      } else {
        cout << "error making rotation" << endl;
      }
      TGeoCombiTrans *transform = new TGeoCombiTrans(_vrect.cx,_vrect.cy,_vrect.cz,rotation);
      _detector->AddNode(rect,irect+1, transform);
    }
	}
}


void
PacDisplayMacro::DrawTracks() {
  static string types[NTRKTYPES] = {"g","s","r"};
  std::vector<PacDispTrackTraj>* trajs(0);
  std::vector<PacDispPoint>* points(0);
  std::string type;
  int color(0);
  for(unsigned itype=0;itype<NTRKTYPES;itype++){
    if(_type.find(types[itype]) != string::npos){
      if(types[itype] == "g"){
        trajs = _gtrajs;
        points = _gpoints;
        type = "generated";
        color = kGreen;
      } else if (types[itype]== "s"){
        trajs = _strajs;
        points = _spoints;
        type = "simulated";
        color = kBlue;
      } else if (types[itype]== "r"){
        trajs = _rtrajs;
        points = _rpoints;
        type = "reconstructed";
        color = kRed;
      }
//      cout << "Found " << trajs->size() <<" tracks of type " << types[itype] 
//      << " total n points = " << points->size() << endl;
// loop over the trajs
      for(unsigned itraj=0;itraj<trajs->size();itraj++){
        PacDispTrackTraj traj = (*trajs)[itraj];
        unsigned npts = max(0,traj.lastpt-traj.firstpt);
//        cout << "Drawing track " << itraj << " color "<< traj.itype
//        << " with first point " <<  traj.firstpt << " and last point " << traj.lastpt << endl;
        TPolyLine3D *tpolyline = new TPolyLine3D(npts);
        _trajs.push_back(tpolyline);
        tpolyline->SetUniqueID(1000*itype + itraj);
//        tpolyline->SetLineColor(traj.itype);
        tpolyline->SetLineColor(color);
        tpolyline->SetLineWidth(3);
// add the points for this trajectory
        for(unsigned ipt=0;ipt<npts;ipt++){
          unsigned jpt = ipt+traj.firstpt;
          if(jpt < points->size())
            tpolyline->SetPoint(ipt, (*points)[jpt].x, (*points)[jpt].y, (*points)[jpt].z);
          else
            cout << "Point index out-of-range!!" << endl;
        }
      }
    }
  }
  DrawTrajs();
} 

void
PacDisplayMacro::DrawTrajs() {
  for(unsigned itraj=0;itraj<_trajs.size();itraj++)
    _trajs[itraj]->Draw();
}  

void
PacDisplayMacro::drawSimHits() {
  for(unsigned ihit=0;ihit<_shits->size();ihit++){
    PacDispSimHit& dshit = (*_shits)[ihit];
    if(dshit.effect >= 3){
      TPolyMarker3D *simhit = new TPolyMarker3D(1);
      simhit->SetUniqueID(10000+ihit);
      simhit->SetPoint(0,dshit.point.x,dshit.point.y,dshit.point.z);
      switch (dshit.effect) {
        case 1:
        // shower
        simhit->SetMarkerColor(kBlack);
        break;
        case 3: case 4: case 8: case 9:
      // interaction
        simhit->SetMarkerColor(kOrange);
        break;
        case 5:
        // bend
        simhit->SetMarkerColor(kCyan);
        break;
        case 6:
        // normal
        simhit->SetMarkerColor(kBlue);
        break;
        case 7:
        // stops
        simhit->SetMarkerColor(kRed);
        break;
        case 10:
      // decay
        simhit->SetMarkerColor(kGreen);
        break;
        case 11: case 12:
      // shower
        simhit->SetMarkerColor(kBlack);
        break;            
        default:
        simhit->SetMarkerColor(kMagenta);
      }
      if(dshit.eloss > 0.01) {
        simhit->SetMarkerStyle(kFullCircle);    //bigger scalable
        simhit->SetMarkerSize(3);
      } else if(dshit.eloss > 0.005) {
        simhit->SetMarkerStyle(kFullCircle);    //bigger scalable
        simhit->SetMarkerSize(2);
      } else if(dshit.eloss > 0.001) {
        simhit->SetMarkerStyle(kFullCircle);    //bigger scalable
        simhit->SetMarkerSize(1);
      } else {
        simhit->SetMarkerStyle(kFullDotMedium); //bit bigger, nonscalable
      }
  //end hit effect
      _hmarks.push_back(simhit);
    }
  }
  DrawMarks();
}

void
PacDisplayMacro::DrawMarks() {
  for(unsigned imark=0;imark<_hmarks.size();imark++)
    _hmarks[imark]->Draw();
}

void PacDisplayMacro::DrawClusters() {
//  cout << "Found " << _clusts->size() << " clusters" << endl;
  for ( unsigned ic=0; ic<_clusts->size(); ic++) {
    PacDispCluster& clust = (*_clusts)[ic];
//    cout << "found cluster " << ic << " x,y,z = " 
//      << clust.x << ","
//      << clust.y << ","
//      << clust.z << " energy = " << clust.energy << endl;
    double dx= 0.5 * clust.rlocal * clust.dtheta;
    double dy= 0.5 * clust.rlocal * sin(clust.theta) * clust.dphi;
    double dz= 50 * clust.energy;  // Half of the tower height
    double xp= clust.x + dz * sin(clust.theta)*cos(clust.phi);
    double yp= clust.y + dz * sin(clust.theta)*sin(clust.phi);
    double zp= clust.z+ dz*cos(clust.theta);
    static char crystalname[STRINGLEN];
    snprintf(crystalname, STRINGLEN, "crystal_%04d",ic);
    TGeoVolume *crystal= _geom->MakeBox(crystalname, _Vacuum, dx,dy,dz);
    crystal->SetUniqueID(_objectID++);
    crystal->SetLineColor(kGreen);
    crystal->SetTransparency(_transparency/2.0);
    TGeoRotation *rotation = new TGeoRotation("rotation",clust.phi*r2d+90,clust.theta*r2d,90);
    TGeoCombiTrans *transform = new TGeoCombiTrans(xp,yp,zp,rotation);
    _event->AddNode(crystal,_objectID,transform);
    
  }
}

void
PacDisplayMacro::PrintLegend() {
	//---------------------------- Printout Output ------------------------------------
	std::cout << std::endl;
	std::cout << "********************** LEGEND ************************" << std::endl;
	std::cout << "Charge Track Color|Neutral Track Color                " << std::endl;
	std::cout << "------------------|-----------------------------------" << std::endl;
	std::cout << "                  |null         = White               " << std::endl;
	std::cout << "reco track = Red  |photon       = Red                 " << std::endl;
	std::cout << "generated = Green |K0L          = Blue                " << std::endl;
	std::cout << "simulated = Blue  |neutron      = Yellow              " << std::endl;
	std::cout << "                  |anti-neutron = Magenta             " << std::endl;
	std::cout << "                  |                                   " << std::endl;
	std::cout << "                  |                                   " << std::endl;
	std::cout << "Curved Line       |Straight Line                      " << std::endl;
	std::cout << std::endl;
	//std::cout << "********************** LEGEND ************************" << std::endl;
	//std::cout << "DetEffect Color   |Material Color         (Color Code)" << std::endl;
	//std::cout << "------------------|-----------------------------------" << std::endl;
	//std::cout << "normal   = Red    |pep-BPipeB     = Light Purple  (40)" << std::endl;
	//std::cout << "stop     = Green  |svt-Silicon    = Tan           (41)" << std::endl;
	//std::cout << "interact = Blue   |dch-Beryllium  = Light Orange  (42)" << std::endl;
	//std::cout << "brems    = Yellow |dch-He-Ibu-Wir = Light Brown   (43)" << std::endl;
	//std::cout << "compt    = Magenta|dch-CFiber     = Dark Tan      (44)" << std::endl;
	//std::cout << "convert  = Cyan   |drc-Quartz     = Reddish Brown (45)" << std::endl;
	//std::cout << "shower   = White  |emc-CsI        = Crimson       (46)" << std::endl;
	//std::cout << "                  |ifr-Aluminum   = Gray Purple   (47)" << std::endl;
	//std::cout << "Cross             |ifr-Iron       = Reddish Purple(48)" << std::endl;
	//std::cout << std::endl;
}

PacDisplayMacro::PacDisplayMacro(const char* file,string s,int transparency,int startevent,
  double rmax, double zmin, double zmax) :
  _gtrajs(new std::vector<PacDispTrackTraj>),
  _gpoints(new std::vector<PacDispPoint>),
  _strajs(new std::vector<PacDispTrackTraj>),
  _spoints(new std::vector<PacDispPoint>),
  _rtrajs(new std::vector<PacDispTrackTraj>),
  _rpoints(new std::vector<PacDispPoint>),
  _shits(new std::vector<PacDispSimHit>),
  _clusts(new std::vector<PacDispCluster>) {
	//---------------- Read File, Decide which events to read -----------------------
	_tfile = new TFile(file);
    	if (_tfile->IsZombie()) {
		cout << "Cannot find file" << endl;
		return;
	}
  _frame = new TGMainFrame();
// find the trees
  _materials = (TTree*)_tfile->Get("materials");
  _cylinders = (TTree*)_tfile->Get("cylinders");
  _rings = (TTree*)_tfile->Get("rings");
  _rects = (TTree*)_tfile->Get("rects");
  _cones = (TTree*)_tfile->Get("cones");
  _gentrks = (TTree*)_tfile->Get("gentracks");
  _simtrks = (TTree*)_tfile->Get("simtracks");
  _rectrks = (TTree*)_tfile->Get("rectracks");
  _simhits = (TTree*)_tfile->Get("simhits");
  _clusters = (TTree*)_tfile->Get("clusters");
  _vcylinders = (TTree*)_tfile->Get("vcylinders");
  _vrings = (TTree*)_tfile->Get("vrings");
  _vrects = (TTree*)_tfile->Get("vrects");
//  
  _maxEv = _simtrks->GetEntries()-1;
// setup the branches
  _materials->SetBranchAddress("material",&_dmat.zeff,&_matbr);
  _cylinders->SetBranchAddress("cylinder",&_dcyl.radius,&_cylbr);
  _rings->SetBranchAddress("ring",&_dring.z,&_ringbr);
  _rects->SetBranchAddress("rect",&_drect.cx,&_rectbr);
  _cones->SetBranchAddress("cone",&_dcone.tanTheta,&_conebr);
// voxels
  _vcylinders->SetBranchAddress("vcylinder",&_vcyl.radius,&_vcylbr);
  _vrings->SetBranchAddress("vring",&_vring.z,&_vringbr);
  _vrects->SetBranchAddress("vrect",&_vrect.cx,&_vrectbr);
// generated tracks
  _gentrks->SetBranchAddress("trajectory",&_gtrajs);
  _gentrks->SetBranchAddress("trajpoint",&_gpoints);
// simulated tracks
  _simtrks->SetBranchAddress("trajectory",&_strajs);
  _simtrks->SetBranchAddress("trajpoint",&_spoints);
// reconstructed tracks
  _rectrks->SetBranchAddress("trajectory",&_rtrajs);
  _rectrks->SetBranchAddress("trajpoint",&_rpoints);
// simhits
  _simhits->SetBranchAddress("simhit",&_shits);
// clusters
  _clusters->SetBranchAddress("cluster", &_clusts);
//--------------------------- Set Default Values -----------------------------------
	_type = s;
  _currentEv = startevent;
	_objectID = 0; 
	_transparency = transparency;
  _rmax = rmax;
  _zmin = zmin;
  _zmax = zmax;
	// Main test window.
   _frame->SetCleanup(kDeepCleanup);
   gSystem->Load("libGeom");
   
	//---------------------------- Create Canvas ------------------------------------
	_can = new TCanvas("PacDisplayMacro");
	_can->cd();
  _can->Iconify();
  _viewer = (TGLViewer *)gPad->GetViewer3D("ogl");
	//Print Legend
	PrintLegend();
	
	//Enable Clicking feature
	if (gROOT->GetVersionInt() >= 52000) {
		_viewer->Connect("Clicked(TObject*)","PacDisplayMacro", this, "OnClicked(TObject*)");
	}

	//Set Camera Refreshing option such that cameras DO NOT refresh when new events are selected
//	_viewer->SetResetCameraOnDoubleClick(kFALSE);
	_viewer->SetResetCamerasOnUpdate(kFALSE);	
   
   // Controls on right
   TGVerticalFrame *controls = new TGVerticalFrame(_frame,500);
   _frame->AddFrame(controls, new TGLayoutHints(kLHintsRight | kLHintsExpandY, 5, 5, 5, 5));

   TGTextButton *nextevt = new TGTextButton(controls, "Next");
   controls->AddFrame(nextevt, new TGLayoutHints(kLHintsExpandX, 0, 0, 0, 5));
   TQObject::Connect(nextevt, "Clicked()","PacDisplayMacro", this, "Next()");
      
   TGTextButton *prevevt = new TGTextButton(controls, "Previous");
   controls->AddFrame(prevevt, new TGLayoutHints(kLHintsExpandX, 0, 0, 0, 5));
   TQObject::Connect(prevevt, "Clicked()","PacDisplayMacro", this, "Prev()");
   
   TGTextButton *quit = new TGTextButton(controls, "Quit");
   controls->AddFrame(quit, new TGLayoutHints(kLHintsBottom | kLHintsExpandX, 0, 0, 0, 5));
   TQObject::Connect(quit, "Clicked()", "TApplication", gApplication, "Terminate()");

   _frame->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
   _frame->DontCallClose();
                             
   _frame->MapSubwindows();
   _frame->Resize();

   _frame-> SetWMSizeHints(_frame->GetDefaultWidth(), _frame->GetDefaultHeight(), 1000, 1000, 200 ,0);
   _frame->SetWindowName("Button Test");
   _frame->MapRaised();

// create geometry
   _geom = new TGeoManager("simple", "Simple geometry");
   _geom->SetVisLevel(4);
   _matVacuum = new TGeoMaterial("Vacuum", 0,0,0);
   _Vacuum = new TGeoMedium("vacuum",1, _matVacuum);
   _top = _geom->MakeBox("Top", _Vacuum, 500.,500.,500.);
   _top->SetVisibility(kFALSE);
   _geom->SetTopVolume(_top); // mandatory!
   _detector = _geom->MakeBox("Detector", _Vacuum, 500.,500.,500.);
   _detector->SetVisibility(kFALSE);
   _top->AddNode(_detector,1, new TGeoTranslation(0,0,0));
// add empty event for now
   _event = _geom->MakeBox("Event", _Vacuum, 500.,500.,500.);
   _event->SetVisibility(kFALSE);
   _top->AddNode(_event,1, new TGeoTranslation(0,0,0));
   Draw();
}

void
PacDisplayMacro::Draw(){
// draw the detector
   if(_type.find("d") != string::npos)DrawDetector();
// if requested, draw voxels
   if(_type.find("v") != string::npos)DrawVoxels();
// draw the first event
   DrawEvent();
}


void
PacDisplayMacro::fillMaterial(int imat,std::string& matname,TGeoMaterial*& material,TGeoMedium*& medium,int& dcolor) {
  static std::map<std::string,TGeoMaterial*> matmap;
  static std::map<std::string,TGeoMedium*> medmap;
  matname = std::string(_dmat.matname);
  if(matmap.find(matname) != matmap.end()){
    material = matmap[matname];
    medium = medmap[matname];
  } else {
    cout << "found new material name " << matname << " zeff = " << _dmat.zeff << " density " << _dmat.density << endl;
    material = new TGeoMaterial(matname.c_str(),_dmat.aeff,(int)_dmat.zeff,_dmat.density,_dmat.radlen,_dmat.intlen);
    medium = new TGeoMedium(matname.c_str(),imat, material);
    matmap[matname] = material;
    medmap[matname] = medium;
  }
//  int icolor = (int)rint(100*(1+log(material->GetDensity())));
//  dcolor = gStyle->GetColorPalette((int)rint(_dmat.zeff*0.9));
  dcolor = gStyle->GetColorPalette((int)rint(_dmat.aeff*0.75));
//  dcolor = (int)rint(_dmat.aeff*0.6);
//  int icolor = (int)rint(2*log(material->GetDensity()));
//  dcolor = kCyan+icolor;
}

void
PacDisplayMacro::loadTrees() {
  _gentrks->LoadTree(_currentEv);
  _gentrks->GetEntry(_currentEv);
  _simtrks->LoadTree(_currentEv);
  _simtrks->GetEntry(_currentEv);
  _rectrks->LoadTree(_currentEv);
  _rectrks->GetEntry(_currentEv);
  _simhits->LoadTree(_currentEv);
  _simhits->GetEntry(_currentEv);
  _clusters->LoadTree(_currentEv);
  _clusters->GetEntry(_currentEv);	
}
