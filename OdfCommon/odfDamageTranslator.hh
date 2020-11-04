#ifndef ODFDAMAGETRANSLATOR_HH
#define ODFDAMAGETRANSLATOR_HH

//--------------------------------------------------------------------------
//
// $Id: odfDamageTranslator.hh 159 2010-01-15 08:50:21Z stroili $
//
// Environment:
//      This software was developed for the BaBar collaboration.  If you
//      use all or part of it, please give an appropriate acknowledgement.
//
// Copyright Information:
//      Copyright (C) 1999   Stanford Linear Accelerator Center
//      Copyright (C) 2001   California Institute of Technology
//
//------------------------------------------------------------------------

#include "BaBar/BaBarODMGTypes.h"

//		---------------------
// 		-- Class Interface --
//		---------------------

/**
 *  This class provides a means for converting damage bit numbers to
 *  human-readable strings.  It is not included in odfDamage itself in 
 *  order to keep that class as simple as possible.
 */

class odfDamageTranslator {

public:
  static const char* name(d_ULong type) {
    return ( type <=31 ? _damageTrans[type] : "-Invalid-" );
  }

//------------------
// Static Members --
//------------------

private:
  static const char* _damageTrans[32];
};

#endif // ODFDAMAGETRANSLATOR_HH
