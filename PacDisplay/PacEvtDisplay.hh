#ifndef PACEvtDISPLAY_HH
#define PACDISPLAY_HH

#include <cstdio>
#include <vector>
#include <map>
#include <string>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include "PacDisplay/PacDispMat.hh"
#include "PacDisplay/PacDispCyl.hh"
#include "PacDisplay/PacDispCone.hh"
#include "PacDisplay/PacDispRing.hh"
#include "PacDisplay/PacDispRect.hh"
#include "PacDisplay/PacDispTrackTraj.hh"
#include "PacDisplay/PacDispPoint.hh"
#include "PacDisplay/PacDispSimHit.hh"
#include "PacDisplay/PacDispCluster.hh"

class AbsRecoCalo;
class TrkRecoTrk;
class GTrack;
class BField;
class PacSimTrack;
class Trajectory;
class DetMaterial;
class TObjString;
class PacCylDetector;
class TParticle;

class PacEvtDisplay {
public:
  PacEvtDisplay();
  virtual ~PacEvtDisplay();
  void init(const char* filename,unsigned resolution,bool verbose=false);
  void reset();
  void finalize();
  void fillTrees();
  void drawDetector();
  void drawVoxels();
  void drawSimHits(const PacSimTrack* track,unsigned istrk);
  void drawGTrack(const GTrack* track,double range, const BField* bfield);
  void drawParticle(const TParticle* part,double range, const BField* bfield);
  void drawSimTrack(const PacSimTrack* track);
  void drawRecTrack(const TrkRecoTrk* track);
  void drawCluster(const AbsRecoCalo* calo);
private:
  bool _verbose;
  unsigned _resolution;
    //disp.root:
  TFile* _dispfile;
    // trees
  TTree *_materials, *_cylinders, *_rings, *_rects, *_cones, *_gentrks, *_simtrks, *_rectrks, *_simhits, *_clusters;
  TTree *_vcylinders, *_vrings, *_vrects;
    // structs to hold geometry objects
  PacDispMat _dmat;
  PacDispCyl _dcyl;
  PacDispRing _dring;
  PacDispRect _drect;
  PacDispCone _dcone;
  PacDispCyl _vcyl;
  PacDispRing _vring;
  PacDispRect _vrect;
  // detector
  PacCylDetector* _detector;
  // material name
  std::map<std::string,int> _matlist;
  // vectors to store multiple objects/event 
  // trajs record which points go with which traj
  std::vector<PacDispTrackTraj> _gtrajs;
  std::vector<PacDispPoint> _gpoints;
  std::vector<PacDispTrackTraj> _strajs;
  std::vector<PacDispPoint> _spoints;
  std::vector<PacDispTrackTraj> _rtrajs;
  std::vector<PacDispPoint> _rpoints;
  // vector of simhits/event
  std::vector<PacDispSimHit> _shits;
  // vector of clusters/event
  std::vector<PacDispCluster> _clusts;
  // utility functions
  void fillTrajPoints(const Trajectory* traj, std::vector<PacDispPoint>& points);
  int fillMat(const DetMaterial* detmat);
};
#endif
