//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkSimpleRep.hh 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//      Base class for simple TrkReps: helix, line, circle.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Steve Schaffner
//
//------------------------------------------------------------------------

#ifndef TRKSIMPLEREP_HH
#define TRKSIMPLEREP_HH

#include "TrkBase/TrkRep.hh"
#include "TrkFitter/TrkHelixFitter.hh"

class TrkSimpTraj;

class TrkSimpleRep : public TrkRep {
public:
  //******************************************
  // Constructors and such 
  //******************************************
  // 2 ctors, one for reps with hits, one for reps without hits
  TrkSimpleRep(TrkRecoTrk*, PdtPid::PidType, bool createHotList);
  TrkSimpleRep(const TrkHotList&, TrkRecoTrk*, PdtPid::PidType);
  TrkSimpleRep(TrkHotList&, TrkRecoTrk*, PdtPid::PidType, bool stealHots);
  TrkSimpleRep(TrkRecoTrk*, PdtPid::PidType, int nActive, int nSvt, int nDch,
               double chi2, double startFoundRange, double endFoundRange);
  // Copy ctor:
  TrkSimpleRep(const TrkSimpleRep& right, TrkRecoTrk* trk, PdtPid::PidType);
  virtual ~TrkSimpleRep();

  //******************************************
  // Global quantities:
  //******************************************
          int    nDof()                const;
          double chisq()               const;
          void   setChisq(double c);
  virtual int    charge()              const;

  //********************************************
  // Information about track at a given position
  //********************************************
  virtual Hep3Vector        momentum(double fltL=0.)      const;
  virtual double            pt(double fltL=0.)            const;
  virtual BbrVectorErr      momentumErr(double fltL)      const;


  // Interface to vertexing algorithms (M.Bondioli 7/17/98)
  // covariance matrices of the track at fixed flight length 
  virtual HepMatrix         posmomCov(double fltL)            const;
  virtual void              getAllCovs(double fltL,
				       HepSymMatrix& xxCov,
				       HepSymMatrix& ppCov,
				       HepMatrix& xpCov)      const;
  // accessors to 2nd derivative of chi2 wrt x and p.
  // x0 and p0 are filled with the pos and 3mom around which expansion 
  // takes place, whilst Weights are filled with the 2nd deriv of chi2.
  // so that:
  //  dx=x-x0   dp=p-p0
  //
  //  chi2(x,p)=0.5 dx^t*Wxx*dx + dx^t*Wxp*dp + 0.5 dp^t*Wpp*dp
  // where:
  //  pos and mom are 3-dim Vectors,
  //  xxWeight ppWeight and xpWeight are 3 by 3 matrices
  //
  virtual void              getAllWeights(double fltL,
					  HepVector& pos,
					  HepVector& mom,
					  HepSymMatrix& xxWeight,
					  HepSymMatrix& ppWeight,
					  HepMatrix&    xpWeight)const;
    virtual void getAllWeights(const HepPoint& pt,
				  HepVector& pos,
				  HepVector& mom,
				  HepSymMatrix& xxWeight,
				  HepSymMatrix& ppWeight,
				  HepMatrix&    xpWeight)  const;
				 
  //******************************************
  // Fitting
  //******************************************
  virtual TrkErrCode fit();
  TrkHelixFitter& fitter()                                  {return _fitter;}

protected:
  TrkSimpleRep& operator=(const TrkSimpleRep &right);
  virtual TrkSimpTraj& simpTraj() = 0;  
  virtual const TrkSimpTraj& simpTraj() const = 0;  

private:
  TrkHelixFitter _fitter;
  double _chisq;

};
#endif



