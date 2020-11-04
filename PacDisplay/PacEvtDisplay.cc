#include "BaBar/BaBar.hh"
#include "PacDisplay/PacEvtDisplay.hh"
#include "AbsEnv/AbsEnv.hh"
#include "PacEmc/PacEmcCluster.hh"
#include "PacEmc/PacReconstructEmc.hh"
#include "PacEnv/PacConfig.hh"
#include "PacDetector/PacCylDetector.hh"
#include "PacGeom/PacCylDetElem.hh"
#include "PacGeom/PacCylDetType.hh"
#include "PacGeom/PacDetector.hh"
#include "PacGeom/PacHelix.hh"
#include "PacGeom/PacPieceTraj.hh"
#include "PacGeom/PacPlaneDetElem.hh"
#include "PacGeom/PacConeDetElem.hh"
#include "PacGeom/PacConeDetType.hh"
#include "PacGeom/PacRectDetType.hh"
#include "PacGeom/PacRingDetType.hh"
#include "PacGeom/PacCylVoxelSet.hh"
#include "DetectorModel/DetElemList.hh"
#include "DetectorModel/DetMaterial.hh"
#include "DetectorModel/DetSet.hh"
#include "PacSim/PacSimTrack.hh"
#include "PacSim/PacSimHit.hh"
#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"
#include "PDT/PdtPid.hh"
#include "ProxyDict/Ifd.hh"
#include "TrajGeom/TrkLineTraj.hh"
#include "TrkBase/TrkExchangePar.hh"
#include "TrkBase/TrkHelixUtils.hh"
#include "TrkEnv/TrkEnv.hh"
#include "AbsCalo/AbsRecoCalo.hh"
#include "G3Data/GVertex.hh"
#include "G3Data/GTrack.hh"
#include "GenEnv/GenEnv.hh"
#include "TrkBase/TrkRecoTrk.hh"
#include "KalmanTrack/KalRep.hh"
#include "KalmanTrack/KalInterface.hh"
#include "PacEmc/PacEmcCluster.hh"
#include "PacEmc/PacReconstructEmc.hh"
#include "TrajGeom/TrkLineTraj.hh"
#include "TrkBase/TrkExchangePar.hh"
#include "TrkBase/TrkHelixUtils.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "BField/BField.hh"

#include <TParticle.h>
#include <TParticlePDG.h>


PacEvtDisplay::PacEvtDisplay(){
}

PacEvtDisplay::~PacEvtDisplay(){
}


void
PacEvtDisplay::init(const char* filename,unsigned resolution,bool verbose) {
  _resolution = resolution;
  _verbose=verbose;
  _dispfile = new TFile(filename,"RECREATE");
  // setup the trees;
  // materials
  _materials = new TTree("materials","materials");
  _materials->Branch("material",&_dmat.zeff,PacDispMat::rootnames());
  // cylinders
  _cylinders = new TTree("cylinders","cylinders");
  _cylinders->Branch("cylinder",&_dcyl.radius,PacDispCyl::rootnames());
  // rings	
  _rings = new TTree("rings","rings");
  _rings->Branch("ring",&_dring.z,PacDispRing::rootnames());
  // rectangles
  _rects = new TTree("rects","rects");
  _rects->Branch("rect",&_drect.cx,PacDispRect::rootnames());
  // cones	
  _cones = new TTree("cones","cones");
  _cones->Branch("cone",&_dcone.tanTheta,PacDispCone::rootnames());
  // generated tracks
  _gentrks = new TTree("gentracks","gentracks");
  _gentrks->Branch("trajectory",&_gtrajs);
  _gentrks->Branch("trajpoint",&_gpoints);
  // simulated tracks
  _simtrks = new TTree("simtracks","simtracks");
  _simtrks->Branch("trajectory",&_strajs);
  _simtrks->Branch("trajpoint",&_spoints);
  // reconstructed tracks
  _rectrks = new TTree("rectracks","rectracks");
  _rectrks->Branch("trajectory",&_rtrajs);
  _rectrks->Branch("trajpoint",&_rpoints);
  // simhits
  _simhits = new TTree("simhits","simhits");
  _simhits->Branch("simhit",&_shits);
  // clusters
  _clusters= new TTree("clusters","clusters");
  _clusters->Branch("cluster",&_clusts);
  // voxel cylinders
  _vcylinders = new TTree("vcylinders","vcylinders");
  _vcylinders->Branch("vcylinder",&_vcyl.radius,PacDispCyl::rootnames());
  // voxel rings
  _vrings = new TTree("vrings","vrings");
  _vrings->Branch("vring",&_vring.z,PacDispRing::rootnames());
  // voxel rectangles
  _vrects = new TTree("vrects","vrects");
  _vrects->Branch("vrect",&_vrect.cx,PacDispRect::rootnames());
// get the detector
  _detector = dynamic_cast<PacCylDetector*>(Ifd<PacDetector>::get(gblPEnv, "Tracking Det" ));
  if(_detector  == 0)
    cerr << "Couldn't find detector!!!! " << endl;
}

