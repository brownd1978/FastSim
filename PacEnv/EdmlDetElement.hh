/* Class: EdmlDetElement
 *
 * A base class representing subdetector elements of an EDML detector.
 */
#ifndef EdmlDetElement_HH
#define EdmlDetElement_HH

#include <string>

///  A base class for an EDML sub-detector element
/**
  * The class has to be specialized by concrete implementations.
  */
class EdmlDetElement {

private:

    /* These methods and are prohibited because of the instance-level cache
     * which can't be trivially cloned in its current implementation.
     */
    EdmlDetElement( );
    EdmlDetElement( const EdmlDetElement& );
    EdmlDetElement& operator=( const EdmlDetElement& );

protected:

    /// Normal c-tor
    /**
      * It's available to the subclasses only. It will set up a context of
      * of an object.
      *
      * OWNERSHIP NOTE: the ownership is NOT transfered into the method (object)
      */
    EdmlDetElement( const std::string& theType,
                       const std::string& theName,
                       const std::string& theMaterial,
                       const std::string& theDevice,
                       int id,
                       float theThick,
                       float theGap,
                       float theOverlap );

public:

    /// The d-tor

    virtual ~EdmlDetElement( );

    /// Dump the object into a string
    /**
      * The current implementation of the method will only print the data members
      * from this level. Subclasses will reimplement the method.
      */
    virtual std::string toString( ) const;

    /// Get a type of the final sub-detector
    /**
      * A non-empty string is returned. 
      *
      * Note, that a value returned by this method is NOT a type of a subclass!
      * It's a logical name for the corresponding sub-detector modelled
      *  by the subclass. This separation is made to decouple a logical
      * structure of EDML configurations from the names of C++ classes
      * which may change. The following mapping from sub-detector types
      * to the relevant C++ classes exists:
      *
      *   type                     final class name
      *   -----------------------  ----------------------------
      *   'CylDetector'            'EdmlCylDetElement'
      *   'RingDetector'            'EdmlPlaneDetElement'
      *   'ConeDetector'            'EdmlConeDetElement'
      */
    inline const std::string& type( ) const { return _type; }

    /// Get a name of the sub-detector
    /**
      * A non-empty string is returned.
      */
    inline const std::string& name( ) const { return _name; }

    /// Get a name of the subdetector material (if any)
    /**
      * The method returns a name of a material from which the sub-detector
      * is made. If no material is associated with the sub-detector then
      * an empty string is returned.
      */
    inline const std::string& material( ) const { return _material; }

    /// Get the measuring device (if any) attached to the sub-detector
    /**
      * The method returns a name of a measuring device associated with
      * the sub-detector. If there is no such device then an empty string
      * is returned.
      */
    inline const std::string& device( ) const { return _device; }
    
    inline int id() const { return _id;}
    inline float thick    ( ) const { return _thick; }
    inline float gap      ( ) const { return _gap; }
    inline float overlap  ( ) const { return _over; }
private:

    // The context of the class
    //
    std::string _type;
    std::string _name;
    std::string _material;
    std::string _device;
    int _id;
    float _thick;
    float _gap;
    float _over;
};

#endif // EdmlDetElement_HH
