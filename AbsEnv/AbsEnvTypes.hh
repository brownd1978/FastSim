#ifndef ABSENVFWDDECL_HH
#define ABSENVFWDDECL_HH

#include "BbrStdUtils/CollectionUtils.hh"

#include <vector>
using std::vector;
#include <set>
using std::set;
class AbsDetIndex;
// typedef struct BbrPtrLess<AbsDetIndex> AbsDetIndPtrLess;
#include "AbsEnv/AbsDetCoord.hh"
#include "AbsEnv/NeighbourStore.hh"
typedef vector<AbsDetIndex*> AbsIndexStore;
  
#endif
