//--------------------------------------------------------------------------
// File and Version Information:
//      $Id $
//
// Description:
//      Class ComPackedDEDX 
//      Class to pack dEdX objects in two bytes.
//      Allocates 6 bytes for the number of samples and 10 for the central values
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      R.Faccini                   Originator. 
//
// Copyright Information:
//      Copyright (C) 1999          U.C.S.D.
//
//   
//
//--------------------------------------------------------------------------
//#include "BaBar/BaBar.hh"
#include "BaBar/BaBar.hh"

//
// C includes
//
#include <stddef.h> 
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>
//
// C++ includes
//
#include <iostream>
#include "CLHEP/config/TemplateFunctions.h"
//
// Bass class includes
//
#include "CommonUtils/ComPackUFloat.hh"
#include "ErrLogger/ErrLog.hh"
#include "CommonUtils/ComPackUFloat.hh"

//
// This class
//
#include "CommonUtils/ComPackedDEDX.hh"

ComPackedDEDX::~ComPackedDEDX(){
  delete _packer;
}

ComPackedDEDX::ComPackedDEDX(double value,int nSamples,double maxVal): _value(value),_nSamples(nSamples)
{
  _packer= new ComPackUFloat(FRACLEN,EXPLEN,BIAS);
  assert (maxVal>0);
  _value/=maxVal;
}

ComPackedDEDX::ComPackedDEDX(const d_UShort& value,double maxVal)
{
  _packer= new ComPackUFloat(FRACLEN,EXPLEN,BIAS);
  ComPackBase<double>::StatusCode flag=_packer->unpack(value&0x3ff,_value);
  _value*=maxVal;
  if(flag != ComPackBase<double>::TAG_OK){
    _value=0;
    ErrMsg(warning) << " Problems in unPacking DeDx " <<(value&0x3ff) << endmsg;
    ErrMsg(warning) << flag  << endmsg;
  }
  _nSamples = (value>>10)&0x3f;
}


  
d_UShort
ComPackedDEDX::packedValue( ) const
{

  d_ULong x;
  ComPackBase<double>::StatusCode status=_packer->pack(_value,x);
  if(status != ComPackBase<double>::TAG_OK){
    x=0;
    ErrMsg(warning) << " Problems in Packing DeDx " <<_value << endmsg;
    ErrMsg(warning) << status << endmsg;
  }
  x &= 0x3ff; // take only 10bits

  d_UShort pv(x); // packed value

  int n=min(_nSamples,(int)pow(2.0,NHITLEN)-1);
  pv |= (n<<10);
  return pv;
}

