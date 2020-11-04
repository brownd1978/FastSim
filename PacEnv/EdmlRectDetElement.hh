/* Class: EdmlRectDetElement
*
  * A class representing ring sub-detector elements in EDML
 */
#ifndef EdmlRectDetElement_HH
#define EdmlRectDetElement_HH

#include "PacEnv/EdmlDetElement.hh"

#include <string>

class EdmlMeasuringDevice;

///  A class representing rectangular plane sub-detector elements in EDML
/**
* This is the final class implementing the following type of
  * subdetectors:
*
  *   'CylDetector'
  */
class EdmlRectDetElement : public EdmlDetElement {

  friend class EdmlParser;

private:

    /* These methods and are prohibited because of the instance-level cache
  * which can't be trivially cloned in its current implementation.
     */
    EdmlRectDetElement( );
  EdmlRectDetElement( const EdmlRectDetElement& );
  EdmlRectDetElement& operator=( const EdmlRectDetElement& );

public:
  enum porient {zparallel=0,zperp}; //
    /// Normal c-tor
    /**
  * OWNERSHIP NOTE: the ownership is NOT transfered into the method (object)
      */
    EdmlRectDetElement( const std::string& theType,
    const std::string& theName,
    const std::string& theMaterial,
    const std::string& theDevice,
    const int          theId,
    const float        theThick,
    const float        thegap,
    const float        theoverlap,
    const porient      theOrientation,
    const float        theRho,
    const float        theZ,
    const float        thePhi,
    const float        theEta,
    const float        theUSize,
    const float        theVSize);

    /// The d-tor

  virtual ~EdmlRectDetElement( );

    /// Dump the object into a string
    /**
  * Implement the corresponding method of the base class.
      */
    virtual std::string toString( ) const;

    // Accessors (also see accessors in the base class)
  inline porient orientation( ) const { return _orient; }
  inline float centerRho ( ) const { return _rho; }
  inline float centerZ ( ) const { return _z; }
  inline float centerPhi ( ) const { return _phi; }
  inline float axisPhi ( ) const { return _eta; }
  inline float uSize( ) const { return _usize; }
  inline float vSize( ) const { return _vsize; }

private:

    // The context of the class
    //
  porient _orient; // plane orientation
  float _rho;  // transverse distance from the Z axis to the rectangle center
  float _z;  // position along the Z axis of the rectangle center
  float _phi;  // angle between ray from Z axis to rect center and the x axis
  float _eta; // for parallel, angle between u and x directions; for perp, angle between w and ray from Z to rect center
  float _usize; // size of rectangle in 'u' direction
  float _vsize; // size of rectangle in 'v' direction
};

#endif // EdmlRectDetElement_HH