void
PacEvtDisplay::finalize(){
  // store objects and close file
  _dispfile->Write();
  _dispfile->Close();
}

void
PacEvtDisplay::reset() {
  // clear out storage from last event
  _gtrajs.clear();
  _strajs.clear();
  _rtrajs.clear();
  _gpoints.clear();
  _spoints.clear();
  _rpoints.clear();
  _shits.clear();
  _clusts.clear();
}

void
PacEvtDisplay::drawDetector(){
//Get Detector Information
  DetElemList elements; // list of DetectorElements
  _detector->detectorModel()->listAllElements(elements);
  DetElemList::const_iterator ielem = elements.begin();
  TwoDCoord* nullpoint = new TwoDCoord(0.0,0.0); 
  while(ielem != elements.end()){
// only draw elements without large gaps
    static double maxgap=0.5;
    const PacDetElem* elem;
    if( (elem = dynamic_cast<const PacDetElem*>(*ielem)) != 0 &&
    elem->gapFraction() < maxgap) {
      const PacCylDetElem* celem;
      const PacPlaneDetElem* pelem;
      const PacConeDetElem* coelem;
      if((celem = dynamic_cast<const PacCylDetElem*>(*ielem)) != 0) {
        const PacCylDetType* ctype = celem->cylType();
        _dcyl.radius = celem->radius();
        _dcyl.lowZ = ctype->lowZ();
        _dcyl.hiZ = ctype->hiZ();
        _dcyl.thickness = ctype->thick();
        const DetMaterial* detmat = &(ctype->material(nullpoint));
        if(detmat != 0){
          _dcyl.imat = fillMat(detmat);
          _cylinders->Fill();
        }
      } else if ((pelem = dynamic_cast<const PacPlaneDetElem*>(*ielem)) != 0) {
      // plane: decide if a ring or a rectangle
        const PacRingDetType* ringtype = dynamic_cast<const PacRingDetType*>(pelem->planeType());
        const PacRectDetType* recttype = dynamic_cast<const PacRectDetType*>(pelem->planeType());
        if(ringtype != 0){
          _dring.z = pelem->midpoint().z();
          _dring.thickness = ringtype->thick();
          _dring.radlow = ringtype->lowrad();
          _dring.radhigh = ringtype->highrad();
          const DetMaterial* detmat = &(ringtype->material(nullpoint));
          if(detmat != 0){
            _dring.imat = fillMat(detmat);
            _rings->Fill();
          }
        } else if(recttype != 0){
          _drect.cx = pelem->midpoint().x();
          _drect.cy = pelem->midpoint().y();
          _drect.cz = pelem->midpoint().z();
          _drect.nx = pelem->normal().x();
          _drect.ny = pelem->normal().y();
          _drect.nz = pelem->normal().z();
          _drect.ux = pelem->udir().x();
          _drect.uy = pelem->udir().y();
          _drect.uz = pelem->udir().z();
          _drect.thickness = recttype->thick();
          _drect.usize = recttype->uSize();
          _drect.vsize = recttype->vSize();
          const DetMaterial* detmat = &(recttype->material(nullpoint));
          if(detmat != 0){
            _drect.imat = fillMat(detmat);
            _rects->Fill();
          }
        }
      } else if ((coelem = dynamic_cast<const  PacConeDetElem*>(*ielem)) != 0) {
        _dcone.tanTheta = coelem->tanTheta();
        _dcone.zVertex = coelem->zVertex();
        const PacConeDetType* cotype = coelem->coneType();
        _dcone.thick = cotype->thick();
        _dcone.lowR = cotype->lowR();
        _dcone.hiR = cotype->hiR();				
        const DetMaterial* detmat = &(cotype->material(nullpoint));	
        if(detmat != 0){
          _dcone.imat = fillMat(detmat);
          _cones->Fill();
        }
      } else {
        cout << "Error: unknown element " << *ielem << endl;
      }
    }
    ielem++;			
  }
}

