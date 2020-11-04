//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsRecoCalo.hh 486 2010-01-13 16:36:34Z stroili $
//
// Description:
//	Abstract class for all calorimetry objects.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Bob Jacobsen            LBNL
//	Stephen J. Gowdy        University of Edinburgh
//
//------------------------------------------------------------------------

#ifndef ABSRECOCALO_HH
#define ABSRECOCALO_HH

//---------------
// C++ Headers --
//---------------
#include <vector>

//----------------------
// Base Class Headers --
//----------------------
#include "AbsCalo/AbsCaloTypes.hh"
#include "AbsEvent/AbsEvtObj.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "CLHEP/Geometry/HepPoint.h"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class BbrError;
class EmcBump;
class EmcCand;
class EmcCluster;
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Vector/LorentzVector.h"
class IfrAbs3D;
class NeutralHad;
class PdtEntry;
class AbsRecoCaloErrorMatrix;

//		---------------------
// 		-- Class Interface --
//		---------------------

class AbsRecoCalo : public AbsEvtObj {

//--------------------
// Instance Members --
//--------------------

public:

  friend class EmcMiniCandCompositeP;
  friend class EmcMiniBumpK;

  // Constructors
  AbsRecoCalo();

  // Copy Constructor

  // Destructor
  virtual ~AbsRecoCalo( );

  // Operators
  virtual bool operator==( const AbsRecoCalo& toCompare ) const;
  virtual bool operator==( const AbsEvtObj& toCompare ) const
                           { return this->AbsEvtObj::operator==( toCompare ); }
  virtual bool operator<( const AbsRecoCalo& toCompare ) const;

  // Selectors (const)

  // This is the energy (best guess)
  virtual double energy( const HepPoint& from = HepPoint(0,0,0), 
			 const PdtEntry* particle = 0 ) const = 0;

  // This is the raw energy
  virtual double rawEnergy( const HepPoint& from = HepPoint(0,0,0), 
			    const PdtEntry* particle = 0 ) const;

  // This is a unit vector describing the direction (in xyz)
  // as seen from the given point "from"
  virtual Hep3Vector direction( const HepPoint& from = HepPoint(0,0,0),
				const PdtEntry* particle = 0 ) const;

  // This is the position in xyz (and not the offset from "from")
  virtual HepPoint position( const HepPoint& from = HepPoint(0,0,0),
			     const PdtEntry* particle = 0 ) const = 0;

  // This is the raw position 
  virtual HepPoint rawPosition( const HepPoint& from = HepPoint(0,0,0),
				const PdtEntry* particle = 0 ) const;

  // This is (px,py,pz,E)
  virtual HepLorentzVector fourMomentum( const HepPoint& from =HepPoint(0,0,0),
					 const PdtEntry* particle=0 ) const;

  // This is the error on the energy
  virtual double energyErr( const HepPoint& from = HepPoint(0,0,0),
			    const PdtEntry* particle = 0 ) const;

  // This is the error on the direction (3x3 with xyz)
  // --> matrix is singular (direction is unit vector)
  virtual BbrError directionErr( const HepPoint& from = HepPoint(0,0,0),
				 const PdtEntry* particle = 0 ) const;

  // This is the error on the position (3x3 with xyz)
  virtual BbrError positionErr( const HepPoint& from = HepPoint(0,0,0),
				const PdtEntry* particle = 0 ) const;

  // This is the error matrix for the four vection (4x4 px,py,pz,E)
  virtual BbrError fourMomentumErr( const HepPoint& from=HepPoint(0,0,0),
				    const PdtEntry* particle = 0 ) const;

  // This is the error matrix of the measurement (4x4 with E,theta,phi,r)
  // --> "r" must be understod as the distance to the "cluster"
  //     from the "from" point
  virtual BbrError errorMatrix( const HepPoint& from = HepPoint(0,0,0), 
				const PdtEntry* particle = 0 ) const = 0;

  // This is the error matrix of the measurement (4x4 with x,y,z,E)
  BbrError errorMatrixXYZ( const HepPoint& from = HepPoint(0,0,0), 
				const PdtEntry* particle = 0 ) const ;

  // Used for overlaps.   Returns a vector of length one - overridden by
  // base classes.
// the following function is deprecrated and should not be called
  virtual AbsRecoCaloPVec madeFrom() const;

// direct overlap test, needed to avoid multi-bump confusion
  virtual bool overlaps(const AbsRecoCalo&) const;

  // The following are a substitution for real dynamic_cast
  virtual const EmcBump* dynamic_cast_EmcBump() const;
  virtual const EmcCand* dynamic_cast_EmcCand() const;
  virtual const EmcCluster* dynamic_cast_EmcCluster() const;
  virtual const IfrAbs3D* dynamic_cast_IfrAbs3D() const;
  virtual const NeutralHad* dynamic_cast_NeutralHad() const;
  virtual EmcBump* dynamic_cast_EmcBump();
  virtual EmcCand* dynamic_cast_EmcCand();
  virtual EmcCluster* dynamic_cast_EmcCluster();
  virtual IfrAbs3D* dynamic_cast_IfrAbs3D();
  virtual NeutralHad* dynamic_cast_NeutralHad();

  const int id() const {return _id;};

  inline static void resetID() { _nextId = 0; }

protected:
  BbrError
  errorMatrixOfEmcClusters(double energy,double theta,double phi) const;

  // Modifiers
  inline void setId(unsigned int id) {_id = id; }

private:
  
  // static id counter
  static unsigned int _nextId;

  unsigned int _id;

};

// global function for the cluster covariance matrix




#endif // ABSRECOCALO




