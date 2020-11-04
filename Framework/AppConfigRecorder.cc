//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppConfigRecorder.cc 631 2010-04-28 06:46:51Z stroili $
//
// Description:
//	Class AppConfigRecorder
//      Do not use this for templated class (foo<T>).  use TemplateTemplate.hh
//      instead.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Akbar Mokhtarani		originator
//	<Author2>		<originator/contributor etc.>
//
// Copyright Information:
//	Copyright (C) 1998	LBNL
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppConfigRecorder.hh"
#include <string>
using std::string;
//-------------
// C Headers --
//-------------
extern "C" {
#include <assert.h>

}

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AbsCommandMemento.hh"
#include "FrameUtil/APPList.hh"
#include "FrameUtil/APPListIterator.hh"
#include "Framework/AppMemento.hh"

#include "CLHEP/String/Strings.h"

#include <iostream>
#include <stdlib.h>
using std::endl;
using std::ostream;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
AppConfigRecorder::AppConfigRecorder()
{
  _unsignedCommandList = new   APPList<AbsCommandMemento< unsigned >* >;
  _longCommandList = new   APPList<AbsCommandMemento< long >* >;
  _boolCommandList = new   APPList<AbsCommandMemento< bool >* >;
  _intCommandList = new   APPList<AbsCommandMemento< int >* >;
  _doubleCommandList = new   APPList<AbsCommandMemento< double >* >;
  _floatCommandList = new   APPList<AbsCommandMemento< float >* >;
  _charCommandList = new   APPList<AbsCommandMemento< char* >* >;
  _HepStringCommandList = new   APPList<AbsCommandMemento< HepString >* >;
  _stdstringCommandList = new   APPList<AbsCommandMemento< std::string >* >;

}


//--------------
// Destructor --
//--------------
AppConfigRecorder::~AppConfigRecorder()
{
  APPListDeleteAll(*_unsignedCommandList);
  delete _unsignedCommandList;
  APPListDeleteAll(*_longCommandList);
  delete _longCommandList;
  APPListDeleteAll(*_boolCommandList);
  delete _boolCommandList;
  APPListDeleteAll(*_intCommandList);
  delete _intCommandList;
  APPListDeleteAll(*_doubleCommandList);
  delete _doubleCommandList;
  APPListDeleteAll(*_floatCommandList);
  delete _floatCommandList;
  APPListDeleteAll(*_charCommandList);
  delete _charCommandList;
  APPListDeleteAll(_argvCommandList);
  APPListDeleteAll(*_HepStringCommandList);
  delete _HepStringCommandList;
  APPListDeleteAll(*_stdstringCommandList);
  delete _stdstringCommandList;
}

void AppConfigRecorder::store( int val, const char* v)
{
  AbsCommandMemento < int > *com = 
    new AbsCommandMemento < int >( val, "int" , v);
  _intCommandList->append( com );
}
void AppConfigRecorder::store( double val, const char* v)
{
  AbsCommandMemento < double > *com = 
    new AbsCommandMemento < double >( val, "double" , v);
  _doubleCommandList->append( com );
}
void AppConfigRecorder::store( bool val, const char* v)
{
  AbsCommandMemento < bool > *com = 
    new AbsCommandMemento < bool >( val, "bool" , v);
  _boolCommandList->append( com );
}
void AppConfigRecorder::store( float val, const char* v)
{
  AbsCommandMemento < float > *com = 
    new AbsCommandMemento < float >( val, "float" , v);
  _floatCommandList->append( com );
}
void AppConfigRecorder::store( string val, const char* v)
{
  AbsCommandMemento < string > *com = 
    new AbsCommandMemento < string >( val, "stdstring" , v);
  _stdstringCommandList->append( com );
}

void AppConfigRecorder::store( HepString val, const char* v)
{
  AbsCommandMemento < HepString > *com = 
    new AbsCommandMemento < HepString >( val, "HepString" , v);
  _HepStringCommandList->append( com );
}

