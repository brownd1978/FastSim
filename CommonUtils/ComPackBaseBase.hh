//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackBaseBase.hh 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      Class ComPackBaseBase.  Untemplated base to ComPackBase, which exists
//      solely to make the *((*&!@$(& return code untemplated!!!!!
//      - Templated base class for utility classes that 
//        take a concrete numerical type and convert it for use in micro 
//        and nano databases.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Dave Brown
//
// Copyright Information:
//      Copyright (C) 2001 Lawrence Berkeley National Lab
//
//--------------------------------------------------------------------------

#ifndef COMPACKBASEBASE_HH
#define COMPACKBASEBASE_HH

class ComPackBaseBase {
public:
  //////////////////
  // Status codes //
  //////////////////

  enum StatusCode {
    TAG_OK, TAG_BAD, TAG_RANGE_ERROR, TAG_VAL_ROUND_DOWN, TAG_VAL_ROUND_UP 
  };
};
#endif
