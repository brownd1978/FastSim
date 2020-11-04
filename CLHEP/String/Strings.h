// -*- C++ -*-
// CLASSDOC OFF
// $Id: Strings.h 600 2010-06-21 07:46:30Z stroili $
// CLASSDOC ON
//
// This file is a part of what might become the CLHEP -
// a Class Library for High Energy Physics.
// 
// This is the definition of the HepString class.
//
// A simple string package for C++.  Strings have automatic
// storage management and value semantics.  Copying of the string
// contents is common in this implementation.
//
// .SS History
// Author: Dag Bruck.
// Modified slightly by Leif Lonnblad
//

#ifndef STRINGS_H
#define STRINGS_H

#ifdef GNUPRAGMA
#pragma interface
#endif

#include "CLHEP/config/CLHEP.h"
#include <iostream>
#include <string>
#undef index

#ifdef HEP_NO_INLINE_IN_DECLARATION
#define inline
#endif

class HepString {
public:
  inline HepString();
  // Constructs empty string.

  // hep string from stl string
  explicit HepString(const std::basic_string<char> &);

  HepString(char);
  // Constructs string from a character.

  HepString(const char *);
  // Constructs string from a character array.

  HepString(const HepString &);
  // Constructs copy of another string.

  HepString(int);
  // Constructs a string from an integer number.

  HepString(double, unsigned precision = 6);
  // Constructs a string from a floating point number. (Note that the precision
  // argument has no effect for the current implementation when running with 
  // libg++

  inline ~HepString();
  // Destroys string, deallocating storage.

  HepString & operator = (const std::basic_string<char> &);
  HepString & operator = (const HepString &);
  HepString & operator = (const char *);
  HepString & operator = (char);
  // Assignment to string.

  HepString & operator += (const std::basic_string<char> &);
  HepString & operator += (const HepString &);
  HepString & operator += (const char *);
  HepString & operator += (char);
  // appends a string or character to string.

  HepString operator + (const std::basic_string<char> &);
  HepString operator + (const HepString &);
  HepString operator + (const char *);
  HepString operator + (char);
  // Concatenates two strings, or a string with a char.

  inline char operator () (unsigned) const;
  // Returns i'th character of string.  No bounds checking!
  // Strings start at 0 (zero).

  HepString operator () (unsigned start, unsigned n) const;
  // Returns the substring starting at `start' which is at
  // most `n' characters long.  Strings start at 0 (zero).

  HepString car() const;
  // Treating the string as a white-space separated list of sub-string,
  // return the first sub-string (analogous to the car function in lisp)

  HepString cdr() const;
  // Treating the string as a white-space separated list of sub-string,
  // return the list after removing the first element (analogous to the
  // cdr function in lisp)

  inline operator const char * () const;
  // Returns pointer to contents of string.

  HepString upper() const;
  HepString lower() const;
  // Returns a string where every lower (upper) case letter is converted
  // to upper (lower) case, according to ctype(3).  Other characters are
  // copied unchanged.

  inline long toInt() const;
  inline double toFloat() const;
  // Converts string contents to integer or double.

  inline unsigned length() const;
  // Returns length of string (excluding terminating null).

  inline bool isEmpty() const;
  inline bool operator ! () const;
  // Returns true if the string is empty

  inline bool operator == (const std::basic_string<char> &) const;
  inline bool operator == (const HepString &) const;
  bool operator == (const char *) const;
  inline bool operator != (const std::basic_string<char> &) const;
  inline bool operator != (const HepString &) const;
  inline bool operator != (const char *) const;
  // Tests for equality.

  inline int index(const HepString &) const;
  inline int index(char) const;
// Returns the first position of a string or character within the string.
// (The fist character has index 0.)

  inline int lIndex(char) const;
// Returns the last position of a character within the string.

private:

  unsigned size;
  // Allocated storage for str (bytes) is equal to lengh of string + 1.

  char* str;
  // Pointer to allocated storage.

  inline void realloc();
  // Reallocates str using realloc().

friend bool operator == (const char *, const HepString &);
  // Tests for equality ("not equal" also defined).

};

#ifdef HEP_NO_INLINE_IN_DECLARATION
#undef inline
#endif

#ifdef HEP_SHORT_NAMES
typedef HepString String;
#endif

std::istream & operator >> (std::istream &, HepString &);
// Extracts a string from an input stream.  Initial whitespace
// is skipped.  The string extends until the next whitespace.

HepString operator + (const char *, const HepString &);
// Concatenation

#ifdef HEP_DEBUG_INLINE

std::ostream & operator << (std::ostream &, const HepString &);
  // Inserts a string in the output stream.  The same formatting
  // as for char* takes place.

bool operator != (const char *, const HepString &);
// Test for equality

#else
#include "CLHEP/String/Strings.icc"
#endif

#endif
