#ifndef IFDSIMPLEPROXYDICT_HH
#define IFDSIMPLEPROXYDICT_HH
//--------------------------------------------------------------------------
// File and Version Information:
//    $Id: IfdSimpleProxyDict.hh 520 2010-01-15 11:40:48Z stroili $
//
// Description:
//
// Author List:
//    Ed Frank                        University of Pennsylvania
//
// History:
//    Ed Frank      17 Nov 96         Creation of first version
//    Ed Frank      24 Feb 97         add invalidateProxyCaches(void)
//    Ed Frank      18 Sep 97         Change from HepAList<T> to array
//                                    implementation to break dependency
//                                    on CLHEP.  IfdSimpleProxyDict is
//                                    just a toy, example implementation
//                                    anyway.
//
// Bugs:
//
//------------------------------------------------------------------------

// 
// Implement a ProxyDict using a stupid linear search in a list.
//
//

#include <stdlib.h>
#include <stddef.h>
#include "ProxyDict/IfdProxyDict.hh"

class IfdKey;
class IfdDictKey;
class IfdData;
class AbsArg;
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
#include <iosfwd>
#else                                                               // BABAR_IOSTREAMS_MIGRATION
class ostream;
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
class IfdSimpleProxyDictEntry;

class IfdSimpleProxyDict : public IfdProxyDict {
public:
  //  IfdSimpleProxyDict( size_t dictSize = 1024 );
  //IfdSimpleProxyDict( size_t dictSize = 8209 );
  IfdSimpleProxyDict( );
  virtual ~IfdSimpleProxyDict();

  virtual bool markForStore( const IfdDictKey& k, AbsArg& a);
  virtual void storeAllMarked( const StoreFlagControl c = clearStoreFlags );

  enum PrintType { noAddresses, showAddresses };
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
  void print( std::ostream& o, const PrintType prtCtl = showAddresses ) const;
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  void print( ostream& o, const PrintType prtCtl = showAddresses ) const;
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
     // Print the dictionary.  prtCtl determines if addresses may be
     // printed in the output or not.
  virtual void testProxyCaches(void);

protected:
  virtual IfdDataProxyIFace* find( const IfdDictKey& k ) const ;
  virtual void               add ( const IfdDictKey& k, IfdDataProxyIFace* p);
     // p can't be const because we are taking ownership of it
     // and will eventually delete it.

private:
  // copy and assignment are not allowed.  Deep copies don't make any sense.
  // Shallow copies don't make any sense either.  too much state in the
  // proxies.
  //
  IfdSimpleProxyDict( const IfdSimpleProxyDict& ) : IfdProxyDict(){ ::abort();}
  IfdSimpleProxyDict& operator=( const IfdSimpleProxyDict&  )
     {  return *this; }

  IfdSimpleProxyDictEntry* findEntry( const IfdKey& k) const;

  IfdSimpleProxyDictEntry** _myData;          // array of ptrs to foo
  size_t                    _dictSize;
  size_t                    _nDictEntries;
  IfdSimpleProxyDictEntry*  _deletionHead;    // List giving insertion order.
				              // Used to control deletion.
  IfdSimpleProxyDictEntry*  _deletionTail;

};

#endif /*  IFDSIMPLEPROXYDICT_HH */

