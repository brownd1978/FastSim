/* Class: PacHitOnTrk
*
* An artificial hit on a track. */

#ifndef PacHitOnTrk_HH
#define PacHitOnTrk_HH

#include "TrajGeom/TrkLineTraj.hh"
#include "TrkBase/TrkEnums.hh"
#include "TrkBase/TrkHitOnTrk.hh"
#include "PacTrk/PacHit.hh"
#include "PacTrk/PacRecHitInfo.hh"
#include "PacTrk/PacTrkHitProximity.hh"

class TrkRep;
class TrkDifTraj;
class PacTrkHitMeas;
class PacTrkHitView;
class PacTrkdEdxMeas;
class PacRecHitInfo;
class PacSimHit;


class PacHitOnTrk : public TrkHitOnTrk {
public:
  PacHitOnTrk(PacHit* PacHit, const PacRecHitInfo& hinfo,const PacTrkHitMeas* measure,
  const PacTrkHitView* view, const PacSimHit* simhit);
  virtual ~PacHitOnTrk();
// merge this hot with onother.  This overwrites the position used in the fit, but nothing
// else.  It also sets the other hot to be inactive.  Note this doesn't make any decisions
// whether hits should be merged, it just merges them (after sanity checks)
  bool merge(PacHitOnTrk* other,double sep);
// overwrite the position of one hit with another
  void overwrite(PacHitOnTrk* other);
  /* Simplistic implementation of TrkHitOnTrk interface */
  virtual PacHitOnTrk* clone(TrkRep* parentRep, const TrkDifTraj* trkTraj = 0) const;
  virtual TrkEnums::TrkViewInfo whatView() const              { return _pachit->whatView(); }
  virtual unsigned layerNumber() const                        { return _pachit->layerNumber(); }
  virtual const TrkLineTraj* hitTraj() const                   { return _hittraj; }
  virtual bool timeResid(double& t, double& error) const      { return false; }
  virtual bool timeAbsolute(double& t, double& error) const   { return false; }
// Implement the detector-specific casting as a logical-only return value:
// the pointers returned by these functions WILL BE INVALID, but non-null as appropriate, to allow
// hit-counting logic to function
  virtual const DchHitOnTrack* dchHitOnTrack() const;
  virtual const SvtHitOnTrack* svtHitOnTrack() const;
// specific measurement information
  const PacTrkHitMeas* measure() const { return _measure; }
  const PacTrkHitView* view() const { return _view; }
  const PacTrkdEdxMeas* dEdxMeasure() const { return _dedxmeas; }
  double hitRMS() const { return _hitrms;}
// access to MC truth; this is OK, as this class is specific to MC applications
  const PacSimHit* simHit() const { return _simhit; }
  const PacRecHitInfo& hitInfo() const { return _hinfo; }
// dE/dx measurement information
  inline double getdEdx() const { return _dedx;}
  inline double getErrdEdx() const { return _ededx;}
  inline void setdEdx(double dedx) { _dedx = dedx; }
  inline void setErrdEdx(double ededx) { _ededx = ededx; }
  inline void setdEdxMeasure(const PacTrkdEdxMeas* dedxmeas) { _dedxmeas = dedxmeas;}
// functions associated with merging and pat. rec. confusion
  bool isMergable(const PacHitOnTrk* other) const;
  // measure the spatial proximity of 2 hits.  Return value states whether the hits are comparable
  // (on same element, with same measurement) or not.
  bool proximity(const PacHitOnTrk* other, PacTrkHitProximity& hprox) const;

protected:
  PacHitOnTrk(const PacHitOnTrk& other, TrkRep* rep, PacHit* PacHit, TrkLineTraj* hittraj);
  virtual TrkErrCode updateMeasurement(const TrkDifTraj* traj, bool maintainAmbiguity);
  void addMerged(const PacHitOnTrk* other);
protected:
  PacRecHitInfo _hinfo;
  PacHit* _pachit;
  TrkLineTraj* _hittraj;
  double _hitrms;
  const PacTrkHitMeas* _measure;
  const PacTrkHitView* _view;
  const PacTrkdEdxMeas* _dedxmeas;
  const PacSimHit* _simhit;
  double _dedx;
  double _ededx;
  // hots with which this one was merged.
  std::vector<const PacHitOnTrk*> _merged;
};


#endif
