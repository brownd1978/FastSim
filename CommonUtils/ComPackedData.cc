// =================================================================== //
// $Id: ComPackedData.cc                                               //
//                                                                     //
// Description:                                                        //
//      See header file                                                //
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
#include "BaBar/BaBar.hh"

#include "CommonUtils/ComPackedData.hh"
#include <stddef.h>

// There's not much here - keep it that way!

ComPackedData::ComPackedData( unsigned long data, size_t size ) 
{ 
  _data=data;
  _size=(size%32); 
}

ComPackedData::ComPackedData () 
{ 
  _data=0;
  _size=0;
}

ComPackedData::~ComPackedData () 
{
}
