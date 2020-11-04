#ifndef IFDTYPEKEYIFACE_HH
#define IFDTYPEKEYIFACE_HH
//--------------------------------------------------------------------------
// File and Version Information:
//    $Id: IfdTypeKeyIFace.hh 520 2010-01-15 11:40:48Z stroili $
//
// Description:
//
// Author List:
//    Ed Frank                        University of Pennsylvania
//
// History:
//    Ed Frank      17 Nov 96         Creation of first version
//    Ed Frank      02 Dec 97         -Make abstract via pure virt dtor
//                                    -Add dumpName()
//                                    -Move print impl. into derivatives
//                                    -Move stuff to .cc (no longer inline)
//
// Bugs:
//    I'd like to have print(ostream&) pure virtual, but then the Sun debugger
//    seems unable to function.
//
//------------------------------------------------------------------------

#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
#include <iostream>
#else                                                               // BABAR_IOSTREAMS_MIGRATION
#include <iostream.h>
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
#include <stdlib.h>
#include "ProxyDict/IfdKey.hh"


// A class that represents a generic TypeKey<T>.  This lets you
// express the idea "not just a Key, but a TypeKey (although i don't
// care which TypeKey<T> it is)"  See DictKey.
//

class IfdTypeKeyIFace : public IfdKey {
public:
    virtual ~IfdTypeKeyIFace();

    // Implement the required members from Key
    //
    virtual int operator==( const IfdKey &k ) const
       { return  ( typeKey == k.getKeyKind()) && ( intVal == k.intVal ); }

#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
    virtual void print( std::ostream &o ) const;
#else                                                               // BABAR_IOSTREAMS_MIGRATION
    virtual void print( ostream &o ) const;
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
       // Print ourselves to an ostream. See IfdTypeKey.hh/cc.  Operator
       // << in ostream uses this, so no terminating endl;

#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
    void dumpName( std::ostream& o ) const;
#else                                                               // BABAR_IOSTREAMS_MIGRATION
    void dumpName( ostream& o ) const;
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
       // just print() with a newline to force a flush.  Intended for
       // use in a debugger.

protected:
    IfdTypeKeyIFace();  //IfdTypeKeyIFaces are only to be created as TypeKeys
    // virtual keyKind getKeyKind( void ) const { return typeKey; }
private:
    // Copy ctor and assignemnt op are not allowed.  This keeps
    // the class behavior like its base, IfdKey.  See that class for
    // more info.
    IfdTypeKeyIFace( const IfdTypeKeyIFace &);
    IfdTypeKeyIFace& operator=( IfdTypeKeyIFace &);
};
#endif /*  IFDTYPEKEYIFACE_HH */
