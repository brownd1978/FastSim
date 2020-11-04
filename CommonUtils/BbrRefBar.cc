#include "BaBar/BaBar.hh"

#include "CommonUtils/BbrRefBar.hh"

#include "CommonUtils/BbrRefFoo.hh"

#include <iostream>
using std::cout;
using std::endl;
using std::ostream;

template<> int BbrCounter<BbrRefBar>::_nAlloc = 0;

BbrRefBar::BbrRefBar() 
  : _a(0.), _instance(0)
{ init(); }

BbrRefBar::BbrRefBar( const BbrRefBar& o ) 
  : _a( o._a ), _instance(0)
{ init(); }

BbrRefBar::~BbrRefBar()
{
  delete _localFoo;
  cout << "*** Deleting BbrRefBar instance number " << _instance;
  cout << " - nb of instances left " 
       << BbrCounter<BbrRefBar>::nAlloc();
  cout << " - nb of pending links " <<  nLinks() << endl;
}

void 
BbrRefBar::init()
{
  dropLink(); // the local cand doesn't count as a link
  _localFoo = new BbrRefFoo(this);
  assert( _localFoo!=0 );
  static int count=0;
  _instance = ++count;
  cout << "==> Creating BbrRefBar instance number " << _instance << endl;
}

BbrRefBar&
BbrRefBar::operator=( const BbrRefBar& o )
{
  if( this!=&o )
    {
      _a=o._a;
    }
  return *this;
}

BbrRefBar*
BbrRefBar::clone() const
{
  return new BbrRefBar( *this );
}

void 
BbrRefBar::print( ostream& o ) const
{
  o << " ooo BbrRefBar instance number " << _instance;
  o << " - pending links : " << nLinks() << endl;
}