void AppConfigRecorder::store( char* val, const char* v)
{
  AbsCommandMemento < char* > *com = 
    new AbsCommandMemento < char* >( val, "char" , v);
  _charCommandList->append( com );
}
void AppConfigRecorder::store( long val, const char* v)
{
  AbsCommandMemento < long > *com = 
    new AbsCommandMemento < long >( val, "long" , v);
  _longCommandList->append( com );
}
void AppConfigRecorder::store( unsigned val, const char* v)
{
  AbsCommandMemento < unsigned > *com = 
    new AbsCommandMemento < unsigned >( val, "unsigned" , v);
  _unsignedCommandList->append( com );
}
void AppConfigRecorder::store( int argc, const char* argv[])
{
  const char* fs = "#";
  int i;
  
  std::string* buff = new std::string();
  
  //put argc and argv[] in buff and intersperse them with fs
  // sprintf(buff, "%d", argc);
  
  for(i=0; i<argc; i++){
    *buff += argv[i];
    *buff += fs;
  }
  _argvCommandList.append(buff);
}

void AppConfigRecorder::Dump(ostream& o, const char* comName)const
{
  const char* fs = "#";

  AbsCommandMemento<bool>** com1;
  APPListIterator<AbsCommandMemento<bool> *> 
    iter1( *_boolCommandList );
  while ( ( com1 = iter1( ) ) ) {
    o << "C" << fs << comName << fs << (*com1)->action() << fs ;
    o << ((*com1)->value() ? "t" : "f") << fs << (*com1)->type();
    o << endl;
  }
  
  AbsCommandMemento<double>** com2;
  APPListIterator<AbsCommandMemento<double> *> 
    iter2( *_doubleCommandList );
  while ( ( com2 = iter2( ) ) ) {
    o << "C" << fs << comName << fs <<  (*com2)->action() << fs;
    o << (*com2)->value();
    o << fs << (*com2)->type()<< endl;
  }

  AbsCommandMemento<int>** com3;
  APPListIterator<AbsCommandMemento<int> *> 
    iter3( *_intCommandList );
  while ( ( com3 = iter3( ) ) ) {
    o << "C" << fs << comName << fs << (*com3)->action() << fs;
    o << (*com3)->value();
    o << fs << (*com3)->type() << endl;
  }

  AbsCommandMemento<float>** com5;
  APPListIterator<AbsCommandMemento<float> *> 
    iter5( *_floatCommandList );
  while ( ( com5 = iter5( ) ) ) {
    o << "C" << fs << comName << fs << (*com5)->action() << fs;
    o << (*com5)->value();
    o << fs << (*com5)->type()<< endl;
  }

  AbsCommandMemento<char*>** com6;
  APPListIterator<AbsCommandMemento<char*> *> 
    iter6( *_charCommandList );
  while ( ( com6 = iter6( ) ) ) {
    o << "C" << fs << comName << fs << (*com6)->action() << fs;
    o << (*com6)->value();
    o << fs << (*com6)->type()<< endl;
  }

  AbsCommandMemento<HepString>** com7;
  APPListIterator<AbsCommandMemento<HepString> *> 
    iter7( *_HepStringCommandList );
  while ( ( com7 = iter7( ) ) ) {
    o << "C" << fs << comName << fs << (*com7)->action() << fs;
    o << (*com7)->value();
    o << fs << (*com7)->type()<< endl;
  }

  AbsCommandMemento<std::string>** com10;
  APPListIterator<AbsCommandMemento<std::string>* >
    iter10( *_stdstringCommandList );
  while ( ( com10 = iter10( ) ) ) {
    o << "C" << fs << comName << fs << (*com10)->action() << fs;
    o << (*com10)->value();
    o << fs << (*com10)->type()<< endl;
  }

  AbsCommandMemento<long>** com9;
  APPListIterator<AbsCommandMemento<long>* > 
    iter9( *_longCommandList );
  while ( ( com9 = iter9( ) ) ) {
    o << "C" << fs << comName << fs << (*com9)->action() << fs;
    o << (*com9)->value();
    o << fs << (*com9)->type()<< endl;
  }

  AbsCommandMemento<unsigned>** com11;
  APPListIterator<AbsCommandMemento<unsigned>* > 
    iter11( *_unsignedCommandList );
  while ( ( com11 = iter11( ) ) ) {
    o << "C" << fs << comName << fs << (*com11)->action() << fs;
    o << (*com11)->value();
    o << fs << (*com11)->type()<< endl;
  }
  
  //general format commands. this is (arc, argv[]) format
  std::string** theComName;
  APPListIterator<std::string*> iterator(_argvCommandList);
  while ( ( theComName = iterator() ) ) {
    o << "C" << fs << *(*theComName) << endl;
  }

}

