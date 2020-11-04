#ifndef EDML_ANY_TYPE_DICT_HH
#define EDML_ANY_TYPE_DICT_HH

// File and Version Information:
//      $Id: EdmlAnyTypeDict.hh,v 1.4 2007/06/12 18:59:40 kelsey Exp $

#include <map>
#include <vector>
#include <sstream>

// The internal machinery of the dictionary

/// The default trait for printing values stored in the dictionary
/**
  * The class will be used in an implementation of the following
  * method:
  *
  *   EdmlAnyTypeDictImpl::HolderBase::toString()
  *
  * The default trait will work for all types for which the following
  * operator is available when compiling the relevant code:
  *
  *   std::ostream& operator<<(std::ostream&, const &T) const
  *
  * For other types one may choose to defined define a specialization
  * of the trait. For example, if we had type 'MyType' which had
  * the following method translating and object into a string:
  *
  *   class MyType {
  *   public:
  *       std::string str() const;
  *       ..
  *   };
  *
  * then the specialized trait class would look like this:
  *
  *   template<>
  *   struct EdmlAnyTypePrinter<MyType > {
  *       static std::string toString( const T& value )
  *       {
  *           return value.str();
  *       }
  *   };
  */
template< typename T >
struct EdmlAnyTypePrinter {

    static std::string toString( const T& value )
    {
        std::ostringstream s;
        s << value;
        return s.str( );
    }
};

/// Specialized trait for printing vectors (collections?)
/**
  * Elements of an input collection will be separated with a singe
  * whitespace character.
  *
  * TODO: Replace std::vector with a more generic sequence base to
  *       allow any types for which the forward iterator is available.
  */
template< typename ET >
struct EdmlAnyTypePrinter<std::vector<ET > > {

    static std::string toString( const std::vector<ET >& value )
    {
        std::ostringstream s;
        bool first = true;
        for( typename std::vector<ET >::const_iterator itr = value.begin( );
            itr != value.end( );
            ++itr ) {

            // No separator in front of the very first
            // element. Allow it before next elements (if any).
            //
            if( first ) first = false;
            else        s << ' ';

            // Printing collection's elements through the elements'
            // trait would allow using non-trivial printing for the elements
            // as well.
            //
            s << EdmlAnyTypePrinter<ET >::toString( *itr );
        }
        return s.str( );
    }
};

namespace EdmlAnyTypeDictImpl {
    struct HolderBase {
        HolderBase( int theTypeId ) : typeId(theTypeId) { }
        virtual ~HolderBase( ) { }
        virtual HolderBase* clone( ) const = 0;
        virtual std::string toString( ) const = 0;
        int typeId;
    };

    template< typename T >
    struct Holder : public HolderBase {
    public:
        Holder( T* thePtr );
        Holder( const Holder<T>& other ) :
	    HolderBase( other ), ptr(new T( *other.ptr )) { }
        Holder<T>& operator=( const Holder<T>& other );
        virtual ~Holder( ) { delete ptr; }
        virtual HolderBase* clone( ) const { return new Holder( *this ); }
        virtual std::string toString( ) const
        {
            return EdmlAnyTypePrinter<T >::toString( *ptr );
        }
        T* ptr;
    private:
        Holder( );
    };
}

/// A dictionary class for objects of any type
/**
  * The class can be used to store heterogenious collections of objects.
  * The only template parameter of the class is:
  *
  *   K - a type of the dictionary's keys
  *
  * Objects stored in the dictionary can be of any types as long as these types
  * support the value semantics (meet the following minimal interface):
  *
  *   class T {
  *   public:
  *     T();
  *     T(const T&);
  *     ~T();
  *     T& operator=(const T&);
  *   };
  *
  * All keys in the dictionary are unique. A type of each object is also remembered (in some form)
  * by the dictionary, therefore next time, when a user will be making a query to find an object
  * for a certain key then the stored object's type will get compared with the one expected by
  * the users, and they should match. Otherwise a error code will get returned.
  *
  * Here is a simple example how to use the dictionary:
  *
  *   EdmlAnyTypeDict<std::string> dict;
  *
  *   if( !dict.exists( "PI" )) {
  *     const double pi = 3.14;
  *     if( !dict.insert( "PI", pi )) {
  *       cerr << "failed to insert 'PI' into the dictionary\n";
  *       ...
  *     }
  *   }
  *   if( !dict.replace( "PI", 3.14195, false )) {
  *     cerr << "failed to replice 'PI' in the dictionary with a more precise value\n";
  *     ...
  *   }
  */
