/*******************************************************************************
* PacMeasurementFactory
  *******************************************************************************/

#include "BaBar/BaBar.hh"

#include "PacDetector/PacMeasurementFactory.hh"

#include "PacEnv/PacConfig.hh"
#include "PacEnv/pstring.hh"
#include "PacEnv/EdmlDetector.hh"
#include "PacEnv/EdmlMeasuringDevice.hh"
#include "PacEnv/EdmlCylDetElement.hh"
#include "PacEnv/EdmlRingDetElement.hh"
#include "PacEnv/EdmlConeDetElement.hh"

#include "PacTrk/PacTrkHitMeas.hh"
#include "PacTrk/PacTrkHitViewDch.hh"
#include "PacTrk/PacTrkHitViewSi.hh"
#include "PacTrk/PacTrkResModelSi.hh"
#include "PacTrk/PacTrkdEdxMeas.hh"

#include "PacEmc/PacEmcModel.hh"
#include "PacEmc/PacEmcClusterMeas.hh"

#include "PacDirc/PacDircMeasurement.hh"

#include "PacIfr/PacIfrMeasurement.hh"

#include "PacForwardPid/PacForwardPidMeasurement.hh"
#include "PacForwardPid/PacFarichMeasurement.hh"

#include "GenEnv/GenEnv.hh"
#include "AbsEnv/AbsEnv.hh"

#include <iostream>
  using namespace std;

#include <assert.h>

PacMeasurement*
  PacMeasurementFactory::get( const std::string& name )
{
  return PacMeasurementFactory::instance().doGet( name );
}

PacMeasurementFactory&
  PacMeasurementFactory::instance()
{
  static PacMeasurementFactory factory;
  return factory;
}

PacMeasurementFactory::PacMeasurementFactory() :
_tried2init(false)
  {}

PacMeasurementFactory::~PacMeasurementFactory()
{
// measurements are owned outside the factory
}

PacMeasurement*
  PacMeasurementFactory::doGet( const std::string& name )
{
  this->init();
  if( !name.empty()) {
    std::map<std::string, PacMeasurement* >::iterator itr = _name2measure.find( name );
    if( itr != _name2measure.end()) return itr->second;
  }
  return 0;
}

