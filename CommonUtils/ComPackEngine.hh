// =================================================================== //
// $Id: ComPackEngine.hh 443 2010-01-14 12:24:42Z stroili $                                                                //
//                                                                     //
// Description:                                                        //
//      Class ComPackEngine                                            //
//      - Concatenates arbitrary length bit strings together.          //
//        o intended to be architechure neutral (works 26 Nov 1999)    //
//        o intended to be reversible (works)                          //
//        o intended to be portable; Rogue Wave Tools RWTPtrVector     //
//          substituted for STL vector<*>, reverse change to port;     //
//          d_ULong -> unsigned long (or equivilent 32 bit int); and   //
//          modify header includes.                                    //
//                                                                     //
// Environment:                                                        //
//      Software developed for the BaBar Detector.                     //
//                                                                     //
// Author List:                                                        //
//      D.E.Azzopardi               Originator.                        //
//                                                                     //
// Copyright Information:                                              //
//      Copyright (C) 1999 D. E. Azzopardi                             //
//                         desva@slac.stanford.edu                     //
//                                                                     //
// Notes:                                                              //
//        o Be extremely careful about modifying this!!!!              //
//        o Once constructed, a given object can work in *either*      //
//          Packing *or* unpacking mode                                //
//                                                                     //
// =================================================================== //

#ifndef COMPACKENGINE_HH
#define COMPACKENGINE_HH

//
// C includes
//
#include <stddef.h>
#include "BaBar/BaBarODMGTypes.h"

#include <vector>

// Forward declarations
class ComPackedData;

class ComPackEngine {
public:
  ComPackEngine (std::vector<ComPackedData*> *, std::vector<unsigned char> * );
  virtual ~ComPackEngine (); 
  unsigned long Unpack ( size_t );
  void Unpack();
  void Pack();

  //Other things that might be useful:
  bool isLittleEndian() { int z = 1; return ((1 == *(char *)&z))?true:false; };

protected:
private:

  // Copy constructors - Copying not allowed
  ComPackEngine( const ComPackEngine& );
  ComPackEngine& operator= ( const ComPackEngine& );

  // private helper functions
  inline unsigned long getRightMask ( size_t );
  inline unsigned long getLeftMask ( size_t );
  inline void storeWord ( unsigned long & , size_t );
  inline void retrieveWord (unsigned long & , size_t );

  // private data

  enum OperationMode {
    UNCOMMITTED, PACKING, UNPACKING
  };

  std::vector <ComPackedData*> * _tagStack;
  std::vector <unsigned char> * _currentStore;
  std::vector <size_t> * _indicies;

  unsigned long _numBits;
  unsigned long _numBytes;
  OperationMode _committed;

};

#endif
