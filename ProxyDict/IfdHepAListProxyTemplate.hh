//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: IfdHepAListProxyTemplate.hh 520 2010-01-15 11:40:48Z stroili $
//
// Description:
//     IfdHepAListProxyTemplate<T> added. This has the same relationship to
//     IfdDataProxyTemplate as IfdHepAListProxy has to IfdDataProxy, and 
//     provides a class template supporting HepAList<T>, performing garbage 
//     collection of the list and its elements when the proxy itself is 
//     deleted.
//        - David Quarrie
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      David Quarrie                   LBNL
//      Ed Frank
//
// History:
//    29 Jul 98  efrank         Changed from HepAList to HepAList.  Move
//                              implementation to .cc file.
//------------------------------------------------------------------------

#ifndef IFDHEPALISTPROXYTEMPLATE_HH
#define IFDHEPALISTPROXYTEMPLATE_HH

//---------------
// C++ Headers --
//---------------
#include <stdlib.h>

//----------------------
// Base Class Headers --
//----------------------
#include "ProxyDict/IfdDataProxyTemplate.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------


//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

template <class T> class HepAList;

//		---------------------
// 		-- Class Interface --
//		---------------------

template<class T> 
class IfdHepAListProxyTemplate : public IfdDataProxyTemplate< HepAList<T> > {

//--------------------
// Instance Members --
//--------------------

public:

    // Constructors-  for more information, see  DictDataProxyTemplate.hh.

    IfdHepAListProxyTemplate( );

    // Destructor

    virtual ~IfdHepAListProxyTemplate();

private:
    // Copy Constructor & assignment op:  not allowed.
    //    The Dec CXX complier has trouble instantiating
    //    if we declare these (to turn off the compiler generated
    //    versions of them) but don't define them (nothing to define!).
    //    So we call abort, even though I'd rather have a link error.
    //
    IfdHepAListProxyTemplate( const IfdHepAListProxyTemplate<T>& ) { ::abort(); }
    IfdHepAListProxyTemplate<T> & operator= ( const IfdHepAListProxyTemplate<T>& )
       { ::abort(); return *this; }

};

#ifdef    BABAR_COMP_INST
#include  "ProxyDict/IfdHepAListProxyTemplate.icc"
#endif // BABAR_COMP_INST

#endif // IFDHEPALISTPROXYTEMPLATE_HH
