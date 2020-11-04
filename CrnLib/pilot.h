#if 0
* This pilot patch was created from kerngen.car patch _kerngen
* This directory was created from kernfor.car patch qdefault
*   Character set is ASCII
*  Internal double-precision
*            copy vectors as floating normally
#endif

#ifndef CERNLIB_CERN
#define CERNLIB_CERN
#endif

#ifdef CERNLIB_CERN
#define CERNLIB_RANLUX
#else
#define CERNLIB_RANFFTN
#define CERNLIB_NOCERN
#endif

#ifdef CERNLIB_HPUX
#ifndef CERNLIB_IMPNONE
#define CERNLIB_IMPNONE
#endif
#endif
#ifdef CERNLIB_IBMRT
#ifndef CERNLIB_IMPNONE
#define CERNLIB_IMPNONE
#endif
#endif
#ifdef CERNLIB_VAX
#ifndef CERNLIB_IMPNONE
#define CERNLIB_IMPNONE
#endif
#endif

#if !defined(CERNLIB_SINGLE)
#ifndef CERNLIB_DOUBLE
#define CERNLIB_DOUBLE
#endif
#endif

#ifndef CERNLIB_PDFLIB
#define CERNLIB_PDFLIB
#endif

#ifndef CERNLIB_STDIO
#define CERNLIB_STDIO
#endif
#ifndef CERNLIB_MOVEFTN
#define CERNLIB_MOVEFTN
#endif

#ifndef CERNLIB_UNIX
#define CERNLIB_UNIX
#endif

#if (defined(CERNLIB_UNIX))&&(!defined(CERNLIB_SINGLE))
#ifndef CERNLIB_DOUBLE
#define CERNLIB_DOUBLE
#endif
#endif

#if defined(CERNLIB_UNIX)
#ifndef CERNLIB_SAVE
#define CERNLIB_SAVE
#endif
#endif

#ifndef CERNLIB_UPLOW
#define CERNLIB_UPLOW
#endif

#if defined(CERNLIB_VAX)||defined(CERNLIB_DECS)
#ifndef CERNLIB_TABS
#define CERNLIB_TABS
#endif
#endif

#if (!defined(CERNLIB_VAX))&&(!defined(CERNLIB_CDC))&&(!defined(CERNLIB_DECS))
#ifndef CERNLIB_LUNF77
#define CERNLIB_LUNF77
#endif
#endif

#if defined(CERNLIB_LINUX)
#ifndef CERNLIB_QMLNX
#define CERNLIB_QMLNX
#endif
#ifndef CERNLIB_QPOSIX
#define CERNLIB_QPOSIX
#endif
#ifndef CERNLIB_QIEEE
#define CERNLIB_QIEEE
#endif
#if (!defined(CERNLIB_PPC))
#  ifdef CERNLIB_QISASTD
#    undef CERNLIB_QISASTD
#  endif
#  ifdef CERNLIB_QORTHOLL
#    undef CERNLIB_QORTHOLL
#  endif
#else
#  ifndef CERNLIB_QISASTD
#    define CERNLIB_QISASTD
#  endif
#  ifndef CERNLIB_QORTHOLL
#    define CERNLIB_QORTHOLL
#  endif
#endif
#ifndef CERNLIB_QINTCOPY
#define CERNLIB_QINTCOPY
#endif
#ifndef CERNLIB_QINTZERO
#define CERNLIB_QINTZERO
#endif
#endif

#if defined(CERNLIB_MACMPW)
#ifndef CERNLIB_QMMPW
#define CERNLIB_QMMPW
#endif
#ifndef CERNLIB_F77
#define CERNLIB_F77
#endif
#ifndef CERNLIB_QIEEE
#define CERNLIB_QIEEE
#endif
#ifndef CERNLIB_QISASTD
#define CERNLIB_QISASTD
#endif
#ifndef CERNLIB_QORTHOLL
#define CERNLIB_QORTHOLL
#endif
#ifndef CERNLIB_QINTZERO
#define CERNLIB_QINTZERO
#endif
#endif
#ifndef CERNLIB_A4
#define CERNLIB_A4
#endif
#ifndef CERNLIB_B32
#define CERNLIB_B32
#endif
#ifndef CERNLIB_HEX
#define CERNLIB_HEX
#endif
#if !defined(CERNLIB_QEBCDIC)
#ifndef CERNLIB_QASCII
#define CERNLIB_QASCII
#endif
#endif
#if defined(CERNLIB_B32)||defined(CERNLIB_B36)
#ifndef CERNLIB_INTDOUBL
#define CERNLIB_INTDOUBL
#endif
#endif
#if defined(CERNLIB_QX_SC)
#ifdef CERNLIB_QXNO_SC
#undef CERNLIB_QXNO_SC
#endif
#endif
#if defined(CERNLIB_QXNO_SC)
#ifdef CERNLIB_QX_SC
#undef CERNLIB_QX_SC
#endif
#endif
#if defined(CERNLIB_QXNO_SC)||defined(CERNLIB_QX_SC)
#ifdef CERNLIB_QXCAPT
#undef CERNLIB_QXCAPT
#endif
#endif
#if (!defined(CERNLIB_QXNO_SC))&&(!defined(CERNLIB_QXCAPT))
#ifndef CERNLIB_QX_SC
#define CERNLIB_QX_SC
#endif
#endif
#if defined(CERNLIB_SHIFT)
#ifndef CERNLIB_PROJSHIFT
#define CERNLIB_PROJSHIFT
#endif
#endif

#ifndef type_of_call
#define type_of_call
#endif

