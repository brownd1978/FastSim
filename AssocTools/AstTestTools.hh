//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AstTestTools.hh 626 2010-09-16 12:26:15Z stroili $
//
// Description:
//	AstTestTools:  simply utility classes and global functions for
//      use with AssocTools regression tests (testAst*).
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Phil Strother                     Original Author
//      Michael Kelsey <kelsey@slac.stanford.edu>
//
// Copyright Information:
//	Copyright (C) 2000  Princeton University
//
//------------------------------------------------------------------------
#ifndef ASTTESTTOOLS_HH
#define ASTTESTTOOLS_HH

// Collaborating classes

#include <vector>
#include <set>
#include <string>

#include <iosfwd>

// Generic container class with RWTPtrXxx semantics

class SimpleClass{
public:
  SimpleClass(int anInt) : _myInt(anInt) {}
  ~SimpleClass() {}
  int getInt() const { return _myInt; }
  bool operator==(const SimpleClass &) const;
  bool operator <(const SimpleClass &) const; 

private:
  int _myInt;
};

bool simpleClassLessThan(const SimpleClass &sc1, const SimpleClass &sc2);


// Hashing functions for RW string pointers

unsigned stringHashFunction(std::string * const &hashThis);
unsigned strValHashFunction(const std::string& hashThis);


// Function to exercise function registration

void AstSTLTestCleaner(std::vector<std::string*> &theVec);


// Display delimited message at start of test

void AstTestBanner(const char* name);

void outputInOrder(std::ostream &, const std::set<std::string>::iterator &, 
		   const std::set<std::string>::iterator &);
void outputInOrder(std::ostream &, const std::set<std::string*>::iterator &, 
		   const std::set<std::string*>::iterator &);
void outputInOrder(std::ostream &, const std::vector<std::string*> &);

#endif




