//
// $Id: NeighbourStore.hh 483 2010-01-13 14:03:08Z stroili $
//

// Moved this here from AbsEnv/AbsEnvTypes.hh to avoid including other things
// contained there in places where this needs to go  PE 030527

#ifndef NeighbourStore_HH
#define NeighbourStore_HH

#include <set>
using std::set;
class AbsDetIndex;
typedef set<AbsDetIndex*> NeighbourStore;

#endif
