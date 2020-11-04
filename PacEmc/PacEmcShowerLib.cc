//-----------------------------------------------------------------------
// File and Version Information:
//     PacEmcShowerLib
//
//  Copyright Information:
//      Copyright (C) 2008      Caltech
//
//  Authors:  Chih-hsiang Cheng    2008/06/18
//-----------------------------------------------------------------------
#include "BaBar/BaBar.hh"

#include "PacEmc/PacEmcShowerLib.hh"

// Constructor
PacEmcShowerLib::PacEmcShowerLib() :
  _filename("")
{
}

PacEmcShowerLib::PacEmcShowerLib(const char* filename) :
  _filename(filename)
{
}

// Destructor
PacEmcShowerLib::~PacEmcShowerLib()
{
}

