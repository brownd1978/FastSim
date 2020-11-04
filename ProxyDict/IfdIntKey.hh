#ifndef IFDINTKEY_HH
#define IFDINTKEY_HH
//--------------------------------------------------------------------------
// File and Version Information:
//    $Id: IfdIntKey.hh 520 2010-01-15 11:40:48Z stroili $
//
// Description:
//
// Author List:
//    Ed Frank                        University of Pennsylvania
//
// History:
//    Ed Frank      17 Nov 96         Creation of first version
//
// Bugs:
//
//------------------------------------------------------------------------

#include "ProxyDict/IfdKey.hh"
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
#include <iostream>
#else                                                               // BABAR_IOSTREAMS_MIGRATION
#include <iostream.h>
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
#include <stdlib.h>


class IfdIntKey : public IfdKey {
public:
   IfdIntKey( const int i );
      // See also protected:IfdIntKey()

   virtual int operator==( const IfdKey& k ) const {
      return ( intKey == k.getKeyKind() ) && ( intVal == k.intVal );
   }

   virtual ~IfdIntKey() {}

   virtual IfdKey* clone(void) const { return new IfdIntKey( intVal ); }
      // Caller owns returned copy of this key.

#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
   virtual void print( std::ostream &o) const 
#else                                                               // BABAR_IOSTREAMS_MIGRATION
   virtual void print( ostream &o) const 
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
     { o << "IfdIntKey(" << intVal << ")"; }

protected:

private:
  // Copy ctor and assignemnt op are not allowed.  This keeps
  // the class behavior like its base, IfdKey.  See that class for
  // more info.
  IfdIntKey( const IfdIntKey &);
  IfdIntKey& operator=( IfdIntKey &);


};


#endif /*  IFDINTKEY_HH */
