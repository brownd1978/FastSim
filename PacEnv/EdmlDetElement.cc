/* An implementation of class EdmlDetElement
 */
#include "BaBar/BaBar.hh"

#include "PacEnv/EdmlDetElement.hh"

#include <sstream>

#include <assert.h>

EdmlDetElement::EdmlDetElement( const std::string& theType,
                                      const std::string& theName,
                                      const std::string& theMaterial,
                                      const std::string& theDevice,
                                      int id,
                                      float theThick,
                                      float theGap,
                                      float theOverlap ) :
    _type    (theType),
    _name    (theName),
    _material(theMaterial),
    _device  (theDevice),
    _id      (id),
    _thick   (theThick),
    _gap     (theGap),
    _over    (theOverlap)
{
    assert( !_type.empty( ));
    assert( !_name.empty( ));
}

EdmlDetElement::~EdmlDetElement( )
{ }

std::string
EdmlDetElement::toString( ) const
{
    std::ostringstream str;
    str << "type: '" << _type << "'"
        << ", name: '" << _name << "'"
        << ", material: '" << _material << "'"
        << ", device: '" << _device << "'"
        << ", id: '" << _id << "'"
        << ", thickness: '" << _thick << "'"
        << ", gap: '" << _gap << "'"
        << ", overlap: '" << _over << "'" << std::endl;
    return str.str( );
}
