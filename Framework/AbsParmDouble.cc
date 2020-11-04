//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsParmDouble.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AbsParm
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
#include "Framework/AbsParmDouble.hh"

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

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AbsParmDouble::AbsParmDouble( 
    const char* const theCommand, AppModule* theTarget, double def )
    : AbsParm( theCommand, theTarget ), _x(def)
{
}

AbsParmDouble::AbsParmDouble( 
    const AbsParmDouble& theOther, AppModule* theTarget)
    : AbsParm( theOther, theTarget)
    , _x(theOther._x)
{
}

//--------------
// Destructor --
//--------------

AbsParmDouble::~AbsParmDouble( )
{
}

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------


int 
AbsParmDouble::setHandler( )
{
    AppModule* theInput = target( );

    if ( _argc == 3 )
    {
      // the following should be done with streams!
       sscanf (getArgument( 2 ), "%lg", &_x);
    }
    else if (_argc<3)
      theInput->fullReport("Need to specify new value");
    else
      theInput->fullReport("Only one value permitted");

    return AbsInterp::OK;
}

int 
AbsParmDouble::listHandler( ) const
{
    AppModule* theInput = target( );
    char buf[100];

    theInput->partialReport("Value of ");
    theInput->partialReport(command());
    theInput->partialReport(" for module ");
    theInput->partialReport(theInput->name());
    sprintf(buf, ": %g", _x);
    theInput->fullReport(buf);

    return AbsInterp::OK;
}


int
AbsParmDouble::getHandler ( )
{
  return AbsInterp::OK;
}

void
AbsParmDouble::restoreState(char* var[])
{
        int rval;
        setArgs(3, var);
        rval = setHandler();
}  
