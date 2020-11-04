/* Class: EdmlDetVolume
 *
 * A class representing a volume wihin an EDML detector
 */
#ifndef EdmlDetVolume_HH
#define EdmlDetVolume_HH

#include <string>
#include <vector>

class EdmlDetElement;

///  A volume class of an EDML configuration
/**
  * The class is a (potentially multi-layered) container for sub-detectors
  * (detector elements).
  */
class EdmlDetVolume {

    friend class EdmlParser;

private:

    /* These methods and are prohibited because of the instance-level cache
     * which can't be trivially cloned in its current implementation.
     */
    EdmlDetVolume( );
    EdmlDetVolume( const EdmlDetVolume& );
    EdmlDetVolume& operator=( const EdmlDetVolume& );

    /// Normal c-tor
    /**
      * It's available to the friends only. It will set up a context of
      * of an object.
      *
      * OWNERSHIP NOTE: the ownership IS transfered into the method (object)
      */
    EdmlDetVolume( const std::string& theName,
                   const std::vector<EdmlDetElement* >& theElements,
                   double zshift=0 );

public:

    /// The d-tor
    /**
      * NOTE: It will destroy all subdetectors (detector elements)
      *       owned by an object.
      */
    virtual ~EdmlDetVolume( );

    /// Get volume name

    inline std::string name( ) const { return _name; }

    /// Get sub-detectors' descriptions
    /**
      * OWNSHIP NOTE: the ownership is NOT returned with the result.
      */
    void elements( std::vector<const EdmlDetElement* >& theElements ) const;
    
    // get Z shift of this volume
    
    double zShift() const { return _zshift;}

private:

    // The context of the class
    //
    std::string _name;
    std::vector<EdmlDetElement* > _elements;
    double _zshift;
};

#endif // EdmlDetVolume_HH
