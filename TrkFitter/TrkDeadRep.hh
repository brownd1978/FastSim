//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkDeadRep.hh 104 2010-01-15 12:13:14Z stroili $
//
// Description: Base class for internal track representation classes -- e.g. 
//   HelixRep, KalRep.  Owns and maintains a TrkHotList; and keeps a 
//   pointer to the track that owns the Rep.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Authors: Steve Schaffner
//
//------------------------------------------------------------------------

#ifndef TRKDEADREP_HH
#define TRKDEADREP_HH
#include "BaBar/BaBar.hh"
#include "PDT/PdtPid.hh"
#include "TrkBase/TrkDirection.hh"
#include "TrkBase/TrkRep.hh"
#include "TrkBase/TrkHotListEmpty.hh"

class TrkHitOnTrk;
#include <iosfwd>
class TrkDifTraj;
class TrkErrCode;
class TrkExchangePar;
#include "CLHEP/Vector/ThreeVector.h"
class HepPoint;
class TrkRecoTrk;
class TrkVolume;
class IfdKey;
class BbrPointErr;
class BbrVectorErr;
template <class T> class HepAList;

// Class interface //
class TrkDeadRep : public TrkRep {
  
public:
  //******************************************
  // Constructors and such 
  //******************************************
  TrkDeadRep(TrkRecoTrk* trk, PdtPid::PidType hypo);
  // copy ctor
  TrkDeadRep(const TrkDeadRep& oldRep, TrkRecoTrk* trk, PdtPid::PidType hypo);
  virtual ~TrkDeadRep();
  // clone() used to copy tracks; cloneNewHypo() for new hypos within track
  virtual TrkDeadRep* clone(TrkRecoTrk* newTrack) const;
  virtual TrkDeadRep* cloneNewHypo(PdtPid::PidType hypo);
  bool operator== (const TrkDeadRep&);
  
  //******************************************
  // Global quantities:
  //******************************************
  virtual TrkDifTraj&       traj();
  virtual const TrkDifTraj& traj()         const;
  virtual int               nDof()         const;
  virtual double            chisq()        const;                     
  virtual int               charge()       const;
  virtual const IfdKey&     myKey()           const;

  //******************************************
  // Information about track at a given position
  //******************************************
  HepPoint          position(double fltL)         const;
  Hep3Vector        direction(double fltL)        const;
  Hep3Vector        momentum(double fltL=0.)      const;
  double            pt(double fltL=0.)            const;        
  double            arrivalTime(double fltL)      const;
  BbrPointErr       positionErr(double fltL)      const;
  BbrVectorErr      directionErr(double fltL)     const;
  BbrVectorErr      momentumErr(double fltL)      const;
  virtual TrkExchangePar helix(double fltLen) const;

  //******************************************
  // Hit list handling
  //******************************************
  // Simple implementations of these are present in the base class; 
  //   complicated reps (e.g. Kalman) may wish to override.
  virtual void addHot(TrkHitOnTrk *theHot);
  virtual void removeHot(TrkHitOnTrk *theHot);
  virtual void activateHot(TrkHitOnTrk *theHot);
  virtual void deactivateHot(TrkHitOnTrk *theHot);
  


  //******************************************
  // Fitting stuff
  //******************************************
  virtual TrkErrCode fit();


  //******************************************
  // Vertexing interface
  //******************************************
  virtual HepMatrix         posmomCov(double fltL)        const;
  virtual void              getAllCovs(double fltL,
                                       HepSymMatrix& xxCov,
                                       HepSymMatrix& ppCov,
                                       HepMatrix& xpCov)  const;

  virtual void              getAllWeights(double fltL,
                                          HepVector& pos,
                                          HepVector& mom,
                                          HepSymMatrix& xxWeight,
                                          HepSymMatrix& ppWeight,
                                          HepMatrix&    xpWeight)
                                            const;
  virtual void getAllWeights(const HepPoint& pt,
				  HepVector& pos,
				  HepVector& mom,
				  HepSymMatrix& xxWeight,
				  HepSymMatrix& ppWeight,
				  HepMatrix&    xpWeight)const;  
  //******************************************
  // Printing
  //******************************************
  virtual void printAll(std::ostream& ostr) const;
  virtual void print(std::ostream& ostr) const;
  
protected:

private:	

  // Should never use op= (clone instead)
  TrkDeadRep&   operator= (const TrkDeadRep&);
};

#endif







