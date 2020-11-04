//
// $Id: odfTime.cc 164 2010-06-14 11:38:47Z stroili $
//

#include "BaBar/BaBar.hh"

#include <time.h>

#include "OdfCommon/odfTime.hh"
#include "OdfCommon/odfQuadWord.hh"

odfTime::odfTime()
{
  struct timespec ts;

  clock_gettime (CLOCK_REALTIME, &ts);

  odfTime t(ts);

  *this = t;
}

odfTime::odfTime(odfBinTime time)
{
  btime = time;
}

odfTime::odfTime(odfUnixTime time)
{
  odfQuadWord longsecond(0,time.second - SECONDS_1970_TO_1997);
  odfQuadWord longusecond(0,time.usecond);
  odfQuadWord upperusecond = longsecond * 1000000UL;
  odfQuadWord usecond      = upperusecond + longusecond;
  odfQuadWord binary       = (usecond * 59UL) + (usecond / 2) ;  // Convert to 59.5 MHz ticks

  btime.upper =  binary.upperword();
  btime.lower =  binary.lowerword();
}

odfTime::odfTime(const struct timespec &time)
{
  odfUnixTime uTime(time.tv_sec, time.tv_nsec / 1000);
  odfTime     oTime(uTime);

  *this = oTime;
}

odfTime::odfTime(d_ULong upper, d_ULong lower)
{
  btime.upper   = upper;
  btime.lower   = lower;
}

odfTime::odfTime(const odfTime& time)
{
  btime  = time.binary();
}

odfTime::~odfTime(){}

odfBinTime odfTime::binary() const
{
  return btime;
}

odfUnixTime odfTime::Unix() const
{
  d_ULong second, usecond;

  // convert from sysclks to microseconds by dividing by 59.5.
  odfQuadWord temp(btime.upper,btime.lower);
  int overflow = btime.upper & 0x80000000; // if highest bit set there will
                                           // be an overflow
  // remove highest bit if overflow will occur
  if (overflow) {
    odfQuadWord mask(0x80000000, 0x0);
    temp = temp - mask;
  }
  d_UShort remainder;
  temp = (temp + temp) / 119;           // temp / 59.5

  // now add in correction for overflow if necessary
  if (overflow) {
    odfQuadWord correction(0xffffffff, 0xffffffff);
    correction = correction / 119;
    temp = temp + correction;
  }

  // get microseconds and seconds separately by dividing by 1,000,000
  // and looking at remainder and quotient.
  // do two divisions of 1000, since we only support USHORT division now.
  remainder = temp % 1000;
  temp = temp / 1000;
  usecond = remainder;
  remainder = temp % 1000;
  temp = temp / 1000;
  usecond = usecond+1000*remainder;
  second  = temp.lowerword() + SECONDS_1970_TO_1997;

  return odfUnixTime(second, usecond);

}

int odfTime::timeSpec(struct timespec *ts) const
{
  if ( ts == 0 )  return -1;
  odfUnixTime u = Unix();
  ts->tv_sec  = u.second;
  ts->tv_nsec = u.usecond * 1000;
  return (0);
}

odfTime odfTime::prevSubFidTime() const
{
  odfQuadWord temp(btime.upper,btime.lower);

  temp = (temp / SUBFIDPERIOD) * SUBFIDPERIOD;


  odfBinTime retval(temp.upperword(), temp.lowerword());

  return odfTime(retval);
}

d_UShort odfTime::modulus() const
{
  d_UShort remainder;

  odfQuadWord temp(btime.upper,btime.lower);

  remainder = temp % SUBFIDPERIOD;

  return remainder;
}

// Now define all the operators...

odfTime& odfTime::operator=(const odfTime& time)
{
  btime   = time.binary();
  return *this;
}

odfTime odfTime::operator+(const odfDuration& time)
{

  odfQuadWord temp(btime.upper,btime.lower);
  const odfQuadWord addend(time.upper,time.lower);

  temp = temp + addend;

  odfBinTime retbin(temp.upperword(), temp.lowerword());

  return odfTime(retbin);
}

odfTime odfTime::operator-(const odfDuration& time)
{
  odfQuadWord temp(btime.upper,btime.lower);
  odfQuadWord deltime(time.upper,time.lower);

  temp = temp - deltime;

  odfBinTime retbin(temp.upperword(), temp.lowerword());

  return odfTime(retbin);
}

odfDuration odfTime::operator-(const odfTime& time)
{
  odfQuadWord temp(btime.upper,btime.lower);
  odfQuadWord otime(time.binary().upper,time.binary().lower);

  temp = temp - otime;

  return odfDuration(temp.upperword(), temp.lowerword());
}

int odfTime::operator==(const odfTime& time) const
{
  return (btime.lower == time.btime.lower) &&
         (btime.upper == time.btime.upper);         
}

int odfTime::operator!=(const odfTime& time) const
{
  return (btime.lower != time.btime.lower) ||
         (btime.upper != time.btime.upper);
}

int odfTime::operator==(odfBinTime time) const
{
  return (btime.lower == time.lower) && (btime.upper == time.upper);
}

int odfTime::operator==(odfUnixTime time) const
{
//   if (!utime.second)
//     utime = Unix(); // Ensures local unix value
//   return (utime.second == time.second) && (utime.usecond == time.usecond);
  //*** Need to implement this!
  return 0;
}

int odfTime::operator>(const odfTime& time) const
{
   return (btime.upper > time.binary().upper) ||
    ((btime.upper == time.binary().upper) && (btime.lower > time.binary().lower));
}

int odfTime::operator>(odfBinTime time) const
{
  return (btime.upper > time.upper) ||
    ((btime.upper == time.upper) && (btime.lower > time.lower));
}

int odfTime::operator>(odfUnixTime time) const
{
//   return (utime.second > time.second) ||
//     ((utime.second == time.second) && (utime.usecond > time.usecond));
  //*** Need to implement this!
  return 0;
}

int odfTime::operator<(const odfTime& time) const
{
  return (btime.upper < time.binary().upper) ||
    ((btime.upper == time.binary().upper)
     && (btime.lower < time.binary().lower));
}

int odfTime::operator<(odfBinTime time) const
{
  return (btime.upper < time.upper) ||
    ((btime.upper == time.upper) && (btime.lower < time.lower));
}

int odfTime::operator<(odfUnixTime time) const
{
//   return (utime.second < time.second) ||
//     ((utime.second == time.second) && (utime.usecond < time.usecond));
  //*** Need to implement this!
  return 0;
}

odfUnixTime& odfUnixTime::operator=(const odfUnixTime& time)
{
  second  = time.second;
  usecond = time.usecond;
  return *this;
}

odfBinTime& odfBinTime::operator=(const odfBinTime& time)
{
  upper   = time.upper;
  lower   = time.lower;
  return *this;
}

odfUnixTime::odfUnixTime(const odfUnixTime& time)
{
  second  = time.second;
  usecond = time.usecond;
}

odfBinTime::odfBinTime(const odfBinTime& time)
{
  upper   = time.upper;
  lower   = time.lower;
}