template< class K >
class EdmlAnyTypeDict {

public:

  /// The type of keys

    typedef K key_type;

private:

  /// The dictionary type for storing kay-value pairs

    typedef std::map<K,EdmlAnyTypeDictImpl::HolderBase*> HolderBaseMap;

public:

  /// The default constructor

    EdmlAnyTypeDict( ) { }

  /// The copy constructor

  EdmlAnyTypeDict( const EdmlAnyTypeDict<K>& other );

  /// The destructor

  virtual ~EdmlAnyTypeDict( );

  /// The assignment operator

  EdmlAnyTypeDict<K>& operator=( const EdmlAnyTypeDict<K>& other );

  /// Clear the dictionary

  void clear();

  /// Check if the dictionary is empty (has no keys registered)

  bool empty( ) const { return _data.empty( ); }

  /// Check if the specified key exists in the dictionary

  bool exists( const K& theKey ) const;

  /// Insert the new object into the dictionary
  /**
    * The method is meant to make a copy of the input object and register it in the dictionary
    * with the specified key. A type of the input object will also be remembered, so that next
    * time when a user will be making a query for that key then the stored type will get compared
    * with the one expected by the user.
    *
    * The method will return 'false' if the specified key is already known to the dictionary.
    */
    template< typename V >
    bool insert( const K& theKey,
                 const V& theValue
               );

  /// Replace an object in the dictionary
  /**
    * The method will replace an existing object in the dictionary for the specified key.
    * Depending on a value of the optional "forceCreateFlag" the key may or may not be allowed
    * to be missing in the dictionary prior to calling the method. If the keys is not known
    * and the flag is set to "false" then 'false' will be returned. Otherwise a new entry will
    * be created.
    *
    * If the key is already known to the dictionary then the new object's type should match
    * the one stored before. Otherwise 'false' will be returned.
    */
    template< typename V >
    bool replace( const K&   theKey,
                  const V&   theValue,
                  const bool forceCreateFlag = true );

  /// Find an object in the dictionary
  /**
    * The method will try to find an existing object in the dictionary for the specified key.
    * If the key is not found then 'false' will be returned. If the key is already known to
    * the dictionary then the expected object's type should match the one stored before.
    * Otherwise 'false' will be returned.
    */
    template< typename V >
    bool find( const K& theKey,
               V&       theValue ) const;

  /// Find and return (if successfull) a value for the specified key
  /**
    * Note that C++ can not derive which template instantiation to use for
    * the method based on a the return value's type. Therefore one should
    * specify the type explicitly, for example, like this:
    *
    *   EdmlAnyTypeDict<std::string> dict;
    *   float f = dict.get<float>( "MyKey" );
    *
    * ATTENTION: This method doesn't have a mechanism for returning a status
    *            of the operation. Therefore it will abort an application if
    *            either the key doesn't exist or if it's type doesn't match
    *            the one expected by the template parameter.
    */
    template< typename V >
    V get( const K& theKey ) const;

    /// Return a string representation of an object stored for the key
    /**
      * If the object doesn't exist then an empty string will be
      * returned. Otherwise an objects streamed into a string using
      * the Standard String Streamer will be returned.
      */
    std::string val2str( const K& theKey ) const;

  /// Get a vector of keys registered in the dictionary
  /**
    * The method would initialize and fill the specified vector with known keys.
    * Note that the keys will not be sorted.
    */
  void keys( std::vector<K>& theVectorOfKeys ) const;

private:
  
  // The storage for keys and objects
  
  HolderBaseMap _data;
};

#ifdef BABAR_COMP_INST
#include "PacEnv/EdmlAnyTypeDict.icc"
#endif

#endif  // EDML_ANY_TYPE_DICT_HH
