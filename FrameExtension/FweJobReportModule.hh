//--------------------------------------------------------------------------
//
// File and Version Information:
//    $Id: FweJobReportModule.hh 505 2010-01-14 15:15:59Z stroili $
//
// Description:
//      class FweJobReportModule
//      This is a module which should be in AppUserBuild.cc
//      Its purpose is to let interface the job reporting class with Framework
//
// Authors:
//      A. De Silva
//
// Environment:
//      This software was developed for the BaBar collaboration.  If you
//      use all or part of it, please give an appropriate acknowledgement.
//
// Copyright Information:
//
//------------------------------------------------------------------------
#ifndef FWEJOBREPORTMODULE_HH
#define FWEJOBREPORTMODULE_HH

#include "Framework/AppModule.hh"
class FweJobReportCommand;

class FweJobReportModule : public AppModule {

public:
  FweJobReportModule(const char* const theName, 
		     const char* const theDescription );
  
  virtual ~FweJobReportModule( );
  
private:

  // Not implemented.
  FweJobReportModule& operator=( const FweJobReportModule& );
  FweJobReportModule( const FweJobReportModule& );

  FweJobReportCommand* _jrCommand;
};

#endif
