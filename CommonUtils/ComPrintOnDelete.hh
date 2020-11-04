//==========================================================================
// File and Version Information:
// 	$Id: ComPrintOnDelete.hh 443 2010-01-14 12:24:42Z stroili $
//
//--------------------------------------------------------------------------
// Description:
//	Class ComPrintOnDelete<T> is a template wrapper for any object that
// has the member function print(ostream &). print(..) is called by the 
// destructor to print a summary of the object's info. 
//
//--------------------------------------------------------------------------
// Purpose:
// The immediate purpose of this class is to give timing summaries from 
// many timers, without having to manage the timers in a repository or have
// other access to all of them from a central location.
//
//--------------------------------------------------------------------------
// Sample User Code for use with ComTimeCop:
//
//   void foo(){
//     ComTimeCop * timer = new CpmTimeCop("a particular timer");
//     static ComPrintOnDelete<ComTimeCop> printTimer(timer, cout);
//     printTimer.pointer()->start();
//     ...
//     printTimer.pointer()->end();
//     ... etc.
//   }
//
// When the program exits, all static objects will be deleted, and the 
// timer's print(cout) function will be called before it is deleted.
// See below for other constructors, modifiers and accessors, and memory
// management notes.
//
//--------------------------------------------------------------------------
// Memory Management and Pointer Ownership: 
//     The pointer of type T*, given to any of the ComPrintOnDelete's 
//     constructors or its setPointer(T*) function, is owned by the
//     ComPrintOnDelete object. Do NOT delete it yourself! 
//
//--------------------------------------------------------------------------
// Copying:
//      Not allowed, since owns pointers which there is no point in sharing.
//
//--------------------------------------------------------------------------
// Collaborating classes:
//
//--------------------------------------------------------------------------
// Compiler Default Member Functions:
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

#ifndef ComPrintOnDelete_HH
#define ComPrintOnDelete_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <iostream>

//----------------------
// Base Class Headers --
//----------------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//--------------------------------------------
// Collaborating Class Forward Declarations --
//--------------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------

template<class T> 
class ComPrintOnDelete {
public:
  // Constructors

  ComPrintOnDelete();
  ComPrintOnDelete(T * pointer);
  ComPrintOnDelete(std::ostream & stream);
  ComPrintOnDelete(T * pointer, std::ostream & stream);

  // Destructor
  virtual ~ComPrintOnDelete();
  
  // Accessors (const)  
  const T * pointer() const;   // note similar non-const function below
  void print() const;             // does _pointer->print(*_stream)
  bool printable() const;         // checks if print() will do anything

  // Modifiers
  T * pointer();      // note similar const function above
  std::ostream * stream(); // use but don't delete (not owned by this object)

  void setPointer(T * pointer);   // owns this pointer
  void setStream(std::ostream & stream);
  

protected:
  // Helper functions
  void initialize();
  void deletePointer();
  

private:
  // forbidden functions:
  // Copy Constructor
  ComPrintOnDelete(const ComPrintOnDelete & original);

  // Assignment operator:
  ComPrintOnDelete & operator=(const ComPrintOnDelete<T> & original);

  // Data members
  std::ostream * _stream;        // not owned by this object
  T * _pointer;             // owned by this object
};

#ifdef    BABAR_COMP_INST
#include "CommonUtils/ComPrintOnDelete.icc"
#endif // BABAR_COMP_INST

#endif
