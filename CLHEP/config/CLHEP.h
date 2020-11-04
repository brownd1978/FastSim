// -*- C++ -*-
// $Id: CLHEP.h 192 2009-03-04 12:20:53Z stroili $
// ---------------------------------------------------------------------------
//
// This file is a part of the CLHEP - a Class Library for High Energy Physics.
//
// This file contains definitions of some usefull utilities and macros.
//
#ifndef _CLHEP_H_
#define _CLHEP_H_

#include <stdlib.h>
#include <limits.h>
#include <math.h>

#if defined(CLHEP_CONFIG_FILE)
// "Stringification" of the include file name
#define CLHEP_xstr(s) CLHEP_str(s)
#define CLHEP_str(s) #s
#include CLHEP_xstr(CLHEP_CONFIG_FILE)
#elif defined(CLHEP_TARGET_H)
#include CLHEP_TARGET_H
#else
#include "CLHEP/config/CLHEP-default.h"
#endif

// CLASSDOC OFF
// **** You should probably not touch anything below this line: ****

typedef double HepDouble;
typedef int    HepInt;
typedef float  HepFloat;
typedef bool   HepBoolean;

#ifdef HEP_SHORT_NAMES
typedef HepBoolean Boolean;
#endif

#ifndef HepSTL
#ifndef HEP_USE_STD_STL
#define HepSTL
#else
#define HepSTL std
#endif
#endif

#ifndef HepStd
#ifndef HEP_USE_STD
#define HepStd
#else
#define HepStd std
#endif
#endif

#ifndef M_PI_2
#define M_PI_2  1.57079632679489661923
#endif

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif

#ifndef M_2PI
#define M_2PI   6.28318530717958647692
#endif

#ifdef HEP_DEBUG_INLINE
#define HEP_NO_INLINE_IN_DECLARATION
#endif

#ifdef HEP_NO_INLINE_IN_DECLARATION
#define HEP_NO_INLINE_IN_TEMPLATE_DECLARATION
#endif

// Default to generate random matrix
//
#ifndef HEP_USE_RANDOM
#define HEP_USE_RANDOM
#endif

// Default to have assigment from three vector and rotation to matrix
//
#ifndef HEP_USE_VECTOR_MODULE
#define HEP_USE_VECTOR_MODULE
#endif

// GNU g++ compiler can optimize when returning an object.
// However g++ on HP cannot deal with this.
//
#undef HEP_GNU_OPTIMIZED_RETURN
 
#ifndef HepIOS
#define HepIOS std::ios
#endif

// All the stuff needed by std::ios versus std::ios_base
#ifdef HEP_USE_IOS_BASE
#ifndef HepIOS
#define HepIOS std::ios_base
#endif
#ifndef HepIOSOpenMode
#define HepIOSOpenMode std::ios_base::openmode
#endif
#ifndef HepIOSBadBit
#define HepIOSBadBit std::ios_base::badbit
#endif
#else
#ifndef HepIOS
#define HepIOS std::ios
#endif
#ifndef HepIOSOpenMode
#define HepIOSOpenMode std::ios::open_mode
#endif
#ifndef HepIOSBadBit
#define HepIOSBadBit std::ios::badbit
#endif
#endif

// CLASSDOC ON
#endif /* _CLHEP_H_ */
