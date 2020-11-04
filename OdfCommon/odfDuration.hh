#ifndef odfDuration_included
#define odfDuration_included

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

#include "BaBar/BaBarODMGTypes.h"

#ifndef VXWORKS
#include <inttypes.h>
#endif

class odfTime;

class odfDuration  {

public:

  odfDuration();

  ~odfDuration();

  odfDuration operator+(odfDuration time);

  odfDuration operator-(odfDuration time);

  odfDuration operator+(d_ULong ticks);

  odfDuration operator-(d_ULong ticks);

  odfDuration operator*(d_ULong factor);

  odfDuration operator/(d_UShort divisor);

  d_UShort operator%(d_UShort divisor);

  int operator==(const odfDuration& time) const;

  int operator!=(const odfDuration& time) const;

  int operator>(const odfDuration& time) const;

  int operator<(const odfDuration& time) const;

  // we should rethink odfTime stuff so this friend isn't necessary
  friend class odfTime;

#ifndef VXWORKS
  // Temporary provision of convenience interfaces for users.
  // Blocked on VxWorks to avoid issues with long long arithmetic.

  /** 
   *  Returns the number of ticks represented by a duration, as a 
   *  64-bit signed integer.
   *
   *  This is signed (although the current odfDuration is, unfortunately,
   *  unsigned) with an eye on keeping the interface unchanged when 
   *  we change odfDuration. 
   */
  int64_t ticks() const;

  /**
   *  Returns the (floating-point) number of seconds represented by
   *  the duration.
   */
  double seconds() const;

  // The final, rewritten odfTime/odfDuration will also have a function
  // here that will fill out a struct timespec.

#endif

private:

  odfDuration(d_ULong upper, d_ULong lower);

  d_ULong lower;
  d_ULong upper;

};


// We also provide here a convenience function that papers over, in a
// link-compatible way, the fact that odfTime::operator-(const odfTime&)
// is not a const member function of odfTime.

odfDuration operator-( const odfTime&, const odfTime& );

#endif  //  odfDuration_included
