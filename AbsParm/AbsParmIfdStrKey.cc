//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsParmIfdStrKey.cc 489 2010-01-13 16:45:39Z stroili $
//
// Description:
//	Class AbsParmIfdStrKey
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
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "AbsParm/AbsParmIfdStrKey.hh"

//-------------
// C Headers --
//-------------
#include <iostream>
#include <sstream>
#include <string>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "Framework/AppModule.hh"
#include "FrameUtil/AbsInterp.hh"
#include "CLHEP/String/Strings.h"


//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AbsParmIfdStrKey::AbsParmIfdStrKey( 
    const char* const theCommand, AppModule* theTarget, HepString def )
    : AbsParm( theCommand, theTarget ),
      _x( new IfdStrKey( def ) )
{
}

AbsParmIfdStrKey::AbsParmIfdStrKey(const AbsParmIfdStrKey& theOther,
  AppModule* theTarget)
      : AbsParm( theOther, theTarget),
      _x( new IfdStrKey( theOther._x->asString() ) )
{
}

//--------------
// Destructor --
//--------------

AbsParmIfdStrKey::~AbsParmIfdStrKey( ) {
  delete _x;
  _x = 0;
}

//-------------
// Operators --
//-------------

int
AbsParmIfdStrKey::operator==(const AbsParmIfdStrKey&other) const{
  // Here we use the value() function to obtasin the IfdKey&, rather
  // than use the IfdStrKey directly.
  return (value()==other.value() && 
	  (strcmp(command(), other.command()) == 0) );
}

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------


int 
AbsParmIfdStrKey::setHandler( )
{
    AppModule* theInput = target( );

    if ( _argc == 3 ) 
      {
	// the following should be done with streams!
	delete _x;
	_x = new IfdStrKey( getArgument( 2 ) );  // getarg(2) is a char*
      } 
    else if (_argc < 3)
      theInput->fullReport("Need to specify new value");
    else
      theInput->fullReport("Only one value permitted");

    return AbsInterp::OK;
}

int 
AbsParmIfdStrKey::listHandler( ) const
{
  AppModule* theInput = target( );

  theInput->partialReport("Value of ");
  theInput->partialReport(command());
  theInput->partialReport(" for module ");
  theInput->partialReport(theInput->name());
  theInput->partialReport(": ");

  // Format the key into a string for output.
  std::ostringstream bufstream;
  bufstream << value();
  theInput->fullReport(bufstream.str().c_str());  // no string interface!

  return AbsInterp::OK;
}

int 
AbsParmIfdStrKey::getHandler()
{
  return AbsInterp::OK;
}

void
AbsParmIfdStrKey::restoreState(char* var[])
{

  int rval;

  // ads 10Jun00 - removed this because it is not true.
  //need to put " around the string to make sure spaces in the 
  //string, if any, are handled correctly
  //  char* temp = new char[strlen(var[2]) + 3];
  // strcpy ( temp, "\x022");
  //strcat ( temp, var[2]);
  //strcat (temp, "\x022");
  //delete var[2];
  //var[2] = temp;

  setArgs(3, var);
  rval = setHandler();

}  


