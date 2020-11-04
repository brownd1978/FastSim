#ifndef ABSPARM_PAIRS_HH
#define ABSPARM_PAIRS_HH
//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsParmPairs.hh 489 2010-01-13 16:45:39Z stroili $
//
// Description:
//	Class AbsParmPairs<T>
//
//      Templated command handler for creating a list of key-value 
//      parameter pairs in TCL
//
//	parm set <key>	<value>	Add a key-value pair 
//      parm list               Display current pairs
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Yury Kolomensly
//
// Copyright Information:
//	Copyright (C) 2004	LBNL and UC Berkeley
//
//------------------------------------------------------------------------

#include "Framework/AbsParm.hh"
#include "Framework/AppConfigRecorder.hh"

#include <string>
#include <map>

// Forward declarations

// Class Interface

template <class K, class V>
class AbsParmPairs : public AbsParm {
public:
  // Constructors and destructor
  AbsParmPairs(const char* const theCommand, AppModule* theTarget);
  AbsParmPairs(const AbsParmPairs& theOther, AppModule* theTarget);

  virtual ~AbsParmPairs();

  // value and symbolic name accessors
  virtual const V* value(const K& key) const;
  virtual const std::map<K, V>* value() const {return _dict;}

  // key-value display
  virtual void print(std::ostream& o) const;

  virtual std::string classDescription() const {
    return "set of key-value pairs.";
  }
  virtual std::string type() const { return "Map"; }

  void dumpState(AppConfigRecorder* r);
  void restoreState(char* []);

  // set the value
  virtual void set(const K& k, const V& v);

protected:
  virtual int setHandler();
  virtual int listHandler() const;
  virtual int getHandler();

  // The following is overloading in an ugly way.  It would be preferable
  // to do something sensible with streams but it works for now.
  // These routines are only used by setHandler!!!!
  
  void read_value(const char* buffer, double &);
  void read_value(const char* buffer, float &);
  void read_value(const char* buffer, long &);
  void read_value(const char* buffer, int &);
  
  void read_value(const char* buffer, std::string &);
  void read_value(const char* buffer, bool &);


private:

  // Not implemented.
  AbsParmPairs( const AbsParmPairs& );
  AbsParmPairs& operator=( const AbsParmPairs& );

  std::map<K, V>* _dict;
};

// Output operator

template <class K, class V>
std::ostream& operator<<(std::ostream& o, const AbsParmPairs<K,V>& anAbsPS);

#ifdef BABAR_COMP_INST
#include "AbsParm/AbsParmPairs.icc"
#endif

#endif /* ABSPARM_NAMED_VALUE_HH */
