#ifndef REC_COPY_PARMS_HH
#define REC_COPY_PARMS_HH
//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: RecCopyParms.hh 32 2010-01-15 11:45:31Z stroili $
//
// Description:
//      Class RecCopyParms -- utility class to add consistent TCL parameters
//	to modules for modifying list objects and moving them from an input to
//	an output list.
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

// Forward Declarations

class AppModule;
class AbsParmIfdStrKey;
template <class V> class AbsParmNamedValue;
class IfdKey;

class RecCopyParms {
public:
  // Flags for what modules should do to tracks
  // Modify in place, remove from input, or copy to output
  enum Action { inplace=0, steal, copy, borrow, none, nActions, preActions=-1 };

  // NOTE:  Public ctor MUST be passed a non-null module
  RecCopyParms(AppModule* theModule,
	       const char *inKeyString="Default",
	       const char *outKeyString="Default",
	       const RecCopyParms::Action&
	             initialAction=RecCopyParms::inplace);

  // NOTE:  Default ctor doesn't create TCL parameters.  Client
  // 	    code _must_ call createParms or risk a core dump
  RecCopyParms();

  virtual ~RecCopyParms();

  // Accessor functions

  const IfdKey& inputKey() const;	// Return parameter values
  const IfdKey& outputKey() const;
  const AbsParmIfdStrKey* inputList() const { return _inputKey; }
  const AbsParmIfdStrKey* outputList() const { return _outputKey; }
  RecCopyParms::Action action() const;

  // Logic flags -- action parameter and keys are evaluated
  bool sameAsInput()    const;
  bool stealFromInput() const;
  bool copyFromInput()  const;
  bool borrowFromInput()  const;
  bool noOutput() const;
  bool sameLists() const;


  // Return string corresponding to action flag
  const char* actionName() const { return actionName(action()); }

  static const char* actionName(const RecCopyParms::Action& anAction);

  // Configuration functions -- can only be used once; will assert otherwise

  void createParms(AppModule* theModule,
		   const char *inKeyString,
		   const char *outKeyString,
		   const RecCopyParms::Action&
		   initialAction=RecCopyParms::inplace);
  
private:
  AbsParmIfdStrKey* _inputKey;		// list key for input tracks
  AbsParmIfdStrKey* _outputKey; 	// list key for output tracks
  AbsParmNamedValue<RecCopyParms::Action>* _outputAction;

  // Hide the copy and assignment operators
  RecCopyParms(const RecCopyParms&);
  RecCopyParms& operator=(const RecCopyParms&);
};

#endif /* REC_COPY_PARMS_HH */
