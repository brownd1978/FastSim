#ifndef ODF_DAMAGE
#define ODF_DAMAGE

/*
** ++
**  Package:
**      odfCommon
**
**  Abstract:
**      Abstract-Base-Class to define behaviour of the unit of information
**      transmitted on a stream.
**
**  Author:
**      Michael Huffer, SLAC, (415) 926-4269
**
**  Creation Date:
**	000 - June 20 1,1997
**
**  Revision History:
**	None.
**
** --
*/

#include "BaBar/BaBarODMGTypes.h"

class odfDamage
  {
  public:
    enum Value {
           PCI_ParityError   = 0,  DroppedContribution    = 1,
           PCI_TargetAbort   = 2,  PCI_MasterAbort        = 3,
           Shutdown          = 4,  I960_BusFault          = 5,
           I960_MemoryFault  = 6,  PayloadTruncated       = 7,
           Timeout           = 8,  MUQ_NAQ                = 9,
           InvalidTransition = 10, TransitionTimeout      = 11,
           OutOfOrder        = 12, OutofSynch             = 13,
           FEX_Error         = 14, IncompleteContribution = 15,
           CLINK_NotReady    = 16, PCFULL                 = 17,
           FEE_FULL          = 18, CLINKB_NotReady        = 19,
           CLINKA_NotReady   = 20, WriteInProgress        = 21,
           CLINK_NotEnabled  = 22, CLINKA_Throttled       = 23,
           DLINKA_NotReady   = 24, DLINKA_TimeOut         = 25,
           DLINKA_Overflow   = 26, DLINKA_NoStartbit      = 27,
           DLINKB_NotReady   = 28, DLINKB_TimeOut         = 29,
           DLINKB_Overflow   = 30, DLINKB_NoStartbit      = 31};
    enum UpcValue {
           FLINKA_TimeOut    = 24, FLINKB_TimeOut         = 25,
           FLINKC_TimeOut    = 26, FLINKA_NotReady        = 27,
           FLINKB_NotReady   = 28, FLINKC_NotReady        = 29};
    odfDamage() {};
    odfDamage(d_ULong value);
    d_ULong  value() const;
    void     increase(odfDamage::Value);
    void     increase(d_ULong);

    odfDamage& operator=(const odfDamage& that);

  private:
    d_ULong _damage;
  };

/*
** ++
**
**
** --
*/

inline odfDamage::odfDamage(d_ULong value) :
  _damage(value)
  {
  }

/*
** ++
**
**
** --
*/

inline void odfDamage::increase(odfDamage::Value value)
  {
    _damage |=  1 << (d_ULong)value;
  }
/*
** ++
**
**
** --
*/

inline void odfDamage::increase(d_ULong value)
  {
    _damage |=  value;
  }

/*
** ++
**
**
** --
*/

inline d_ULong odfDamage::value() const
  {
  return _damage;
  }

/*
** ++
**
**
** --
*/

inline odfDamage& odfDamage::operator=(const odfDamage& that){
  _damage = that._damage;
  return *this;
}

#endif
