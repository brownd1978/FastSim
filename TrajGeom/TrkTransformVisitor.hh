//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkTransformVisitor.hh 524 2010-01-15 12:09:29Z stroili $
//
// Description:
//     Applies a HepTransform to a TrkGeomTraj to rotate and translate it.  
//     Part of a Visitor pattern; use by creating a TransformVisitor object 
//     and passing it to the accept() function in a TrkGeomTraj.  The 
//     visitor will create a new (transformed) trajectory, which can 
//     be obtained via the newTraj() function.  Ownership of the new 
//     trajectory is passed to the caller.
//
// Sample use:
//   TrkLineTraj myLine(start, end);
//   HepTransformation myTran(myRotation, myTranslation);
//   TrkTransformVisitor visitor(myTran);
//   myLine.accept(visitor);
//   TrkGeomTraj* newTraj = visitor.newTraj();
//    . . .
//   delete newTraj;
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Steve Schaffner
//
//------------------------------------------------------------------------

#ifndef TRKTRANSFORMVISITOR_HH
#define TRKTRANSFORMVISITOR_HH
#include "BbrGeom/TrkGeomTrajVisitor.hh"
#include <memory>

class HepTransformation;
class TrkGeomTraj;

// Class interface //
class TrkTransformVisitor : public TrkGeomTrajVisitor {

public:
  TrkTransformVisitor(const HepTransformation&);
  virtual ~TrkTransformVisitor();

  virtual void visitLine(const TrkLineTraj*);
  //  virtual void visitParabola(const TrkParabolaTraj*);
  virtual void visitPieceLine(const TrkPieceLineTraj*);
  virtual void visitHelix(const BbrHelixTraj*);
  
  // transfers ownership!
  TrkGeomTraj* newTraj();

private:
  const HepTransformation& _trans;
  std::unique_ptr<TrkGeomTraj> _newTraj;
  // Preempt 
  TrkTransformVisitor&   operator= (const TrkTransformVisitor&);
  TrkTransformVisitor(const TrkTransformVisitor &);
};

#endif
