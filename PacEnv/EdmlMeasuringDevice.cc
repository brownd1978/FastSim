/* An implementation of class EdmlMeasuringDevice
 */
#include "BaBar/BaBar.hh"

#include "PacEnv/EdmlMeasuringDevice.hh"

#include <sstream>
#include <vector>

#include <assert.h>

EdmlMeasuringDevice::EdmlMeasuringDevice( const std::string& theType,
                                          const std::string& thename,
                                          double sensitiveTimeWindow ) :
    EdmlAnyTypeDict<std::string >( ),
    _type(theType),
    _name  (thename),
    _stwin(sensitiveTimeWindow)
{
    assert( !_type.empty( ));
    assert( !_name.empty( ));
}

std::string
EdmlMeasuringDevice::toString( ) const
{
    std::ostringstream str;

    // Dump members common to all types of measuring devices
    //
    str << "type: '" << _type << "'"
        << ", name: '" << _name << "'";

    // Proceed with members stored in the base dictionary
    //
    std::vector<base_type::key_type > keys;
    this->keys( keys );
    for( size_t i = 0; i < keys.size( ); ++i ) {
        base_type::key_type k = keys[i];
        str << ", " << k << ": '" << this->val2str( k ) << "'";
    }
    return str.str( );
}
