//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: IfdHepAListProxy.hh 520 2010-01-15 11:40:48Z stroili $
//
// Description:
//	A proxy for HepALists.
//      Note that this proxy will delete all the items in the HepAList
//      when its dtor is called.  Thus, HepALists owned by this proxy
//      own their data.
//
//      Note that this  proxy does not override the testCache(void)
//      implementation provided by the IfdDataProxy<T> base class.  Thus,
//      a call to testCache(void) has no effect on this proxy, i.e.,
//      this class never thinks its cache is invalid once it has been
//      set.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Ed Frank 		University of Pennsylvania
//
//------------------------------------------------------------------------

#ifndef IFDHEPALISTPROXY_HH
#define IFDHEPALISTPROXY_HH

//---------------
// C++ Headers --
//---------------
#include <stdlib.h>

//----------------------
// Base Class Headers --
//----------------------
#include "ProxyDict/IfdDataProxy.hh"

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
class IfdHepAListProxy : public IfdDataProxy< HepAList<T> > {

//--------------------
// Instance Members --
//--------------------

public:
//  Note: The workaround to a VxWorks template instantiation bug requires that
//  the destructor be the first function defined in a templated class.  Please
//  do not move this or put functions above it!


    virtual ~IfdHepAListProxy( );

    // Constructors-  for more information, see  IfdDataProxy.hh.
    // Basically, you have two choices.  Simply hand a HepAList<T>
    // to this proxy, or hand a function that can create a HepAList<T>
    // to the ctor.  In the first case, a future get() will just return
    // the list you provided the ctor.  In the second case, the fist
    // get() will cause a call to the function you provided, and future
    // get's will just return whatever the first call returned (cache).
    // Note:  I may at a future time IMPOSE THE CONSTRAINT that the
    // ctor taking a HepAList<T>* expects to receive an _empty_ list.
    //

    IfdHepAListProxy( HepAList<T>* t);
    IfdHepAListProxy(HepAList<T>* (*f)(const IfdKey& k));


    // virtual void testCache( void )
    //    accept the definition provided by our base class.


    // Selectors (const)

    // Modifiers

private:
    // Copy Constructor & assignment op:  not allowed.
    //    The Dec CXX complier has trouble instantiating
    //    if we declare these (to turn off the compiler generated
    //    versions of them) but don't define them (nothing to define!).
    //    So we call abort, even though I'd rather have a link error.
    //
    IfdHepAListProxy( const IfdHepAListProxy<T>& ) { ::abort(); }
    IfdHepAListProxy<T> & operator= ( const IfdHepAListProxy<T>& )
       { ::abort(); return *this; }

};

#include  "ProxyDict/IfdHepAListProxy.icc"

#endif // IFDHEPALISTPROXY_HH
