#include "BaBar/BaBar.hh"
#include "PacEnv/PacBuildEnv.hh"
#include "PacEnv/PacConfig.hh"

#include "AbsEnv/AbsEnv.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "TrkEnv/KalContext.hh"
#include "TrkEnv/TrkEnv.hh"
#include "PDT/Pdt.hh"
#include "PacGeom/PacCylBField.hh"
#include "PacGeom/PacGradBField.hh"
#include "ProxyDict/Ifd.hh"
#include "TrkGeom/TrkCentralVolume.hh"
#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"
#include "ProxyDict/IfdDataProxy.hh"
#include "BdbTime/BdbTime.hh"

#include "MatEnv/MatElementList.hh"
#include "MatEnv/MatIsotopeList.hh"
#include "MatEnv/MatMaterialList.hh"
#include "MatEnv/MatDBInfo.hh"
#include "Framework/AppFileName.hh"
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RanecuEngine.h"

TrkBuildCoreEnv::TrkBuildCoreEnv() : _trkenv(0), _genenv(0), _bfield(0), _matinfo(0), _context(0), _inner_volume(0), _outer_volume(0), _rng(0){
}

void 
TrkBuildCoreEnv::buildCore() {
	_genenv = new GenEnv();
	gblEnv->setGen(_genenv);
	//Put proxies into the environment
  BdbTime* toUse = new BdbTime; // current program time
	_genenv->setEventTime(toUse);
/* Initialize particle data */

	std::string isotopes = gconfig.getcstr("Materials.isotopes","PacEnv/IsotopesList.data");
	std::string elements = gconfig.getcstr("Materials.elements","PacEnv/ElementsList.data");
	std::string materials = gconfig.getcstr("Materials.materials","PacEnv/MaterialsList.data");

	Pdt::readMCppTable(AppFileName("PDT/pdt.table").pathname().c_str());

	// Materials;
	_matinfo = new MatDBInfo();
	_genenv->materialInfo() = _matinfo;

	//Put proxies into the environment.  I'm overriding the usual CDB ones so that we can edit
	// materials directly in the ascii files.
	IfdDataProxyTemplate<MatIsotopeList> * isoProxy = 
	  new IfdDataProxy<MatIsotopeList>(new MatIsotopeList(AppFileName(isotopes.c_str()).pathname()));
	assert(isoProxy != 0);
	bool iput = Ifd<MatIsotopeList>::put(gblPEnv, isoProxy);
	assert(iput);

	IfdDataProxyTemplate<MatElementList> * elmProxy = 
		new IfdDataProxy<MatElementList>(new MatElementList(AppFileName(elements.c_str()).pathname()));
	assert(elmProxy != 0);
	iput = Ifd<MatElementList>::put(gblPEnv, elmProxy);
	assert(iput);

	IfdDataProxyTemplate<MatMaterialList> * mtrProxy = 
		new IfdDataProxy<MatMaterialList>(new MatMaterialList(AppFileName(materials.c_str()).pathname()));
	assert(mtrProxy != 0);
	iput = Ifd<MatMaterialList>::put(gblPEnv, mtrProxy);
	assert(iput);

  // setup random number generator.  Note that the seed is initialized to a fixed value here, it _must_ be
  // controlled externally to guarantee independent results
  _rng = 0;
  // set the CLHEP static to be this engine (this is not the preferred access mechanism, but it works)
  HepRandom::setTheEngine ( _rng );
  HepRandomEngine* test = HepRandom::getTheEngine();
  assert(test == 0);
}

