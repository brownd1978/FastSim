//-----------------------------------------------------------------------
// File and Version Information:
//     PacEmcModel :  The model assumes a cylinder barrel (minimum requirement)
//        and a ring or cone forward endcap and a ring or cone backward endcap.
//        The input arguments are: 
//          barrelR, barrelZmin, barrelZmax:  the radius and z range of barrel
//          fwdZ, fwdLowR, fwdHiR, fwdCotTheta: the z (plane coordinate of the
//                ring or vertex coordinate of the cone), the R range, and
//                the inverse of the slope of the cone surface. If fwdCotTheta
//                is zero, the geometry is a ring.
//             ** The R ranges are the distances from the z vertex to the edges
//                of the cone in space along the cone surface.
//          bwdZ, bwdLowR, bwdHiR, bwdCotTheta: same things for backward.
//
//
// Copyright Information:
//      Copyright (C) 2008      Caltech
//
//  Authors:  Chih-hsiang Cheng    2008/07/22
//-----------------------------------------------------------------------
#ifndef PacEmcModel_HH
#define PacEmcModel_HH

#include <iostream>
#include <vector>

#include "CLHEP/Geometry/HepPoint.h"

class TwoCoordIndex;
class PacEmcAbsCalibrator;
class EdmlDetector;
class PacEmcModel {

public:
  
  enum EmcRegion {Unknown=-1, FWD=0, Barrel, BWD, NRegions};

  static PacEmcModel* createNewModel();
  static PacEmcModel* getModel();

  PacEmcModel();
  virtual ~PacEmcModel();

  void init(const EdmlDetector* det);

  void addBarrel(const double R, const double zmin, const double zmax, const double thick);
  void addForward(const double Rmin, const double Rmax, const double tanth, const double zv, const double thick);
  void addForward(const double Rmin, const double Rmax, const double zv, const double thick);
  void addBackward(const double Rmin, const double Rmax, const double tanth, const double zv, const double thick);
  void addBackward(const double Rmin, const double Rmax, const double zv, const double thick);

  void buildCalibrator();
  const PacEmcAbsCalibrator* getCalibrator(EmcRegion region) const;

  const bool initialized() const { return _initialized; }
  const long getTciIndex(const long ix, const long iy) const;

  const TwoCoordIndex* realCoordToTci(const HepPoint& pos) const;
  const TwoCoordIndex* localCoordToTci(const HepPoint& pos) const;
  const double tciToRealThetaLocal(const TwoCoordIndex& tci) const;
  const double tciToRealPhiLocal(const TwoCoordIndex& tci) const;
  const HepPoint where(const TwoCoordIndex& tci, const int eid=-1) const;
  const HepPoint whereLocal(const TwoCoordIndex& tci, const int eid=-1) const;
  const HepPoint whereLocal(const double theta, const double phi, const int eid=-1) const;
  const TwoCoordIndex* getTci(long index) const;
  const TwoCoordIndex* getTci(long ix, long iy) const;
  EmcRegion region(const long i) const;
  //  EmcRegion region(const TwoCoordIndex& tci) const;
  EmcRegion region(const HepPoint &point) const;
  const double deltaFwdTheta() const { return _deltaFwdTheta; }
  const double deltaFwdPhi(const long i) const;
  const double deltaBwdTheta() const { return _deltaBwdTheta; }
  const double deltaBwdPhi() const { return _deltaBwdPhi; }
  const double deltaBarrelTheta(const long i) const;
  const double deltaBarrelPhi() const { return _deltaBarrelPhi; }
  const double deltaTheta(const TwoCoordIndex& tci) const ;
  const double deltaTheta(const long itheta) const ;
  const double deltaPhi(const TwoCoordIndex& tci) const;
  const double deltaPhi(const long itheta) const;
  const double rMoliere(const TwoCoordIndex& tci) const;
  const double rMoliereFluc(const TwoCoordIndex& tci) const;
  const double crystalGap(const TwoCoordIndex& tci) const;

  const double energyResCoef(const TwoCoordIndex& tci) const;
  const double energyResConst(const TwoCoordIndex& tci) const;
  const double energyResEPower(const TwoCoordIndex& tci) const;
  const double energyResCoef(EmcRegion region) const;
  const double energyResConst(EmcRegion region) const;
  const double energyResEPower(EmcRegion region) const;
  const double energyResExpCoef(EmcRegion region) const;
  const double energyResExpConst(EmcRegion region) const;
  const double energyResExpEPower(EmcRegion region) const;

