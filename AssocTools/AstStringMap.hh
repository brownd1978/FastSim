//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AstStringMap.hh 436 2010-01-13 16:51:56Z stroili $
//
// Description:
//	This class is meant to be used to store an unowned pointer to
//      and object that you reference with a std::string key.
//
// Environment:
//      This software was developed for the BaBar collaboration.  If you
//      use all or part of it, please give an appropriate acknowledgement.
//
// Author List:
//      Stephen J. Gowdy        Originator
//
// Copyright Information:
//      Copyright (C) 2004      Stanford Linear Accelerator Center
//
//------------------------------------------------------------------------

#ifndef ASTSTRINGMAP_HH
#define ASTSTRINGMAP_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <iosfwd>
#include <map>
#include <string>
#include <vector>

using std::map;
//----------------------
// Base Class Headers --
//----------------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------

/**
 * Output contents of map to ostream, requires operator<< for T
 *
 *  @param stream  Stream to output on
 *  @param map     Map to output contents of
 *  @return             reference to stream
 *
 *  @see Template#myFunction
 */
template < class T > class AstStringMap;

template < class T >
std::ostream&
operator<<( std::ostream& stream, const AstStringMap< T >& map );


/**
 *  This class is meant to be used to store an unowned pointer to
 *  and object that you reference with a std::string key.
 *
 *  This software was developed for the BaBar collaboration.  If you
 *  use all or part of it, please give an appropriate acknowledgement.
 *
 *  Copyright (C) 2004 Stanford Linear Accelerator Center
 *
 *  @see Template
 *
 *  @version $Id: AstStringMap.hh 436 2010-01-13 16:51:56Z stroili $ 
 *
 *  @author Stephen J. Gowdy  Originator;
 */

template<class T> 
class AstStringMap {

//--------------------
// Declarations     --
//--------------------

  // Typedefs, consts, and enums

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AstStringMap();

  // Destructor
  virtual ~AstStringMap( );

  // Operators
    
  /**
   * Find object using a string key.
   *
   *  @param key  String to use to lookup object
   *  @return             Pointer to object (0 if no object found)
   *
   */
  virtual T* operator[]( const std::string& key ) const;

  // Selectors (const)
  virtual int size() const;
  
  // Modifiers

  /**
   * Store a new reference to a pointer with a string as key
   *
   *  @param key  String to be used as a key for object
   *  @return             reference for user to set to new object pointer
   *
   *  @see Template#myFunction
   */
  virtual T*& insert( const std::string& key );

  virtual void clear();

protected:

  // Helper functions
  inline int stringToBucket( const std::string& key ) const;
  inline uint64_t stringToInt( const std::string& key ) const;

private:

  // Friends
  friend std::ostream& operator<<<T>( std::ostream& stream,
				   const AstStringMap< T >& map );

  // Data members

  // Here we map strings upto and including eight characters to a 64 bit int
  std::map< uint64_t, T* > _intMap;
  // for output, remember the string
  std::map< uint64_t, std::string > _intStringMap;

  // Here we have a std::map for each letter for longer strings
  std::vector< std::map< std::string, T* > > _stringMap;

  // Note: if your class needs a copy constructor or an assignment operator, 
  //  make one of the following public and implement it.
  AstStringMap( const AstStringMap<T>& );  // Copy Constructor
  AstStringMap<T>& operator= ( const AstStringMap<T>& );

//------------------
// Static Members --
//------------------

public:

  // Selectors (const)

  // Modifiers

private:

  // Data members

};

#include  "AssocTools/AstStringMap.icc"


#endif  // ASTSTRINGMAP_HH