int
PacEvtDisplay::fillMat(const DetMaterial* detmat) {
  std::string matname = *(detmat->name());
  std::map<std::string,int>::const_iterator ifound = _matlist.find(matname);
  if(ifound == _matlist.end()){
// new material
    int imat = _matlist.size();
    _matlist[matname] = imat;
    if (_verbose)cout << "found new material " << matname << " ID # = " << imat << endl;
    _dmat.zeff = detmat->zeff();
    _dmat.aeff = detmat->aeff();
    _dmat.density = detmat->density();
    _dmat.radlen = detmat->radiationLength();
    _dmat.intlen = detmat->intLength();
    snprintf(_dmat.matname,STRINGSIZE,"%s",matname.c_str());
    _materials->Fill();
  }
  return _matlist[matname];
}

void
PacEvtDisplay::drawVoxels(){
//Get Detector Information
  DetElemList elements; // list of DetectorElements
  const std::vector<PacCylVoxelSet*>& vsets = _detector->voxelSets();
  for(std::vector<PacCylVoxelSet*>::const_iterator ivset = vsets.begin();ivset!=vsets.end();ivset++){
    const std::vector<PacCylDetElem*>& relems = (*ivset)->relems();
    const std::vector<PacPlaneDetElem*>& zelems = (*ivset)->zelems();
    const std::vector<PacPlaneDetElem*>& felems = (*ivset)->felems();
    for(std::vector<PacCylDetElem*>::const_iterator icyl=relems.begin();icyl != relems.end();icyl++){
      const PacCylDetElem* celem = *icyl;
      if(celem != 0){
        const PacCylDetType* ctype = celem->cylType();
        _vcyl.radius = celem->radius();
        _vcyl.lowZ = ctype->lowZ();
        _vcyl.hiZ = ctype->hiZ();
        _vcyl.thickness = ctype->thick();
        _vcyl.imat = 0;
        _vcylinders->Fill();
      }
    }
    for(std::vector<PacPlaneDetElem*>::const_iterator iring=zelems.begin();iring!=zelems.end();iring++){
      const PacPlaneDetElem* relem = *iring;
      if(relem != 0){
        const PacRingDetType* ringtype = dynamic_cast<const PacRingDetType*>(relem->planeType());
        if(ringtype != 0){
          _vring.z = relem->midpoint().z();
          _vring.thickness = ringtype->thick();
          _vring.radlow = ringtype->lowrad();
          _vring.radhigh = ringtype->highrad();
          _vring.imat = 0;
          _vrings->Fill();
        } else {
          cout << "Z element is not a ring! " << endl;
        }
      }
    }
    for(std::vector<PacPlaneDetElem*>::const_iterator iphi=felems.begin();iphi!=felems.end();iphi++){
      const PacPlaneDetElem* felem = *iphi;
      if(felem != 0){
        const PacRectDetType* recttype = dynamic_cast<const PacRectDetType*>(felem->planeType());
        if(recttype != 0){
          _vrect.cx = felem->midpoint().x();
          _vrect.cy = felem->midpoint().y();
          _vrect.cz = felem->midpoint().z();
          _vrect.nx = felem->normal().x();
          _vrect.ny = felem->normal().y();
          _vrect.nz = felem->normal().z();
          _vrect.ux = felem->udir().x();
          _vrect.uy = felem->udir().y();
          _vrect.uz = felem->udir().z();
          _vrect.thickness = recttype->thick();
          _vrect.usize = recttype->uSize();
          _vrect.vsize = recttype->vSize();
          _vrect.imat = 0;
          _vrects->Fill();
        } else {
          cout << "Phi element is not a rectangle! " << endl;
        }
      }
    }
  }
}

