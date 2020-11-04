//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkSimpleTimeAdjuster.hh 103 2010-01-15 12:12:27Z stroili $
//
// Description:
//      Class TrkSimpleTimeAdjuster
//     
//      Simple(st possible?) implementation of a TrkTimeAdjuster...
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Gerhard Raven
//
// Copyright Information:
//      Copyright (C) 1999              UC, San Diego
//
//------------------------------------------------------------------------

#ifndef TRKSIMPLETIMEADJUSTER_HH
#define TRKSIMPLETIMEADJUSTER_HH

#include "TrkEnv/TrkTimeAdjuster.hh"

class TrkSimpleTimeAdjuster: public TrkTimeAdjuster
{
public:
        TrkSimpleTimeAdjuster(double SvtOffsetDch, double DrcOffsetDch,
                              double EmcOffsetDch, double IfrOffsetDch, 
                              double PepOffsetDch);
        ~TrkSimpleTimeAdjuster();

        virtual double deltaTimePep() const;
        virtual double deltaTimeSvt() const;
        virtual double deltaTimeDch() const;
        virtual double deltaTimeDrc() const;
        virtual double deltaTimeEmc() const;
        virtual double deltaTimeIfr() const;

        virtual void setDeltaTimePep(double delta);
        virtual void setDeltaTimeSvt(double delta);
        virtual void setDeltaTimeDch(double delta);
        virtual void setDeltaTimeDrc(double delta);
        virtual void setDeltaTimeEmc(double delta);
        virtual void setDeltaTimeIfr(double delta);

private:
        enum { _svt,_dch,_drc,_emc,_ifr,_pep,_nDet };
        double _offset[_nDet];
};
#endif
