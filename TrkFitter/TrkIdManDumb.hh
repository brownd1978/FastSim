//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkIdManDumb.hh 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//     Class for generating dummy id numbers (always same value).  Users 
// can create instances, or use a singleton instance (value = -1).
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Steve Schaffner
//
//------------------------------------------------------------------------
#ifndef TRKIDMANDUMB_HH
#define TRKIDMANDUMB_HH
#include "TrkBase/TrkIdManager.hh"

// Class interface //
class TrkIdManDumb : public TrkIdManager {

public:
  TrkIdManDumb(long val);
  virtual ~TrkIdManDumb();
  static TrkIdManDumb* instance();

  virtual long nextId();
  virtual long lastId() const;
  virtual void setMax(long maxid);
private:	
  long _theValue;

  long theValue()       const                             {return _theValue;}
  // Preempt 
  TrkIdManDumb&   operator= (const TrkIdManDumb&);
  TrkIdManDumb(const TrkIdManDumb &);
};

#endif







