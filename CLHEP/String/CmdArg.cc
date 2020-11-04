// $Id: CmdArg.cc 192 2009-03-04 12:20:53Z stroili $
//
// CmdArg.cc - classes to parse command line options
//

#ifdef GNUPRAGMA
#pragma implementation
#endif

#include <stdlib.h>
#include <stdio.h>

#include "CLHEP/String/CmdArg.h"

HepCmdArg::HepCmdArg(char optchar, const HepString & keyword,
		     const HepString & value, const HepString & description)
: _keyword(keyword), _valueName(value), _description(description),
  _charName(optchar), _isPositional(false), _isRequired(false) {}

HepCmdArg::HepCmdArg(char optchar, const HepString & keyword,
		     const HepString & description)
: _keyword(keyword), _description(description), _charName(optchar),
  _isPositional(false), _isRequired(false) {}

HepCmdArg::HepCmdArg(const HepString & value, const HepString & description)
: _valueName(value), _description(description), _charName('\0'),
  _isPositional(true), _isRequired(false) {}

HepCmdArg::~HepCmdArg() {}

void HepCmdArg::required(bool r) {
  _isRequired = r;
}

HepCmdArgInt::HepCmdArgInt(char optchar, const HepString & keyword,
			   const HepString & value,
			   const HepString & description, int defValue)
: HepCmdArg(optchar, keyword, value, description), _value(defValue) {}

HepCmdArgInt::HepCmdArgInt(const HepString & value,
			   const HepString & description)
: HepCmdArg(value, description), _value(0) {
  required();
}

HepCmdArgInt::HepCmdArgInt(const HepString & value, 
			   const HepString & description, int defValue)
: HepCmdArg(value, description), _value(defValue) {}

HepCmdArgInt::~HepCmdArgInt() {}

int HepCmdArgInt::setValue(int argc, char **arg) {
  if (argc < 1) return -1;
  if (sscanf(*arg,"%d", &_value) != 1) return -1;
  return 1;
}

HepCmdArgFloat::HepCmdArgFloat(char optchar, const HepString & keyword,
			       const HepString & value,
			       const HepString & description, float defValue)
: HepCmdArg(optchar, keyword, value, description), _value(defValue) {}

HepCmdArgFloat::HepCmdArgFloat(const HepString & value,
			       const HepString & description)
: HepCmdArg(value, description), _value(0) {
  required();
}

HepCmdArgFloat::HepCmdArgFloat(const HepString & value, 
			       const HepString & description, float defValue)
: HepCmdArg(value, description), _value(defValue) {}

HepCmdArgFloat::~HepCmdArgFloat() {}

int HepCmdArgFloat::setValue(int argc, char **arg) {
  if (argc < 1) return -1;
  if (sscanf(*arg,"%f",&_value) != 1) return -1;
  return 1;
}

HepCmdArgString::HepCmdArgString(char optchar, const HepString & keyword, 
				 const HepString & value,
				 const HepString & description,
				 const char * defValue)
: HepCmdArg(optchar, keyword, value, description), _value(defValue) {}

HepCmdArgString::HepCmdArgString(const HepString & value,
				 const HepString & description)
: HepCmdArg(value, description) {
  required();
}

HepCmdArgString::HepCmdArgString(const HepString & value,
				 const HepString & description, 
				 const HepString & defValue)
: HepCmdArg(value, description), _value(defValue) {}

HepCmdArgString::~HepCmdArgString(){}

int HepCmdArgString::setValue(int argc, char **arg) {
  if (argc < 1) return -1;
  _value = (char *)*arg;
  return 1;
}

HepCmdArgBoolean::HepCmdArgBoolean(char optchar, const HepString & keyword,
				   const HepString & description,
				   bool defValue)
: HepCmdArg(optchar, keyword, description), _value(defValue) {}

HepCmdArgBoolean::~HepCmdArgBoolean() {}

int HepCmdArgBoolean::setValue(int, char **) {
  _value = bool(!_value);
  return 0;
}

HepCmdArgStringList::HepCmdArgStringList(const HepString & value,
					 const HepString & description)
: HepCmdArg(value, description) {
  required();
}

HepCmdArgStringList::HepCmdArgStringList(const HepString & value,
					 const HepString & description, 
					 const HepString & defValue)
: HepCmdArg(value, description) {
  _list.append(new HepString(defValue));
}

HepCmdArgStringList::~HepCmdArgStringList() {
  HepAListDeleteAll(_list);
}

int HepCmdArgStringList::setValue(int argc, char **arg) {
  if (argc < 1) return -1;
  HepAListDeleteAll(_list);
  for (int i=0; i<argc; i++) _list.append(new HepString(arg[i]));
  return _list.length();
}
