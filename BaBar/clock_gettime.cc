// No clock_gettime in <time.h> on Mac

#if defined(__APPLE_CC__)

#include "BaBar/BaBar.hh"

#include <sys/time.h>
#include <time.h>
#include <errno.h>

extern int errno;

int
clock_gettime( clockid_t dummy, timespec* ts )
{
  int retVal = -1;

  if ( dummy != CLOCK_REALTIME )
    {
      errno = EINVAL;
    }
  else
    {
      struct timeval linuxTempTV;
      gettimeofday( &linuxTempTV, 0 );
      ts->tv_sec = linuxTempTV.tv_sec;
      ts->tv_nsec = linuxTempTV.tv_usec * 1000;
      retVal = 0;
    }

  return retVal;
}

#else // __APPLE_CC__

/* Do something useless so this .o file isn't empty */
static int aDummyInt = 0;

#endif // __APPLE_CC__
