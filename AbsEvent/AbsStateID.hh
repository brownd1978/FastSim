//------------------------------------------------------------------------------
// File and Version Information:
//      $Id: AbsStateID.hh 487 2010-01-13 16:40:34Z stroili $
//
// Description:
//      Class AbsStateID. Transient abstract class representing state id.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory
//
// Author List:
//      Jacek Becla             Original author
//
// Copyright Information:
//      Copyright (C) 2002      Stanford Linear Accelerator Center
//
//------------------------------------------------------------------------------

#ifndef ABSSTATEID_HH
#define ABSSTATEID_HH

#include "BaBar/BaBar.hh"
#include "BaBar/BaBarODMGTypes.h"

 
class AbsStateID {

public:

    // Destructor
    virtual ~AbsStateID() {};

    // Selectors
    virtual d_UShort origin()  const = 0;
    virtual d_UShort local()   const = 0;
    virtual d_ULong  modTime() const = 0;

protected:

    // Constructors
    AbsStateID() {};

};

#endif /* ABSSTATEID_HH */



