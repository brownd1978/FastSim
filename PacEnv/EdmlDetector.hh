/* Class: EdmlDetector
 *
 * A top-level class representing an EDML detector
 */
#ifndef EdmlDetector_HH
#define EdmlDetector_HH

#include <string>
#include <vector>

class EdmlDetVolume;
class EdmlDetElement;

///  A top-level detector class of an EDML configuration
/**
  * The class is a (potentially multi-layered) container for detector volumes.
  */
class EdmlDetector {

    friend class EdmlParser;

private:

    /* These methods and are prohibited because of the instance-level cache
     * which can't be trivially cloned in its current implementation.
     */
    EdmlDetector( );
    EdmlDetector( const EdmlDetector& );
    EdmlDetector& operator=( const EdmlDetector& );

    /// Normal c-tor
    /**
      * It's available to the friends only. It will set up a context of
      * of an object.
      *
      * OWNERSHIP NOTE: the ownership IS transfered into the method (object)
      */
    EdmlDetector( const std::string& theName,
                  const std::vector<EdmlDetVolume* >& theVolumes );
                  
    void append(const std::vector<EdmlDetVolume* >& theVolumes );
                  

public:

    /// The d-tor
    /**
      * NOTE: It will destroy all volumes  owned by an object.
      */
    virtual ~EdmlDetector( );

    /// Get detector name

    inline std::string name( ) const { return _name; }

    /// Get volumes
    /**
      * OWNSHIP NOTE: the ownership is NOT returned with the result.
      */
    void volumes( std::vector<const EdmlDetVolume* >& theVolumes ) const;

    /// Get all elements from all volumes
    /**
      * OWNSHIP NOTE: the ownership is NOT returned with the result.
      */
    void elements( std::vector<const EdmlDetElement* >& theElements ) const;

private:

    // The context of the class
    //
    std::string _name;
    std::vector<EdmlDetVolume* > _volumes;
};

#endif // EdmlDetector_HH
