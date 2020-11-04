#ifndef IfdDataProxyUnowned_HH
#define IfdDataProxyUnowned_HH
//--------------------------------------------------------------------------
// File and Version Information:
//    $Id: IfdDataProxyUnowned.hh 520 2010-01-15 11:40:48Z stroili $
//
// Description:
//
//   Proxy to unowned data.  Copied shamelessly from IfdDataProxy
//
// Author List:
//    Ed Frank                        University of Pennsylvania
//
// History:
//    Ed Frank      17 Nov 96         Creation of first version
//    Ed Frank      24 Feb 97         add invalidateCache(void)
//                  30 May 97         change name of above to testCahce.
//
// Bugs:
//    Memory leak if the T* does not delete what it owns in its destructor.
//
//------------------------------------------------------------------------

// Please see the comments in IfdDataProxyUnownedIFace.hh
//

#include "ProxyDict/IfdDataProxyTemplate.hh"


class IfdKey;
class IfdProxyDict;
class IfdAbsArg;

template <class T>
class IfdDataProxyUnowned : public IfdDataProxyTemplate<T> {
public:

  //  Note: The workaround to a VxWorks template instantiation bug 
  // requires that the destructor be the first function defined in a
  // templated class.  Please do not move this or put functions above it!

  virtual ~IfdDataProxyUnowned() {} // possible mem leak here. 
                                           // See notes below.

  IfdDataProxyUnowned(T* t);

  IfdDataProxyUnowned(T* (*f)(const IfdKey& ));
     // Makes a proxy that caches a T* and uses a T* handler(Key& k) to
     // set the T*, which starts off as NULL.

  virtual T*   faultHandler( IfdProxyDict*, const IfdKey&, AbsArg&     );
  virtual void storeHandler( IfdProxyDict*, const IfdKey&, AbsArg&, T* );
  virtual void testCache( void );
  

protected:
   IfdDataProxyUnowned();
      //This stupid ctor makes it easer to write a copy ctor in a
      //derived class that just calls ::abort().  There must be a
      //better way, but this is good enough for now.

   // These are protected rather than private because derivaties
   // for Proxy'ing container classes may need to do deletions
   // in the _myT.  See, for example, PDHepAListProxy.

   T* (*handler)( const IfdKey& k);	// ptr to a fn of Key& returning T*

private:
  // Copy ctor and assignment op:  not allowed.
  //   Note:  we must define thise (not just declare them) or
  //   DEC CXX has trouble instantiating templates.  hence the
  //   aborts, eventhough I'd rather just get a link error
  //
  IfdDataProxyUnowned( const IfdDataProxyUnowned<T>&  ) { ::abort(); }
  IfdDataProxyUnowned<T>& operator=(const IfdDataProxyUnowned<T>& )
     { ::abort();  return *this; }

};

 
#ifdef    BABAR_COMP_INST
#include  "ProxyDict/IfdDataProxyUnowned.icc"
#endif // BABAR_COMP_INST

#endif /*  IfdDataProxyUnowned_HH */
