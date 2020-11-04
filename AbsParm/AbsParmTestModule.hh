//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsParmTestModule.hh 489 2010-01-13 16:45:39Z stroili $
//
// Description:
//	This is a test module for the AbsParm package.  It serves several
//      purposes:
//         1.  Since the list command is template code, it does not get
//             compiled during the library build.  So, we make sure to
//             instantiate it here in order to make sure it compiles.
//         2.  Actually test the code.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Ed Frank                   University of Pennsylvania
//
// Copyright Information:
//	Copyright (C) 1994
//
// History:
//      16Dec98  Ed Frank          First version
//      13Jul99  Michael Kelsey    Add testing of AbsParmNamedObject
//
// BUGS:
//------------------------------------------------------------------------

#ifndef ABSPARMTESTMODULE_HH
#define ABSPARMTESTMODULE_HH

#include "Framework/AppModule.hh"
#include "AbsParm/AbsParmVector.hh"
#include "AbsParm/AbsParmNamedValue.hh"
#include "AbsParm/AbsParmPairs.hh"

class AbsParmTestModule : public AppModule {
public:
  // Constructors
  AbsParmTestModule(const char* const theName = "AbsParmTestModule", 
    const char* const theDescription = "AbsParm regression test");
  AbsParmTestModule( const char* const newName,
		     const char* const theDescription,
		     const AbsParmTestModule& theOther );

  // Destructor
  virtual ~AbsParmTestModule( );

  // Operators
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob  ( AbsEvent* anEvent );
  virtual AppResult abortJob( AbsEvent* anEvent );
  virtual AppModule* clone(const char* cloneName);

private:

  enum AbsParmTestEnum { APEVal1=1, APEVal2=5, APEVal3=-12 };

  AbsParmVector<double>      _doubleParmList;
  AbsParmVector<float>       _floatParmList;
  AbsParmVector<long>        _longParmList;
  AbsParmVector<int>         _intParmList;
  AbsParmVector<std::string>   _stringParmList;
  AbsParmVector<bool>         _boolParmList;

  AbsParmNamedValue<AbsParmTestEnum> _enumParm;
  AbsParmVector<int>         _symIntParmList;
  AbsParmPairs<std::string, int> _pairList;

};

#endif // ABSPARMTESTMODULE_HH
