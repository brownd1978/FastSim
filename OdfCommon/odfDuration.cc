/*
** ++
**  Package:
**	odfCommon
**
**  Abstract:
**	Class and support function for handling durations (delta times)
**
**  Author:
**      Rowan Hamilton, SLAC???
**
**  Creation Date:
**	000 - ???
**
**  Revision History:
**	001 - October 2, 1998 - RiC; added ==, > and < operators
**
** --
*/

#include "OdfCommon/odfDuration.hh"
#include "OdfCommon/odfQuadWord.hh"
#include "OdfCommon/odfTime.hh"

odfDuration::odfDuration(d_ULong upperword, d_ULong lowerword){

  upper = upperword;
  lower = lowerword;
}

odfDuration::odfDuration(){

  upper = 0;
  lower = 0;
}

odfDuration::~odfDuration(){
}

odfDuration odfDuration::operator+(odfDuration time)
{

  odfQuadWord temp(upper,lower);
  odfQuadWord addend(time.upper,time.lower);

  temp = temp + addend;

  return odfDuration(temp.upperword(),temp.lowerword());
}

odfDuration odfDuration::operator-(odfDuration time)
{

  odfQuadWord temp(upper,lower);
  odfQuadWord deltime(time.upper,time.lower);

  temp = temp - deltime;

  return odfDuration(temp.upperword(),temp.lowerword());
}

odfDuration odfDuration::operator+(d_ULong ticks)
{
  odfQuadWord temp(upper,lower);
  odfQuadWord addend(0,ticks);

  temp = temp + addend;

  return odfDuration(temp.upperword(),temp.lowerword());
}

odfDuration odfDuration::operator-(d_ULong ticks)
{
  odfQuadWord temp(upper,lower);
  odfQuadWord temp1(0,ticks);

  temp = temp - temp1;

  return odfDuration(temp.upperword(),temp.lowerword());
}

odfDuration odfDuration::operator*(d_ULong factor)
{

  odfQuadWord temp(upper,lower);

  temp = temp * factor;

  return odfDuration(temp.upperword(),temp.lowerword());
}

odfDuration odfDuration::operator/(d_UShort factor)
{

  odfQuadWord temp(upper,lower);

  temp = temp / factor;

  return odfDuration(temp.upperword(),temp.lowerword());
}

d_UShort odfDuration::operator%(d_UShort divisor)
{

  odfQuadWord temp(upper,lower);

  return temp % divisor;
}

int odfDuration::operator==(const odfDuration& duration) const
{
  return (lower == duration.lower) && (upper == duration.upper);
}

int odfDuration::operator!=(const odfDuration& duration) const
{
  return (lower != duration.lower) || (upper != duration.upper);
}

int odfDuration::operator>(const odfDuration& duration) const
{
   return (upper > duration.upper) ||
     ((upper == duration.upper) && (lower > duration.lower));
}

int odfDuration::operator<(const odfDuration& duration) const
{
  return (upper < duration.upper) ||
    ((upper == duration.upper) && (lower < duration.lower));
}


#ifndef VXWORKS
// Temporary functions pending re-engineering of odfTime/odfDuration:

int64_t 
odfDuration::ticks() const
{
  int64_t upper64( upper );
  int64_t lower64( lower );
  return ( upper64 << 32 ) + lower64;
}

double 
odfDuration::seconds() const
{
  double dticks = ticks();
  // Unfortunately there is presently no symbolic constant holding the
  // nominal PEP-II master clock frequency.
  return dticks / (5.95e7);
}

#endif // ifndef VXWORKS

// Global subtraction operator for const odfTimes:

odfDuration 
operator-( const odfTime& t1, const odfTime& t2 )
{
  odfTime t1x( t1 );  // make a non-const temporary
  return t1x-t2;
}
