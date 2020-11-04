//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsParmBool.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AbsParmBool
//
//      See header for more info
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Bob Jacobsen		Original Author
//      Marc Turcotte           Merged in R.Kapur's TK interface 5/98
//
// Copyright Information:
//	Copyright (C) 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1998      University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AbsParmBool.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <iostream>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppModule.hh"
#include "FrameUtil/AbsInterp.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AbsParmBool::AbsParmBool( 
    const char* const theCommand, AppModule* theTarget, bool def )
    : AbsParm( theCommand, theTarget ), _x(def)
{
}

AbsParmBool::AbsParmBool( const AbsParmBool& theOther, AppModule* theTarget)
    : AbsParm( theOther, theTarget ), _x(theOther._x)
{
}

//--------------
// Destructor --
//--------------

AbsParmBool::~AbsParmBool( ){
}

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------


int 
AbsParmBool::setHandler( )
{
  AppModule* theInput = target( );
  
  if ( _argc == 3 )
    // the following should be done with streams!
    if (getArgument( 2 )[0]=='t' || getArgument(2)[0] =='T'
	|| getArgument(2)[0] =='1') 
      {
	_x = true;
      }
    else if (getArgument( 2 )[0]=='f' || getArgument(2)[0] =='F'
	     || getArgument(2)[0] =='0') 
      {
	_x = false;
      }
    else {
      theInput->fullReport("value must start with [TFtf10]");
    }
  else if (_argc<2)
    theInput->fullReport("Need to specify new value");
  else
    theInput->fullReport("Only one value permitted");
  
  return AbsInterp::OK;
}

int 
AbsParmBool::listHandler( ) const
{
  AppModule* theInput = target( );
  
  theInput->partialReport("Value of ");
  theInput->partialReport(command());
  theInput->partialReport(" for module ");
  theInput->partialReport(theInput->name());
  theInput->partialReport(": ");
  (value() ? theInput->fullReport("t") : theInput->fullReport("f") );
  
  return AbsInterp::OK;
}

int 
AbsParmBool::getHandler(){ 
  return AbsInterp::OK;
}

void
AbsParmBool::restoreState(char* var[])
{
        int rval;
        setArgs(3, var);
        rval = setHandler();
}  













