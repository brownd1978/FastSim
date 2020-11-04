// -*- C++ -*- 
// CLASSDOC OFF
// $Id: CmdArg.h 478 2010-01-22 08:54:39Z stroili $
// CLASSDOC ON
//
// CmdArg.h - C++ base class for one command line argument.
//
// This code was inspired by the cmdline package of Brad Appleton
//    <brad@ssd.csd.harris.com>  
//
// Usage:
//  HepCmdArg and its subclasses are used to represent one command line
//  argument, either optional or positional. Optional arguments are those
//  specified just after the command with a '-'. Positional arguments occur
//  after all optional arguments.
// 
//  Optional arguments can be either short args (eg. '-x', '-H') or long
//  args (eg. '--nevents', '--Cut'). All arguments are case-sensitive.
//
//  In the descriptions below, the following parameters are used:
//    char optchar         the character that specifies the short arg
//    HepString keyword    the long arg string (no '-'s)
//    HepString value      a string describing the value to follow the flag
//    HepString descrip    a description of the parameter
//    defValue             default value of the appropriate type (optional)
//  Note that any of these parameters can by 0. "Value" and "description"
//  are only used for the usage message.
//
//  Optional Arguments:
//
//    HepCmdArgInt(optchar, keyword, value, description, int defValue = 0)
//    HepCmdArgFloat(optchar, keyword, value, description, float defValue = 0)
//    HepCmdArgString(optchar, keyword, value, description, char *defValue = 0)
//
//    HepCmdArgBoolean(optchar, keyword, description, int defValue = 0)
//
//  Required Positional Arguments (user must supply):
//
//    HepCmdArgInt(value, description)
//    HepCmdArgFloat(value, description)
//    HepCmdArgString(value, description)
//
//  Optional Positional Arguments (user may supply):
//
//    HepCmdArgInt(value, description, int defValue)
//    HepCmdArgFloat(value, description, float defValue)
//    HepCmdArgString(value, description, char *defValue)
//
//  HepCmdArgBoolean
//    Booleans do not take values on the command line. Each
//    time the option is seen on the command line, the boolean's value is
//    toggled. 
//
//  Getting the result:
//    After parsing, the result for a particular option is obtained by
//    simply using the HepCmdArg variable as if were the same type as its
//    value. E.g.
//        HepCmdArgInt nev(...);
//                :
//                :
//        if ( i < nev ) ...
//
// See also:
//  CmdLine.h


#ifndef _CMDARG_H_
#define _CMDARG_H_

#ifdef GNUPRAGMA
#pragma interface
#endif

#include "CLHEP/config/CLHEP.h"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/String/Strings.h"

#ifdef HEP_NO_INLINE_IN_DECLARATION
#define inline
#endif

class HepCmdArg {

public:

  HepCmdArg(char optchar, const HepString & keyword,
	    const HepString & value, const HepString & description);
  HepCmdArg(char optchar, const HepString & keyword,
	    const HepString & description);
  HepCmdArg(const HepString & value, const HepString & description);
     
  virtual ~HepCmdArg();
     
  inline char charName() const;
  // Get the character (short-option) name of this argument.

  inline const HepString & keywordName() const;
  // Get the keyword (long-option) name of this argument

  inline const HepString & valueName() const;
  // Get the value name of this argument.

  inline const HepString & description() const;
  // Get the description (help-message) of this argument.

  virtual int setValue(int argc, char **arg) = 0;
  // Set the value from a character string
  // return the number of args used, -1 if failed
    
  inline bool isPositional() const;

  inline bool isRequired() const;
  void required(bool r = true);
 
protected:

  HepString _keyword;
  HepString _valueName;
  HepString _description;
  char _charName;
  bool _isPositional;
  bool _isRequired;
};



class HepCmdArgInt : public HepCmdArg {

public:
  HepCmdArgInt(char optchar, const HepString & keyword,
	       const HepString & value, const HepString & description,
	       int defValue = 0);
  HepCmdArgInt(const HepString & value, const HepString & description);
  HepCmdArgInt(const HepString & value, const HepString & description,
	       int defValue);

  virtual ~HepCmdArgInt();

  inline operator int();
  // integer cast operator. this is how you get the value out!

  inline HepCmdArgInt & operator= (int);
  // assignment of value

  virtual int setValue(int argc, char **arg);

private:

  int _value;

};



class HepCmdArgFloat : public HepCmdArg {

public:
  HepCmdArgFloat(char optchar, const HepString & keyword,
		 const HepString & value, const HepString & description,
			float defValue = 0.0);
  HepCmdArgFloat(const HepString & value, const HepString & description);
  HepCmdArgFloat(const HepString & value,const HepString & description,
		 float defValue);

  virtual ~HepCmdArgFloat();

  inline operator float();
  // float cast operator. this is how you get the value out!

  inline HepCmdArgFloat & operator= (float);
  // assignment of value

  virtual int setValue(int argc, char **arg);

private:

  float _value;

};



class HepCmdArgString : public HepCmdArg {

public:
  HepCmdArgString(char optchar, const HepString & keyword,
			 const HepString & value,
			 const HepString & description,
			 const char * defValue = 0);
  HepCmdArgString(const HepString & value,
			 const HepString & description);
  HepCmdArgString(const HepString & value,
			 const HepString & description,
			 const HepString & defValue);

  virtual ~HepCmdArgString();

  inline operator const char * ();
  // const char* cast operator. This is how you get the value out!

  inline HepCmdArgString & operator= (const HepString &);
  // assignment of value

  virtual int setValue(int argc, char **arg);
    
private:

  HepString _value;

};



class HepCmdArgBoolean : public HepCmdArg {

public:
  HepCmdArgBoolean(char optchar, const HepString & keyword,
		       const HepString & description,
		       bool defValue = false);

  virtual ~HepCmdArgBoolean();

  inline operator bool();
  // int cast operator. this is how you get the value out!

  inline HepCmdArgBoolean & operator= (bool);
  // assignment of value

  virtual int setValue(int, char **);

private:

  bool _value;

};

class HepCmdArgStringList : public HepCmdArg {

public:
  HepCmdArgStringList(const HepString & value,
			     const HepString & description);
  HepCmdArgStringList(const HepString & value, const HepString & description,
		      const HepString & defValue);

  virtual ~HepCmdArgStringList();

  inline const HepString & operator[] (unsigned i) const;

  inline unsigned length() const;

  inline const HepAList<HepString> & stringList() const;

  virtual int setValue(int argc, char **arg);

private:

  HepAList<HepString> _list;

};

#ifdef HEP_NO_INLINE_IN_DECLARATION
#undef inline
#endif

#ifdef HEP_SHORT_NAMES
typedef HepCmdArg CmdArg;
typedef HepCmdArgInt CmdArgInt;
typedef HepCmdArgFloat CmdArgFloat;
typedef HepCmdArgString CmdArgString;
typedef HepCmdArgBoolean CmdArgBoolean;
typedef HepCmdArgStringList CmdArgStringList;
#endif

#ifndef HEP_DEBUG_INLINE
#include "CLHEP/String/CmdArg.icc"
#endif

#endif
