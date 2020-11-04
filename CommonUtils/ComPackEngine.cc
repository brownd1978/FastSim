// =========================================== //
// An Architecture Neutral                     //
// Bit String Packing Engine                   //
//                                             //
// o Copyright (C) 1999 D.E.Azzopardi          //
//                                             //
// See header file for more information        //
// =========================================== //
#include "BaBar/BaBar.hh"
#include "BaBar/BaBarODMGTypes.h"

// C includes
#include <stdio.h>
#include <stddef.h>
#include <math.h>

// C++ includes
#include <vector>
#include <iostream>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "ErrLogger/ErrLog.hh"
#include "CommonUtils/ComPackedData.hh"
#include "CommonUtils/ComPackEngine.hh"
using std::endl;

// Pointer to a vector of pointers to tags
ComPackEngine::ComPackEngine
(std::vector<ComPackedData*> * tagStack, 
 std::vector<unsigned char> * storageVector )
{
  _tagStack = tagStack;
  _currentStore = storageVector;
  _committed = UNCOMMITTED;
  
  // Sum total number of bits, bytes
  size_t topLimit (_tagStack->size());
  _numBits = 0;

  // Indicies
  _indicies = new std::vector <size_t> (topLimit);

  std::vector<ComPackedData*>::iterator iter = _tagStack->begin();
  while (iter != _tagStack->end() ) {
    size_t tagSize = (*iter)->getPackedSize();
    if (tagSize > 0) 
      {
	// Accumlate total number of bits
	// and create index.
	_indicies->push_back(_numBits);
	_numBits += tagSize;
      }
    else
      {
	ErrMsg(fatal) << "Encountered tag size of zero bits" << endmsg;
	}
    iter ++;
  }

  // Got to make sure this is a multiple of four for fast access...
  _numBytes = _numBits/8;
  if (0 !=_numBits%8) _numBytes++;
  _numBytes+=_numBytes%4;

  //  cout << "_numBytes = " << _numBytes << endl;

  // Make sure the storage vector is large enough
  if ( _currentStore->size() < _numBytes ) 
    {
      ErrMsg(warning) << "Storage vector size inconsistent with accumulated tag length!" << endl
		      << "Resizing from " << _currentStore->size() << " to " << _numBytes << endmsg;
      _currentStore->resize (_numBytes);
    }


#ifdef undefined
  ErrMsg(warning) << "A ComPackEngine (" << this << ") deployed. " << endmsg
		  << "Total bit string size is " << _numBits << " bits "
		  << "and " << _numBytes << " bytes." << endmsg;
#endif

}

ComPackEngine::~ComPackEngine ()
{
  delete _indicies;
  _indicies = 0;
}

void
ComPackEngine::Pack ()
{
  if ( UNPACKING == _committed ) return;
  _committed = PACKING;
  
  size_t storeIndex(0), freeBits(32);
  unsigned long storeValue(0);
  
  std::vector<ComPackedData*>::iterator iter = _tagStack->begin();
  while (iter != _tagStack->end()) {
    unsigned long tagValue = (*iter)->getPackedValue();
    size_t        tagSize = (*iter)->getPackedSize();
    iter++;
    
    // Sanity check - do we need this?
    tagValue &= getRightMask ( tagSize );
    
    if ( 0 == freeBits ) 
      {
	storeWord (storeValue, storeIndex);
	freeBits = 32;
	storeValue = 0;
	storeIndex += 4;
      }
    
    if (tagSize <= freeBits ) 
      {
	tagValue <<= (freeBits - tagSize);
	storeValue |= tagValue;
	freeBits-=tagSize;
      } 
    else // not enough freeBits...
      {
	// store what we can, then get new address and store remainder...
	unsigned long firstPart = tagValue;
	firstPart >>= (tagSize - freeBits);
	firstPart &= getRightMask ( freeBits ); // probably redundant
	storeValue |= firstPart;
	storeWord (storeValue, storeIndex);
	storeIndex += 4;
	unsigned long otherPart = tagValue;
	otherPart &= getRightMask ( tagSize - freeBits );
	freeBits = 32 - tagSize + freeBits;
	otherPart <<= freeBits;
	storeValue = otherPart;
      }
  }
  // Last one
  storeWord (storeValue, storeIndex);
}

unsigned long 
ComPackEngine::Unpack ( size_t index )
{
  size_t topLimit ( _tagStack->size() );
  if ( index > topLimit ) index %= topLimit;
  size_t bitIndex = ((*_indicies)[index]);
  size_t tagSize = ((*_tagStack)[index])->getPackedSize();
  size_t byteIndex = bitIndex / 32;
  byteIndex *= 4; // Do this to get rounding in above line!!!
  size_t offset = bitIndex % 32;
  long nextWordOffset = offset+tagSize-32;

  unsigned long storedVal, retVal, mask;
  retrieveWord (storedVal, byteIndex);
  mask = getRightMask ( (32-offset) );
  retVal = storedVal & mask;

  if ( nextWordOffset <= 0 )
    {
      mask = getLeftMask ( (offset + tagSize) );
      retVal &= mask;
      retVal >>= (-nextWordOffset);
    }
  else
    {
      retVal <<= nextWordOffset;
      mask = getLeftMask ( nextWordOffset );
      byteIndex += 4;
      retrieveWord (storedVal, byteIndex);
      storedVal &= mask;
      storedVal >>= ( 32 - nextWordOffset );
      retVal |= storedVal;
    }
  return retVal;

}

