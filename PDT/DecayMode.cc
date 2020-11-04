//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: DecayMode.cc 445 2010-01-15 08:53:50Z stroili $
//
//------------------------------------------------------------------------
//
//  Moved some stuff here from DecayMode.hh to avoid nested includes
//
#include "BaBar/BaBar.hh"
#include "PDT/DecayMode.hh"
#include "PDT/PdtEntry.hh"

#include <iomanip>
#include <vector>
using std::ostream;
using std::setprecision;
using std::setw;
using std::vector;

DecayMode::DecayMode(float bf, vector<PdtEntry*> *l ) {
  _branchingFraction = bf;
  _children = l;
}

DecayMode::~DecayMode() { 
  if ( _children ) { 
    //need to destroy also -4602
    vector<PdtEntry*>::iterator iter=_children->begin();
    while ( iter != _children->end() ) { delete *iter; ++iter; }
    _children->clear();
  }
  delete _children; 
  _children=0;
}

void 
DecayMode::printOn(ostream& os) const {
  os <<   "               " << setw(5) << setprecision(3)
    << _branchingFraction
    << " ->";
  int l = _children->size();
  for(int i=0; i<l; i++)
    os << " " << (*_children)[i]->name();
  os << '\n';
} 

