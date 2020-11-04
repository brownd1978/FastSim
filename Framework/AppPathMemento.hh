//--------------------------------------------------------------------------
// file and version information:
//      $Id: AppPathMemento.hh 509 2010-01-14 15:18:55Z stroili $
//
// description:
//	class AppPathMemento.
//      do not use this for template class (foo<t>).  use templatetemplate.hh
//      instead.
//
// environment:
//	software developed for the babar detector at the slac b-factory.
//
// author list:
//	akbar mokhtarani		originator
//	<author2>		<originator/contributor etc.>
//
// copyright information:
//	copyright (c) 1998	lbnl
//
//------------------------------------------------------------------------

#ifndef APPPATHMEMENTO_HH
#define APPPATHMEMENTO_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppSequenceMemento.hh"
#include "Framework/AppExecutable.hh"
//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
template < class T > class APPList;

//		---------------------
// 		-- Class Interface --
//		---------------------

class AppPathMemento : public AppSequenceMemento  {

public:
  
  // Constructors
  AppPathMemento(AppExecutable *theExec);

  // Destructor
  virtual ~AppPathMemento( );  
  
private:

  // Not implemented.
  AppPathMemento( const AppPathMemento& );
  AppPathMemento& operator=( const AppPathMemento& );
  
};

#endif // APPPATHMEMENTO_HH



