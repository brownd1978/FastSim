// ------------------------------------------------------------------------------
// File and Version Information:
//   PacTrkHitView
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 14May2008
// ------------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
#include "PacTrk/PacTrkHitView.hh"
#include "PacTrk/PacHitOnTrk.hh"
#include "PacSim/PacSimHit.hh"

PacTrkHitView::PacTrkHitView(TrkEnums::TrkViewInfo view) : _view(view)
{}

PacTrkHitView::~PacTrkHitView()
{}

double
PacTrkHitView::efficiency(const PacSimHit&) const {
	return 1.0;
}

