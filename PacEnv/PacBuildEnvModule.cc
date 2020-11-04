//--------------------------------------------------------------------------
// File and Version Information:
// PacBuildEnvModule
//
// Description:
//	Class PacBuildEnvModule. Module to instantiate the environment in
//  a Pacrat framework job.
//
// Environment:
//	Software developed for SuperB
//
// Author List:
//       David Brown, LBNL
//
// Copyright Information:
//	Copyright (C) 2008		Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------

#include "PacEnv/PacBuildEnvModule.hh"
#include "PacEnv/PacConfig.hh"
#include "ErrLogger/ErrLog.hh"
#include "DetectorModel/DetSet.hh"
#include "Framework/AppFileName.hh"


PacBuildEnvModule::PacBuildEnvModule( const char* const theName, 
    const char* const theDescription ) : AppModule(theName,theDescription)
    , _configFile("ConfigFile",this,"PacDetector/pacrat_SuperB.xml")
    , _extraConfig("extraConfig",this,"none")
{
  commands()->append( &_configFile);
  commands()->append( &_extraConfig);
}
  
PacBuildEnvModule::~PacBuildEnvModule( ) {
}

AppResult
PacBuildEnvModule::beginJob( AbsEvent* anEvent ) {
  // read in the pac geometry definition file
  // Note: we should make sure here that the config file is read successfully....
  //config file should be something like "RELEASE/PacTrk/testTrackReconstruction.cfg"
  ErrMsg(routine) << "Reading configuration file " << _configFile.value() << endmsg;
  gconfig.parsefile(AppFileName(_configFile.value().c_str()).pathname().c_str());
  bool readStatus = true;  // gconfig.parsefile should return a bool representing the status
  if ( !readStatus ) { 
    ErrMsg(error) << "Error reading file " << _configFile.value() <<  endmsg;
  }
  assert(readStatus); // make sure we successfully read the config before building the detector
  
  // see if we have extra configuration
  if(_extraConfig.value() != "none"){
    ErrMsg(routine) << "Reading extra configuration file " << _extraConfig.value() << endmsg;
    gconfig.parsefile(AppFileName(_extraConfig.value().c_str()).pathname().c_str());
    bool readStatus = true;  // gconfig.parsefile should return a bool representing the status
    if ( !readStatus ) { 
      ErrMsg(error) << "Error reading file " << _extraConfig.value() <<  endmsg;
    }
    assert(readStatus); // make sure we successfully read the config before building the detector
  }
  
// create general environment
  _env.buildCore();
// create tracking environment
  _env.buildTrk();
// in future, build other environments for other subsystems

  return AppResult::OK;
}
