//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppMethodBase.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	AppMethodBase Class Definiton.
//
// Environment:
//
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//      This software was developed in cooperation with CLEO.
//
// Author List:
//	Marc Turcotte	                Original Author February '98
//      Marc Turcotte                   Changed for Frame/Record 3/26/98
//      Marc Turcotte                   Changed to deliver Frames 4/7/98
//      Marc Turcotte                   Changed for expanded binder 6/19/98
//      Marc Turcotte                   Changed AppStatus to AppResult 6/23/98
//
// Copyright Information:
//	Copyright (C) 1998		University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef APPMETHODBASE_HH
#define APPMETHODBASE_HH

//
// Collaborating Class Headers
//

#include "Framework/AppResult.hh"
class AppRecord;
class AppFrame;

//
// Class Interface
//

class AppMethodBase{

protected:
  bool _hasExecuted;
  
public:
  AppMethodBase(bool theInitialExecuteState);
  virtual ~AppMethodBase();
  virtual AppResult execute(AppFrame* aFrame )=0;
  void reset(bool toThis);

private:
  // Not implemented.
  AppMethodBase( const AppMethodBase& );
  AppMethodBase& operator=( const AppMethodBase& );

};

#endif







