#ifndef IFDSIMPLEPROXYDICTENTRY_HH
#define IFDSIMPLEPROXYDICTENTRY_HH
//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: IfdSimpleProxyDictEntry.hh 520 2010-01-15 11:40:48Z stroili $
//
// Description:
//    this class is internal to the implementation of IfdSimpleProxyDict
//    and should not be used elsewhere, i.e., call me if you need it.
//
//    class is just an ntuple of key,proxy,storeFlag,AbsArg for entry
//    into the dictionary.
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

class IfdDictKey;
class IfdDataProxyIFace;
class AbsArg;

class IfdSimpleProxyDictEntry {
public:
  IfdSimpleProxyDictEntry( const IfdDictKey& k, IfdDataProxyIFace* p );
  ~IfdSimpleProxyDictEntry();

private:  
  // Warning:  it is dangerous to let anyone other than _deep_ ProxyDict
  // internals have access to an IfdDictKey as an IfdDictKey.

  IfdDictKey              *key;
  IfdDataProxyIFace       *proxy;
  bool                     storeFlag;
  AbsArg                  *arg;
  IfdSimpleProxyDictEntry *next;

  IfdSimpleProxyDictEntry *nextDelete;

  // do not add to this list w/o contacting me. -Ed
  friend class IfdSimpleProxyDict;
  friend class IfdNuclearProxyDict;
  friend class IfdSimpleProxyDictNucleus;
};

#endif //  IFDSIMPLEPROXYDICTENTRY_HH