void
ComPackEngine::Unpack () 
{
  if ( PACKING == _committed ) return;
  _committed = UNPACKING;

  size_t storeIndex(0), freeBits(0);
  unsigned long storedValue (0);

  std::vector<ComPackedData*>::iterator iter = _tagStack->begin();
  while (iter != _tagStack->end()) {
      size_t tagSize = (*iter)->getPackedSize();
      // unsigned long retrievedValue = Unpack ( j );
      // Mask off leftmost tagbits of storedValue;
      unsigned long mask = getLeftMask (tagSize);
      unsigned long retrievedValue;

      if ( 0 == freeBits ) 
	{
	  retrieveWord (storedValue, storeIndex);
	  storeIndex += 4;
	  freeBits = 32; 
	}
      
      if ( tagSize < freeBits )
	{
	  retrievedValue = storedValue & mask;
	  retrievedValue >>= (32-tagSize);
	  storedValue <<= tagSize;
	  freeBits -= tagSize;
	}
      else if ( tagSize == freeBits )
	{
	  retrievedValue = storedValue & mask;
	  retrievedValue >>= (32-tagSize);
	  retrieveWord (storedValue, storeIndex);
	  storeIndex += 4;
	  freeBits = 32; 
	}
      else
	{
	  mask = getLeftMask ( freeBits );
	  retrievedValue = storedValue & mask;
	  retrieveWord (storedValue, storeIndex);
	  storeIndex += 4;
	  size_t remaining = tagSize - freeBits;
	  mask = getLeftMask ( remaining );
	  unsigned long otherValue;
	  otherValue = storedValue & mask;
	  otherValue >>= freeBits;
	  retrievedValue |= otherValue;
	  retrievedValue >>= (32 - tagSize);
	  storedValue <<= remaining;
	  freeBits = 32 - remaining;
	}
      // Store retrieved value
      (*iter)->setPackedValue (retrievedValue);
      iter++;
    }
}   

// Utility functions
inline unsigned long ComPackEngine::getRightMask ( size_t size_in_bits )
{
  // Table lookups
  static const unsigned long bitMaskTable[32] =
  {
    0x00000001, 0x00000003, 0x00000007, 0x0000000f,
    0x0000001f, 0x0000003f, 0x0000007f, 0x000000ff,
    0x000001ff, 0x000003ff, 0x000007ff, 0x00000fff,
    0x00001fff, 0x00003fff, 0x00007fff, 0x0000ffff,
    0x0001ffff, 0x0003ffff, 0x0007ffff, 0x000fffff,
    0x001fffff, 0x003fffff, 0x007fffff, 0x00ffffff,
    0x01ffffff, 0x03ffffff, 0x07ffffff, 0x0fffffff,
    0x1fffffff, 0x3fffffff, 0x7fffffff, 0xffffffff,
  };
  size_in_bits--;
  size_in_bits&=0x1f;
  return bitMaskTable[size_in_bits];
}

inline unsigned long ComPackEngine::getLeftMask ( size_t size_in_bits )
{
  // Table lookups
  static const unsigned long bitMaskTable[32] =
  {
    0x80000000, 0xc0000000, 0xe0000000, 0xf0000000,
    0xf8000000, 0xfc000000, 0xfe000000, 0xff000000,
    0xff800000, 0xffc00000, 0xffe00000, 0xfff00000,
    0xfff80000, 0xfffc0000, 0xfffe0000, 0xffff0000,
    0xffff8000, 0xffffc000, 0xffffe000, 0xfffff000,
    0xfffff800, 0xfffffc00, 0xfffffe00, 0xffffff00,
    0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0,
    0xfffffff8, 0xfffffffc, 0xfffffffe, 0xffffffff,
  };
  size_in_bits--;
  size_in_bits&=0x1f;
  return bitMaskTable[size_in_bits];
}

//
// Architechture neutral store and retrieve in a vector of chars...
// Stored Least Significant Byte first
//
// (Little-endian, like x86 machines)
//
inline void ComPackEngine::storeWord ( unsigned long & toStore, size_t index )
{
  //if ((index+4)>_currentStore->size()) return;
  unsigned long castMe = toStore;
  unsigned char theValue = (unsigned char) castMe;
  int i=4;
  do 
    {
      (*_currentStore)[index++] = theValue;
      castMe >>= 8;
      theValue = (unsigned char) castMe;
    } while (i--);
}

inline void ComPackEngine::retrieveWord (unsigned long & toRetrieve, size_t index)
{
  unsigned char val8bit;
  unsigned long val32bit;
  val8bit = (*_currentStore)[index++];
  toRetrieve=val8bit;
  val8bit = (*_currentStore)[index++];
  val32bit = val8bit;
  val32bit <<= 8;
  toRetrieve|=val32bit;
  val8bit = (*_currentStore)[index++];
  val32bit = val8bit;
  val32bit <<= 16;
  toRetrieve|=val32bit;
  val8bit = (*_currentStore)[index];
  val32bit = val8bit;
  val32bit <<= 24;
  toRetrieve|=val32bit;
}
