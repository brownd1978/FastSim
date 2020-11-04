//#ifndef PacDisplayMacro_HH
//#define PacDisplayMacro_HH
#include <vector>
#include <string>
#include "TGeoVolume.h"
#include "TGeoMaterial.h"
#include "TGeoManager.h"
#include "TGeoMedium.h"
#include "TGFrame.h"
#include "TTree.h"
#include "TPolyLine3D.h"
#include "TPolyMarker3D.h"
#include "PacDisplay/PacDispTrackTraj.rdl"
#include "PacDisplay/PacDispPoint.rdl"
#include "PacDisplay/PacDispSimHit.rdl"
#include "PacDisplay/PacDispCluster.rdl"
#include "PacDisplay/PacDispMat.rdl"
#include "PacDisplay/PacDispCyl.rdl"
#include "PacDisplay/PacDispRing.rdl"
#include "PacDisplay/PacDispRect.rdl"
#include "PacDisplay/PacDispCone.rdl"

class TObject;
class TFile;
class TGLScene;
class TGLViewer;
class TCanvas;
class TString;
//class TGLSceneBase;
//class TGLLockable;

class PacDisplayMacro {
public:
  ~PacDisplayMacro();
  PacDisplayMacro(const char* file = "display.root",std::string s = "srhde",int transpareny = 90,int startevent=0,
    double rmax=1000,double zmin=-1000, double zmax=1000);
  void Next();
  void Prev();
  void DisplayChargePid(int charge,int pid);
  const char* effectName(int effect);
  void OnClicked(TObject *obj);
//  void OnClicked(Int_t event, Int_t x, Int_t y, TObject *obj);
  void DrawEvent();
  void resetEvent();
  void DrawDetector();
  void DrawTracks();
  void DrawTrajs();
  void DrawMarks();
  void DrawVoxels();
  void DrawClusters();
  void drawSimHits();
  void PrintLegend();
  void loadTrees();
  void Draw();
  
  void setRMax(double rmax) { _rmax = rmax;}
  void setZMax(double zmax) { _zmax = zmax;}
  void setZMin(double zmin) { _zmin = zmin;}
  void setEvent(int event) { _currentEv = event;}
  
private:

  void fillMaterial(int,std::string& matname,TGeoMaterial*& matmed,TGeoMedium*& medium,int& dcolor);
  
  TFile *_tfile;
  TTree *_materials, *_cylinders, *_rings, *_rects, *_cones, *_gentrks, *_simtrks, *_rectrks, *_simhits, *_clusters;
  TTree *_vcylinders, *_vrings, *_vrects;
  
// branch local varaibles

  PacDispMat _dmat;
  PacDispCyl _dcyl;
  PacDispRing _dring;
  PacDispRect _drect;
  PacDispCone _dcone;
  PacDispCyl _vcyl;
  PacDispRing _vring;
  PacDispRect _vrect;

  std::vector<PacDispTrackTraj>* _gtrajs;
  std::vector<PacDispPoint>* _gpoints;
  std::vector<PacDispTrackTraj>* _strajs;
  std::vector<PacDispPoint>* _spoints;
  std::vector<PacDispTrackTraj>* _rtrajs;
  std::vector<PacDispPoint>* _rpoints;
  std::vector<PacDispSimHit>* _shits;
  std::vector<PacDispCluster>* _clusts;
// branch pointers
  TBranch *_matbr, *_cylbr, *_ringbr, *_rectbr, *_conebr, *_gtrajbr, *_gptbr, *_strajbr, *_sptbr, *_rtrajbr, *_rptbr, *_shitbr, *_clustbr;
  TBranch *_vcylbr, *_vringbr, *_vrectbr;
  
  TGLViewer * _viewer;
  TCanvas* _can;
  TGMainFrame* _frame;
  int _objectID; 
  int _currentEv;
  int _maxEv;
  std::string _type;
  int _transparency;
  double _rmax;
  double _zmin,_zmax;
  TGeoManager *_geom;
  TGeoMaterial *_matVacuum;
  TGeoMedium *_Vacuum;
  TGeoVolume *_top, *_detector, *_event;
  std::vector<TPolyLine3D*> _trajs;
  std::vector< TPolyMarker3D*> _hmarks;
  	
};
//#endif
