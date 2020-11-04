//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComExtendedSyntaxParser.hh 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      Class ComExtendedSyntaxParser
//      This is a parser to get the key/value pairs from any extended command 
//      arguments.
//  
//      eg. arg%key1=value1,value2%key2=value3-value4
//      will return the arg, keys and values in a usable form.
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

#ifndef COMEXTENDEDSYNTAXPARSER_HH
#define COMEXTENDEDSYNTAXPARSER_HH

#include <string>
#include <vector>
#include <map>
#include <utility>

#include <iosfwd>
class ComExtendedArgValues;

class ComExtendedSyntaxParser {

public:

  // Constructor specifies (in addition to the string to be parsed)
  // uniqueKeys : true = identical keys will result in values under the one key
  // firstDelimitor : delimitor separating extended argument from the main one.
  // keywordDelimitors : separates subsequent keyword/value pairs
  ComExtendedSyntaxParser(const char* theSyntax, 
			  bool uniqueKeys = false,
			  const char firstDelimitor = '%',
			  const char keywordDelimitor = '%');
  ~ComExtendedSyntaxParser();

  // Return the main argument 
  const std::string& getArgument() const {return _argument;}

  // Return the extended arguments
  const std::multimap<std::string, ComExtendedArgValues*>& getExtendedArgs() 
    const { return _extendedArgs;}

  // Return the original string to be parsed.
  const std::string& getOriginalSyntax() const {
    return _originalSyntax; }

  // Print (argument and extended syntax key/value pairs)
  void print(std::ostream& ostr) const;

private:

  bool _uniqueKeys;
  char _firstDelimitor;
  char _keywordDelimitor;
  std::string _originalSyntax;
  std::string _argument;
  std::multimap<std::string, ComExtendedArgValues*> _extendedArgs;

  // Extract the extended arguments in key/value pairs
  void extractExtendedArgs(int pos);
  void extractKeyValuePairs(const std::string& theStr);

  // Check that the strings have only "approved" characters
  bool hasOnlyChars(const std::string& theStr, 
		    const char* allowedChars) const;
  bool hasBadChars(const std::string& theStr, const char* badChars) const;

  // Remove any leading/trailing spaces
  std::string& trimString(std::string& theString);

  // Not implemented.
  ComExtendedSyntaxParser( const ComExtendedSyntaxParser& );
  ComExtendedSyntaxParser& operator=( const ComExtendedSyntaxParser& );

};

#endif
