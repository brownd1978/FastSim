//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppMethodBase.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	AppMethodBase Class Implementation.
//
// Environment:
//
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//      This software was developed in cooperation with CLEO.
//
// Author List:
//	Marc Turcotte	                Original Author February '98
//
// Copyright Information:
//	Copyright (C) 1998		University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//
// This Class's Header 
//
#include "Framework/AppMethodBase.hh"

//
// C Headers 
//

//
// Collaborating Class Headers 
//

//
// Local Macros, Typedefs, Structures, Unions and Forward Declarations 
//


//
// Public Function Member Definitions
//

//
// Constructor(s)
//

AppMethodBase::AppMethodBase(bool theInitialExecuteState) :
_hasExecuted(theInitialExecuteState){}

//
// Destructor 
//
AppMethodBase::~AppMethodBase() {}

//
// Methods
//

void
AppMethodBase::reset(bool toThis) {_hasExecuted=toThis;} 