  const double thetaResolution(const TwoCoordIndex& tci) const;
  const double phiResolution(const TwoCoordIndex& tci) const;
  const double thetaResolution(EmcRegion region) const;
  const double phiResolution(EmcRegion region) const;
  const double radialResolution(const TwoCoordIndex& tci) const;
  const double radialResolution(EmcRegion region) const;
  const double radialResolution(const long itheta) const;

  const double thickness(const TwoCoordIndex& tci) const;
  const double thickness(EmcRegion region) const;

  const double barrelBegin() const { return _barrelBegin; }
  const double barrelEnd() const { return _barrelEnd; }
  const double fwdBegin() const { return _fwdBegin; }
  const double fwdEnd() const { return _fwdEnd; }
  const double bwdBegin() const { return _bwdBegin; }
  const double bwdEnd() const { return _bwdEnd; }
  const int barrelNRings() const { return _barrelNRings; }
  const int barrelNPhis() const { return _barrelNPhis; }
  const int fwdNRings() const { return _fwdNRings; }
  std::vector< int > fwdNPhis() { return _fwdNPhis; }
  const int bwdNRings() const { return _bwdNRings; }
  const int bwdNPhis() const { return _bwdNPhis; }
  const int nPhis(const long theta) const;

  const unsigned nbinsPhiSimTrkKE() const { return _nbinsPhiSimTrkKE; }
  const unsigned nbinsThetaSimTrkKE() const { return _nbinsThetaSimTrkKE; }
  const double simTrkKEThreshold() const { return _simTrkKEThreshold; }

  const double barrelRadius(const unsigned i) const { return _barrelR[i]; }
  const double barrelZmin(const unsigned i) const { return _barrelZmin[i]; }
  const double barrelZmax(const unsigned i) const { return _barrelZmax[i]; }
  const double fwdRmin(const unsigned i) const { return _fwdRmin[i]; }
  const double fwdRmax(const unsigned i) const { return _fwdRmax[i]; }
  const double fwdTanth(const unsigned i) const { return _fwdTanth[i]; }
  const double fwdZv(const unsigned i) const { return _fwdZv[i]; }
  const double bwdRmin(const unsigned i) const { return _bwdRmin[i]; }
  const double bwdRmax(const unsigned i) const { return _bwdRmax[i]; }
  const double bwdTanth(const unsigned i) const { return _bwdTanth[i]; }
  const double bwdZv(const unsigned i) const { return _bwdZv[i]; }

  const double hadShowerSigma(const TwoCoordIndex& tci) const ;
  const double hadShowerMaxR(const TwoCoordIndex& tci) const ;
  const double hadShowerQuanta(const TwoCoordIndex& tci) const ;
  const double hadShowerSmear(const TwoCoordIndex& tci) const ;
  const double hadShowerMinE(const TwoCoordIndex& tci) const ;
  const double ecutoff(const TwoCoordIndex& tci) const ;
  const double digiThreshold(const TwoCoordIndex& tci) const ;

  const double emShowerEMfrac() const { return _emShowerEMfrac; }
  const double emShowerEMfracSigma() const { return _emShowerEMfracSigma; }
  const double hadShowerEMfrac() const { return _hadShowerEMfrac; }
  const double hadShowerEMfracSigma() const { return _hadShowerEMfracSigma; }

  const double emEssentricity(const TwoCoordIndex& tci) const ;
  const double emEssenRMS(const TwoCoordIndex& tci) const ;
  const double emAsymmetry(const TwoCoordIndex& tci) const ;
  
  const double noiseOccupancy(const TwoCoordIndex& tci) const ;
  const double noiseOccSigma(const TwoCoordIndex& tci) const ;
  const double noiseExpConst(const TwoCoordIndex& tci) const ;

  const double showerDepth(const TwoCoordIndex& tci) const ;

  const double signalTLo(const EmcRegion& re) const { return _signalTLo[re];}
  const double signalTHi(const EmcRegion& re) const { return _signalTHi[re];}
  const double acceptWindowLo(const EmcRegion& re) const { return _acceptWindowLo[re];}
  const double bkgWindowLo(const EmcRegion& re) const { return _bkgWindowLo[re];}
  const double pulseTshift(const EmcRegion& re) const { return _pulseTshift[re];}
  

  //  const double signalShapeIntegral(const double& t0, const EmcRegion& region, bool normalize=false);
  const double signalFraction(const double & t0, const  EmcRegion& region) const;

  const double timeResolution(const PacEmcModel::EmcRegion region) const;

  const Hep3Vector& origin() const { return _origin; }

  const double pulseShape(const double t, const double t0, const PacEmcModel::EmcRegion region) const;
  const double pulseShapeNorm(const double t, const double t0, const PacEmcModel::EmcRegion region) const;
  void findPulseMax();


