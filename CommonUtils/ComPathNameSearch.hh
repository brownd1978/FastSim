//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: ComPathNameSearch.hh 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//	Global function ComPathNameSearch
//
//      Finds a pathname for a file, using a standard 
//      search order and the BFRECOPATH environment variable
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Bob Jacobsen		Original Author
//
// Copyright Information:
//	Copyright (C) 1998	Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------
#ifndef COMPATHNAMESEARCH
#define COMPATHNAMESEARCH

#include "CLHEP/String/Strings.h"

HepString ComPathNameSearch( const char *fileName, bool printWarning = true );

#endif