void
TrkBuildCoreEnv::buildTrk() {
	
	_trkenv = new TrkEnv();
	gblEnv->setTrk(_trkenv);

// BField
/* Construct the B field and detector */
	double B_nom = gconfig.getdouble("PacField.B_nom");
	double gradient = gconfig.getdouble("PacField.gradient",0.0);
	double zg1 = gconfig.getdouble("PacField.zg1",0.0);
	double zg2 = gconfig.getdouble("PacField.zg2",0.0);
	
	Hep3Vector DeltaB = gconfig.getHep3Vector("PacField.DeltaB");
	Hep3Vector B(DeltaB.x(), DeltaB.y(), B_nom + DeltaB.z());
	double radius = gconfig.getdouble("PacField.B_radius");
	double zlow = gconfig.getdouble("PacField.B_zlow");
	double zhi = gconfig.getdouble("PacField.B_zhi");
	if(gradient == 0.0)
	  _bfield = new	PacCylBField(B,radius,zlow,zhi,-DeltaB.z());
	else{
    assert(zg2 < 0.0 || zg1 > 0.0);
    double b1,b2;
	  if(zg2 < 0.0){
      b1 = B_nom + (zg1-zg2)*gradient;
      b2 = B_nom;
    } else {
      b2 = B_nom + (zg2-zg1)*gradient;
      b1 = B_nom;
    }
	  _bfield = new	PacGradBField(b1,b2,zg1,zg2,radius,zlow,zhi);
  }
	assert(_bfield != 0);
	bool iput = Ifd<BField>::put(gblPEnv,_bfield,"Default");
	assert(iput);

		// KalContext
	_context = new KalContext;
	PacConfig tconfig = gconfig.getconfig("TrackReconstructor.");

	_context->setDistanceTolerance(tconfig.getdouble("distancetolerance",1.0));
	_context->setIntersectionTolerance(tconfig.getdouble("intersectiontolerance",1.0));
	_context->setMaxParamDiff(trkIn, tconfig.getdouble("maxparamdiff",1.0));
	_context->setMaxParamDiff(trkOut, tconfig.getdouble("maxparamdiff",1.0));
	_context->setMaxIterations(tconfig.getint("maxiterations",0));
	_context->setMaxIntersections(tconfig.getint("maxintersections",0));
	_context->setSmearFactor(tconfig.getdouble("smearfactor",1e8));
	_context->setLocalSiteDMom(tconfig.getdouble("localsitedmom",0.01));
	_context->setLocalSiteDeflect(tconfig.getdouble("localsitedeflect",0.01));
	_context->setMinGap(tconfig.getdouble("mingap",0.1));
	_context->setTrajBuffer(tconfig.getdouble("trajbuffer",0.1));
	_context->setMaxDMom(tconfig.getdouble("maxdmom",0.3));
	_context->setMaxSiteDMom(tconfig.getdouble("maxsitedmom",0.25));
	_context->setMinDOF(tconfig.getint("minxydof",3), TrkEnums::xyView);
	_context->setMinDOF(tconfig.getint("minzdof",2), TrkEnums::zView);
	_context->setMinDOF(tconfig.getint("mindof",1), TrkEnums::bothView);
	_context->setMaxMomDiff(tconfig.getdouble("maxmomdiff",0.0005));
	_context->setStopHots(tconfig.getbool("stophots",false));
	_context->setForbidAmbigFlips(tconfig.getbool("forbidambigflips",true));
	_context->setMomUpdateFactor(tconfig.getdouble("momupdatefactor",1.0));
	_context->setDefaultType((PdtPid::PidType)tconfig.getint("defaulttype",PdtPid::null));


		/* BFieldIntegrator */
	_context->setBFieldIntegration(
		tconfig.getdouble("minstep", 1.0),
		tconfig.getdouble("maxstep", 10.0),
		tconfig.getdouble("maxfrac", 2.0),
		tconfig.getdouble("tolerance", 1.0),
		tconfig.getdouble("divminstep", 2.0),
		tconfig.getdouble("divmaxstep", 20.0),
		tconfig.getdouble("divmaxfrac", 2.0),
		tconfig.getdouble("divtolerance", 2.0)
		);

	_context->setBendSites(tconfig.getbool("bend_sites", false));
	_context->setMaterialSites(tconfig.getbool("material_sites", true));
	_trkenv->_theKalContext = _context;

	// origin
	_trkenv->_theOrigin = HepPoint(0.0,0.0,0.0);

	/* Create detector volume that includes the beampipe */
	double inner_radius = tconfig.getdouble("inner_radius", 2);
	double outer_radius = tconfig.getdouble("outer_radius", 100);
	double inner_zmin = tconfig.getdouble("inner_zmin", -100);
	double inner_zmax = tconfig.getdouble("inner_zmax", +100);
	double outer_zmin = tconfig.getdouble("outer_zmin", -100);
	double outer_zmax = tconfig.getdouble("outer_zmax", +100);
	_inner_volume = new TrkCentralVolume("Inner Volume", inner_radius, inner_zmin, inner_zmax);
	_outer_volume = new TrkCentralVolume("Outer Volume", outer_radius, outer_zmin, outer_zmax);
		//    _context->setVolume(trkIn, inner_volume);
		//    _context->setVolume(trkOut, outer_volume);
	std::vector< TrkVolumeHandle >* vols = new std::vector< TrkVolumeHandle >(2);
	Ifd< std::vector< TrkVolumeHandle > >::put( gblPEnv,vols,"Tracking Volumes" );
	vols->push_back(TrkVolumeHandle(_inner_volume,TrkVolumeHandle::beampipe));
	vols->push_back(TrkVolumeHandle(_outer_volume,TrkVolumeHandle::dch));

}

TrkBuildCoreEnv::~TrkBuildCoreEnv() {
  delete _trkenv;
  delete _genenv;
  delete _matinfo;
// these are deleted by Ifd
//  delete _context;
//	delete _bfield;
  delete _inner_volume;
  delete _outer_volume;
  delete _rng;
}
