//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppFileName.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppFileName permits multiple path search. 
//      Doesn't need to be part of the Framework
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Luca Lista		Original Author
//
// Copyright Information:
//      Copyright (C) 1999      INFN 
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

#include "Framework/AppFileName.hh"
#include "CommonUtils/ComPathNameSearch.hh"

AppFileName::AppFileName( const char* const fileName ) :
  _fileName( fileName )
{
}

AppFileName::AppFileName( const AppFileName& theOther ) :
  _fileName(theOther._fileName)
{
}

AppFileName::~AppFileName( ) 
{
}

std::string AppFileName::pathname() const
{
  return std::string((const char*) ComPathNameSearch( _fileName.c_str() ) ); 
}

bool AppFileName::ifExist() const
{
  return ( ComPathNameSearch(_fileName.c_str()).length() != 0 ? true : false );
}


