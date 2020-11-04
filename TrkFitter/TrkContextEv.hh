//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkContextEv.hh 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//     Derived class of TrkContext.  Adds precisely one feature: the 
// ctor to fill in default values from AbsEvent and AbsEnv.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Steve Schaffner
//
//------------------------------------------------------------------------
#ifndef TRKCONTEXTEV_HH
#define TRKCONTEXTEV_HH
#include "TrkBase/TrkContext.hh"
class AbsEvent;


// Class interface //
class TrkContextEv : public TrkContext {

public:
                 TrkContextEv(AbsEvent*);
  virtual        ~TrkContextEv();

  virtual TrkId  getId()    const;


private:	
  TrkIdManager* _idman;

  TrkIdManager* idManager() const                     {return _idman;}
  // Preempt 
  TrkContextEv&   operator= (const TrkContextEv&);
  TrkContextEv(const TrkContextEv &);
};

#endif
