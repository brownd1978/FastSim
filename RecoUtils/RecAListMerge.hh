#ifndef RecAListMerge_hh
#define RecAListMerge_hh
//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: 
//
// Description:
//      Module RecAListMerge -- utility module to merge together
//      an arbitrary set of input lists to a single output list, which
//      can be either a deep or shallow copy of the original, and can treat
//      (pointer!) duplicates either as an error, silently suppress
//      duplicates, or it will respect the duplication.  Note that
//      the instantiation class _must_ have a copy constructor to
//      support deep copy.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Dave Brown
//
// Copyright Information:
//      Copyright (C) 2005 LBNL
//
//------------------------------------------------------------------------

// Forward Declarations

#include "Framework/AppModule.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "AbsParm/AbsParmVector.hh"
#include "AbsParm/AbsParmNamedValue.hh"

#include <vector>

template <class A> class HepAList;
class IfdStrKey;

template <class A>
class RecAListMerge : public AppModule {
public:
// enum flags for behavior
  enum outputCopy {shallow=0,deep};
  enum duplicatePolicy {error=0,ignore, respect};
// constructor
  RecAListMerge( const char* name,
                 const char* description);
  virtual ~RecAListMerge();
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
private:
// policy flags
  AbsParmNamedValue<outputCopy> _copy;
  AbsParmNamedValue<duplicatePolicy> _dpol;
// parameters to control input/output
  AbsParmVector<std::string> _input; // lists to merge
  AbsParmIfdStrKey _output;		// merged output list
// cache Ifd keys
  std::vector<IfdStrKey*> _keys;
};

#ifdef    BABAR_COMP_INST
#include  "RecoUtils/RecAListMerge.icc"
#endif // BABAR_COMP_INST

#endif
