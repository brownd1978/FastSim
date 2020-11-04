//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsParmFilename.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AbsParmFileName
//
//      See header for more info
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Bob Jacobsen		Original Author
//      Marc Turcotte           Merged in R.Kapur's TK interface 5/98
//      Michael Kelsey		Add comments about search path
//
// Copyright Information:
//	Copyright (C) 1996	Lawrence Berkeley Laboratory
//      Copyright (C) 1998      University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AbsParmFilename.hh"
#include "FrameUtil/AbsInterp.hh"

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
#include "Framework/AppConfigRecorder.hh"
#include "Framework/AppModule.hh"

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AbsParmFilename::AbsParmFilename( 
    const char* const theCommand, AppModule* theTarget, std::string def )
    : AbsParm( theCommand, theTarget ), _fileName( def.c_str())
{
}

AbsParmFilename::AbsParmFilename(
    const AbsParmFilename& theOther, AppModule* theTarget)
    : AbsParm( theOther, theTarget)
    , _fileName( theOther._fileName)
{
}

//--------------
// Destructor --
//--------------

AbsParmFilename::~AbsParmFilename( )
{
}

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------


int AbsParmFilename::setHandler( )
{
  AppModule* theInput = target( );
  
  if ( _argc == 3 )
  {
    // the following should be done with streams!
    _fileName.set( getArgument( 2 ) );  // getarg(2) is a char*
      }
  else if (_argc < 3)
    theInput->fullReport("Need to specify new value");
  else
    theInput->fullReport("Only one value permitted");
  
  return AbsInterp::OK;  
}

int AbsParmFilename::listHandler( ) const
{
    AppModule* theInput = target( );

    theInput->partialReport("Value of ");
    theInput->partialReport(command());
    theInput->partialReport(" for module ");
    theInput->partialReport(theInput->name());
    theInput->partialReport(": ");
    theInput->fullReport(value());

    return AbsInterp::OK;
}

int AbsParmFilename::getHandler()
{
  return AbsInterp::OK;
}

std::string AbsParmFilename::pathname()  const
{
  std::string theCompleteName(_fileName.pathname());
  if (theCompleteName.length() == 0) {    
    //
    // This reproduces the behaviour of the original version of AbsParmFileName
    //
    //cerr << "Could not find datafile " << _fileName.value() << endl;
    if (_fileName.value().at(0) != '/') {
    //  cerr << "   Path was \"" << _fileName.getFilenameSearchPath() 
    //	   << "\"" << endl; 
      exit(1);
    }

  }
  return theCompleteName; 
}

void AbsParmFilename::restoreState(char* var[])
{
  int rval;
  //need to put " around the string to make sure spaces in the 
  //string, if any, are handled correctly
  //  char* temp = new char[strlen(var[2]) + 3];
  //strcpy ( temp, "\x022");
  //strcat ( temp, var[2]);
  //strcat (temp, "\x022");
  //delete var[2];
  //var[2] = temp;
  setArgs(3, var);
  rval = setHandler();

}  

