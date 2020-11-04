//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComCrc.hh 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      Class ComCrc
//      o Small utility class, taken from ODF code, taken from FSF code
//      o Calculates a CRC, either in one pass, or incrementally
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      See implementation file.
//
// Copyright Information:
//      Copyleft, see implementation.
//
//--------------------------------------------------------------------------

#ifndef COMCRC_HH
#define COMCRC_HH

//
// C includes
//
#include "BaBar/BaBarODMGTypes.h"

class ComCrc {
public:

  //////////////////
  // Mode codes   //
  //////////////////
  enum CrcMode {
    CRC_BURST, CRC_STREAM 
  };

  ///////////////////
  // Constructors: //
  ///////////////////
  ComCrc(CrcMode); 

  ///////////////////
  // Destructor:   //
  ///////////////////
  virtual ~ComCrc(){};

  ///////////////////
  // Accessors:    //
  ///////////////////
  bool getCrc(d_ULong &);
  CrcMode getMode() const { return _theMode;};
  d_ULong getSize() const { return _bytesRead;};

  ///////////////////
  // Operations:   //
  ///////////////////
  bool calcCrc ( unsigned char *, unsigned int );
  bool calcCrc ( unsigned char );
  bool calcCrc ( d_ULong );
  void resetCrc () { _bytesRead = _theCrc = 0; }; // Only makes sense in stream mode

protected:
  d_ULong _theCrc;
  CrcMode _theMode;
  size_t _bytesRead;

};

#endif

