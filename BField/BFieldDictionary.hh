//-----------------------------------------------------------------------------
// File and Version Information
//      $Id: BFieldDictionary.hh 497 2010-01-14 09:06:53Z stroili $
//
// Description:
//      Class BFieldDictionary
//      Simple dictionary of transient BField's
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Jane Tinslay    
//
// Copyright Information:
//      Copyright (C) 2005          SLAC
//
//-----------------------------------------------------------------------------
#ifndef BFIELDDICTIONARY_HH
#define BFIELDDICTIONARY_HH

#include <map>
#include <string>
#include <iosfwd>

class BField;

class BFieldDictionary {

public:

  /// Normal constructor
  /**
    * MEMORY MANAGEMENT NOTE: Assume ownership of BField's because they don't provide
    *                         a polymorphic cloning mechanism.
    */
  BFieldDictionary( const std::map<std::string, BField*>& theMap );

  virtual ~BFieldDictionary( );

  BField* getField( const std::string& key ) const;

  unsigned entries( ) const { return _map.size( ); }

  void print( std::ostream& s ) const;

private:

  std::map<std::string, BField*> _map;
};

#endif // BFIELDDICTIONARY_HH
