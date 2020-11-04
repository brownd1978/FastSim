#include "BaBar/BaBar.hh"

#include "CommonUtils/BbrRefFoo.hh"

#include <iostream>
using std::cout;
using std::endl;
using std::ostream;
template<> int BbrCounter<BbrRefFoo>::_nAlloc = 0;

BbrRefFoo::BbrRefFoo() : _bar( new BbrRefBar ), _instance(0)
{ init(); }

BbrRefFoo::BbrRefFoo( const BbrRefFoo& o ) : _bar( o._bar ), _instance(0)
{ init(); }

BbrRefFoo::BbrRefFoo( BbrRefBar* bar ) : _bar(bar), _instance(0)
{ 
  init(); 
} 

BbrRefFoo::~BbrRefFoo( ) 
{
  cout << "*** Deleting BbrRefFoo instance number " << _instance
       << endl;
}

void 
BbrRefFoo::init()
{
  static int count=0;
  _instance = ++count;
  cout << "==> Creating BbrRefFoo instance number " << _instance << endl;
}

void 
BbrRefFoo::print( ostream& o ) const
{
  o << " ooo BbrRefFoo instance number " << _instance;
  o << " - value : " << _bar->a() << endl;
}

BbrRefFoo&
BbrRefFoo::operator=( const BbrRefFoo& o )
{
  if( this!=&o && _bar!=o._bar )
    {
      _bar=o._bar;
    }
  return *this;
}

bool 
BbrRefFoo::operator==( const BbrRefFoo& o ) const
{
  return _bar==o._bar;
}

bool 
BbrRefFoo::operator!=( const BbrRefFoo& o ) const
{
  return _bar!=o._bar;
}

const BbrRefBar*
BbrRefFoo::myBar() const 
{
  return _bar();
}

BbrRefBar*
BbrRefFoo::myBar()
{
  return _bar();
}

BbrRefBar*
BbrRefFoo::myOwnBar()
{
  return _bar.ownRef();
}

void 
BbrRefFoo::setA( double a )
{
  myOwnBar()->setA( a );
}

double 
BbrRefFoo::a() const
{
  return myBar()->a();
}
