/* Class: EdmlRingDetElement
*
  * A class representing ring sub-detector elements in EDML
 */
#ifndef EdmlRingDetElement_HH
#define EdmlRingDetElement_HH

#include "PacEnv/EdmlDetElement.hh"

#include <string>

  class EdmlMeasuringDevice;

///  A class representing ring sub-detector elements in EDML
/**
* This is the final class implementing the following type of
  * subdetectors:
*
  *   'CylDetector'
  */
class EdmlRingDetElement : public EdmlDetElement {

  friend class EdmlParser;

private:

    /* These methods and are prohibited because of the instance-level cache
  * which can't be trivially cloned in its current implementation.
     */
    EdmlRingDetElement( );
  EdmlRingDetElement( const EdmlRingDetElement& );
  EdmlRingDetElement& operator=( const EdmlRingDetElement& );

public:

    /// Normal c-tor
    /**
  * OWNERSHIP NOTE: the ownership is NOT transfered into the method (object)
      */
    EdmlRingDetElement( const std::string& theType,
    const std::string& theName,
    const std::string& theMaterial,
    const std::string& theDevice,
    const int          theId,
    const float        theThick,
    const float        thegap,
    const float        theoverlap,
    const float        theZ,
    const float        theLowRadius,
    const float        theHiRadius);

    /// The d-tor

  virtual ~EdmlRingDetElement( );

    /// Dump the object into a string
    /**
  * Implement the corresponding method of the base class.
      */
    virtual std::string toString( ) const;

    // Accessors (also see accessors in the base class)

  inline float z        ( ) const { return _z; }
  inline float lowradius( ) const { return _lowradius; }
  inline float hiradius ( ) const { return _hiradius; }

private:

    // The context of the class
    //
  float _z;
  float _lowradius;
  float _hiradius;
};

#endif // EdmlRingDetElement_HH
