#ifndef ODFTRANSITIONTRANSLATOR_HH
#define ODFTRANSITIONTRANSLATOR_HH

//--------------------------------------------------------------------------
//
// $Id: odfTransitionTranslator.hh 159 2010-01-15 08:50:21Z stroili $
//
// Environment:
//      This software was developed for the BaBar collaboration.  If you
//      use all or part of it, please give an appropriate acknowledgement.
//
// Authors:
//      A.M. Eisner, Univ. Calif. Santa Cruz
//
// History:
//      18-Jul-2001, AME  - Initial version (adapted from gpdf's version
//                          of odfDamageTranslator).
//
// Copyright Information:
//      Copyright (C) 2001   Stanford Linear Accelerator Center
//
//------------------------------------------------------------------------

#include "OdfCommon/odfTransitionId.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------

/**
 *  This class provides a means for converting transition id numbers to
 *  human-readable strings.   The names used are the same as the enum names
 *  in OdfCommon/odfTransitionId.hh.
 */

class odfTransitionTranslator {
public:
  static const char* name(odfTransitionId::Value type) {
    if ( static_cast<int>(type) < odfTransitionId::numberof && type >= 0) {
      return _transTrans[type];
    } else {
      return "-InvalidId-";
    }
  }

//------------------
// Static Members --
//------------------

private:
  static const char* _transTrans[odfTransitionId::numberof];
};

#endif // ODFTRANSITIONTRANSLATOR_HH
