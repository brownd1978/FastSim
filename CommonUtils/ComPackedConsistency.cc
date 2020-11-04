//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackedConsistency.cc 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      Class ComPackedConsistency 
//      Class to pack Consistency objects in two times two bytes.
//      Allocates 12 bits for likelihood, 12 for consistency and 4 for status.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      R.Faccini                   Originator. 
//      D.Aston           More precision and range for 2001 microDST
//
// Copyright Information:
//      Copyright (C) 1999          U.C.S.D.
//      Copyright (C) 2000          SLAC
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
#include "CommonUtils/ComPackUFloat.hh"
#include "ErrLogger/ErrLog.hh"

//
// This class
//
#include "CommonUtils/ComPackedConsistency.hh"

ComPackedConsistency::~ComPackedConsistency(){
}

ComPackedConsistency::ComPackedConsistency(const Consistency& c,double scaleLike): Consistency(c)
{
  assert(scaleLike>0);
  _likelihood/=scaleLike;
}

ComPackedConsistency::ComPackedConsistency(const d_UShort& value,const d_UShort& status)
{
  ComPackUFloat packer(FRACLEN,EXPLEN,BIAS);
  ComPackBase<double>::StatusCode flag= packer.unpack(status&0xfff,_value);
  if(flag != ComPackBase<double>::TAG_OK){
    _value=0;
    ErrMsg(warning) << " Problems in unPacking Consistency " <<(status&0xfff) << endmsg;
    ErrMsg(warning) << flag  << endmsg;
  }
  flag=packer.unpack((value&0xfff),_likelihood);
  if(flag != ComPackBase<double>::TAG_OK){
    _likelihood=0;
    ErrMsg(warning) << " Problems in unPacking likelihood " << (value&0xfff)<< endmsg;
    ErrMsg(warning) << flag << endmsg;
  }
  _stat = (Consistency::ConsistentStatus)((status>>12)&0x3);// 13-14 bits
  _sign = (Consistency::ConsistentSign)(((status>>14)&0x3)-1);// 15-16 bits
}


  
d_UShort
ComPackedConsistency::packedValue( ) const
{
  // we pack the likelihood in here
  ComPackUFloat packer(FRACLEN,EXPLEN,BIAS);
  d_ULong x;
  ComPackBase<double>::StatusCode  ok=packer.pack(_likelihood,x);
  if(ok == ComPackBase<double>::TAG_VAL_ROUND_UP && 
     _likelihood <  packer.getMinVal() ){
    // this is the only allowed not-OK case
  } else if(ok != ComPackBase<double>::TAG_OK){
    x=0;
    ErrMsg(warning) << " Problems in Packing likelihood " <<likelihood() << endmsg;
    ErrMsg(warning) << ok << endmsg;
  }
  
  x &= 0xfff; // take only 12 bits
  
  return x;
}

d_UShort
ComPackedConsistency::packedStatus( ) const
{
  // we pack the consistency and its status in here
  ComPackUFloat packer(FRACLEN,EXPLEN,BIAS);
  d_ULong x;
  ComPackBase<double>::StatusCode ok=packer.pack(_value,x);
  if(ok == ComPackBase<double>::TAG_VAL_ROUND_UP && 
     _value < packer.getMinVal() ){
    // this is the only allowed not-OK case
  } else if(ok != ComPackBase<double>::TAG_OK){
    x=0;
    ErrMsg(warning) << " Problems in Packing Consistency " <<consistency() << endmsg;
    ErrMsg(warning) << ok << endmsg;
  }
  x &= 0xfff; // take only 12 bits

  d_UShort pv(x); // packed value
  d_Octet st= status();
  assert(st<4); // if  the consistency status gets larger we are in trouble...
  pv |= (st<<12);
  int iSig =sign()+1;
  assert( iSig>=0 && iSig<4 );
  pv |= (iSig<<14);
  return pv;
}
