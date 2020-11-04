//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkContextCustom.hh 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//     TrkContext that permits the user to set the id number by passing in 
// a TrkId.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Steve Schaffner
//
//------------------------------------------------------------------------
#ifndef TRKCONTEXTCUSTOM_HH
#define TRKCONTEXTCUSTOM_HH
#include "TrkBase/TrkContext.hh"
#include "TrkBase/TrkId.hh"

class BField;
class TrkRecoTrk;

// Class interface //
class TrkContextCustom : public TrkContext {

public:
  TrkContextCustom(const BField*, const TrkId&);
  TrkContextCustom(const TrkRecoTrk&);
  virtual ~TrkContextCustom();

  virtual TrkId getId()     const;

  
private:	
  TrkId _id;

  // Preempt 
  TrkContextCustom&   operator= (const TrkContextCustom&);
  TrkContextCustom(const TrkContextCustom &);
};

#endif







