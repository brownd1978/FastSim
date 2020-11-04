#ifndef MEMRCIPTR_HH
#define MEMRCIPTR_HH

/**
 * @file SmrPtr/MemRCIPtr.h
 * 
 * Declaration of class MemRCIPtr and related functions
 *
 * @author Andrei A. Salnikov
 * @date   Jun 13, 2001
 *
 * Copyright (C) BINP, 2001 
 * 
 * $Id: MemRCIPtr.hh 517 2010-01-15 08:24:18Z stroili $
 */

// iostream forward declarations
#include <iostream>

/**
 * This class represents a reference counted pointer type. Contrary
 * to MemRCPtr it implements intrusive RC, that is the class T should
 * inherit from MemRefCounted class. The benefits are big, so if you can
 * modify your classes then make them inherit from MemRefCounted and 
 * use this class as a smart pointer.
 */
template< class T >
class MemRCIPtr {
public:

  /// Constructor
  explicit MemRCIPtr( T* p = 0 ) : p_ ( p ) {
    if ( p_ ) p_->mem_rc_increment() ;
  }

  /// Copy constructor
  MemRCIPtr(const MemRCIPtr< T >& o) : p_ ( o.p_ ) {
    if ( p_ ) p_->mem_rc_increment() ;
  }

  /// Conversion constructor
  template <typename U>
  MemRCIPtr(const MemRCIPtr< U >& o) : p_ ( o.get() ) {
    if ( p_ ) p_->mem_rc_increment() ;
  }

  /// Destructor 
  ~MemRCIPtr() {
    if ( p_ && ! p_->mem_rc_decrement() ) delete p_ ;
  }

  /// Assigment operator
  MemRCIPtr< T >& operator=(const MemRCIPtr< T >& o) {
    if ( this->p_ != o.p_ ) {
      if ( p_ && ! p_->mem_rc_decrement() ) delete p_ ;
      if ( ( p_ = o.p_ ) != 0 ) p_->mem_rc_increment() ;
    }
    return *this ;
  }

  /// Conversion assigment operator
  template <typename U>
  MemRCIPtr< T >& operator=(const MemRCIPtr< U >& o) {
    if ( this->p_ != o.p_ ) {
      if ( p_ && ! p_->mem_rc_decrement() ) delete p_ ;
      if ( ( p_ = o.p_ ) != 0 ) p_->mem_rc_increment() ;
    }
    return *this ;
  }

  /// Assigment operator
  void reset( T* p = 0 ) {
    if ( p_ && ! p_->mem_rc_decrement() ) delete p_ ;
    if ( ( p_ = p ) != 0 ) p_->mem_rc_increment() ;
  }

  /// dereferencing 
  T* get() const { return p_ ; }

  /// dereferencing 
  T* operator->() const { return get() ; }

  /// dereferencing 
  T& operator*() const { return *get() ; }

  /// ref count
  size_t count() const { return p_ ? p_->mem_rc_count() : 0 ; }

  // cast "operators"
  template <typename U>
  MemRCIPtr< U > stat_cast() const { 
    return  MemRCIPtr< U > ( static_cast<U*> ( this->get() ) ) ;
  }
  template <typename U>
  MemRCIPtr< U > dyna_cast() const { 
    return  MemRCIPtr< U > ( dynamic_cast<U*> ( this->get() ) ) ;
  }

  /// Output method.
  void print(std::ostream& s) const;

  void dump(std::ostream& s) const ;

protected:

private:

  T* p_ ;

};


/// Output for class MemRCIPtr in "standard" form 
template< class T >
inline 
std::ostream& 
operator<<(std::ostream& s, const MemRCIPtr< T >& t) 
{ 
  t.print(s);
  return s ;
}

#ifdef    BABAR_COMP_INST
#include "MemUtil/MemRCIPtr.icc"
#endif  // BABAR_COMP_INST 

#endif // MEMRCIPTR_HH

// Local Variables: ***
// mode: c++ ***
// End: ***
