
#ifndef HEPSTRING_TEST
#define HEPSTRING_TEST

//-- HepString Migration related functions --
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

inline string Car(const char * p) {
  string ret;
  while ( *p && isspace(*p) ) p++;
  while ( *p && !isspace(*p) ) ret += *(p++);
  return ret;
}
inline string Cdr(const char * p) {
  while ( *p && isspace(*p) ) p++;
  while ( *p && !isspace(*p) ) p++;
  while ( *p && isspace(*p) ) p++;
  return string(p);
}
inline int Find(string str,char c) {
  if(str.find(c)==string::npos) return -1;
  else return str.find(c);
}
inline int LFind(string str,char c) { 
  if(str.rfind(c)==string::npos) return -1; 
  else return str.rfind(c); 
} 
inline int Find(string str, const string & s) {
  if(str.find(s)==string::npos) return -1;
  else return str.find(s);
}
inline char At(string str,unsigned i) {
  return i < str.length() ? str.c_str()[i] : '\0';
}
inline string num2string(const int& number)
{
   ostringstream oss;
   oss << number;
   return oss.str();
}
inline string num2string(const double& number, unsigned precision=6)
{
  ostringstream oss;
  oss.precision(precision);
  oss << number;
  return oss.str();
}

#endif 
