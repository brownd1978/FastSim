//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackedConsistencyBroken.cc 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      Class ComPackedConsistencyBroken 
//      Class to pack Consistency objects in 2.5 bytes.
//      Allocates 8 bits for consistency, 8 for likelihoods and 4 for status bits.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//      Used only in old "micro" written before end of Y2K 
//
// Author List:
//      R.Faccini               Originator of ComPackedConsistency. 
//      D.Aston   cloned from ComPackedConsistency in CommonUtils V00-03-02. 
//
// Copyright Information:
//      Copyright (C) 1999          U.C.S.D.
//
//   
// Quote:
//      "It ain't the way that you do it, it's what you do"
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

//
// Bass class includes
//
#include "CommonUtils/ComPackUFloatBroken.hh"
#include "ErrLogger/ErrLog.hh"

//
// This class
//
#include "CommonUtils/ComPackedConsistencyBroken.hh"

ComPackedConsistencyBroken::~ComPackedConsistencyBroken(){
}

ComPackedConsistencyBroken::ComPackedConsistencyBroken(const Consistency& c,double scaleLike): Consistency(c)
{
  assert(scaleLike>0);
  _likelihood/=scaleLike;
}

ComPackedConsistencyBroken::ComPackedConsistencyBroken(const d_UShort& value,const d_UShort& status)
{
  ComPackUFloatBroken packer(FRACLEN,EXPLEN,BIAS);
  ComPackBase<double>::StatusCode flag= packer.unpack(value&0xff,_value);
  if (_value>1.0) _value= 1.0;   
  if(flag != ComPackBase<double>::TAG_OK){
    _value=0;
    ErrMsg(warning) << " Problems in unPacking Consistency " <<(value&0xff) << endmsg;
    ErrMsg(warning) << flag  << endmsg;
  }
  flag=packer.unpack((value&0xff00)>>8,_likelihood);
  if (_likelihood>1.0) _likelihood= 1.0;
  if(flag != ComPackBase<double>::TAG_OK){
    _likelihood=0;
    ErrMsg(warning) << " Problems in unPacking likelihood " << (value&0xff00)<< endmsg;
    ErrMsg(warning) << flag << endmsg;
  }
  _stat = (Consistency::ConsistentStatus)(status&0x3);// first 2 bits
  _sign = (Consistency::ConsistentSign)(((status>>2)&0x3)-1);// 3-4 bits
}


  
d_UShort
ComPackedConsistencyBroken::packedValue( ) const
{

  ComPackUFloatBroken packer(FRACLEN,EXPLEN,BIAS);
  double cons= (_value >= packer.getMinVal())?_value:0;
  d_ULong x;
  ComPackBase<double>::StatusCode status=packer.pack(cons,x);
  if(status != ComPackBase<double>::TAG_OK){
    x=0;
    ErrMsg(warning) << " Problems in Packing Consistency " <<consistency() << endmsg;
    ErrMsg(warning) << status << endmsg;
  }
  x &= 0xff; // take only 1 byte

  d_UShort pv(x); // packed value

  double like= (_likelihood >= packer.getMinVal())?_likelihood:0;
  status=packer.pack(like,x);
  if(status != ComPackBase<double>::TAG_OK){
    x=0;
    ErrMsg(warning) << " Problems in Packing likelihood " <<likelihood() << endmsg;
    ErrMsg(warning) << status << endmsg;
  }
  
  x &= 0xff; // take only 1 byte
  
  pv |= (x<<8);
  return pv;
}

d_UShort
ComPackedConsistencyBroken::packedStatus( ) const
{
  d_Octet st = status();
  assert(st<4); // if  the consistency status gets larger we are in trouble...
  int iSig =sign()+1;
  st |= (iSig<<2);
  return st;
}
