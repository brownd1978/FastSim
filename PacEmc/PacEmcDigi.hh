
#ifndef PACEMCDIGI_HH
#define PACEMCDIGI_HH

#include <iostream>
#include "AbsEnv/TwoCoordIndex.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include <map>

class GTrack;

typedef std::map<const GTrack*, double> GTDouble ;

class PacEmcDigi {

public:

  enum WaveFormTime {InSigWin=0,OutOfSigWin};

  PacEmcDigi();
  PacEmcDigi(const PacEmcDigi& other);
  PacEmcDigi(double energy,  const TwoCoordIndex& tci);
  PacEmcDigi(double energy,  const long tciIndex);

  virtual ~PacEmcDigi();

  virtual PacEmcDigi *clone() const;

  virtual bool operator==(const PacEmcDigi & otherDigi) const;
  virtual bool operator!=(const PacEmcDigi & otherDigi) const;
  virtual void operator+=(const PacEmcDigi & otherDigi);
  //  virtual void operator*=(double x) { _energy*= x;}
  virtual void scale(double x) { _energy*= x; }
  virtual void scalesim(double x) { _simenergy*= x; }

  virtual const long tciIndex() const { return _tciIndex; }
  virtual const TwoCoordIndex* tci() const;
  virtual long theta() const {return _thetaIdx;}
  virtual long phi() const {return _phiIdx;}
  virtual double thetaReal() const {return _thetaReal;}
  virtual double phiReal() const {return _phiReal;}
  virtual double deltaTheta() const {return _deltaTheta; };  
  virtual double deltaPhi() const {return _deltaPhi; };
  virtual const HepPoint & where() const;
  virtual const HepPoint & whereLocal() const;
  virtual double energy() const {return _energy;}
  virtual void setEnergy(double x) {_energy = x; }
  virtual void addEnergy(double x) {_energy += x; }
  virtual double simenergy() const {return _simenergy;}
  virtual void setSimEnergy(double x) {_simenergy = x; }
  virtual void addSimEnergy(double x) {_simenergy += x; }
  virtual void print( std::ostream& o ) const;
  virtual const GTDouble* gtrkWeights() const { return &_gtrkWeights; }
  virtual void addGTrackWeight(const GTrack *gtrk, const double weight);
  virtual const double waveFormPeakE() const { return _waveFormPeakE; }
  virtual const WaveFormTime waveFormInTime() const { return _wtime;}
  virtual void setWaveFormPeakE(double x) { _waveFormPeakE=x; }
  virtual void setWaveFormInTime(WaveFormTime x) { _wtime=x; }

private:

  // Data members
  double _energy;
  double _simenergy;
  long _tciIndex, _thetaIdx, _phiIdx;
  double _thetaReal, _phiReal;
  double  _deltaTheta, _deltaPhi;
  HepPoint _where, _whereLocal;
  GTDouble _gtrkWeights;
  double _waveFormPeakE;
  WaveFormTime _wtime;

  void init();

public:

  // Selectors (const)

  static double getRescaleFactor(){ return _rescaleFactor; };
  static double getPositionDepth(){ return _positionDepth; };

private:

  static double _rescaleFactor;
  static double _positionDepth;


};
#endif
