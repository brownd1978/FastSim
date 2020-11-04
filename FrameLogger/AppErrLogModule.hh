#ifndef APPERRLOGMODULE_HH
#define APPERRLOGMODULE_HH

#include "Framework/AppModule.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "Framework/AbsParmGeneral.hh" 


class AppErrLogModule : public AppModule
{
public:
 
  // Constructors
  AppErrLogModule( const char* const theName = "AppErrLogModule", 
      const char* const theDescription = "Make histogram of ErrMsg severities" );
 
  // Destructor
  virtual ~AppErrLogModule( );
 
  // Operators
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult endJob ( AbsEvent* anEvent );
  
private:
  void initHisto();

  static AppErrLogModule* myInstance;

  AbsParmGeneral<bool> _makeHisto;

  bool _beginJobDone;
};
#endif  /* APPERRLOGMODULE_HH */
