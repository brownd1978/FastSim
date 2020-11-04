// ------------------------------------------------------------------------------
// File and Version Information:
//   PacMeasurement; abstract base class defining the behavior of an active element
//   in PacDetector
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 14May2008
// ------------------------------------------------------------------------------
#ifndef PacMeasurement_HH
#define PacMeasurement_HH

class PacSimHit;
class TrkHotList;
class PacEmcCluster;
class PacDircResponse; 
class PacIfrResponse;
class PacForwardPidResponse;
class PacFarichResponse;
#include "CLHEP/Random/RandomEngine.h"
#include <vector>
using namespace CLHEP;
 
class PacMeasurement {
public:

  enum MeasurementType {TrkHit=1,DIRC=2,Calor=3,IFR=4,ForwardPID=5,Farich=6,dEdx=7};
  enum MeasurementTime {InTime=0,OutOfTime,Mixed};
  // base class knows what type of measurement it is
  PacMeasurement(MeasurementType mtype,const char* name="unknown");
  virtual ~PacMeasurement() = 0;
  MeasurementType measurementType() const { return _mtype;}
  // actions which a measurement can perform.  These have a default noop implementaiton
  // dedicated subclasses should override these as appropriate
  // generate hots for a tracking detector
  //  defunct: this is now accessible only through downcasting
//  virtual void createHots(const PacSimHit& hit, TrkHotList* hotlist,bool active=true) const;
  virtual void createRing(const PacSimHit& hit, PacDircResponse* ring) const;
  virtual void createEmcCluster(const PacSimHit& hit, PacEmcCluster* cluster, const bool verbose) const;
  virtual void createIfrHits(const PacSimHit& hit, std::vector<PacIfrResponse>* ifr2DCluster) const;
  virtual bool createForwardPidHit( const PacSimHit& hit, 
				    PacForwardPidResponse* forwardpidhit ) const;
  virtual void createFarichHit( const PacSimHit& hit, 
				PacFarichResponse* farichhit ) const;			    
	// test whether a measurement is 'in time' or not.  t=0 is defined as the physics event collision time.
	// 'in' means 100% of the signal is measured, 'out' means 0%, 'mixed' is between.  In case of 'mixed'
	//  the 'fraction' corresponds to how much of an out-of-time signal is measured (and is otherwise ignored).
	// default implementation is to apply a symmetric time window cut around 0 (physics event time)
	// Note this is a virtual funciton, specific measurements should override to provide the correct physical behavior
  virtual MeasurementTime inTime(double time,double& fraction) const;
	// allow resetting the random engine.  Subclasses shoudl override this if necessary;
	virtual void setRandomEngine(HepRandomEngine* engine);
  // allow setting a basic time window
  void setSensitiveTimeWindow(double stwin) { _hstwin = stwin/2.0; }
  double halfSensitiveTimeWindow() const { return _hstwin;}
  void setName(std::string& name) { _name=name; }
private:
  MeasurementType _mtype;
  // random number engine
  mutable HepRandomEngine* _randengine;
  double _hstwin; // half the sensitive time window
  std::string _name; // measurement name
};

#endif

