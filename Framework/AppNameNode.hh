//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppNameNode.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppNameNode. Class to act as node in Commands. The node
//	contains the name of the executable and the execution mode.
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

#ifndef APPNAMENODE_HH
#define APPNAMENODE_HH

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppNameNode {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AppNameNode( const char* const theName, const int theMode = 0 );
  
  // Destructor
  virtual ~AppNameNode( );
  
  // Selectors (const)
  int   mode( ) const;
  char* name( ) const;
  
  // Modifiers
  void setMode( int newMode );
  
private:
  // Not implemented.
  AppNameNode( const AppNameNode& );
  AppNameNode& operator=( const AppNameNode& );

  int   _mode;
  char* _name;
};

#endif
