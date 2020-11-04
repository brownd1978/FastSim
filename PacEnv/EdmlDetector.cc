/* An implementation of class EdmlDetector
 */
#include "BaBar/BaBar.hh"

#include "PacEnv/EdmlDetector.hh"
#include "PacEnv/EdmlDetVolume.hh"

EdmlDetector::EdmlDetector( const std::string& theName,
                            const std::vector<EdmlDetVolume* >& theVolumes ) :
    _name   (theName),
    _volumes(theVolumes)
{ }

EdmlDetector::~EdmlDetector( )
{
    for( std::vector<EdmlDetVolume* >::iterator itr = _volumes.begin( );
        itr != _volumes.end( ); ++itr ) delete *itr;
    _volumes.resize( 0 );
}

void
EdmlDetector::volumes( std::vector<const EdmlDetVolume* >& theVolumes ) const
{
    theVolumes.resize( 0 );
    theVolumes.reserve( _volumes.size( ));
    for( std::vector<EdmlDetVolume* >::const_iterator itr = _volumes.begin( );
        itr != _volumes.end( ); ++itr ) theVolumes.push_back( *itr );
}
void
EdmlDetector::elements( std::vector<const EdmlDetElement* >& theElements ) const
{
    theElements.resize( 0 );
    for( std::vector<EdmlDetVolume* >::const_iterator vItr = _volumes.begin( );
        vItr != _volumes.end( ); ++vItr ) {

        std::vector<const EdmlDetElement* > elements;
        (*vItr)->elements( elements );

        for( std::vector<const EdmlDetElement* >::const_iterator eItr = elements.begin( );
            eItr != elements.end( ); ++eItr ) theElements.push_back( *eItr );
    }
}

void
EdmlDetector::append(const std::vector<EdmlDetVolume* >& theVolumes ) {
  _volumes.insert(_volumes.end(),theVolumes.begin(),theVolumes.end());
}

