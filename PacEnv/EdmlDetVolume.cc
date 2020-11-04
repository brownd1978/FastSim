/* An implementation of class EdmlDetVolume
 */
#include "BaBar/BaBar.hh"

#include "PacEnv/EdmlDetVolume.hh"
#include "PacEnv/EdmlDetElement.hh"

EdmlDetVolume::EdmlDetVolume( const std::string& theName,
                              const std::vector<EdmlDetElement* >& theElements,
                              double zshift ) :
    _name    (theName),
    _elements(theElements),
    _zshift  (zshift)
{ }

EdmlDetVolume::~EdmlDetVolume( )
{
    for( std::vector<EdmlDetElement* >::iterator itr = _elements.begin( );
        itr != _elements.end( ); ++itr ) delete *itr;
    _elements.resize( 0 );
}

void
EdmlDetVolume::elements( std::vector<const EdmlDetElement* >& theElements ) const
{
    theElements.resize( 0 );
    theElements.reserve( _elements.size( ));
    for( std::vector<EdmlDetElement* >::const_iterator itr = _elements.begin( );
        itr != _elements.end( ); ++itr ) theElements.push_back( *itr );
}

