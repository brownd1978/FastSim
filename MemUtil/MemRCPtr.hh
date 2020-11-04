#ifndef MEMRCPTR_HH
#define MEMRCPTR_HH

/**
 * @file SmrPtr/MemRCPtr.h
 * 
 * Declaration of class MemRCPtr and related functions
 *
 * @author Andrei A. Salnikov
 * @date   May  9, 2001
 *
 * Copyright (C) BINP, 2001 
 * 
 * $Id: MemRCPtr.hh 517 2010-01-15 08:24:18Z stroili $
 */

// iostream forward declarations
#include <iostream>

// base class header
#include "MemUtil/MemRCIPtr.hh"
#include "MemUtil/MemRefCounted.hh"

template <typename T>
class MemRCPtrLittleHelper : protected MemRefCounted {
  friend class MemRCIPtr<MemRCPtrLittleHelper> ;
public: 
  MemRCPtrLittleHelper ( T*ptr ) : MemRefCounted(), ptr_(ptr) {}
  ~MemRCPtrLittleHelper() { delete ptr_ ; }
  T* get() const { return ptr_ ; }
private:
  T* ptr_ ;
};

/**
 *  This class represents a reference counted pointer type.
 */
template< class T >
class MemRCPtr : private MemRCIPtr< MemRCPtrLittleHelper<T> > {

  typedef MemRCPtrLittleHelper<T> Helper ;
  typedef MemRCIPtr< Helper > Base ;

public:

  /// Constructor
  explicit MemRCPtr( T* ptr = 0 ) : Base( ptr == 0 ? 0 : new Helper(ptr) ) {}

  /// Copy constructor
  MemRCPtr(const MemRCPtr< T >& o ) throw() : Base( o ) {}

  /// Conversion constructor
  template <typename U>
  MemRCPtr(const MemRCPtr< U >& o ) throw() : Base( o ) {}

  /// Destructor 
  ~MemRCPtr() throw() {}

  /// Assigment operator
  MemRCPtr< T >& operator=(const MemRCPtr< T >& o) throw() {
    Base::operator=(o) ;
    return *this ;
  }

  /// Conversion assigment operator
  template <typename U>
  MemRCPtr< T >& operator=(const MemRCPtr< U >& o) throw() {
    Base::operator=(o) ;
    return *this ;
  }

  /// get real pointer
  T* get() const throw() { 
    Helper* h = Base::get() ; 
    return h ? h->get() : 0 ; 
  }

  /// get real pointer
  T* operator->() const throw() { return get() ; }

  /// get an object
  T& operator*() const throw() { return *get() ; }

  /// reset the pointer
  void reset( T* ptr = 0 ) {
    Base::reset ( ptr == 0 ? 0 : new Helper(ptr) ) ;
  }

  /// ref count
  size_t count() const { return Base::count() ; }

  /// Output method.
  void print(std::ostream& s) const ;

  void dump(std::ostream& s) const ;

protected:

private:

};


/// Output for class MemRCPtr in "standard" form 
template< class T >
inline 
std::ostream& 
operator<<(std::ostream& s, const MemRCPtr< T >& t) 
{ 
  t.print(s);
  return s ;
}

#ifdef    BABAR_COMP_INST
#include "MemUtil/MemRCPtr.icc"
#endif  // BABAR_COMP_INST 

#endif // MEMRCPTR_HH

// Local Variables: ***
// mode: c++ ***
// End: ***
