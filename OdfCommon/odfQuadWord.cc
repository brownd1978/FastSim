#include "BaBar/BaBar.hh"
#include "OdfCommon/odfQuadWord.hh"

#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
#include <iostream>
#else                                                               // BABAR_IOSTREAMS_MIGRATION
#include <iostream.h>
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
#include <limits.h>
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
using std::cout;
using std::endl;
using std::ostream;
#endif                                                              // BABAR_IOSTREAMS_MIGRATION

#ifdef VXWORKS
#define ODFQW_U32_MAX ULONG_MAX
#else
#define ODFQW_U32_MAX ((d_ULong)(-1))
#endif



odfQuadWord::odfQuadWord(d_ULong upper, d_ULong lower){

  _upperword = upper;
  _lowerword = lower;
}

odfQuadWord::odfQuadWord(){

  _upperword = 0;
  _lowerword = 0;
}

odfQuadWord::odfQuadWord(const odfQuadWord& oldwords){

  _upperword = oldwords.upperword();
  _lowerword = oldwords.lowerword();
}

odfQuadWord::~odfQuadWord(){

}

odfQuadWord odfQuadWord::operator*(d_ULong factor){

  d_ULong i,j;
  odfQuadWord temp;

  // do multiplication by doing shifts and adds.
  for (i=0;i<=(sizeof(d_ULong)*8 - 1);i++){
    j = 1 << i;
    if (factor & j) {
      temp = temp+(*this<<i);
    }
  }
  return temp;
}

odfQuadWord odfQuadWord::operator-(odfQuadWord subtract){
  d_ULong tempupperword, templowerword;

  // check for negative result, which isn't supported here.
  if ((_upperword < subtract.upperword()) ||
      ((_upperword == subtract.upperword()) && (_lowerword < subtract.lowerword()))){
    cout << "odfQuadWord subtraction result < 0 not supported" << endl;
    return *this;}

  if (_lowerword >= subtract.lowerword()){
    // we don't have to borrow
    templowerword = _lowerword-subtract.lowerword();
    tempupperword = _upperword-subtract.upperword();}
  else{
    // we have to borrow. get fancy: do subtraction in reverse
    // order, take complement, and add 1.
    templowerword = ~(subtract.lowerword()-_lowerword)+1;
    // subtract 1 here for the borrow;
    tempupperword = _upperword-subtract.upperword()-1;
  }

  return odfQuadWord(tempupperword,templowerword);
}

odfQuadWord odfQuadWord::operator/(d_UShort divisor){
  odfQuadWord temp;
  d_UShort remainder;
  temp = Divide(divisor, remainder);
  return temp;
}

d_UShort odfQuadWord::operator%(d_UShort divisor){
  odfQuadWord temp;
  d_UShort remainder;
  temp = Divide(divisor, remainder);
  return remainder;
}

odfQuadWord odfQuadWord::Divide(d_UShort divisor, d_UShort &remainder){

  // do the long division in units of "short"

  d_ULong piece;
  d_ULong tempupperword,templowerword;

  tempupperword = 0;
  templowerword = 0;

  // do the top 16 bits (48:63)
  piece = Extract(_upperword,16,31);
  tempupperword = Insert(tempupperword,piece/divisor,16,31);
  remainder = piece%divisor;

  // do the next 16 bits (32:47)
  piece = Extract(_upperword,0,15);
  piece = piece + (remainder*0x10000);
  tempupperword = Insert(tempupperword,piece/divisor,0,15);
  remainder = piece%divisor;

  // do the next 16 bits (16:31)
  piece = Extract(_lowerword,16,31);
  piece = piece + (remainder*0x10000);
  templowerword = Insert(templowerword,piece/divisor,16,31);
  remainder = piece%divisor;

  // do the lowest 16 bits (0:15)
  piece = Extract(_lowerword,0,15);
  piece = piece + (remainder*0x10000);
  templowerword = Insert(templowerword,piece/divisor,0,15);
  remainder = piece%divisor;

  return odfQuadWord(tempupperword,templowerword);
}

odfQuadWord odfQuadWord::operator<<(d_ULong factor){

  d_ULong carrybits;
  d_ULong tempupperword,templowerword;

  tempupperword = 0;
  templowerword = 0;

  if (factor == 0) {
    templowerword = _lowerword;
    tempupperword = _upperword;
    return odfQuadWord(tempupperword,templowerword);}
  if (factor == 32) {
    // the "<<32" doesn't seem to work, so do this case by hand.
    tempupperword = _lowerword;
    templowerword = 0;
    return odfQuadWord(tempupperword,templowerword);}
  if (factor > 32) {
    cout << "odfQuadWord left shift > 32 error!" << endl;
    return odfQuadWord(tempupperword,templowerword);}
  carrybits = Extract(_lowerword, 32 - factor, 31);
  templowerword = _lowerword << factor;
  tempupperword = _upperword << factor;
  tempupperword = tempupperword + carrybits;
  return odfQuadWord(tempupperword,templowerword);
}

odfQuadWord odfQuadWord::operator+(odfQuadWord factor){

  d_ULong tempupperword,templowerword;

  tempupperword = 0;
  templowerword = 0;

  if (_upperword > ODFQW_U32_MAX - factor.upperword()){
    cout << "Error, 64 Bit overflow in addition." << endl;
    return odfQuadWord(tempupperword,templowerword);}

  tempupperword = _upperword + factor.upperword();

  if (_lowerword > ODFQW_U32_MAX - factor.lowerword()) tempupperword++;

  templowerword = _lowerword + factor.lowerword();

  return odfQuadWord(tempupperword,templowerword);
}

#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
std::ostream& 
operator<<( std::ostream& s, odfQuadWord& value )
#else                                                               // BABAR_IOSTREAMS_MIGRATION
ostream& 
operator<<( ostream& s, odfQuadWord& value )
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
{
  s << value.upperword() << "," << value.lowerword();
  return s;
}
