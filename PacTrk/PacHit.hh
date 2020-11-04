/* Class: PacHit
 *
 * A trivial implememantion of TrkFundHit, to be used by PacHitOnTrk. */

#ifndef PacHit_HH
#define PacHit_HH

#include "TrkBase/TrkDetElemId.hh"
#include "TrkBase/TrkEnums.hh"
#include "TrkBase/TrkFundHit.hh"

class GTrack;

class PacHit : public TrkFundHit {
public:
    PacHit(TrkEnums::TrkViewInfo view, int id, TrkDetElemId::systemIndex sys);
    virtual ~PacHit();

    PacHit* clone() const;
// obsolete inteface
    virtual TrkEnums::TrkViewInfo whatView() const      { return _view; }
    virtual TrkDetElemId elemId() const                 { return _eid; }
    virtual const GTrack* getGTrack() const             { return NULL; }
// fake the layer number
    int layerNumber() const { return _eid.systemElemId();}

protected:
    PacHit(const PacHit& other);
    TrkEnums::TrkViewInfo _view;
    TrkDetElemId _eid;
};

#endif
