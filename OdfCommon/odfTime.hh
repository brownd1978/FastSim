#ifndef odfTime_included
#define odfTime_included

//--------------------------------------------------------------------------
//
// odfTime
// 
// Environment:
//      This software was developed for the BaBar collaboration.  If you
//      use all or part of it, please give an appropriate acknowledgement.
//
// Copyright Information:
//      Copyright (C) 1997,1999      Stanford University, Caltech
//
//------------------------------------------------------------------------


#include <time.h>

#include "BaBar/BaBarODMGTypes.h"
#include "OdfCommon/odfDuration.hh"


#define SUBFIDPERIOD 873 // Number of sysclks in a SubFiducial period

// The following excludes leap seconds.  Is this a problem?
// (97*365+24)*24*60*60 = 3,061,065,600; 24 leap years.  1900 wasn't a leap year
#define SECONDS_1900_TO_1997 3061065600UL

// (27*365+7)*24*60*60 = 852,076,800;     7 leap years in the period
#define SECONDS_1970_TO_1997 852076800UL


// Forward declare input and output streams so that I/O functions can be 
// declared below.
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
#include <iosfwd>
#else                                                               // BABAR_IOSTREAMS_MIGRATION
class istream;
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
#include <iosfwd>
#else                                                               // BABAR_IOSTREAMS_MIGRATION
class ostream;
#endif                                                              // BABAR_IOSTREAMS_MIGRATION

class odfUnixTime
{
public:
  d_ULong second;
  d_ULong usecond;
  odfUnixTime& operator=(const odfUnixTime& time);
  odfUnixTime(const odfUnixTime& time);
  odfUnixTime() {}
  odfUnixTime(d_ULong sec, d_ULong usec) : second(sec), usecond(usec) {}
};

class odfBinTime   // The order here is important for the fcPM class
{
public:
  d_ULong lower;
  d_ULong upper;
  odfBinTime& operator=(const odfBinTime& time);
  odfBinTime(const odfBinTime& time);
  odfBinTime() {}
  odfBinTime(d_ULong up, d_ULong low) : lower(low), upper(up) {}
};

class odfTime{

public:
  odfTime();

  odfTime(odfBinTime time);

  odfTime(odfUnixTime time);

  odfTime(const struct timespec &time);

  odfTime(d_ULong upper, d_ULong lower);

  odfTime(const odfTime& time);

  ~odfTime();

  odfBinTime binary() const;

  odfUnixTime Unix() const;

  int timeSpec(struct timespec *ts) const;

  odfTime prevSubFidTime() const;

  d_UShort modulus() const;

  // Now define all the operators...

  odfTime& operator=(const odfTime& time);

  odfTime operator+(const odfDuration& time);

  odfTime operator-(const odfDuration& time);

  odfDuration operator-(const odfTime& time);

  int operator==(const odfTime& time) const;

  int operator!=(const odfTime& time) const;

  int operator==(odfBinTime time) const;

  int operator==(odfUnixTime time) const;

  int operator>(const odfTime& time) const;

  int operator>(odfBinTime time) const;

  int operator>(odfUnixTime time) const;

  int operator<(const odfTime& time) const;

  int operator<(odfBinTime time) const;

  int operator<(odfUnixTime time) const;

  /** 
   *  Input-from-istream function.  Primarily used in implementation of
   *  operator>>.
   *
   *  Will not change the object unless it finds a valid odfTime at
   *  the current stream position.  The regular expression for this
   *  is "[ \t]*[0-9A-Fa-f]+/[0-9A-Fa-f]+"  (that is, optional white
   *  space followed by two hex numbers separated by a forward slash).
   *  Neither of the two hex numbers may exceed eight digits.
   *  
   *  Leaves the stream pointing at the first character that can't
   *  be part of the odfTime.
   *
   *  NB: Implemented in odfTimeIO.cc, not odfTime.cc.
   */
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
  std::istream& read( std::istream& );
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  istream& read( istream& );
#endif                                                              // BABAR_IOSTREAMS_MIGRATION

  /** 
   *  Output-to-ostream function.  Primarily used in implementation of
   *  operator<<.
   *
   *  Output format is "%x/%8.8x", i.e., the upper and lower components
   *  of the time in lowercase hex, separated by a slash, with the
   *  lower component padded to eight digits with leading zeros.
   *
   *  NB: Implemented in odfTimeIO.cc, not odfTime.cc.
   */
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
  std::ostream& print( std::ostream& ) const;
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  ostream& print( ostream& ) const;
#endif                                                              // BABAR_IOSTREAMS_MIGRATION

private:

  odfBinTime btime;

};


// Declare input operator -- a global function for "istream >> odfTime".
// NB: Implemented in odfTimeIO.cc, not odfTime.cc.
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
std::istream& operator>>( std::istream&, odfTime& );
#else                                                               // BABAR_IOSTREAMS_MIGRATION
istream& operator>>( istream&, odfTime& );
#endif                                                              // BABAR_IOSTREAMS_MIGRATION

// Declare output operator similarly for "ostream << odfTime".
// NB: Implemented in odfTimeIO.cc, not odfTime.cc.
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
std::ostream& operator<<( std::ostream&, const odfTime& );
#else                                                               // BABAR_IOSTREAMS_MIGRATION
ostream& operator<<( ostream&, const odfTime& );
#endif                                                              // BABAR_IOSTREAMS_MIGRATION


#endif  // odfTime_included
