//--------------------------------------------------------------------------
//
// Class BbrRefCountedPointer
//      Nonintrusive reference-counted pointer template implementation base.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Copyright Information:
//	Copyright (C) 1999	       California Institute of Technology
//
//--------------------------------------------------------------------------

#ifndef BBRREFCOUNTEDPOINTERBASE_HH
#define BBRREFCOUNTEDPOINTERBASE_HH


/**
 *  BbrRefCountedPointerBase is the base class of and provides a
 *  non-typesafe implementation of BbrRefCountedPointer template
 *  instances.  For a description of these, see BbrRefCountedPointer.
 *
 *  @see BbrRefCountedPointer
 *
 *  @version $Id: BbrRefCountedPointerBase.hh 443 2010-01-14 12:24:42Z stroili $
 *  @author  Alex Samuel (samuel) 1999.01.05 
 */

class BbrRefCountedPointerBase
{

protected:

// constructors

    BbrRefCountedPointerBase();
    BbrRefCountedPointerBase(const BbrRefCountedPointerBase& rcPointer);
    ~BbrRefCountedPointerBase();


// accessors

    /**
     *  Returns true iff this rc pointer is 0.
     */
    bool isNull() const;

    /**
     *  Returns a void* which points to the object pointed to by this
     *  rc pointer. Should not be called if this rc pointer is null.
     */
    void* getPointer() const;

    /**
     *  Returns the number of rc pointers, including this one, pointing
     *  to the same object as this one. Should not be called if this
     *  rc pointer is null.
     */
    int getReferenceCount() const;


// operations

    /**
     *  Takes possession of the a pointer and sets this rc pointer to
     *  point to it. A new pointer record is created.
     *
     *  Precondition: this rc pointer must be null.  
     */
    void take(void* pointer);

    /**
     *  Releases posession of the object pointed to, and deletes the
     *  pointer record, and returns a pointer to the object. The
     *  object is not deleted.  This rc pointer is set to null.
     *  
     *  Precondition: this rc pointer must be the only one pointing to
     *  the object, i.e. the reference count must be one.
     */
    void* release();

    void operator= (const BbrRefCountedPointerBase& rcPointer);


// implementation nested classes

    class Record
    {

    public:

    // constructors

	Record(void* pointer);


    // data members

	void* _pointer;
	int _referenceCount;


    };


// helper functions

    int clear();
    

private:

// helper functions

    // Sets the pointer record to the provided value. The reference count
    // in the record is incremented and returned.
    int setRecord(Record* record);

    // Clears the pointer record. The reference count in the record is
    // first decremented, and if it drops to zero, the record (but not
    // the object) is deleted. Callers should keep a copy of the
    // actual pointer value in case the record is deleted.
    int clearRecord();
    

// data members

    Record* _record;


};


#endif  // #ifndef BBRREFCOUNTEDPOINTERBASE_HH
