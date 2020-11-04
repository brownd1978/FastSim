// -*- C++ -*-
//
//
// This file is a part of what might become CLHEP -
// a Class Library for High Energy Physics.
//
// This file should define some platform dependent features necessary the
// CLHEP class library and should be included in all CLHEP source files.
// It also defines some usefull utilities. Go through the first part carefully
// and change the definition of the macros to suit you platform.
//
//
// $Id: CLHEP-CC-4.2.h 192 2009-03-04 12:20:53Z stroili $
// -----------------------------------------------------------
// 19-Jul-1999  G.Cosmo    Update to CLHEP 1.4
//
#ifndef _CLHEP_COMPILER_H_
#define _CLHEP_COMPILER_H_

// Define if your FORTRAN compiler post-pends an underscore on all
// routine names. This is done automatically by the configure script.
#define FORTRAN_PPU 1

// Define if your C++ compiler has bool.
// This is done automatically by the configure script.
#undef HEP_HAVE_BOOL

// Define if your C++ compiler allows empty template parameter
// list (template<>) for full specialization.
// This is done automatically by the configure script.
#define HEP_HAVE_EMPTY_TEMPLATE_PARAMETER_LIST 1

// Define if your C++ compiler supports namespaces.
// This is done automatically by the configure script.
#undef HEP_HAVE_NAMESPACE

// Define if your C++ compiler has explicit.
// This is done automatically by the configure script.
#undef HEP_HAVE_EXPLICIT

// Define if your C++ compiler has typename.
// This is done automatically by the configure script.
#undef HEP_HAVE_TYPENAME

// Define if your C++ compiler has standard std namespace.
// This is done automatically by the configure script.
#undef HEP_USE_STD

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
#undef HEP_CC_NEED_SUB_WITHOUT_CONST

// Define if your C++ compiler requires that a compare function
// for "qsort" be defined with extern "C" linkage.
// This is done automatically by the configure script.
#undef HEP_QSORT_NEEDS_FUNCTION_WITH_EXTERN_C_LINKAGE

// Define if your system has STL installed.
// (flag introduced in BaBar)
#undef HEP_HAVE_STL

#endif // _CLHEP_COMPILER_H_



