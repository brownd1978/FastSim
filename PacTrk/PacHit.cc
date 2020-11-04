/*******************************************************************************
 * PacHit
 *******************************************************************************/

#include "BaBar/BaBar.hh"
#include "PacTrk/PacHit.hh"


PacHit::PacHit(TrkEnums::TrkViewInfo v, int id, TrkDetElemId::systemIndex sys)
    : _view(v), _eid(id,sys)
{
}

PacHit::PacHit(const PacHit& other)
    : _view(other._view), _eid(other._eid)
{
}

PacHit::~PacHit() { 
}

PacHit* PacHit::clone() const {
    return new PacHit(*this);
}
