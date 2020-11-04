#ifndef ABSARGCAST_HH
#define ABSARGCAST_HH

class AbsArg;

template <class T>
class AbsArgCast {

public:
  static T* value( AbsArg& a );
  static T* value( AbsArg* a );

};

 
#ifdef    BABAR_COMP_INST
#include  "ProxyDict/AbsArgCast.icc"
#endif // BABAR_COMP_INST

#endif //ABSARGCAST_HH
