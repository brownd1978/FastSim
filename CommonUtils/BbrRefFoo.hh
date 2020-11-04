#ifndef BbrRefFoo_hh
#define BbrRefFoo_hh

#include "CommonUtils/BbrCounter.hh"

#include "CommonUtils/BbrReferencer.hh"
#include "CommonUtils/BbrRefBar.hh"
// template <class T> class BbrReferencer;
// class BbrRefBar;

#include <iosfwd>

class BbrRefFoo  : public BbrCounter<BbrRefFoo>
{
  
  BbrReferencer<BbrRefBar> _bar;
  int _instance;
  
public:
  
  BbrRefFoo( );
  BbrRefFoo( BbrRefBar* );
  BbrRefFoo( const BbrRefFoo& );
  virtual ~BbrRefFoo();

  const BbrRefBar* myBar() const;
  BbrRefBar* myBar();
  BbrRefBar* myOwnBar();

  void init();

  void setA( double a );
  double a() const;

  void print( std::ostream& ) const;

  BbrRefFoo& operator=( const BbrRefFoo& );
  bool operator==( const BbrRefFoo& ) const;
  bool operator!=( const BbrRefFoo& ) const;

};

#endif
