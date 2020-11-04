#include "Experiment/Experiment.hh"
#include "EvtGenBase/EvtMRes.hh"

EvtMRes::~EvtMRes()
{
    for(int i=0; i<_children.size(); ++i)
        delete _children[i];
}
