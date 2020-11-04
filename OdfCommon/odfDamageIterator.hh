#ifndef ODF_DAMAGEITERATOR
#define ODF_DAMAGEITERATOR

/*
** ++
**  Package:
**      OdfCommon
**
**  Abstract:
**
**  Author:
**      Michael Huffer, SLAC, (415) 926-4269
**
**  Creation Date:
**	000 - May 6 1,1998
**
**  Revision History:
**	None.
**
** --
*/

#include "OdfCommon/odfDamage.hh"
#include "BaBar/BaBarODMGTypes.h"

class odfDamageIterator
  {
  public:
    odfDamageIterator(const odfDamage& damage);
    virtual ~odfDamageIterator() {;}
    virtual int process(odfDamage::Value) = 0;   
    void        reiterate(d_ULong mask = 0xFFFFFFFF);
  private:
    d_ULong _damage;
  };

/*
** ++
**
**
** --
*/

inline odfDamageIterator::odfDamageIterator(const odfDamage& damage) : 
  _damage(damage.value()) 
  {
  }

/*
** ++
**
**
** --
*/

inline void odfDamageIterator::reiterate(d_ULong mask) 
  {
  d_ULong offset  = 0;
  d_ULong next    = 1 << 0;
  d_ULong current = _damage & mask;

  do
    {
    if(!(current & next)) continue;
    current &= ~next;
    if(!process((odfDamage::Value)offset)) break;
    }
  while(offset++, next <<= 1, current);

  }

#endif
