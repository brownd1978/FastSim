//-------------------------------------------------------------------------
// PacEmcTMInfo
//
//  Class PacEmcTMInfo. This class contains information generated during
//      track matching.  A simplified version of Babar's EmcTMInfo.
//
//  Chih-hsiang Cheng   Caltech 2009/03/05
//-------------------------------------------------------------------------
#ifndef PACEMCTMINFO_HH
#define PACEMCTMINFO_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <vector>
#include <map>

//----------------------
// Base Class Headers --
//----------------------
#include "AbsEvent/AbsEvtObj.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

#include "CLHEP/Geometry/HepPoint.h"
#include "EmcData/EmcTrkInterInfo.hh"
#include "ProbTools/Consistency.hh"

class TrkRecoTrk;
class AbsRecoCalo;
class TrkPocaBase;

//              ---------------------
//              -- Class Interface --
//              ---------------------

class PacEmcTMInfo: public AbsEvtObj
{

public:

  // Constructors
  PacEmcTMInfo ();

  PacEmcTMInfo (HepPoint theEntrance, int charge,
		const EmcTrkInterInfo interinfo, const AbsRecoCalo * calo,
		double deltaXY = 0.0, double deltaZ = 0.0 );

  PacEmcTMInfo ( const PacEmcTMInfo & );

  // Destructor
  virtual ~PacEmcTMInfo ();

  virtual PacEmcTMInfo * clone() const { return new PacEmcTMInfo( *this); }

  // Operators
  bool operator==( const PacEmcTMInfo& other ) const;

  //  get the charge of the associated track
  virtual int getCharge() const { return _charge; };

  //  return track bump matching consistency
  virtual const Consistency getConsistency() const;
  void setConsistency(const Consistency &con);

  virtual const HepPoint getEntrancePoint() const;
  virtual double getXYSeparation() const {return _deltaXY;}
  virtual double getZSeparation() const {return _deltaZ;}

  virtual const TrkRecoTrk * getTrkRecoTrk() const { return _track; }
  virtual const AbsRecoCalo * getAbsRecoCalo() const { return _calo; }
  virtual const EmcTrkInterInfo getTrkInterInfo() const { return _trkInterInfo; }

  virtual void print( std::ostream & ) const;

  const TrkPocaBase* getPoca() const { return _poca; }
  void setPoca(const TrkPocaBase& poca);

protected:

  HepPoint _entryPoint;
  mutable double _deltaXY, _deltaZ;

  const EmcTrkInterInfo    _trkInterInfo;

  mutable const TrkRecoTrk    * _track;
  mutable int                   _charge;  // cache for the charge

  /* pointer to the matched calorimeter object; not owned */
  const AbsRecoCalo   * _calo;


  Consistency _consistency;

private:

  // Data members
  TrkPocaBase* _poca;


};
#endif
