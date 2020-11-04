/* Class: EdmlCylDetElement
*
  * A class representing cylindric sub-detector elements in EDML
 */
#ifndef EdmlCylDetElement_HH
#define EdmlCylDetElement_HH

#include "PacEnv/EdmlDetElement.hh"

#include <string>

  class EdmlMeasuringDevice;

///  A class representing cylindric sub-detector elements in EDML
/**
* This is the final class implementing the following type of
  * subdetectors:
*
  *   'CylDetector'
  */
class EdmlCylDetElement : public EdmlDetElement {

  friend class EdmlParser;

private:

    /* These methods and are prohibited because of the instance-level cache
  * which can't be trivially cloned in its current implementation.
     */
    EdmlCylDetElement( );
  EdmlCylDetElement( const EdmlCylDetElement& );
  EdmlCylDetElement& operator=( const EdmlCylDetElement& );

public:

    /// Normal c-tor
    /**
  * OWNERSHIP NOTE: the ownership is NOT transfered into the method (object)
      */
    EdmlCylDetElement( const std::string& theType,
    const std::string& theName,
    const std::string& theMaterial,
    const std::string& theDevice,
    int                theId,
    const float        theThick,
    const float        theGap,
    const float        theOverlap,
    const float        theZmin,
    const float        theZmax,
    const float        theRadius);

    /// The d-tor

  virtual ~EdmlCylDetElement( );

    /// Dump the object into a string
    /**
  * Implement the corresponding method of the base class.
      */
    virtual std::string toString( ) const;

    // Accessors (also see accessors in the base class)

  inline float zmin   ( ) const { return _zmin;    }
  inline float zmax   ( ) const { return _zmax;    }
  inline float radius ( ) const { return _radius;  }

private:

    // The context of the class
    //
  float _zmin;
  float _zmax;
  float _radius;
};

#endif // EdmlCylDetElement_HH
