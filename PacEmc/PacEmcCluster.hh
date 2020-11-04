//-----------------------------------------------------------------------
// File and Version Information:
//   PacEmcCluster
//     Class for PacEmcCluster for fast simulation, a stripped down version
//     from Babar's EmcCluster.
//
// Author List:
//     Chih-hsiang Cheng       Caltech   (initial version 2008/06/13)
//
//-----------------------------------------------------------------------
#ifndef PACEMCCLUSTER_HH
#define PACEMCCLUSTER_HH

#include <vector>
using std::vector;

#include "AbsEnv/TwoCoordIndex.hh"
#include "AbsCalo/AbsRecoCalo.hh"
#include "AbsCalo/AbsCaloTypes.hh"
#include "PacEmc/PacEmcDigi.hh"
#include "PacEmc/PacEmcModel.hh"
#include "CLHEP/Matrix/SymMatrix.h"
#include <iostream>

class PacEmcClusterEnergySums;
class PacEmcXClMoments;
class TrkLineTraj;

//              ---------------------
//              -- Class Interface --
//              ---------------------

typedef std::vector< PacEmcDigi* >::const_iterator PacEmcDigiConstIterator;
typedef std::vector< PacEmcDigi* >::iterator PacEmcDigiIterator;

struct CluShapePars {
  CluShapePars() : _init(false),_RMxp(0),_RMxm(0),_RMyp(0),_RMym(0),_emEMFrac(-1),_hadEMFrac(-1), _hadSx(1), _hadSy(1), _noiseOcc(0) {}
  // accessors
  bool init() const { return _init; }
  void init(bool b) { _init=b;}
  double RMxp() const { return _RMxp; }
  double RMxm() const { return _RMxm; }
  double RMyp() const { return _RMyp; }
  double RMym() const { return _RMym; }
  double emEMFrac() const { return _emEMFrac; }
  double hadEMFrac() const { return _hadEMFrac; }
  double hadSx() const { return _hadSx; } // hadron shower envelop sigma x
  double hadSy() const { return _hadSy; } // hadron shower envelop sigma y
  double noiseOcc() const { return _noiseOcc; }
  // modifiers
  void copyPars(const CluShapePars *c) {
    _init= c->init();
    _RMxp= c->RMxp();    _RMxm= c->RMxm();
    _RMyp= c->RMyp();    _RMym= c->RMym();
    _emEMFrac= c->emEMFrac();
    _hadEMFrac= c->hadEMFrac();
    _hadSx= c->hadSx();
    _hadSy= c->hadSy();
    _noiseOcc= c->noiseOcc();
  }
  void setRMPars(const CluShapePars *c) {
    setRM(c->RMxp(),c->RMxm(),c->RMyp(),c->RMym());
  }
  void setRM(const double RMxp,const double RMxm,const double RMyp,const double RMym) {
    _RMxp=RMxp; _RMxm=RMxm; _RMyp=RMyp; _RMym=RMym;
  }
  void setEmEMFrac(const double x) {_emEMFrac=x; }
  void setHadEMFrac(const double x) {_hadEMFrac=x; }
  void setHadSigma(const double x, const double y) {_hadSx=x; _hadSy=y;}
  void setNoiseOcc(const double x) { _noiseOcc=x; }
  // data members
  bool _init;
  double _RMxp, _RMxm, _RMyp, _RMym;
  double _emEMFrac, _hadEMFrac, _hadSx, _hadSy;
  double _noiseOcc;
};

class PacEmcCluster : public AbsRecoCalo {

public:
  
  // Typedefs, consts, and enums
  enum CentroidMethod { gravity=0, logarithmic, neighbour, lilo };
  enum NoiseType { LowEnergy=0, BadTiming, EnergyTime } ;

  // Constructor
  PacEmcCluster(CentroidMethod algo= logarithmic);
  // Desctructor
  virtual ~PacEmcCluster();
  
  // override the dynamic-cast method to pretend to give an EmcCand.
  // THIS IS A KLUDGE which allows BtaTuple to dump neutrals, but it cannot be a long-term
  // solution.  This may also cause crashes in analysis code trying to interpret BtaCandidates
  // directly in terms of EmcCand and not AbsRecoCalo
  virtual const EmcCand* dynamic_cast_EmcCand() const { return (EmcCand*)0x1; }

  virtual const std::vector< PacEmcDigi* >* pacEmcDigis() const;
  virtual const std::vector< PacEmcDigi* >* localMaxima();
  virtual PacEmcDigi* getDigi(const TwoCoordIndex& tci) const;
  virtual PacEmcDigi* getDigi(const long &itheta, const long &iphi) const;

  //  virtual const std::set< const TwoCoordIndex* >* extendedTcis() const {return &_extendedTcis;}

  // Total Energy of Cluster
  virtual double energy( const HepPoint& from = HepPoint(0,0,0),
                         const PdtEntry* particle = 0  ) const;
  virtual double rawEnergy() const;

  virtual double calibFactor() const;

  const CentroidMethod centroidAlgo() const { return _centroidAlgo; }

  // Theta centroid of cluster
  virtual double theta() const;
  // Phi   centroid of cluster
  virtual double phi() const;

  // Theta centroid of cluster in index units
  virtual double thetaIndex() const;
  virtual int    thetaIndexInt() const;
  // Phi   centroid of cluster in index units 
  virtual double phiIndex() const;
  virtual int    phiIndexInt() const;

  // Theta centroid of cluster using crystal index number
  virtual double thetaFromIndex() const;
  // Phi   centroid of cluster using crystal index number
  virtual double phiFromIndex() const;

  // Radius that covers the entire cluster
  virtual double radius() const;

  // Angle of major axis wrt phi
  virtual double major_axis() const;

