#ifndef BbrRefBar_hh
#define BbrRefBar_hh

#include "CommonUtils/BbrCounter.hh"
#include "CommonUtils/BbrRefCounted.hh"

#include <iosfwd>
class BbrRefFoo;
template <class T> class BbrReferencer;

class BbrRefBar : public BbrCounter<BbrRefBar>, 
		  public BbrRefCounted<BbrRefBar>
{

  BbrRefFoo* _localFoo;
  double _a;
  int _instance;
  
public:

  BbrRefBar();
  BbrRefBar( const BbrRefBar& );

  virtual ~BbrRefBar();

  BbrRefBar& operator=( const BbrRefBar& );

  void init();

  BbrRefBar* clone() const;

  void print( std::ostream& ) const;
  
  void setA( double a ) { _a=a; }
  double a() const { return _a; }

  friend class BbrReferencer<BbrRefBar>;
  friend class BbrRefFoo;
};

#endif
