//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppFileName.hh 509 2010-01-14 15:18:55Z stroili $
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

#ifndef APPFILENAME_HH
#define APPFILENAME_HH

#include <string>

class AppFileName
{
public:

  // Constructors
  AppFileName( const char* const );
  AppFileName( const AppFileName& theOther);
  
  // Destructor
  ~AppFileName( );
  
  // value accessor 
  const std::string& value() const;
  
  // get the pathname (ie path and file name.)
  std::string pathname() const;

  // check if the file exists
  bool ifExist() const;

  // set the value
  void set(const std::string& );
  
private:
  std::string _fileName;

  // Not implemented.
  AppFileName& operator=( const AppFileName& );

};

// inline implementations
#include "Framework/AppFileName.icc"

#endif





