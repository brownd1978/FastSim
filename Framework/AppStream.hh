//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppStream.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppStream. The output stream class.
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

// Note the case sentivity here - it avoids a conclict with the APPStream class
#ifndef AppSTREAM_HH
#define AppSTREAM_HH

//-------------
// C Headers --
//-------------
#include <stddef.h>

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class AppPath;
class AbsEvent;
template<class T> class APPList;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppStream {

  //--------------------
  // Instance Members --
  //--------------------

public:
  
  // Constructors
  AppStream( 
	    const char* const theName, 
	    const char* const theDestination = NULL, 
	    const char* const theDescription = NULL );
  
  // Destructor
  virtual ~AppStream( );
  
  // Selectors (const)
  unsigned int          events     ( ) const;
  const char*           name       ( ) const;
  const char*           description( ) const;
  const char*           destination( ) const;
  bool                  isEnabled  ( ) const;
  virtual bool                  isOpen     ( ) const = 0;
  APPList < AppPath* >* paths      ( ) const;
  
  AppPath* fetch( const char* const theName ) const;
  bool     has  ( const char* const theName ) const;
  
  // Modifiers
  void setEnabled    ( bool mode );
  
  // phase III.1 modification - made method virtual  
  virtual void setDestination( const char* const theName );

  // Operations
  virtual bool open ( ) = 0;
  virtual void close( ) = 0;
  virtual bool write( AbsEvent* aNode );
  
  
  virtual void append ( const AppPath* const thePath );
  virtual void remove ( const AppPath* const thePath );
  virtual void remove ( const char* const theName );
  virtual void wipeout( );
  
protected:
  
  virtual bool writeToDestination( AbsEvent* aNode ) = 0;
  
  APPList < AppPath* >* _paths;
  char*                 _name;
  char*                 _description;
  char*                 _destination;
  bool                  _isEnabled;
  unsigned int          _events;

private:

  // Not implemented.
  AppStream( const AppStream& );
  AppStream& operator=( const AppStream& );

};

#endif
