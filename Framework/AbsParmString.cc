//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsParmString.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AbsParmString
//
//      See header for more info
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Ed Frank                University of Pennsylvania
//      Marc Turcotte           University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AbsParmString.hh"

//-------------
// C Headers --
//-------------
#include <iostream>
#include <string.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppModule.hh"
#include "Framework/AppConfigRecorder.hh"
#include "FrameUtil/AbsInterp.hh"

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
AbsParmString::AbsParmString( const char* const theCommand,
				    AppModule* theTarget,
				    const std::string& def )
    : AbsParm( theCommand, theTarget )
    , _x( def )
{
}

AbsParmString::AbsParmString( const AbsParmString& theOther,
				    AppModule* theTarget )
    : AbsParm( theOther, theTarget)
    , _x( theOther._x )
{
}
 
//--------------
// Destructor --
//--------------
AbsParmString::~AbsParmString()
{
}

//-------------
// Operators --
//-------------

int
AbsParmString::operator==( const AbsParmString& other ) const
{
  // Here we use the value() function to obtain the std::string&, rather
  // than use the std::string directly.
  return ( value() == other.value() && 
	   (strcmp(command(),other.command()) == 0) );
}

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

int 
AbsParmString::setHandler( )
{
    AppModule* theInput = target( );

    if ( _argc == 3 ) {
      // the following should be done with streams!
      _x = getArgument( 2 );  // getarg(2) is a char*
    } else if (_argc<2)
      theInput->fullReport("Need to specify new value");
    else
      theInput->fullReport("Only one value permitted");

    return AbsInterp::OK;
}

int 
AbsParmString::listHandler( ) const
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

int 
AbsParmString::getHandler()
{
  return AbsInterp::OK;
}

const std::string&
AbsParmString::value() const
{
  return _x;
}

void
AbsParmString::restoreState(char* var[])
{

  int rval;
  //need to put " around the string to make sure spaces in the 
  //string, if any, are handled correctly
  //char* temp = new char[strlen(var[2]) + 3];
  //strcpy ( temp, "\x022");
  //strcat ( temp, var[2]);
  //strcat (temp, "\x022");
  //delete var[2];
  //var[2] = temp;
  setArgs(3, var);
  rval = setHandler();

}  
