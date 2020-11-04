//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppSequence.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppSequence. This class describes sequences within the BaBar
//	Application Framework.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//      Marc Turcotte                   Modified for new style begin/end
//      Marc Turcotte                   Added support for new style event.
//      Marc Turcotte                   Added execute member function for
//                                      dynamic dispatch support. 3/4/98
//      Marc Turcotte                   Introduced AppStopType 3/20/98
//      Marc Turcotte                   Renamed execute to frame 3/26/98
//      Marc Turcotte                   Merged R.Kapur's TK interface 5/15
//      Marc Turcotte                   Modified for AppStatus 6/20/98
//      Marc Turcotte                   Added input/output Frame 6/22/98
//      Marc Turcotte                   Added theDispatchStopType to
//                                      signatures 6/23/98
//      Marc Turcotte                   Changed AppStatus for AppResult 6/23/98
//      Marc Turcotte                   Changed for new AbsEvent sigs 6/24/98
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1998              University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef APPSEQUENCE_HH
#define APPSEQUENCE_HH

//-------------
// C Headers --
//-------------
#include <stddef.h>

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppExecutable.hh"
#include "Framework/AppResult.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class AppExecNode;
class AppNameNode;
class AppFramework;
class AppStopType;
class AppFrame;
class AppReportAttributes;
template<class Item> class APPList;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppSequence : public AppExecutable {

  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppSequence( const char* const theName, const char* const theDescription );
  
  // Copy Constructor
  //    AppSequence( const AppSequence& );
  
  // Destructor
  virtual ~AppSequence( );
  
  // Operations
  
  virtual AppResult      beginJob( AbsEvent* anEvent );
  virtual AppResult      beginRun( AbsEvent* anEvent );
  
  virtual AppResult      event      ( AbsEvent*  anEvent );
  virtual AppResult      inputEvent ( AbsEvent*& anEvent );
  virtual AppResult      outputEvent( AbsEvent*  anEvent );
  virtual AppResult      other      ( AbsEvent*  anEvent );
  
  virtual AppResult            frame( AppFrame* aFrame,
				      const AppStopType& theDispatchStopType );
  // Frame dispatch
  virtual AppResult       inputFrame( AppFrame*& aFrame,
				      AppStopType& theDispatchStopType );     
  // Frame input
  virtual AppResult      outputFrame( AppFrame* aFrame );        
  // Frame output
  
  virtual AppResult      endRun  ( AbsEvent* anEvent );
  virtual AppResult      endJob  ( AbsEvent* anEvent );
  
  virtual AppResult      abortJob( AbsEvent* anEvent);
  
  // Selectors (const)
  
  AppExecNode*   head  ( )              const;
  bool           passed( )              const;
  virtual void   addToReport(AppReportAttributes* attrib, int mode = 0) const;
  virtual void   report(AppReportAttributes* attrib, int mode = 0) const;
  
  
  void buildNodeList(APPList<AppExecNode*>& destination, 
		     APPList<AppExecNode*>& source);
  
  
  APPList< AppExecNode* >*  nodes( ) const {return _nodes;}
  
  // Modifiers
  
  virtual void append(
		      const AppNameNode* const aNode, 
		      const AppNameNode* const anOther = NULL );
  virtual void append(
		      const AppExecutable* const anExec, 
		      int mode = 0,
		      const AppExecutable* const theOther = NULL,
		      int otherMode = 0 );
  virtual void append(
		      const char* const theName, 
		      int mode = 0,
		      const char* const theOther = NULL,
		      int otherMode = 0 );
  virtual void insert(
		      const AppNameNode* const aNode, 
		      const AppNameNode* const anOther = NULL );
  virtual void insert( 
		      const AppExecutable* const anExec, 
		      int mode = 0,
		      const AppExecutable* const theOther = NULL,
		      int otherMode = 0 );
  virtual void insert( 
		      const char* const theName, 
		      int mode = 0,
		      const char* const theOther = NULL, 
		      int otherMode = 0 );
  virtual void remove( 
		      const AppExecutable* const anExec,
		      int mode = 0 );
  virtual void remove( 
		      const char* const theName, 
		      int mode = 0 );
  
  void setPassed( bool flag );
  
private:

  // Not implemented.
  AppSequence( const AppSequence& );
  AppSequence& operator=( const AppSequence& );
  
  // Data members
  
  APPList< AppExecNode* >* _nodes;
};

#endif
