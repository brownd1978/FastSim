/* Class: EdmlConeDetElement
*
  * A class representing Coneindric sub-detector elements in EDML
 */
#ifndef EdmlConeDetElement_HH
#define EdmlConeDetElement_HH

#include "PacEnv/EdmlDetElement.hh"

#include <string>

  class EdmlMeasuringDevice;

///  A class representing Coneindric sub-detector elements in EDML
/**
* This is the final class implementing the following type of
  * subdetectors:
*
  *   'ConeDetector'
  */
class EdmlConeDetElement : public EdmlDetElement {

  friend class EdmlParser;

private:

    /* These methods and are prohibited because of the instance-level cache
  * which can't be trivially cloned in its current implementation.
     */
    EdmlConeDetElement( );
  EdmlConeDetElement( const EdmlConeDetElement& );
  EdmlConeDetElement& operator=( const EdmlConeDetElement& );

public:

    /// Normal c-tor
    /**
  * OWNERSHIP NOTE: the ownership is NOT transfered into the method (object)
      */
    EdmlConeDetElement( const std::string& theType,
    const std::string& theName,
    const std::string& theMaterial,
    const std::string& theDevice,
    int                theId,
    const float        theThick,
    const float        theGap,
    const float        theOverlap,
    const float        theRmin,
    const float        theRmax,
    const float        theTanTheta,
    const float        theZVert);
    
  // alternate constructor, between cylindrical points (rho1,Z1) and (rho2,Z2)
    
  EdmlConeDetElement( const std::string& theType,
  const std::string& theName,
  const std::string& theMaterial,
  const std::string& theDevice,
  int                theId,
  const float        theThick,
  const float        theGap,
  const float        theOverlap,
  const std::pair<float,float>& cylpt1,
  const std::pair<float,float>& cylpt2);

    /// The d-tor

  virtual ~EdmlConeDetElement( );

    /// Dump the object into a string
    /**
  * Implement the corresponding method of the base class.
      */
    virtual std::string toString( ) const;

    // Accessors (also see accessors in the base class)

  inline float rmin   ( ) const { return _rmin;    }
  inline float rmax   ( ) const { return _rmax;    }
  inline float tanth  ( ) const { return _tanth;  }
  inline float zvert  ( ) const { return _zvert;  }

private:

    // The context of the class
    //
  float _rmin;
  float _rmax;
  float _tanth;
  float _zvert;
};

#endif // EdmlConeDetElement_HH
