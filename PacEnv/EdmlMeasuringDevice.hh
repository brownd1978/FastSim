/* Class: EdmlMeasuringDevice
 *
 * A container class representing measuring devices in EDML.
 */
#ifndef EdmlMeasuringDevice_HH
#define EdmlMeasuringDevice_HH

#include "PacEnv/EdmlAnyTypeDict.hh"

#include <string>

///  A container class for a measuring device in EDML
/**
  * The contents of objects of the class has to be specialized by concrete
  * implementations. The container class is good for any types of measuring
  * devices. Its "dynamic" type is determined by the "type" data members.
  * The corresponding (for the given type) vartiables are stored through
  * the base class
  *
  * @see class EdmlAnyTypeDict
  */
class EdmlMeasuringDevice : public EdmlAnyTypeDict<std::string > {

public:

    typedef EdmlAnyTypeDict<std::string > base_type;

public:

    /// Normal c-tor
    /**
      * It will set up a "dynamic" type and an identity of an object.
      * Its actual "variables" will be stored through the base class.
      */
    EdmlMeasuringDevice( const std::string& theType,
                         const std::string& thename,
                          double sensitiveTimeWindow );

    /// Dump the object into a string

    std::string toString( ) const;

    /// Get a type of the final device.
    /**
      * A non-empty string is returned.
      */
    inline std::string type( ) const { return _type; }

    /// Get an identifier of the sub-detector
    /**
      * A non-empty string is returned.
      */
    inline std::string name( ) const { return _name; }

    inline double sensitiveTimeWindow() const { return _stwin; }
private:

    // The context of the class
    //
    std::string _type;
    std::string _name;
    double _stwin; // sensitive time window
};

#endif // EdmlMeasuringDevice_HH
