#ifndef REC_ALIST_COPY_PARMS_HH
#define REC_ALIST_COPY_PARMS_HH
//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: RecAListCopyParms.hh 32 2010-01-15 11:45:31Z stroili $
//
// Description:
//      Class RecAListCopyParms; templated subclass of RecCopyParms which handles
//      the list managment in case the data object is a HepAList
//
//	NOTE:  This class must be created from a Framework Module.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Michael Kelsey <kelsey@slac.stanford.edu
//      Dave Brown   copied from TrkCopyParms
//
// Copyright Information:
//      Copyright (C) 2001  Trustees of Stanford University
//
//------------------------------------------------------------------------

#include "RecoUtils/RecCopyParms.hh"

template <class A> class HepAList;
class AbsEvent;

template <class LISTOBJ> class RecAListCopyParms : public RecCopyParms {
public:

  // NOTE:  Public ctor MUST be passed a non-null module
  RecAListCopyParms(AppModule* theModule,
		    const char *inKeyString="Default",
		    const char *outKeyString="Default",
		    const RecCopyParms::Action&
		    initialAction=RecCopyParms::inplace);

  // NOTE:  Default ctor doesn't create TCL parameters.  Client
  // 	    code _must_ call createParms or risk a core dump
  RecAListCopyParms();

  virtual ~RecAListCopyParms();
// The list accessors
  HepAList<LISTOBJ>* findInputList() const { return _input; }
  HepAList<LISTOBJ>* findOutputList() const { return _output; }
// add an output object for the given input.  This does 'the right thing',
// depending on the state of the parameters
  LISTOBJ* addOutput(LISTOBJ& input);
// remove the object from the output list (if it exists)
  void removeOutput(LISTOBJ& output);
// setup and post-processing for each event.  These perform whatever list manipulation
// is required for the various modes
  void prepare(AbsEvent* event);
  void finalize();
protected:
// function to create copy objcts.
  LISTOBJ* copyObject(const LISTOBJ&) const;
private:
  // Hide the copy and assignment operators
  RecAListCopyParms(const RecAListCopyParms<LISTOBJ>&);
  RecAListCopyParms& operator=(const RecAListCopyParms<LISTOBJ>&);
  HepAList<LISTOBJ>* _removed;// list of objects removed from the input list
  HepAList<LISTOBJ>* _input; // cache of input list
  HepAList<LISTOBJ>* _output; // cache of output list
};

#include  "RecoUtils/RecAListCopyParms.icc"

#endif

