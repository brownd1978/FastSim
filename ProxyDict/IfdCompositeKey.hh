#ifndef IFDCOMPOSITEKEY_HH
#define IFDCOMPOSITEKEY_HH
//--------------------------------------------------------------------------
// File and Version Information:
//    $Id: IfdCompositeKey.hh 520 2010-01-15 11:40:48Z stroili $
//
// Description:
//
// Author List:
//    Ed Frank                        University of Pennsylvania
//
// History:
//    Ed Frank      17 Nov 96         Creation of first version
//    Ed Frank      29 Nov 96         NO_INLINE_COPY_CTOR added
//    Ed Frank      13 Feb 96         NO_INLINE_COPY_CTOR removed.
//
// CPP Macros:
//    NO_INLINE_COPY_CTOR      Turns off inlining of copy ctor.  HP can not
//                             support the for() in an inline right now.
// Bugs:
//
//------------------------------------------------------------------------

#include <stdlib.h>
#include "ProxyDict/IfdKey.hh"
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
#include <iostream>
#else                                                               // BABAR_IOSTREAMS_MIGRATION
#include <iostream.h>
#endif                                                              // BABAR_IOSTREAMS_MIGRATION


class IfdCompositeKey : public IfdKey {
public:
   IfdCompositeKey();
   virtual ~IfdCompositeKey();

   virtual int  operator==( const IfdKey &k ) const;
   virtual void add( const IfdKey& k );

   virtual IfdKey* clone( void ) const { return new IfdCompositeKey( *this );}
      //  caller owns returned copy


   // NOTE:  The caller _OWNS_ the items reurned by car, cdr, and tail.
   //
   IfdKey*          car()  const;     // 1st element of the list
   IfdCompositeKey* cdr()  const;     // 2nd through last elements of the list.
   IfdKey*          tail() const;     // last element of the  list

#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
   virtual void print( std::ostream &o) const;
#else                                                               // BABAR_IOSTREAMS_MIGRATION
   virtual void print( ostream &o) const;
#endif                                                              // BABAR_IOSTREAMS_MIGRATION

   // The next two methods are provisional. Contact efrank@upenn5.hep.upenn.edu
   // before using them.

   // virtual unsigned int hash( void );
   //   Rather than overriding hash(), we accumulate the hash value
   //   as add()'s are done.  Otherwise repeated calls on hash() would
   //   be expensive.

   inline const IfdKey* operator[] (unsigned int u) { return kid[ u ]; }
      // caller does NOT own what is returned

private:
   IfdCompositeKey( const IfdCompositeKey& k );


protected:
   // Fragile, non-paramentric and dirty, but fast.  
   enum { maxChildren=4 };
   const IfdKey* kid[ maxChildren ];

   // virtual keyKind getKeyKind( void ) const { return compositeKey; }

private:
  // No assignment allowed.  Ownership problems result when
  // copying the owned ptrs.  Also, this keeps this class like
  // the base IfdKey class.

  IfdCompositeKey& operator=( const IfdCompositeKey& ) 
     {  return *this;}

};

#include "ProxyDict/IfdCompositeKey.icc"

#endif /*  IFDCOMPOSITEKEY_HH */
