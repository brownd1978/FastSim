// -*- C++ -*-
// CLASSDOC OFF
// $Id: CLHEP-CC-6.0.h 192 2009-03-04 12:20:53Z stroili $
// -----------------------------------------------------------
// CLASSDOC ON
//
// This file is a part of the CLHEP - a Class Library for High Energy Physics.
//
// This file should define some platform dependent features necessary the
// CLHEP class library and should be included in all CLHEP source files.
// It also defines some usefull utilities. Go through the first part carefully
// and change the definition of the macros to suit you platform.
//
// This version of the file is for Sun Workshop 6.  Do not confuse this
// with the _compiler_ version, which is 5.2 for Workshop 6 update 1, and
// 5.3 for Workshop 6 update 2.
//
//
#ifndef _CLHEP_COMPILER_H_
#define _CLHEP_COMPILER_H_

// Define if your FORTRAN compiler post-pends an underscore on all
// routine names. This is done automatically by the configure script.
#define FORTRAN_PPU 1

// Define if your C++ compiler has STL classes defined in the std namespace.
// This is done automatically by the configure script.
#define HEP_USE_STD_STL 1

// Define if your C++ compiler has I/O streams defined in the std namespace.
// This is done automatically by the configure script.
// #define HEP_USE_STD 1
#undef HEP_USE_STD

// Define if your C++ compiler has <sstream>.
// This is done automatically by the configure script.
#define HEP_HAVE_SSTREAM 1

// Define if your C++ compiler has bool.
// This is done automatically by the configure script.
#define HEP_HAVE_BOOL

// Define if your C++ compiler allows empty template parameter
// list (template<>) for full specialization.
// This is done automatically by the configure script.
// #define HEP_HAVE_EMPTY_TEMPLATE_PARAMETER_LIST 1
#undef HEP_HAVE_EMPTY_TEMPLATE_PARAMETER_LIST

// Define if your C++ compiler supports namespaces.
// This is done automatically by the configure script.
#define HEP_HAVE_NAMESPACE 1

// Define if your C++ compiler has explicit.
// This is done automatically by the configure script.
#define HEP_HAVE_EXPLICIT 1

// Define if your C++ compiler has typename.
// This is done automatically by the configure script.
#define HEP_HAVE_TYPENAME 1

// Define if the definition of the "sqr" template requires parameter
// without const. That was noticed for MS VC++ 5.0.
// This is done automatically by the configure script.
#undef HEP_SQR_NEEDS_PARAMETER_WITHOUT_CONST
 
// Define if the definition of the "abs" template requires parameter
// without const. That was noticed for g++.
// This is done automatically by the configure script.
#undef HEP_ABS_NEEDS_PARAMETER_WITHOUT_CONST
 
// Define if your C++ compiler requires the "sub" function (see the
// Matrix/ module) without const. Such a bug was noticed for some
// versions of DEC CXX, SGI CC and HP aCC.
// This is done automatically by the configure script.
// #define HEP_CC_NEED_SUB_WITHOUT_CONST
#undef HEP_CC_NEED_SUB_WITHOUT_CONST

// Define if your C++ compiler requires that a compare function
// for "qsort" be defined with extern "C" linkage.
// This is done automatically by the configure script.
#define HEP_QSORT_NEEDS_FUNCTION_WITH_EXTERN_C_LINKAGE
//#undef HEP_QSORT_NEEDS_FUNCTION_WITH_EXTERN_C_LINKAGE

// Define if your system has STL installed.
// (flag introduced in BaBar)
#undef HEP_HAVE_STL
// #define HEP_HAVE_STL

//
// The following define fixes a problem we currently (16 august 2000)
// have with the solaris 7 WS 5 compiler: it fails to properly instantiate
// stream operators (involves templates). This can probably be taken out with
// WS 6.
//
#define BABAR_SOL7WS5_FIX 1

#endif // _CLHEP_COMPILER_H_



