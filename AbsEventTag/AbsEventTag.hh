//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AbsEventTag.hh 488 2010-01-13 16:43:49Z stroili $
//
// Description:
//      Class AbsEventTag. Interface definition file for transient event tag.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      David R. Quarrie                Original Author
//
// Copyright Information:
//      Copyright (C) 1998              Lawrence Berkeley Laboratory
//
// Revision History:
//	20020325  M. Kelsey -- Bug fix: call-throughs to getXXX need "return"
//------------------------------------------------------------------------

#ifndef ABSEVENTTAG_HH
#define ABSEVENTTAG_HH

//-----------------
// BaBar Headers --
//-----------------
#include "BaBar/BaBar.hh"

//----------------------
// Base Class Headers --
//----------------------
#include "AbsEvent/AbsEvtObj.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class AbsEventTagBoolIter;
class AbsEventTagIntIter;
class AbsEventTagFloatIter;
template <class T> class ComPackBase;

#include <string>

/**
 ** The AbsEventTag class. This is an abstract interface for the event tag.
 **
 ** The event tag contains named integer, float and boolean elements. The
 ** name of each element must be unique, independent of the data type.
 **
 ** Copyright (C) 1998              Lawrence Berkeley National Laboratory
 **
 ** @version $Id: AbsEventTag.hh 488 2010-01-13 16:43:49Z stroili $ 
 **
 ** @author (David R. Quarrie)		Originator
 **/

//              ---------------------
//              -- Class Interface --
//              ---------------------
 
class AbsEventTag : public AbsEvtObj {

//--------------------
// Instance Members --
//--------------------

public:

    // Operations

    /**
     ** Retrieve a boolean tag element. The "key" argument is used as
     ** the key for identifying the element. The function value indicates
     ** whether the specified tag element exists, and the "value" argument
     ** is used to return the value of the element.
     **/
    virtual bool getBool ( bool&  value, const char* const key ) const = 0;

    bool getBool ( bool&  value, const std::string& key ) const{
      return getBool(value,key.c_str());
    }

    /**
     ** Retrieve an integer tag element. The "key" argument is used as
     ** the key for identifying the element. The function value indicates
     ** whether the specified tag element exists, and the "value" argument
     ** is used to return the value of the element.
     **/
    virtual bool getInt  ( int&   value, const char* const key ) const = 0;

    bool getInt ( int&  value, const std::string& key ) const{
      return getInt(value,key.c_str());
    }

    /**
     ** Retrieve float tag element. The "key" argument is used as
     ** the key for identifying the element. The function value indicates
     ** whether the specified tag element exists, and the "value" argument
     ** is used to return the value of the element.
     **/
    virtual bool getFloat( float& value, const char* const key ) const = 0;

    bool getFloat ( float&  value, const std::string& key ) const{
      return getFloat(value,key.c_str());
    }

    /**
     ** Insert a boolean tag element. The "key" argument is used as
     ** the key for identifying the element. The function value indicates
     ** whether the insertion was successful. A failure indicates that
     ** the specified name is already in use for an element of another
     ** basic datatype. The "value" argument specifies the value of the 
     ** element.
     **/
    virtual bool putBool ( bool   value, const char* const key ) = 0;

    /**
     ** Insert an integer tag element. The "key" argument is used as
     ** the key for identifying the element. The function value indicates
     ** whether the insertion was successful. A failure indicates that
     ** the specified name is already in use for an element of another
     ** basic datatype. The "value" argument specifies the value of the 
     ** element.
     **/
    virtual bool putInt  ( int    value, const char* const key ) = 0;

    /**
     ** Insert a float tag element. The "key" argument is used as
     ** the key for identifying the element. The function value indicates
     ** whether the insertion was successful. A failure indicates that
     ** the specified name is already in use for an element of another
     ** basic datatype. The "value" argument specifies the value of the 
     ** element.
     **/
    virtual bool putFloat( float  value, const char* const key ) = 0;

    /**
     ** Insert a packed integer tag element. The ownership of the packer is 
     ** taken. 
     **/
    virtual bool putInt  ( int    value, const char* const key,
			   ComPackBase<int>* packer) = 0;

    /**
     ** Insert a packed float tag element. The ownership of the packer is 
     ** taken. 
     **/
    virtual bool putFloat( float  value, const char* const key,
			   ComPackBase<float>* packer) = 0;


    /**
     ** Merge the contents with another tag 
     ** @param iwin  specifies which value to use in case of name collision:
     **        true  -- use own value
     **        false -- use other value
     **/
    virtual bool merge( AbsEventTag* anotherTag, bool iwin ) = 0;

    /**
     ** Return an iterator that allows the boolean elements to be
     ** retrieved from the tag. It is the responsibility of the client
     ** to delete this iterator after use.
     **/
    virtual AbsEventTagBoolIter*  boolIter ( ) = 0;

    /**
     ** Return an iterator that allows the integer elements to be
     ** retrieved from the tag. It is the responsibility of the client
     ** to delete this iterator after use.
     **/
    virtual AbsEventTagIntIter*   intIter  ( ) = 0;

    /**
     ** Return an iterator that allows the float elements to be
     ** retrieved from the tag. It is the responsibility of the client
     ** to delete this iterator after use.
     **/
    virtual AbsEventTagFloatIter* floatIter( ) = 0;

  // needed for STL maps
  inline bool operator< (const AbsEventTag& a) const { return this<&a; }
};

#endif
