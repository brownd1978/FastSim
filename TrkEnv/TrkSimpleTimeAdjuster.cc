#include "BaBar/BaBar.hh"
#include "TrkEnv/TrkSimpleTimeAdjuster.hh"
#include "ErrLogger/ErrLog.hh"

TrkSimpleTimeAdjuster::TrkSimpleTimeAdjuster(double svt, double drc,
                                             double emc, double ifr,double pep )
{
        _offset[_svt]=svt;
        _offset[_dch]=0; // DCH is THE reference for the time being
        _offset[_drc]=drc;
        _offset[_emc]=emc;
        _offset[_ifr]=ifr;
        _offset[_pep]=pep;
}                                       

TrkSimpleTimeAdjuster::~TrkSimpleTimeAdjuster()
{;}

double
TrkSimpleTimeAdjuster::deltaTimePep() const 
{
        return _offset[_pep];
}

double
TrkSimpleTimeAdjuster::deltaTimeSvt() const 
{
        return _offset[_svt];
}

double
TrkSimpleTimeAdjuster::deltaTimeDch() const 
{
        return _offset[_dch];
}

double
TrkSimpleTimeAdjuster::deltaTimeDrc() const 
{
        return _offset[_drc];
}

double
TrkSimpleTimeAdjuster::deltaTimeEmc() const 
{
        return _offset[_emc];
}

double
TrkSimpleTimeAdjuster::deltaTimeIfr() const 
{
        return _offset[_ifr];
}


void
TrkSimpleTimeAdjuster::setDeltaTimePep(double delta)  
{
        _offset[_pep] = delta;
}

void
TrkSimpleTimeAdjuster::setDeltaTimeSvt(double delta)  
{
        _offset[_svt] = delta;
}

void
TrkSimpleTimeAdjuster::setDeltaTimeDch(double delta)  
{
        _offset[_dch] = delta;
}

void
TrkSimpleTimeAdjuster::setDeltaTimeDrc(double delta)  
{
        _offset[_drc] = delta;
}

void
TrkSimpleTimeAdjuster::setDeltaTimeEmc(double delta)  
{
        _offset[_emc] = delta;
}

void
TrkSimpleTimeAdjuster::setDeltaTimeIfr(double delta)  
{
        _offset[_ifr] = delta;
}

