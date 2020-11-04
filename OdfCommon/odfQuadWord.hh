#ifndef odfQuadWord_included
#define odfQuadWord_included

#include "BaBar/BaBarODMGTypes.h"

#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
#include <iosfwd>
#else                                                               // BABAR_IOSTREAMS_MIGRATION
class ostream;
#endif                                                              // BABAR_IOSTREAMS_MIGRATION

class odfQuadWord {

public:

  odfQuadWord( d_ULong upper, d_ULong lower );
  odfQuadWord( );
  odfQuadWord( const odfQuadWord& );
  ~odfQuadWord( );

  const d_ULong& upperword() const { return _upperword; }
  const d_ULong& lowerword() const { return _lowerword; }

  odfQuadWord       operator*(  d_ULong  multiplier );
  odfQuadWord       operator/(  d_UShort divisor );
  d_UShort          operator%(  d_UShort divisor );
  odfQuadWord       operator<<( d_ULong  shift );
  odfQuadWord       operator+(  odfQuadWord addend );
  odfQuadWord       operator-(  odfQuadWord addend );

#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
  friend std::ostream&   operator<<( std::ostream& s, const odfQuadWord& value );
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  friend ostream&   operator<<( ostream& s, const odfQuadWord& value );
#endif                                                              // BABAR_IOSTREAMS_MIGRATION

private:

  odfQuadWord Divide( d_UShort divisor, d_UShort &remainder);

  inline d_ULong Extract( d_ULong source, d_UShort start, d_UShort end);
  inline d_ULong Insert(  d_ULong source, d_ULong arg, d_UShort start, d_UShort end);

  d_ULong _upperword;
  d_ULong _lowerword;

};


inline d_ULong odfQuadWord::Extract(d_ULong source, d_UShort start, d_UShort end)
{
  if (start > end) return 0xffffffff;
  source = source << (31 - end);
  source = source >> (31 - end + start);
  return source;
}

inline d_ULong odfQuadWord::Insert(d_ULong source, d_ULong arg, d_UShort start, d_UShort end)
{
  d_ULong dummy1,dummy2;
  d_ULong mask = 0xffffffff;
  if (start > end) return 0xffffffff;
  dummy1 = mask << start;
  dummy2 = mask >> (31 - end);
  mask   = dummy1 & dummy2;
  arg    = arg << start;
  arg    = arg & mask;
  source = source & ~mask;
  source = source |  arg;
  return source;
}

#endif  // odfQuadWord_included
