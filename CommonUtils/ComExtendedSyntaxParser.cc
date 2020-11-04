//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComExtendedSyntaxParser.cc 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      Class ComExtendedSyntaxParser
//      This is a parser to get the key/value pairs from any extended command 
//      arguments.
//  
//      eg. arg%key1=value1,value2%key2=value3-value4
//      will return the arg, keys and values in a useable form.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      A. De Silva
//
// Copyright Information:
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"

#include "CommonUtils/ComExtendedSyntaxParser.hh"
#include "CommonUtils/ComExtendedArgValues.hh"

#include "ErrLogger/ErrLog.hh"

#include <stdlib.h>
#include <iostream>
using std::ostream;

const std::string upperChar("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
const std::string lowerChar("abcdefghijlkmnopqrstuvwxyz");
const std::string numeric("01234567890");
const std::string specialChar("/:*");
const std::string allowedKeys(upperChar+lowerChar+numeric);
const std::string allowedIntValues(numeric);
const std::string allowedStrValues(upperChar+lowerChar+numeric+specialChar);

ComExtendedSyntaxParser::ComExtendedSyntaxParser(const char* theSyntax,
						 bool uniqueKeys,
						 const char firstDelimitor,
						 const char keywordDelimitor) 
  : _uniqueKeys(uniqueKeys)
  , _firstDelimitor(firstDelimitor)
  , _keywordDelimitor(keywordDelimitor)
  , _originalSyntax(theSyntax)
  , _argument(theSyntax) 
{
  int endArg  = _argument.find(_firstDelimitor);
  if (endArg != std::string::npos) {
    extractExtendedArgs(endArg);
    _argument.erase(endArg);
  }
}


ComExtendedSyntaxParser::~ComExtendedSyntaxParser() {
  std::multimap<std::string, ComExtendedArgValues*>::iterator iter;
  for (iter = _extendedArgs.begin(); 
       iter != _extendedArgs.end(); iter++) {
    delete iter->second;
  }
}

void
ComExtendedSyntaxParser::extractExtendedArgs(int pos) {
  while (pos != std::string::npos) {
    int posEndArg = _argument.find(_keywordDelimitor,pos+1);
    int theLen = (posEndArg == std::string::npos) ? 
      _argument.size() : posEndArg;
    theLen = theLen - pos - 1;
    extractKeyValuePairs(_argument.substr(pos+1, theLen));
    pos = posEndArg;
  }
}

void
ComExtendedSyntaxParser::extractKeyValuePairs(const std::string& theStr) {

  // get the key from the map (or create it)
  int endKeyPos = theStr.find('='); 
  if (endKeyPos == std::string::npos) {
    ErrMsg(fatal) << "Extended syntax key " 
		  << theStr
		  << " is missing an '='." << endmsg;
  }
  std::string theKey(theStr.substr(0,endKeyPos));
  theKey = trimString(theKey);
  if (! hasOnlyChars(theKey,allowedKeys.c_str())) {
    ErrMsg(fatal) << "Extended syntax key "
		  << theKey 
		  << " should only have [A-z0-9]" << endmsg;
  }

  ComExtendedArgValues* theValues;
  if (_uniqueKeys) {
    std::multimap<std::string, ComExtendedArgValues*>::iterator found =
      _extendedArgs.find(theKey);
    if ( found == _extendedArgs.end() ) {
      theValues = new ComExtendedArgValues();
    _extendedArgs.insert(std::multimap<std::string, ComExtendedArgValues*>::value_type(theKey, theValues));
    }
    else {
      theValues = found->second;
    }   
  } 
  else {
    theValues = new ComExtendedArgValues();
    _extendedArgs.insert(std::multimap<std::string, ComExtendedArgValues*>::value_type(theKey, theValues));
  }
  
  // insert the values into map associated with the key
  int pos = endKeyPos;  
  while (pos != std::string::npos) {

    // find the delimiter ","
    int delimiterPos = theStr.find(',',pos+1);
    int theLen = (delimiterPos == std::string::npos) ? 
      theStr.size() : delimiterPos;
    theLen = theLen - pos - 1;
    std::string tempValue=theStr.substr(pos+1,theLen);

    int rangePos = tempValue.find('-');
    if (rangePos == std::string::npos) {
      // it is not a range
      tempValue = trimString(tempValue);
      if ( tempValue.size() == 0) {
	ErrMsg(fatal) << "Extended syntax value is missing : "
		      << theStr
		      << endmsg;
      }
      if (hasOnlyChars(tempValue, allowedIntValues.c_str()) ) {
	unsigned int intVal1 = strtoul(tempValue.c_str(), 0, 0);
	theValues->addIndividualValues( intVal1);
      } 
      else if (hasOnlyChars(tempValue, allowedStrValues.c_str()) ) {
        theValues->addIndividualValues( tempValue );
      }
      else {	
	ErrMsg(fatal) << "Extended syntax value "
		      << tempValue
		      << " must be [A-z0-9" 
		      << specialChar
		      << "]." << endmsg;
      }

    } 
    else {
      // it is a range
      std::string tempValue2 = tempValue.substr(rangePos+1);
      tempValue.erase(rangePos);
      tempValue = trimString(tempValue);
      tempValue2 = trimString(tempValue2);
      if ( tempValue.size() == 0 || tempValue2.size() == 0) {
	ErrMsg(fatal) << "Extended syntax range is missing: " 
		      << theStr
		      << endmsg;
      }
      if ( hasOnlyChars(tempValue, allowedIntValues.c_str()) && 
	   hasOnlyChars(tempValue2, allowedIntValues.c_str()) ) {
	unsigned int theIntVal1 = strtoul(tempValue.c_str(), 0, 0);
	unsigned int theIntVal2 = strtoul(tempValue2.c_str(), 0, 0);
	theValues->addRangeOfValues(theIntVal1, theIntVal2);
      }
      else if ( hasOnlyChars(tempValue, allowedStrValues.c_str()) &&
		hasOnlyChars(tempValue2, allowedStrValues.c_str()) ) {
	theValues->addRangeOfValues(tempValue, tempValue2);
      }
      else {
	ErrMsg(fatal) << "Extended syntax value "
		      << tempValue << " and " << tempValue2
		      << " must be [A-z0-9"
		      << specialChar
		      << "]." << endmsg;
      }     
    }

    pos = delimiterPos;
  }
}

bool
ComExtendedSyntaxParser::hasOnlyChars(const std::string& theStr, 
					     const char* allowedChars) const
{
  int badCharPos = theStr.find_first_not_of(allowedChars);
  return (badCharPos == std::string::npos) ? true : false;
}

bool
ComExtendedSyntaxParser::hasBadChars(const std::string& theStr, 
				     const char* badChars) const
{
  int badCharPos = theStr.find_first_of(badChars);
  return (badCharPos != std::string::npos) ? true : false;
}

std::string&
ComExtendedSyntaxParser::trimString(std::string& theStr) 
{
  theStr.erase(theStr.find_last_not_of(' ') +1);
  theStr.erase(0,theStr.find_first_not_of(' '));
  return theStr;
}


void
ComExtendedSyntaxParser::print(ostream& ostr) const {

  ostr << _argument;
  
  std::multimap<std::string, ComExtendedArgValues*>::const_iterator iter;
  for (iter = _extendedArgs.begin(); iter != _extendedArgs.end(); iter++) {
    const std::string& key = iter->first;
    ComExtendedArgValues* argValues = iter->second;
    ostr << "\n" << key << " : " ;
    argValues->print(ostr);
  } 

}





