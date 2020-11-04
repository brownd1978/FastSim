//==========================================================================
// File and Version Information:
// 	$Id: Com3State.hh 443 2010-01-14 12:24:42Z stroili $
//
//--------------------------------------------------------------------------
// Description:
//	Just as bool can have one of two values, a Com3State is a 3-state 
// machine that has 3 possible states. These are Com3True, Com3False, and 
// Com3Neutral. These values are macros which expand into static function 
// calls, returnng the Com3State objects which define the values.
// These values can also be compared to the bool values 
// true and false. However, there are no implicit
// conversions between Com3State and bool, since that quickly runs into 
// ambiguities which are often compiler dependent. A Com3State may be created
// from a bool using the constructor Com3State(bool, int), which avoids
// this problem, or assigned to a bool using operator=, yielding Com3True if 
// the bool is true, and Com3False if the bool is false.  
// The rules governing comparison operations are:
// true == Com3True
// true != Com3Neutral
// true != Com3False
// false == Com3False
// false != Com3Neutral
// false != Com3True
//
//--------------------------------------------------------------------------
// Collaborating classes:
//
//--------------------------------------------------------------------------
// Sample User Code:
//
//--------------------------------------------------------------------------
// Compiler Default Member Functions:
//  Copy constructor and assignment operator
//
//--------------------------------------------------------------------------
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//--------------------------------------------------------------------------
// Author List:
//	Abi Soffer              (Original author)
//
//--------------------------------------------------------------------------
// Copyright Information:
//	Copyright (C) 1998	Colorado State University
//
//==========================================================================

#ifndef Com3State_HH
#define Com3State_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------

//----------------------
// Base Class Headers --
//----------------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//--------------------------------------------
// Collaborating Class Forward Declarations --
//--------------------------------------------
#include <iosfwd>
#include <iosfwd>

//-----------------
// Shortcut Macros:
//-----------------
// These define the 3 possible states, which are used to set all Com3State 
// objects, and against which all objects can be conveniently compared:
#define Com3True Com3State::com3True()
#define Com3Neutral Com3State::com3Neutral()
#define Com3False Com3State::com3False()

//		---------------------
// 		-- Class Interface --
//		---------------------

class Com3State {
  //----------------
  // Static Members:
  //----------------

public:
  // Static objects that publicly define the 3 states. The 3 macors above
  // are expanded into these functions:
  static inline Com3State com3True();
  static inline Com3State com3Neutral();
  static inline Com3State com3False();


  //------------------
  // Instance Members:
  //------------------

public:
  // Constructors
  Com3State();                          // _state defaults to Com3Neutral
  Com3State(const Com3State & s);       // Assignment operator
  Com3State(bool b, int ignored);       // int protects against ambiguities

  // Destructor: 
  virtual ~Com3State();

  // Assignments:
  Com3State & operator=(const Com3State & s);
  Com3State & operator=(bool b);
  
  // Comparison operators:
  friend inline bool operator==(Com3State c1, Com3State c2);
  friend inline bool operator!=(Com3State c1, Com3State c2);

  friend inline bool operator==(bool b, Com3State c);
  friend inline bool operator!=(bool b, Com3State c);

  friend inline bool operator==(Com3State c, bool b);
  friend inline bool operator!=(Com3State c, bool b);

  // Accessors:
  // operator<< calls the virtual print() method:
  friend inline std::ostream & operator<<(std::ostream & stream, Com3State c);
  virtual std::ostream & print(std::ostream & stream) const;

  // Modifiers:
  // operator>> calls the virtual read(stream) method:
  friend inline std::istream & operator>>(std::istream & stream, Com3State & c);

  // read(...) reads a word up to the first white space or EOF, and interprets
  // "t" or "true" or "Com3True" (all case-insensitive) as Com3True,
  // "f" or "false" or "Com3False" (all case-insensitive) as Com3False, 
  // "n" or "neutral" or "Com3Neutral" (all case-insensitive) as ComNeutral.
  // All other inputs result in a call to abort().
  virtual std::istream & read(std::istream & stream);


private:
  // enums:
  enum Com3Value {TRUE3 = 1, NEUTRAL3 = 0,  FALSE3 = -1};

  // Constructor:
  Com3State(Com3Value v);

  // Data members
  Com3Value _state;
};

// Inline implementations
#include "CommonUtils/Com3State.icc"

#endif
