//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkContextEv.cc 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//     
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Steve Schaffner
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
#include "TrkFitter/TrkContextEv.hh"
#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/AbsEvent.hh"
#include "ProxyDict/Ifd.hh"
#include "TrkFitter/TrkIdManDefault.hh"
#include "TrkFitter/TrkIdManDumb.hh"
#include "TrkBase/TrkId.hh"
#include "ErrLogger/ErrLog.hh"
#include "TrkEnv/TrkEnv.hh"
#include "AbsEnv/AbsEnv.hh"
TrkContextEv::TrkContextEv(AbsEvent* anEvent) : 
  TrkContext(gblEnv->getTrk()->magneticField())
{
  TrkIdManager* manTmp;
  if (anEvent == 0) {
    manTmp = TrkIdManDumb::instance();
  } else {
    // Default Id manager
    static IfdStrKey defKey("Default");
    TrkIdManager* manDef = 
      Ifd< TrkIdManager >::get( anEvent, defKey );
    if (0 == manDef) {
      manDef = new TrkIdManDefault(1);
      IfdDataProxy<TrkIdManager>* theProxy = 
    	new IfdDataProxy<TrkIdManager>(manDef);
      if (!(Ifd<TrkIdManager>::put(anEvent,theProxy,defKey))){
    	ErrMsg(error) << "TrkContextEv: Oops - couldn't put TrkIdManager"
		      << endmsg;
      }
    }
    manTmp = manDef;
  }
  _idman = manTmp;
}

TrkContextEv::~TrkContextEv() 
{
}


//------------------------------------------------------------------------
TrkId
TrkContextEv::getId() const {
//------------------------------------------------------------------------
  return TrkId(idManager());
}