void
PacEvtDisplay::drawGTrack(const GTrack* gentrk,double range,const BField* bfield) {
	// get GTrack information
	const PdtEntry* pdt = gentrk->pdt();
	assert(pdt != 0);
	Hep3Vector mom = gentrk->p4().vect();
	const GVertex* gvtx = gentrk->vertex();
	HepPoint pos;
	if(gvtx != 0)
		pos = gvtx->position();
	else
		cout << "No GVertex found for GTrack!!" << endl;
	// set traj parameters
  int pid(0);
	int charge = (int)pdt->charge();
	if(charge == 0) {
		pid = (int)pdt->pidNeutId();
	} else {
		pid = pdt->pidId();
	}	
  _gtrajs.push_back(PacDispTrackTraj(pdt->lundId(),charge,kGreen,_gpoints.size(),_gpoints.size()+_resolution));
	//check for magnetic field
	bool hasfield = bfield != 0 && fabs(bfield->bFieldZ(pos)) > 0.0;
	// charged particles
	if(charge != 0 && hasfield ){
	  // temp data
    HepVector hpars(5);
    double dcharge = pdt->charge();
    double localflt(0.0);				
	// compute helix parameters and create helix trajectory
		TrkHelixUtils::helixFromMom(hpars,localflt,pos,mom,dcharge,*bfield);
		PacHelix helix(hpars,localflt,range);
		fillTrajPoints(&helix,_gpoints);
	} else {
		// neutral particle, or no BField; create line traj
		TrkLineTraj line(pos,mom,range);
		fillTrajPoints(&line,_gpoints);
	}
}

void
PacEvtDisplay::drawParticle(const TParticle* tpart,double range,const BField* bfield) {
  Hep3Vector mom(tpart->Px(),tpart->Py(),tpart->Pz());
  HepPoint pos(tpart->Vx(),tpart->Vy(),tpart->Vz());
  int pid = tpart->GetPdgCode();
  const TParticlePDG* pdg = (const_cast<TParticle*>(tpart))->GetPDG();
  int charge(0);
  if(pdg != 0)
    charge = 3*pdg->Charge();
  _gtrajs.push_back(PacDispTrackTraj(pid,charge,kGreen,_gpoints.size(),_gpoints.size()+_resolution));
	//check for magnetic field
	bool hasfield = bfield != 0 && fabs(bfield->bFieldZ(pos)) > 0.0;
	// charged particles
	if(charge != 0 && hasfield ){
	  // temp data
    HepVector hpars(5);
    double localflt(0.0);				
	// compute helix parameters and create helix trajectory
		TrkHelixUtils::helixFromMom(hpars,localflt,pos,mom,charge,*bfield);
		PacHelix helix(hpars,localflt,range);
		fillTrajPoints(&helix,_gpoints);
	} else {
		// neutral particle, or no BField; create line traj
		TrkLineTraj line(pos,mom,range);
		fillTrajPoints(&line,_gpoints);
	}
}


