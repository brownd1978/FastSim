// =================================================================== //
// $Id: ComPackedData.hh 443 2010-01-14 12:24:42Z stroili $                                                                //
//                                                                     //
// Description:                                                        //
//      Class ComPackedData                                            //
//      - Tiny class representing the minimum amount of information    //
//        required by the ComPackEngine in order to do its job.        //
//      - Probably inherit from this to include tags for names and     //
//        descriptions of individual tags.                             //
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
// =================================================================== //

#ifndef COMPACKEDDATA_HH
#define COMPACKEDDATA_HH

#include <stddef.h>

class ComPackedData {
public:
  ComPackedData ( unsigned long , size_t );
  ComPackedData ();
  ~ComPackedData ();

  void setPackedValue ( const unsigned long data ) { _data=data; };
  void setPackedSize ( const size_t size ) { _size=size; };
  unsigned long getPackedValue () const { return _data; };
  size_t getPackedSize () const { return _size; };

  // For RogueWave
  bool operator==( const ComPackedData &a ) const { return ((_data==a.getPackedValue())&&(_size==a.getPackedSize())); };

private:
  unsigned long _data;
  size_t _size;
};
#endif
