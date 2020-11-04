//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppExecNode.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppExecNode. Class to act as node in Sequences & Paths. The
//	node contains the associated target (module or sequence) and the
//	execution mode.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//      Marc Turcotte                   Merged R.Kapur's TK interface 5/98
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1998              University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef APPEXECNODE_HH
#define APPEXECNODE_HH

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class AppExecutable;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppExecNode {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AppExecNode( const AppExecutable* const theTarget, const int theMode = 0 );
  
  // Destructor
  virtual ~AppExecNode( );
  
  // Selectors (const)
  bool           isExecuted( ) const {return _executed;}
  int            mode      ( ) const {return _mode;}
  AppExecutable* target    ( ) const {return (AppExecutable*)_target;}
  
  // Modifiers
  void setExecuted( bool flag ) {_executed = flag;}
  void setMode    ( int newMode ) {_mode = newMode;}
  
private:

  // Not implemented.
  AppExecNode( const AppExecNode& );
  AppExecNode& operator=( const AppExecNode& );
  
  bool                 _executed;
  int                  _mode;
  const AppExecutable* _target;
};

#endif
