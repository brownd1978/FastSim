//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackedDEDXBroken.cc 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      Class ComPackedDEDXBroken 
//      Class to pack dEdX objects in two bytes.
//      Allocates 6 bits for the number of samples and 10 for the central values
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//      Used only in old "micro" written before end of Y2K 
//
// Author List:
//      R.Faccini               Originator of ComPackedDEDX. 
//      D.Aston   cloned from ComPackedDEDX in CommonUtils V00-03-02. 
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
#include "CommonUtils/ComPackUFloatBroken.hh"
#include "ErrLogger/ErrLog.hh"

//
// This class
//
#include "CommonUtils/ComPackedDEDXBroken.hh"

ComPackedDEDXBroken::~ComPackedDEDXBroken(){
  delete _packer;
}

ComPackedDEDXBroken::ComPackedDEDXBroken(double value,int nSamples,double maxVal): _value(value),_nSamples(nSamples)
{
  _packer= new ComPackUFloatBroken(FRACLEN,EXPLEN,BIAS);
  assert (maxVal>0);
  _value/=maxVal;
}

ComPackedDEDXBroken::ComPackedDEDXBroken(const d_UShort& value,double maxVal)
{
  _packer= new ComPackUFloatBroken(FRACLEN,EXPLEN,BIAS);
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
ComPackedDEDXBroken::packedValue( ) const
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

