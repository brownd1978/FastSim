//
//  What is it ? A simple tool to monitor time in the code
//
//  Author : Gautier Hamel de Monchenault, LBNL & Saclay
//
//  Usage :
//
//  #include "CommonUtils/ComTimeCop.hh"
//
//     static ComTimeCop t1("point 1");         // use LWP virtual time
//     static ComTimeCop t2("point 2", false ); // use real time
//     ...
//     t1.start();
//     ...
//     t1.end();
//     ...
//     t2.start();
//     ...
//     t2.end();
//     ...
//     t1.print(cout);
//     t2.print(cout);
//
//
//   A printout would be :
//  
// point 1  11 mean=1962.727 msec, rms=283.488 msec, latest=1930.000 msec
// point 2  11 mean=  10.000 msec, rms= 22.563 msec, latest=  10.000 msec
//
// On this example, '11' is the number of samplings
//
// Note : internally, this is using sys/time functions.  Here is an 
//         excerpt of the man pages :
// NAME
//     gethrtime, gethrvtime - get high resolution time
//
// SYNOPSIS
//     #include <sys/time.h>
//     hrtime_t gethrtime(void);
//     hrtime_t gethrvtime(void);
//
// DESCRIPTION
//     gethrtime() returns the current high-resolution  real  time.
//     Time  is  expressed as nanoseconds since some arbitrary time
//     in the past; it is not correlated in any way to the time  of
//     day,  and  thus  is not subject to resetting, drifting, etc.
//     via adjtime(2) or settimeofday(3C).   The  hi-res  timer  is
//     ideally  suited  to  performance  measurement  tasks,  where
//     cheap, accurate interval timing is required.
//
//     gethrvtime() returns the current high-resolution LWP virtual
//     time, expressed as total nanoseconds of execution time.
//
//     gethrtime() and gethrvtime() both return an hrtime_t,  which
//     is a 64-bit (long long) signed integer.
//
//

#ifndef ComTimeCop_hh
#define ComTimeCop_hh

#include <errno.h>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

#include "ErrLogger/ErrLog.hh"

timeval use_getrusage();
timeval use_gettimeofday();

inline timeval operator-( const timeval&, const timeval& );
inline timeval operator+( const timeval&, const timeval& );
inline long convertToNanoSec( const timeval& );

struct ComTimeCop 
{
  double time;
  double latestDt;
  double rms;
  timeval t0;
  int  n;
  const char* name;
  timeval (*timeFunc)();
  
  ComTimeCop(const char* s="noName", bool virtualTime=true ) 
    : time(0.), latestDt(0), rms(0.), n(0), name(s) 
    { if(virtualTime) timeFunc = &use_getrusage;
      else            timeFunc = &use_gettimeofday;  }
  virtual ~ComTimeCop() {;}
  void start() { t0=(*timeFunc)(); }
  void end()
    {
      latestDt = convertToNanoSec( (*timeFunc)()-t0 );
      update(latestDt);
    }
  void update( double dt ) 
  {    
    double sumt  = n*time + dt;
    double sumt2 = n*(rms*rms+time*time) + dt*dt; 
    n++;
    time = sumt/n;
    rms  = sqrt( sumt2/n - time*time ); 
  }
  void print(std::ostream& o) const {
    o << name << "\t";
    o << std::setiosflags(std::ios::fixed) << std::setw(6);
    o << n ;
    o << std::setw(10) << std::setprecision(3);
    o << "\tmean=" << time*1.e-06 << "msec,\trms=" << rms*1.e-06;
    o << "msec,\tlatest=" << latestDt*1.e-06 << "msec"<< std::endl;
  }
};

timeval use_getrusage()
{
  rusage time;

  if ( getrusage( RUSAGE_SELF, &time ) != 0 )
    ErrMsg( error ) << "Failed to get rusage due to: " << strerror( errno )
		    << endmsg;

  return (time.ru_utime+time.ru_stime);
}

timeval use_gettimeofday()
{
  timeval time;
  if ( gettimeofday( &time, 0 ) != 0 )
    ErrMsg( error ) << "Failed to get timeofday due to: " << strerror( errno )
		    << endmsg;

  return time;
}

timeval
operator-( const timeval& t1, const timeval& t2 )
{
  timeval totalTime;
  totalTime.tv_sec = t1.tv_sec - t2.tv_sec;
  totalTime.tv_usec = t1.tv_usec - t2.tv_usec;

  if ( totalTime.tv_usec < 0 )
    {
      totalTime.tv_sec--;
      totalTime.tv_usec += 1000000;
    }

  return totalTime;
}

timeval
operator+( const timeval& t1, const timeval& t2 )
{
  timeval totalTime;
  totalTime.tv_sec = t1.tv_sec + t2.tv_sec;
  totalTime.tv_usec = t1.tv_usec + t2.tv_usec;

  if ( totalTime.tv_usec >= 1000000 )
    {
      totalTime.tv_sec++;
      totalTime.tv_usec -= 1000000;
    }

  return totalTime;
}

long
convertToNanoSec( const timeval& t )
{
  return ( t.tv_sec*1000000000 + t.tv_usec*1000 );
}

#endif
