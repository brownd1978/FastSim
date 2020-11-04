//--------------------------------------------------------------------------
//
// Class BbrRefCountedPointer
//      Nonintrusive reference-counted pointer template.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Copyright Information:
//	Copyright (C) 1999	       California Institute of Technology
//
//--------------------------------------------------------------------------

#ifndef BBRREFCOUNTEDPOINTER_HH
#define BBRREFCOUNTEDPOINTER_HH

#include "CommonUtils/BbrRefCountedPointerBase.hh"


/**
 *  BbrRefCountedPointer is a pointer template that provides reference
 *  counting and automatic deletion.  It is nonintrusive, in that the
 *  inheritance hiearchy of the target classes need not be modified
 *  for use with this template.  Instead, this template uses an added
 *  level of indirection to wrap objects of existing classes.
 *
 *  The BbrRefCountedPointer assumes memory responsiblity for the
 *  object it points to.  Therefore, only one BbrRefCounted pointer
 *  should explicitly be pointed at an object.  Additional pointers
 *  can be constructed using BbrRefCountedPointer's copy constructor.
 *
 *  @version $Id: BbrRefCountedPointer.hh 443 2010-01-14 12:24:42Z stroili $
 *  @author  Alex Samuel (samuel) 1999.01.05 
 */

template <class T>
class BbrRefCountedPointer :
    private BbrRefCountedPointerBase
{

public:

// constructors

    /**
     *  Creates a new rc pointer. Its value is initially null.
     */
    BbrRefCountedPointer();

    /**
     *  Creates a new rc pointer initialized to point to an object.
     *  The pointer argument used to specify the object is zeroed, and
     *  the rc pointer takes possession of the object. Equivalent to
     *  using the default constructor followed by a call to take.
     *
     *  @param     pointer    a pointer variable containing the value
     *                        to which this object will be set; the 
     *                        argument pointer is then set to 0
     */
    BbrRefCountedPointer(T*& pointer);

    /**
     *  Creates a new rc pointer. Its value is initalized to the value
     *  of another rc pointer.
     */
    BbrRefCountedPointer(const BbrRefCountedPointer<T>& rcPointer);

    /**
     *  Destroys this rc pointer. If no other rc pointers point to the
     *  object to which it points, the object is deleted.
     */
    ~BbrRefCountedPointer();


// accessors

    bool isNull() const;


// operations

    /**
     *  Sets this rc pointer to the contents of the supplied pointer
     *  variable.  The pointer variable argument is set to null. The
     *  rc pointer assumes subsequent memory responsibility for the
     *  object pointed to.  
     *
     *  @param     pointer    a pointer variable containing the value
     *                        to which this object will be set; the 
     *                        argument pointer is then set to 0
     */
    void take(T*& pointer);

    /**
     *  Releases the object pointed to by the rc pointer. The reference
     *  count on the object must be one in order for the object to be
     *  released.
     *
     *  @returns              a pointer to the object formerly owned by 
     *                        the rc pointer 
     */
    T* release();

    /**
     *  Zeros the rc pointer. If no other rc pointer point to the object,
     *  it is deleted automatically.
     */
    void null();


// operators

    T* getPointer() const;

    T* operator-> () const;
    T& operator* () const;
    operator T* () const;

    bool operator== (T* pointer) const;
    bool operator!= (T* pointer) const;

    /**
     *  Sets the value of this rc pointer to the value of another rc pointer.
     */
    const BbrRefCountedPointer<T>& 
	operator= (const BbrRefCountedPointer<T>& rcPointer);


};


#ifdef  BABAR_COMP_INST
#include "CommonUtils/BbrRefCountedPointer.icc"
#endif  // #ifdef BBR_COMP_INST


#endif  // #ifndef BBRREFCOUNTEDPOINTER_HH