  void print( std::ostream& o = std::cout );
  

protected:

private:

  void setupThetaRange();
  void readConfig();
  double sAtan(const double &at);
  EmcRegion regionLocal(const double &theta) const;  // EMC coordinate
  EmcRegion region(const double theta) const {return PacEmcModel::Unknown;}// Make sure nobody is using it


  bool _initialized;

  std::vector< double > _barrelR;
  std::vector< double > _barrelZmin;
  std::vector< double > _barrelZmax;
  std::vector< double > _barrelThick;
  std::vector< double > _fwdRmin;
  std::vector< double > _fwdRmax;
  std::vector< double > _fwdTanth;
  std::vector< double > _fwdZv;
  std::vector< double > _fwdThick;
  std::vector< double > _bwdRmin;
  std::vector< double > _bwdRmax;
  std::vector< double > _bwdTanth;
  std::vector< double > _bwdZv;
  std::vector< double > _bwdThick;

  int _nfwdLayers, _nbarrelLayers, _nbwdLayers;

  double _barrelBegin;
  double _barrelEnd;
  int _barrelNRings;
  int _barrelNPhis;
  double _etaBarrelBegin;
  double _etaBarrelEnd;
  double _deltaBarrelEta;
  double _deltaBarrelPhi;
  std::vector< double > _deltaBarrelTheta;

  double _fwdBegin;
  double _fwdEnd;
  int _fwdNRings;
  double _deltaFwdTheta;
  std::vector< int > _fwdNPhis;
  std::vector< int > _fwdNPhisAccum;
  std::vector< double > _deltaFwdPhi;

  double _bwdBegin;
  double _bwdEnd;
  int _bwdNRings;
  int _bwdNPhis;
  double _deltaBwdTheta;
  double _deltaBwdPhi;

  unsigned _nCrysFwd;
  unsigned _nCrysBarrel;
  unsigned _nCrysBwd;

  unsigned _firstFwdRing, _firstBarrelRing, _firstBwdRing;
  unsigned _nCrysBeforeFwd, _nCrysBeforeBarrel, _nCrysBeforeBwd;
  
  std::vector<double> _thickness;

  std::vector<double> _rMoliere;
  std::vector<double> _rMoliereFluc;

  std::vector<double> _crystalGap;  // gap (distance) between crystals

  // number of bins of a 2D histogram that stores simtrk kinetic energy at EMC
  unsigned _nbinsPhiSimTrkKE, _nbinsThetaSimTrkKE;
  double _simTrkKEThreshold;

  // The energy is smeared by a Gaussian and an exponential on the 
  // lower-side tail. The Gaussian sigma = sqrt[(a/E^p1)^2 + b^2].
  // The exponential decay constant is tau= sqrt[(c/E^p2)^2 + d^2]
  std::vector<double> _eResCoef;    // a
  std::vector<double> _eResConst;   // b
  std::vector<double> _eResEPower;  // p1
  std::vector<double> _eResExpCoef;  // c
  std::vector<double> _eResExpConst;  // d
  std::vector<double> _eResExpEPower;  // p2

  std::vector<double> _thetaResolution;
  std::vector<double> _phiResolution;
  std::vector<double> _radialResolution;   // Used for track-emc matching

  std::vector<double> _hadShowerSigma, _hadShowerMaxR, _hadShowerQuanta, _hadShowerSmear, _hadShowerMinE, _ecutoff, _digiThreshold;

  std::vector<double> _emEssentricity, _emEssenRMS, _emAsymmetry;
  std::vector<double> _noiseOccupancy, _noiseOccSigma, _noiseExpConst;
  
  double _emShowerEMfrac, _emShowerEMfracSigma;
  double _hadShowerEMfrac, _hadShowerEMfracSigma;

  std::vector<double> _showerDepth;
  // Calibration parameters
  std::string _calibType;
  std::vector<double> _calibParsFWD, _calibParsBarrel, _calibParsBWD;

  std::vector<const PacEmcAbsCalibrator*> _calibrators;

  // Single timing window and shape
  std::vector<double> _signalTLo, _signalTHi, _acceptWindowLo, _bkgWindowLo;
  std::vector<double> _sigF1, _sigTau1, _sigTau2;  // scintillator light
  std::vector<double> _sigFilterT1, _sigFilterT2;     // CR-RC filter time const.
  std::vector<double> _pulseShapeMax, _pulseTshift;
  //  std::vector<double> _signalIntNorm;

  // hit time resolution; gaussian sigma
  std::vector<double> _timeResolution;

  std::vector<TwoCoordIndex*> *_tciList;

  double _zshift;
  Hep3Vector _origin;
  bool _foundPulseMax;

};
#endif
