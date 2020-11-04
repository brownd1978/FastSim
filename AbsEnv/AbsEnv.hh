//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsEnv.hh 483 2010-01-13 14:03:08Z stroili $
//
// Description:
//	Class AbsEnv
//
//      Top level of the "environment tree" for BaBar reconstruction
//      software
//
//      A global symbol "gblEnv" is defined to hold a pointer to
//      the AbsEnv object.  The environment is generally expected to
//      be passed as a parameter, starting with the module entrypoints,
//      but this global will give access from inside classes for now.
//
//      In general, if you are coding a member function call to a 
//      class that needs access to environment info, make the last 
//      argument an AbsEnv* and default it to gblEnv.  For example,
//           doFoo(double dist, AbsEnv* env=gblEnv)
//      This way, the argument being omitted does a reasonable default
//      thing.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Bob Jacobsen		        Original Author
//      Stephen J. Gowdy                Revised
//
// Copyright Information:
//	Copyright (C) 1995		Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------

#ifndef ABSENV_HH
#define ABSENV_HH

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class GenEnv;
class PepEnv;
class EmcEnv;
class DchEnv;
class SvtEnv;
class IfrEnv;
class DrcEnv;
class TrkEnv;
class L1DctEnv;
class L1EmtEnv;
class L1GltEnv;
class L3TEnv;
class OprEnv;
class CfgEnv;
class BdbConfigEnv;
class IfdProxyDict;
class DhpMonEnv;
class BtaEnv;
class FiltEnv;
class DnaEnv;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AbsEnv {

public:
  // Constructors
  AbsEnv();
  
  // Destructor
  virtual ~AbsEnv();

  // member functions
  virtual GenEnv* getGen();
  virtual PepEnv* getPep();
  virtual EmcEnv* getEmc();
  virtual DchEnv* getDch();
  virtual SvtEnv* getSvt();
  virtual IfrEnv* getIfr();
  virtual DrcEnv* getDrc();
  virtual TrkEnv* getTrk();
  virtual L1DctEnv* getL1Dct();
  virtual L1EmtEnv* getL1Emt();
  virtual L1GltEnv* getL1Glt();
  virtual L3TEnv* getL3T();
  virtual OprEnv* getOpr();
  virtual CfgEnv* getCfg();
  virtual BdbConfigEnv* getBdbConfig();
  virtual DhpMonEnv* getDhpMon();
  virtual BtaEnv* getBta();
  virtual FiltEnv* getFilt();
  virtual DnaEnv* getDna();
    
  virtual void setGen(   GenEnv*   envPointer );
  virtual void setPep(   PepEnv*   envPointer );
  virtual void setEmc(   EmcEnv*   envPointer );
  virtual void setDch(   DchEnv*   envPointer );
  virtual void setSvt(   SvtEnv*   envPointer );
  virtual void setIfr(   IfrEnv*   envPointer );
  virtual void setDrc(   DrcEnv*   envPointer );
  virtual void setTrk(   TrkEnv*   envPointer );
  virtual void setL1Dct( L1DctEnv* envPointer );
  virtual void setL1Emt( L1EmtEnv* envPointer );
  virtual void setL1Glt( L1GltEnv* envPointer );
  virtual void setL3T(   L3TEnv*   envPointer );
  virtual void setOpr(   OprEnv*   envPointer );
  virtual void setCfg(   CfgEnv*   envPointer );
  virtual void setBdbConfig( BdbConfigEnv* envPointer );
  virtual void setDhpMon( DhpMonEnv* envPointer);
  virtual void setBta( BtaEnv* envPointer );
  virtual void setFilt( FiltEnv* envPointer );
  virtual void setDna( DnaEnv* envPointer );
    
  virtual void resetGen() { _genEnv = 0; }
  virtual void resetPep() { _pepEnv = 0; }
  virtual void resetEmc() { _emcEnv = 0; }
  virtual void resetDch() { _dchEnv = 0; }
  virtual void resetSvt() { _svtEnv = 0; }
  virtual void resetIfr() { _ifrEnv = 0; }
  virtual void resetDrc() { _drcEnv = 0; }
  virtual void resetTrk() { _trkEnv = 0; }
  virtual void resetL1Dct() { _l1DctEnv = 0; }
  virtual void resetL1Emt() { _l1EmtEnv = 0; }
  virtual void resetL1Glt() { _l1GltEnv = 0; }
  virtual void resetL3T() { _l3tEnv = 0; }
  virtual void resetOpr() { _oprEnv = 0; }
  virtual void resetCfg() { _cfgEnv = 0; }
  virtual void resetBdbConfig() { _bdbConfigEnv = 0; }
  virtual void resetDhpMon() { _dhpMonEnv = 0; }
  virtual void resetBta() { _btaEnv = 0; }
  virtual void resetFilt() { _filtEnv = 0; }
  virtual void resetDna() { _dnaEnv = 0; }

private:
  GenEnv* _genEnv;
  PepEnv* _pepEnv;
  EmcEnv* _emcEnv;
  DchEnv* _dchEnv;
  SvtEnv* _svtEnv;
  IfrEnv* _ifrEnv;
  DrcEnv* _drcEnv;
  TrkEnv* _trkEnv;
  L1DctEnv* _l1DctEnv;
  L1EmtEnv* _l1EmtEnv;
  L1GltEnv* _l1GltEnv;
  L3TEnv* _l3tEnv;
  OprEnv* _oprEnv;
  BdbConfigEnv* _bdbConfigEnv;
  CfgEnv* _cfgEnv;
  DhpMonEnv* _dhpMonEnv;
  BtaEnv* _btaEnv;
  FiltEnv* _filtEnv;
  DnaEnv* _dnaEnv;
};


//-------------------------
// global pointer to env --
//-------------------------

extern AbsEnv* gblEnv;
extern IfdProxyDict* gblPEnv;

#endif // ABSENV_HH
