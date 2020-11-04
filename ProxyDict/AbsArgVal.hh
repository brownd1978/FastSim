#ifndef ABSARGVAL_HH
#define ABSARGVAL_HH

// Notes:
//   1.  AbsArgVal do NOT OWN the T* that you build them with.  AbsArgs should
//        be thought of a a wrapper around the T* they  are built from.
//
//   2.   I expect common usage to look like
//           T myT;
//           void f( AbsArg );         // typical client signature.
//
//           f( AbsArgVal<T>( T ) );   //  call semantics.
//
//        Notice thta the signature is in terms of the abstract base class, but
//        we build a wrapper (concrete derivative) to do the call.  In
//        order to speed  this up, AbsArgVal<T> has _myKey as a class 
//        variable (static).
//
//    3.  forget about const-ness.  It's impossible to do (AbsArg is really
//        just a smart pointer, at heart).  Be happy you've got type saftey 
//        rather than void*.



#include "ProxyDict/AbsArg.hh"

class IfdTypeKeyIFace;

template <class T> class AbsArgCast;

template <class T>
class AbsArgVal : public AbsArg {
public:
  //  Note: The workaround to a VxWorks template instantiation bug requires
  // that  the destructor be the first function defined in a templated 
  // class.  Please do not move this or put functions above it!

  virtual ~AbsArgVal();
  AbsArgVal( T* t);
  AbsArgVal( T& t);

  virtual const IfdTypeKeyIFace&  getTypeKey() const;
  virtual AbsArg* clone() const;

private:
  T* _myT;

  friend class AbsArgCast<T>;
};

#include  "ProxyDict/AbsArgVal.icc"

#endif //ABSARGVAL_HH
