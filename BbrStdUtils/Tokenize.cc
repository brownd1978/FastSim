//--------------------------------------------------------------------------
// File and Version Information:
//
// $Id: Tokenize.cc 494 2010-01-13 17:08:01Z stroili $
//
// Description:
//      The class is a replacement for Rogue Wave's RWCTokenizer.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//       A. De Silva
//
// Copyright Information:
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"

#include "BbrStdUtils/Tokenize.hh"

#include <string>

babar::String::Tokenize::Tokenize(const std::string& str) 
  : _nextPos(0)
  , _strPtr(new std::string(str))
{
}

babar::String::Tokenize::~Tokenize() 
{
  delete _strPtr;
}

std::string 
babar::String::Tokenize::getStr(const std::string& token) {
  int wordBeg, wordEnd;
  std::string result("\0");
  wordBeg = _strPtr->find_first_not_of(token, _nextPos);
  if (wordBeg != std::string::npos) {
    wordEnd = _strPtr->find_first_of(token, wordBeg);
    wordEnd = (wordEnd != std::string::npos) ? wordEnd : _strPtr->size();
    result.assign(*_strPtr, wordBeg, wordEnd-wordBeg);
    _nextPos = wordEnd;
  }

  return result;
}

  


