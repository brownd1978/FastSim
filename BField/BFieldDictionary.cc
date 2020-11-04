//-----------------------------------------------------------------------------
// File and Version Information
//      $Id: BFieldDictionary.cc 497 2010-01-14 09:06:53Z stroili $
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
#include "BaBar/BaBar.hh"

#include "BField/BFieldDictionary.hh"
#include "BField/BField.hh"

#include "ErrLogger/ErrLog.hh"

#include <iostream>

BFieldDictionary::BFieldDictionary(const std::map<std::string, BField*>& theMap)
  :_map(theMap)
{}

BFieldDictionary::~BFieldDictionary() 
{
  std::map<std::string, BField*>::const_iterator iter = _map.begin();
  while (iter != _map.end()) {
    delete iter->second;
    iter++;
  }
}

BField*
BFieldDictionary::getField(const std::string& name) const
{
  std::map<std::string, BField*>::const_iterator iter = _map.find(name);
  if (iter == _map.end()) {
    ErrMsg(warning) <<"No BField with name "<<name<<endmsg;
    return 0;
  }
  return iter->second;
}

void
BFieldDictionary::print( std::ostream& s ) const
{
  std::map<std::string, BField*>::const_iterator iter = _map.begin();
  while (iter != _map.end()) {
    s << "Field"
      << " Key = " << iter->first
      << " -> " << iter->first << std::endl;
    ++iter;
  }
}
