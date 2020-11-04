// ------------------------------------------------------------------------------
// File and Version Information:
//   PacMeasurement
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 14May2008
// ------------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "PacGeom/PacMeasurement.hh"
// default sensitive time window
#define SENSITIVETIMEWINDOW 1e-6

PacMeasurement::PacMeasurement(MeasurementType mtype,const char* name) : _mtype(mtype),
 _hstwin(SENSITIVETIMEWINDOW/2.0), _name(name)
{}

PacMeasurement::~PacMeasurement()
{}

void 
PacMeasurement::createRing(const PacSimHit& hit, PacDircResponse* ring) const 
{}

void
PacMeasurement::createEmcCluster(const PacSimHit& hit, PacEmcCluster *cluster, const bool verbose) const
{}

void 
PacMeasurement::createIfrHits(const PacSimHit& hit, std::vector<PacIfrResponse>* ifr2DCluster) const 
{}

bool
PacMeasurement::createForwardPidHit( const PacSimHit& hit, 
				     PacForwardPidResponse* forwardpidhit ) const
{ return( true ); }

void
PacMeasurement::createFarichHit( const PacSimHit& hit, 
				 PacFarichResponse* farichhit ) const
{}

void
PacMeasurement::setRandomEngine(HepRandomEngine* engine) {
  _randengine = engine;
}

PacMeasurement::MeasurementTime
PacMeasurement::inTime(double time,double& fraction) const{
// simple symmetric time window is assumed for now.  Parameter come from the config file
  if(fabs(time)< _hstwin)
    return PacMeasurement::InTime;
  else
    return PacMeasurement::OutOfTime;
}

