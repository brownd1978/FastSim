//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsRecoCaloErrorMatrix.hh 486 2010-01-13 16:36:34Z stroili $
//
// Description:
//	Helper class to compute the error matrix in AbsRecoCalo
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Stephane Willocq        University of Massachusetts
//
//------------------------------------------------------------------------

#ifndef ABSRECOCALOERRORMATRIX_HH
#define ABSRECOCALOERRORMATRIX_HH

#include "CLHEP/Vector/ThreeVector.h"

//		---------------------
// 		-- Class Interface --
//		---------------------

class AbsRecoCaloErrorMatrix {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AbsRecoCaloErrorMatrix();

  // Destructor
  virtual ~AbsRecoCaloErrorMatrix();

  // Helper functions to compute the error matrix
  Hep3Vector vecFromXtalCtr( const Hep3Vector* aVector ) const;
  double sigmaVsE( const double* paramVector, const double energy ) const;
  double sigmaPhi( double, double, double ) const;
  double sigmaTheta( double, double, double ) const;


protected:

  // Modifiers

private:

};



#endif // ABSRECOCALOERRORMATRIX




