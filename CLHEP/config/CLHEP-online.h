// -*- C++ -*-
//
//  Establish some definitions useful for BaBar online builds for the
//  VxWorks, SunOS and Linux architectures
//
// $Id: CLHEP-online.h 192 2009-03-04 12:20:53Z stroili $
//

#ifndef _CLHEP_COMPILER_H_
#define _CLHEP_COMPILER_H_

// If your compiler cannot handle the persistent streams you should
// define the following macro
#define HEP_NO_PERSISTENT_STREAMS
#undef HEP_HAVE_STL
#undef HEP_BOOL_IS_NOT_INT
#undef HEP_USE_RANDOM
#undef CLHEP_MAX_MIN_DEFINED
#undef CLHEP_SQR_ABS_DEFINED

#if (defined VXWORKS || (_XOPEN_SOURCE - 0 == 500) || defined __linux__)
#define HEP_HAVE_BOOL
#define HEP_HAVE_TYPENAME
#define HEP_QSORT_NEEDS_FUNCTION_WITH_EXTERN_C_LINKAGE
#else
#undef HEP_HAVE_BOOL
#endif

#endif