void
PacEvtDisplay::drawSimTrack(const PacSimTrack* simtrk) {
  const PdtEntry* pdt = simtrk->pdt();
	assert(pdt != 0);
  // set traj parameters
  int pid(0);
	int charge = (int)pdt->charge();
	if(charge == 0) {
		pid = (int)pdt->pidNeutId();
	} else {
		pid = pdt->pidId();
	}
  _strajs.push_back(PacDispTrackTraj(pdt->lundId(),charge,kBlue,_spoints.size(),_spoints.size()+_resolution));
  const PacPieceTraj* simtraj = simtrk->getTraj();
  fillTrajPoints(simtraj,_spoints);
}

void
PacEvtDisplay::drawRecTrack(const TrkRecoTrk* rectrk) {
  int pid = (int)rectrk->defaultType();
//Get Reconstructed Track data
  KalInterface kinter;
  rectrk->attach(kinter,rectrk->defaultType());
  const KalRep* kalrep = kinter.kalmanRep();
  int charge = kalrep->charge();
  _rtrajs.push_back(PacDispTrackTraj(pid,charge,kRed,_rpoints.size(),_rpoints.size()+_resolution));
  const TrkDifPieceTraj& rectraj = kalrep->pieceTraj();
  fillTrajPoints(&rectraj,_rpoints);
}

void
PacEvtDisplay::drawSimHits(const PacSimTrack* track,unsigned istrk) {
	const std::vector<PacSimHit>& points = track->getHitList();
  PacDispSimHit dhit;
  dhit.isimtrk = istrk;
	for(int i = 0; i < points.size(); i++) {
		dhit.point.x = points[i].position().x();
		dhit.point.y = points[i].position().y();
		dhit.point.z = points[i].position().z();
		dhit.effect = points[i].detEffect();
		dhit.eloss = points[i].momentumIn().mag()-points[i].momentumOut().mag();
		_shits.push_back(dhit);
	}
}

void
PacEvtDisplay::drawCluster(const AbsRecoCalo *calo) {
  const PacEmcCluster *cluster= dynamic_cast<const PacEmcCluster*>(calo);
  PacEmcModel *model=  PacEmcModel::getModel();

  PacDispCluster dclust;
  for ( unsigned i=0; i< cluster->pacEmcDigis()->size(); i++) {
    const PacEmcDigi *digi= (*cluster->pacEmcDigis())[i];

    // w.r.t. EMC geometry (0,0,0)
    dclust.theta= digi->whereLocal().theta();
    dclust.phi= digi->whereLocal().phi();
    dclust.ith= digi->theta();
    dclust.iph= digi->phi();
    dclust.rlocal= digi->whereLocal().mag();
    dclust.dtheta= model->deltaTheta( digi->theta() );
    dclust.dphi= model->deltaPhi( digi->theta() );

    // global coordinate
    HepPoint pos= digi->where();
    dclust.x= pos.x();
    dclust.y= pos.y();
    dclust.z= pos.z();
    dclust.energy= digi->energy();

    _clusts.push_back(dclust);
  }
}

void
PacEvtDisplay::fillTrees(){
  _gentrks->Fill();
  _simtrks->Fill();
  _rectrks->Fill();
  _simhits->Fill();
  _clusters->Fill();
}

void
PacEvtDisplay::fillTrajPoints(const Trajectory* traj, std::vector<PacDispPoint>& tpoints) {
	double smin = traj->lowRange();
	double smax = traj->hiRange();
	double step = (smax - smin)/_resolution;
	for(unsigned n = 0; n < _resolution; n++) {		
    double s = smin + n*step;
		HepPoint p = traj->position(s);
    tpoints.push_back(PacDispPoint(p.x(),p.y(),p.z()));
	}
}
