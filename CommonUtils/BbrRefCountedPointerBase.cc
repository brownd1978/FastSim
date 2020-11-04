//--------------------------------------------------------------------------
//
// File and Version Information:
// 	$Id: BbrRefCountedPointerBase.cc 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      Nonintrusive reference-counted pointer template implementation base.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Alex Samuel, originator
//
// Copyright Information:
//	Copyright (C) 1999	       California Institute of Technology
//
//--------------------------------------------------------------------------

#include "BaBar/BaBar.hh"

#include "CommonUtils/BbrRefCountedPointerBase.hh"

#include <assert.h>

#ifdef BBRREFCOUNTEDPOINTER_DEBUG
#include <iostream>
using std::cout;
using std::endl;
#endif


BbrRefCountedPointerBase::
BbrRefCountedPointerBase() :
    _record(0)
{
}


BbrRefCountedPointerBase::
BbrRefCountedPointerBase(const BbrRefCountedPointerBase& rcPointer) :
    _record(0)
{
    if(!rcPointer.isNull())
    {
#ifdef BBRREFCOUNTEDPOINTER_DEBUG
	cout << "RCPointer constructor copying "
	     << rcPointer.getPointer()
	     << endl;
#endif

	setRecord(rcPointer._record);
    }
}


BbrRefCountedPointerBase::
~BbrRefCountedPointerBase()
{
    // children must call clear()
    assert(isNull());
}


bool BbrRefCountedPointerBase::
isNull() const
{
    return _record == 0;
}


void* BbrRefCountedPointerBase::
getPointer() const
{
    assert(!isNull());
    return _record->_pointer;
}


int BbrRefCountedPointerBase::
getReferenceCount() const
{
    assert(!isNull());
    return _record->_referenceCount;
}


void BbrRefCountedPointerBase::
take(void* pointer)
{
    if(!isNull())
        clear();

#ifdef BBRREFCOUNTEDPOINTER_DEBUG
    cout << "RCPointer taking posession of " 
	 << pointer
	 << endl;
#endif

    (void) setRecord(new Record(pointer));
}


void* BbrRefCountedPointerBase::
release()
{
    void* pointer = getPointer();

#ifdef BBRREFCOUNTEDPOINTER_DEBUG
    cout << "RCPointer releasing posession of "
	 << pointer
	 << endl;
#endif

    bool isReleased = (clear() == 0);

    // make sure the pointer is fully released, i.e. the rc is zero
    assert(isReleased);

    return pointer;
}


void BbrRefCountedPointerBase::
operator= (const BbrRefCountedPointerBase& rcPointer)
{
    assert(isNull());

    if(!rcPointer.isNull())
    {
#ifdef BBRREFCOUNTEDPOINTER_DEBUG
	cout << "RCPointer assigning to "
	     << rcPointer.getPointer()
	     << endl;
#endif

        setRecord(rcPointer._record);
    }
}


int BbrRefCountedPointerBase::
clear()
{
    return clearRecord();
}


int BbrRefCountedPointerBase::
setRecord(Record* record)
{
    assert(isNull());
    assert(record != 0);
    assert(_record == 0);

    _record = record;
    int refCount = ++_record->_referenceCount;

#ifdef BBRREFCOUNTEDPOINTER_DEBUG
	cout << "RCPointer setting pointer to "
	     << record->_pointer
	     << ", "
	     << "incrementing ref count to "
	     << refCount
	     << endl;
#endif

    return refCount;
}


int BbrRefCountedPointerBase::
clearRecord()
{
    int refCount = --_record->_referenceCount;
    assert(refCount >= 0);

#ifdef BBRREFCOUNTEDPOINTER_DEBUG
	cout << "RCPointer clearing pointer from "
	     << _record->_pointer
	     << ", "
	     << "decrementing ref count to "
	     << refCount
	     << endl;
#endif

    if(refCount == 0)
    {
#ifdef BBRREFCOUNTEDPOINTER_DEBUG
	cout << "RCPointer deleting record at "
	     << (void*) _record
	     << endl;
#endif	

	delete _record;
    }

    _record = 0;

    return refCount;
}


BbrRefCountedPointerBase::Record::
Record(void* pointer) :
    _pointer(pointer),
    _referenceCount(0)
{
    assert(pointer != 0);

#ifdef BBRREFCOUNTEDPOINTER_DEBUG
	cout << "RCPointer new record at "
	     << (void*) this
	     << endl;
#endif	
}


