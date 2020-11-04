//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkTransformVisitor.cc 524 2010-01-15 12:09:29Z stroili $
//
// Description:
//     
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Steve Schaffner
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
#include "TrajGeom/TrkTransformVisitor.hh"
#include "TrajGeom/TrkPieceLineTraj.hh"
#include "TrajGeom/TrkLineTraj.hh"
#include "CLHEP/Geometry/Transformation.h"
#include "ErrLogger/ErrLog.hh"
#include <assert.h>

TrkTransformVisitor::TrkTransformVisitor(const HepTransformation& t)
        : _trans(t) 
{
}

TrkTransformVisitor::~TrkTransformVisitor()
{
}

void
TrkTransformVisitor::visitLine(const TrkLineTraj* theTraj)
{
  HepPoint oldStart = theTraj->position( theTraj->lowRange() );
  HepPoint   oldEnd = theTraj->position( theTraj->hiRange() );
  HepPoint newStart = _trans.transFrom(oldStart);
  HepPoint   newEnd = _trans.transFrom(oldEnd);
  _newTraj.reset( new TrkLineTraj(newStart, newEnd) );
}

void
TrkTransformVisitor::visitHelix(const BbrHelixTraj*)
{
  ErrMsg(error) << 
    "TrkTransformVisitor: no transformation implemented for helix." << endmsg;
}

void
TrkTransformVisitor::visitPieceLine(const TrkPieceLineTraj* theTraj)
{
  const std::vector<std::pair<double,TrkGeomTraj*> >& trajList = theTraj->trajList();
  assert(!trajList.empty());
  // Create pieceTraj from 1st piece of old (after transforming)
  Trajectory *t = trajList[0].second;
  HepPoint oldStart = t->position( t->lowRange() );
  HepPoint   oldEnd = t->position( t->hiRange() );
  HepPoint newStart = _trans.transFrom(oldStart);
  HepPoint   newEnd = _trans.transFrom(oldEnd);
  TrkPieceLineTraj *pt = new TrkPieceLineTraj(newStart, newEnd);
  _newTraj.reset( pt );
  for (int i= 1; i < trajList.size(); ++i) {
     HepPoint   oldEnd = trajList[i].second->position( trajList[i].second->hiRange() );
     HepPoint   newEnd = _trans.transFrom(oldEnd);
     pt->append(newEnd);
  }
}

TrkGeomTraj*
TrkTransformVisitor::newTraj()
{
  // Only return the new trajectory once, to maintain correct ownership
  TrkGeomTraj* trjPtr = _newTraj.release();
  _newTraj.reset(0);
  return trjPtr;
}