void AppConfigRecorder::Dump_tcl(ostream& o, const char* comName)const
{
  const char* fs = " ";

  AbsCommandMemento<bool>** com1;
  APPListIterator<AbsCommandMemento<bool>* > 
    iter1( *_boolCommandList );
  while ( ( com1 = iter1( ) ) ) {
    o << "  " << comName << fs << (*com1)->action() << fs ;
    o << ((*com1)->value() ? " t " : " f ") ;
    o << endl;
  }
  
  AbsCommandMemento<double>** com2;
  APPListIterator<AbsCommandMemento<double>* > 
    iter2( *_doubleCommandList );
  while ( ( com2 = iter2( ) )  ) {
    o << "  " << comName << fs <<  (*com2)->action() << fs 
      << (*com2)->value()<<endl;
  }

  AbsCommandMemento<int>** com3;
  APPListIterator<AbsCommandMemento<int>* > 
    iter3( *_intCommandList );
  while ( ( com3 = iter3( ) ) ) {
    o << "  " << comName << fs <<  (*com3)->action() << fs 
      << (*com3)->value()<<endl;
  }

  AbsCommandMemento<float>** com5;
  APPListIterator<AbsCommandMemento<float>* > 
    iter5( *_floatCommandList );
  while ( ( com5 = iter5( ) ) ) { 
    o << "  " << comName << fs <<  (*com5)->action() << fs 
      << (*com5)->value()<<endl;
  }

  AbsCommandMemento<char*>** com6;
  APPListIterator<AbsCommandMemento<char*>* > 
    iter6( *_charCommandList );
  while ( ( com6 = iter6( ) ) ) {
    o << "  " << comName << fs <<  (*com6)->action() << fs << "\"" <<
      (*com6)->value() << "\""<<endl;
  }

  AbsCommandMemento<HepString>** com7;
  APPListIterator<AbsCommandMemento<HepString>* > 
    iter7( *_HepStringCommandList );
  while ( ( com7 = iter7( ) ) ) {
    o << "  " << comName << fs <<  (*com7)->action() << fs << "\"" <<
      (*com7)->value() << "\"" <<endl;
  }

  AbsCommandMemento<std::string>** com10;
  APPListIterator<AbsCommandMemento<std::string>* >
    iter10( *_stdstringCommandList );
  while ( ( com10 = iter10( ) ) ) {
    o << "  " << comName << fs <<  (*com10)->action() << fs << "\"" <<
      (*com10)->value() << "\"" <<endl;
  }

  AbsCommandMemento<long>** com9;
  APPListIterator<AbsCommandMemento<long>* > 
    iter9( *_longCommandList );
  while ( ( com9 = iter9( ) ) ) {
    o << "  " << comName << fs <<  (*com9)->action() << fs 
      << (*com9)->value()<<endl;
  }

  AbsCommandMemento<unsigned>** com11;
  APPListIterator<AbsCommandMemento<unsigned>* > 
    iter11( *_unsignedCommandList );
  while ( ( com11 = iter11( ) ) ) {
    o << "  " << comName << fs <<  (*com11)->action() << fs 
      << (*com11)->value()<<endl;
  }
  
  //general format commands. this is (arc, argv[]) format
  int j;
  char* v[20];
  for (j=0; j < 20; j++) {
    v[j] = 0;
  }
  std::string** theComName;
  APPListIterator<std::string*> iterator(_argvCommandList);
  while ( ( theComName = iterator() ) ) {
    parseLine( const_cast<char*>((*theComName)->c_str()), "#", v);
    j=0;
    o << "  ";
    while(v[j]){
      o <<  v[j] << fs;
      delete [] v[j];
      v[j] = 0;
      j++;
    } 

    o<<endl;

  }

}
