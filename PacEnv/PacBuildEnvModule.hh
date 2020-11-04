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

#ifndef PacBuildEnvModule_hh
#define PacBuildEnvModule_hh

#include "Framework/AppModule.hh"
#include "PacEnv/PacBuildEnv.hh"
#include "Framework/AbsParmString.hh"
class PacDetector;
class DetSet;

class PacBuildEnvModule : public AppModule
{
public:
  PacBuildEnvModule( const char* const theName, 
    const char* const theDescription );
  virtual ~PacBuildEnvModule( );
// all work is done at beginJob time
  virtual AppResult beginJob( AbsEvent* anEvent );
private:
  TrkBuildCoreEnv _env;
  AbsParmString _configFile;
  AbsParmString _extraConfig;
};



#endif
