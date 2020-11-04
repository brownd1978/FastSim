#ifndef PacTrkHitMeas_hh
#define PacTrkHitMeas_hh

class PacHitOnTrk;

#include "PacGeom/PacMeasurement.hh"
#include "PacTrk/PacTrkHitView.hh"
#include <vector>

class PacTrkHitMeas : public PacMeasurement {

public:
  enum HitType {SiPixel=0,SingleSiStrip,DoubleSiStrip,Wire};
// Describe the type, and pass the necessary view objects.  The length of the view vector
// is tested for consistency against the type.  This object takes ownership of the views
  PacTrkHitMeas(HitType type,std::vector<PacTrkHitView*>& views);
// override timing test
  virtual PacMeasurement::MeasurementTime inTime(double time,double& fraction) const;  
  virtual ~PacTrkHitMeas();
  // accessors
  HitType type() const { return _htype;}
  // views constrained by this hit
  int nviews() const { return _views.size();}
  const PacTrkHitView* hitView(int iview) const { return _views[iview];}
  // Override hit-specific action
  virtual void createHots(const PacSimHit& hit, std::vector<PacHitOnTrk*>& hotlist, bool isActive=true) const;
  // 2-hit time resolution
  bool timeResolved(double t1, double t2) const { return fabs(t2-t1) >  _dt2hit; }
  double twoHitTimeResolution() const { return _dt2hit; }
  void setTwoHitTimeResolution(double dt2hit) { _dt2hit = dt2hit; }
private:
  double _dt2hit;
  HitType _htype;
  std::vector<PacTrkHitView*> _views;
};

#endif