void
PacMeasurementFactory::init() {

  if( _tried2init ) return;
  _tried2init = true;

    // Check if the EDML detector description is available. If so then use
    // it in the constructor. Otherwise just warn.
    //
  const EdmlDetector* detector = gconfig.edml_detector();
  if( !detector ) {
    ErrMsg(fatal) << "PacMeasurementFactory::init(): no EDML model in this application." << endmsg;
  }

    // Create an EMC model
    //
    // NOTE: It's important to get the model initialzied before we'll
    //       be creating the corresponding EMC measurements.
    //
  PacEmcModel* emcModel(0);

    // Get measuring devices description from EDML and turn them into
    // the transient representation.
    //
  std::vector<EdmlMeasuringDevice > devices;
  gconfig.edml_devices(devices);

  for(std::vector<EdmlMeasuringDevice >::const_iterator device=devices.begin();
  device!=devices.end();++device) {

    if(ErrLogging(routine)){
      cout << "Creating measurement: " << device->name() << endl;
    }
    PacMeasurement* meas=0;
    
    if("DoubleSideSiStrips" == device->type()) {

      vector<PacTrkHitView*> views;
//            for(int iview=TrkEnums::xyView;iview<=TrkEnums::zView;iview++){
//              TrkEnums::TrkViewInfo view = (TrkEnums::TrkViewInfo)iview;

      vector<double> res_pars;
      int model = (int) device->get<float>("phiResModel");
      if (0 == model) {
        res_pars.push_back(device->get<float>("phiResPar0"));
      } else if (1 == model) {
        res_pars.push_back(device->get<float>("phiResPar0"));
        res_pars.push_back(device->get<float>("phiResPar1"));
        res_pars.push_back(device->get<float>("phiResPar2"));
        res_pars.push_back(device->get<float>("phiResPar3"));
        res_pars.push_back(device->get<float>("phiResPar4"));
        res_pars.push_back(device->get<float>("phiResPar5"));
        res_pars.push_back(device->get<float>("phiResPar6"));
      }
      PacTrkResModelSi* phiResModel = new PacTrkResModelSi(TrkEnums::xyView, model,res_pars);
      views.push_back(new PacTrkHitViewSi(TrkEnums::xyView
        ,phiResModel
        ,device->get<float>("NormRes")
        ,device->get<float>("phiPTRatio")
        ,device->get<float>("phingFrac")
        ,device->get<float>("phingFactor")
        ,device->get<float>("phiEff")));

      res_pars.clear();
      model = (int) device->get<float>("zResModel");
      if (0 == model) {
        res_pars.push_back(device->get<float>("zResPar0"));
      } else if (1 == model) {
        res_pars.push_back(device->get<float>("zResPar0"));
        res_pars.push_back(device->get<float>("zResPar1"));
        res_pars.push_back(device->get<float>("zResPar2"));
        res_pars.push_back(device->get<float>("zResPar3"));
        res_pars.push_back(device->get<float>("zResPar4"));
        res_pars.push_back(device->get<float>("zResPar5"));
        res_pars.push_back(device->get<float>("zResPar6"));
      }
      PacTrkResModelSi* zResModel = new PacTrkResModelSi(TrkEnums::zView,model,res_pars);
      views.push_back(new PacTrkHitViewSi(TrkEnums::zView
        ,zResModel
        ,device->get<float>("NormRes")
        ,device->get<float>("zPTRatio")
        ,device->get<float>("zngFrac")
        ,device->get<float>("zngFactor")
        ,device->get<float>("zEff")));
        
      
      meas = new PacTrkHitMeas(PacTrkHitMeas::DoubleSiStrip,views);

    } else if("SiPixels" == device->type()) {
      vector<PacTrkHitView*> views;
          //            for(int iview=TrkEnums::xyView;iview<=TrkEnums::zView;iview++){
          //              TrkEnums::TrkViewInfo view = (TrkEnums::TrkViewInfo)iview;

      vector<double> res_pars;
      int model = (int) device->get<float>("phiResModel");
      if (0 == model) {
        res_pars.push_back(device->get<float>("phiResPar0"));
      } else if (1 == model) {
        res_pars.push_back(device->get<float>("phiResPar0"));
        res_pars.push_back(device->get<float>("phiResPar1"));
        res_pars.push_back(device->get<float>("phiResPar2"));
        res_pars.push_back(device->get<float>("phiResPar3"));
        res_pars.push_back(device->get<float>("phiResPar4"));
        res_pars.push_back(device->get<float>("phiResPar5"));
        res_pars.push_back(device->get<float>("phiResPar6"));
      }
      PacTrkResModelSi* phiResModel = new PacTrkResModelSi(TrkEnums::xyView,model,res_pars);

      views.push_back(new PacTrkHitViewSi(TrkEnums::xyView
        ,phiResModel
        ,device->get<float>("NormRes")
        ,device->get<float>("phiPTRatio")
        ,device->get<float>("ngFrac")
        ,device->get<float>("ngFactor")
        ,device->get<float>("Eff")));

      res_pars.clear();
      model = (int) device->get<float>("zResModel");
      if (0 == model) {
        res_pars.push_back(device->get<float>("zResPar0"));
      } else if (1 == model) {
        res_pars.push_back(device->get<float>("zResPar0"));
        res_pars.push_back(device->get<float>("zResPar1"));
        res_pars.push_back(device->get<float>("zResPar2"));
        res_pars.push_back(device->get<float>("zResPar3"));
        res_pars.push_back(device->get<float>("zResPar4"));
        res_pars.push_back(device->get<float>("zResPar5"));
        res_pars.push_back(device->get<float>("zResPar6"));
      }
      PacTrkResModelSi* zResModel = new PacTrkResModelSi(TrkEnums::zView,model,res_pars);
      views.push_back(new PacTrkHitViewSi(TrkEnums::zView
        ,zResModel
        ,device->get<float>("NormRes")
        ,device->get<float>("zPTRatio")
        ,device->get<float>("ngFrac")
        ,device->get<float>("ngFactor")
        ,device->get<float>("Eff")));
                
      meas = new PacTrkHitMeas(PacTrkHitMeas::SiPixel,views);


    } else if("DriftChamber" == device->type()) {

      vector<PacTrkHitView*> views;
      if(ErrLogging(routine)){
        cout<<"PacMeasurementFactory: Parameters of DCH reso. function:"<<endl;
        cout<<(device->get<float>("rms_par0"))<<" "
          <<(device->get<float>("rms_par1"))<<" "
          <<(device->get<float>("rms_par2"))<<" "
          <<(device->get<float>("rms_par3"))<<" "
          <<(device->get<float>("rms_par4"))<<" "
          <<(device->get<float>("rms_par5"))<<endl;
      }
      vector<double> hparams;
      hparams.push_back(device->get<float>("rms_par0"));
      hparams.push_back(device->get<float>("rms_par1"));
      hparams.push_back(device->get<float>("rms_par2"));
      hparams.push_back(device->get<float>("rms_par3"));
      hparams.push_back(device->get<float>("rms_par4"));
      hparams.push_back(device->get<float>("rms_par5"));
      PacHitRes hres(hparams);
      views.push_back(new PacTrkHitViewDch(hres,
        device->get<float>("ngFrac"),device->get<float>("ngFactor"),
        device->get<float>("eff_par0"),
        device->get<float>("eff_par1"),
        device->get<float>("cell_size"),
        device->get<float>("angle")));

      meas = new PacTrkHitMeas(PacTrkHitMeas::Wire,views);

    } else if("dEdx" == device->type()){
      // create the dedx measurement object
      vector<double> dedx_pars;
      dedx_pars.push_back(device->get<float>("dedx_par1"));
      dedx_pars.push_back(device->get<float>("dedx_par2"));
      dedx_pars.push_back(device->get<float>("dedx_par3"));
      dedx_pars.push_back(device->get<float>("trunc_frac"));
      
      PacTrkHitMeas::HitType htype = (PacTrkHitMeas::HitType) device->get<float>("HitType");
      if(htype == PacTrkHitMeas::DoubleSiStrip){

        dedx_pars.push_back(device->get<float>("svt_thresh"));
        dedx_pars.push_back(device->get<float>("svt_fit_p0"));
        dedx_pars.push_back(device->get<float>("svt_fit_p1"));
        dedx_pars.push_back(device->get<float>("svt_fit_p2"));
        dedx_pars.push_back(device->get<float>("svt_fit_p3"));
        dedx_pars.push_back(device->get<float>("svt_fit_p4"));
      }
      meas = new PacTrkdEdxMeas(htype,dedx_pars);      
      
    } else if("EmcCluster" == device->type()) {
       if(emcModel == 0){
         emcModel = PacEmcModel::createNewModel();
         emcModel->init(detector);
       }
      meas = new PacEmcClusterMeas();

    } else if("DircCylinder" == device->type()) {

      meas = new PacDircMeasurement();

    } else if("IfrCylinder" == device->type()) {

      meas = new PacIfrMeasurement();

    } else if( "ForwardPidDetector" == device->type() ) {

      meas = new PacForwardPidMeasurement();

    } else if( "FarichDetector" == device->type() ) {

      meas = new PacFarichMeasurement();

    } else {
      ErrMsg(error) << "unknown measurement: " << device->name() << endmsg;
      continue;
    }
    // set the default time-sensitive window
    double stwin = device->sensitiveTimeWindow();
    ErrMsg(warning) << "Setting sensitive time window for " << device->name() << " measurement to " << stwin << endmsg;
    meas->setSensitiveTimeWindow(stwin);
    _name2measure[device->name()] = meas;
  }
}
