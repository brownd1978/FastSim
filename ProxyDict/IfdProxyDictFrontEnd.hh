#ifndef IFDPROXYDICTFRONTEND_HH
#define IFDPROXYDICTFRONTEND_HH
//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: IfdProxyDictFrontEnd.hh 520 2010-01-15 11:40:48Z stroili $
//
// Description:
//    A user of a ProxyDict class uses a front end to gain access to
//    the dictionary.  The front end is responsible for enforcing
//    policies that provide type safety.  (These policies are leveraged
//    off of the compilers template mechanism.)  The principal mechanism
//    for achieving this is the IfdDictKey, which has a private constructor,
//    yet is required to access the dictionary.  The front end is the
//    only agent in the system that may create an IfdDictKey.
//
//    IfdProxyDictFrontEnd is an abstract base class that exists to allow
//    more than one front end to be written.  The idea is that different
//    projects may want differnt semantics and rules for dictinoary access.
//    IfdProxyDictFrontEnd exists to form a friend relationship with 
//    IfdDictKey.  The protected methods IfdProxyDictFrontEnd::newDictKey()
//    allow derivatives of IfdProxyDictFrontEnd to create the keys they
//    need to gain access to the dictionary.
//
//    Derivatives should call newDictKey with an argument that is an
//    IfdTypeKey<T>.  The T used here should be the name of the front
//    end, which itself is templated.  For example,  class Ifd<T> is
//    a sample frontend class.  Ifd<T> handles data of type T.  It inherits
//    from IfdProxyDictFrontEnd, and it calls newDictKey() with an argument
//    of type IfdTypeKey< Ifd<T> >.  This puts a token into the key
//    which encodes *both* the front=end used to access the dictionary
//    and the type of the data being handled.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Ed Frank		University of Pennsylvania
//
// Copyright Information:
//	Copyright (C) 1997	
//
//------------------------------------------------------------------------

#include "ProxyDict/IfdDictKey.hh"

class IfdTypeKeyIFace;

class IfdProxyDictFrontEnd {
public:
  IfdProxyDictFrontEnd();
  virtual ~IfdProxyDictFrontEnd();   // I want this pure virtual, but our
			             // compilers choke on it.

protected:

  // Specific frontends (our derivatives) implement their ProxyDict
  // accessors as static methods.  For them to call the following
  // helpers, the helpers must be static too.
  
  // NOTE: The caller owns the returned IfdDictKey and must delete it.
  //
  static IfdDictKey* newDictKey( const IfdTypeKeyIFace& );
  static IfdDictKey* newDictKey( const IfdTypeKeyIFace&, const IfdKey& k );
};


#endif //IFDPROXYDICTFRONTEND_HH