  // Second moment matrix (2X2) in theta, phi space
  virtual HepSymMatrix secondMomentMatrix() const ;

  // Digi with k-th largest energy in cluster
  const PacEmcDigi* maxima() const;

  // Digi with next to cluster centroid
  //const PacEmcDigi centrum() const;

  // Direction to ctr clus
  virtual HepPoint position( const HepPoint &from=HepPoint(0,0,0), 
                             const PdtEntry* particle=0 ) const;

  // Direction to ctr clus
  virtual HepPoint rawPosition( const HepPoint &from=HepPoint(0,0,0), 
                                const PdtEntry* particle=0 ) const;

  // Error Matrix for Cluster
  virtual BbrError errorMatrix( const HepPoint &from=HepPoint(0,0,0), 
                                const PdtEntry* particle=0 ) const;
  virtual BbrError errorMatrixXYZ( const HepPoint &from=HepPoint(0,0,0), 
				   const PdtEntry* particle=0 ) const;

  // Invariant mass of cluster
  virtual double mass() const;

  // Where is cluster?
  virtual HepPoint where() const;

  // The region this cluster belongs to.
  virtual PacEmcModel::EmcRegion region() const;

  // x co-ordinate of clus
  virtual double x() const;
  // y co-ordinate of clus
  virtual double y() const;
  // z co-ordinate of clus
  virtual double z() const;

  // Distance from centre of cluster to a point
  virtual double distanceToCentre( const HepPoint& aPoint ) const;
  virtual double distanceToCentre( const PacEmcDigi* aDigi ) const;

  virtual const unsigned numberOfDigis() const ;
  virtual const unsigned nBumps() const;
  virtual void setNBumps(unsigned n);
  void invalidateCache(bool keepLocalMax=false);

  virtual const PacEmcClusterEnergySums& esums() const;
  virtual const PacEmcXClMoments& Xmoments() const;

// represent the cluster as a line in space
  TrkLineTraj clusterTraj() const;

  void print( std::ostream& o = std::cout , bool verbose= false);

  // Modifiers ====
  void addDigi( const PacEmcDigi& d, bool merge= false );
  void addDigi( const TwoCoordIndex* tci, const double &energy, bool merge= false );
  void addNoise();
  void removeNoise(NoiseType type= LowEnergy, const double ecut=-1);
  void addCluster( PacEmcCluster* c);
  void mergeCluster( const PacEmcCluster* c);
  void scale(double x);
  void searchLocalMax();
  void setDigiEnergy(const TwoCoordIndex& tci, double e, double se=-1);
  void sortDigis();
  bool isCalibrated() { return _calibrated; }
  void setFirstSimhitTime(const double &t) {_firstSimhitTime= t;}
  void setRecordTime(const double &t) { _recordTime= t;}
  void setRecordTimeErr(const double &t) { _recordTimeErr= t;}
  void simulateHitTime();

  void digiTciRange();
  long tciThetaLow();
  long tciThetaHi();
  long tciPhiLow();
  long tciPhiHi();

  void fluctuateDigis();
  void fluctuateDigi(PacEmcDigi* digi, const double& fa, const double& fb, const double& ep, double cexp=0, double dexp=0, double pexp=0);
  void fluctuateCluster();
  double smearE(const double& energy, const double& fa, const double& fb, const double& ep, double cexp=0, double dexp=0, double pexp=0);
  bool overlap(PacEmcCluster &other) const;
  void addSimTrkEnergyChange(const double& e) { _simtrkEnergyChange+= e; }
  void setSimTrkEnergyChange(const double& e) { _simtrkEnergyChange= e; }
  double simtrkEnergyChange() const { return _simtrkEnergyChange; }
  double firstSimhitTime() const { return _firstSimhitTime;}
  double recordTime() const { return _recordTime;}
  double recordTimeErr() const { return _recordTimeErr;}

  // Whether two clusters are connected
  bool isConnectedTo(const PacEmcCluster* other) const;

  CluShapePars* shapePars() const { return _shapePars; }

  virtual void setCalibFactor(const double x); 

protected:

  // Helper functions

  virtual void clear(); // make cluster empty

private:

  // Members
  std::vector< PacEmcDigi* > _pacEmcDigis;
  std::vector< PacEmcDigi* > _localMaxima;

  std::set< const TwoCoordIndex* > _extendedTcis;  // all digis' tcis plus immediate neighbor

  // Cachable data 
  mutable bool _energyValid;
  mutable double _rawenergy;
  mutable double _calibFactor;
  mutable bool _whereValid;
  mutable HepPoint _where;
  mutable bool _regionValid;
  mutable bool _rangeValid;
  mutable bool _foundLocalMax;
  mutable bool _calibrated;
  mutable bool _extendedTciValid;
  mutable bool _radiusValid;
  mutable double _radius;
  mutable double _simtrkEnergyChange;
  mutable double _firstSimhitTime, _recordTime, _recordTimeErr;
  unsigned _nbumps;
  mutable PacEmcModel::EmcRegion _region;

  mutable PacEmcClusterEnergySums* _theClusEnergySums;
  mutable PacEmcXClMoments* _theClusXClMoments;

  mutable HepSymMatrix _secondMoment;

  const CentroidMethod _centroidAlgo;
  long _tciThetaLow,_tciThetaHi,_tciPhiLow,_tciPhiHi;
  CluShapePars *_shapePars;

  void findExtendedTcis();

  // Disallow copy, op=
  PacEmcCluster(const PacEmcCluster &);
  PacEmcCluster & operator=(const PacEmcCluster &);


//------------------
// Static Members --
//------------------

public:

  static HepPoint gravWhere( const PacEmcCluster* me ) ;
  static HepPoint privateLogWhere( const PacEmcCluster* me ) ;

};

#endif

