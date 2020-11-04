//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppFileStream.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppFileStream. The output stream class, implementing files.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------

#ifndef APPFILESTREAM_HH
#define APPFILESTREAM_HH

//-------------
// C Headers --
//-------------
#include <stddef.h>

//---------------
// C++ Headers --
//---------------
#include <fstream>

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppStream.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppFileStream : public AppStream {

  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppFileStream( 
		const char* const theName, 
		const char* const theFile        = NULL, 
		const char* const theDescription = NULL );
  
  // Destructor
  virtual ~AppFileStream( );
  
  // Selectors (const)
  const char* filename( ) const;
  bool        isOpen  ( ) const;
  
  // Modifiers
  void setFilename( const char* const theName );
  
  // Operations
  bool open ( );
  void close( );
  
protected:
  
  bool writeToDestination( AbsEvent* aNode );
  
  std::fstream            _file;

private:

  // Not implemented.
  AppFileStream( const AppFileStream& );
  AppFileStream& operator=( const AppFileStream& );

};

#endif
