//--------------------------------------------------------------------------
// file and version information:
//      $Id: AppSequenceMemento.hh 509 2010-01-14 15:18:55Z stroili $
//
// description:
//	class AppSequenceMemento.
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

#ifndef APPSEQUENCEMEMENTO_HH
#define APPSEQUENCEMEMENTO_HH

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
#include "Framework/AbsModuleMemento.hh"
#include "Framework/AppExecutable.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
template < class T > class APPList;
#include <iosfwd>

//		---------------------
// 		-- Class Interface --
//		---------------------

class AppSequenceMemento : public AbsModuleMemento
{

  //--------------------
  // Declarations     --
  //--------------------
  
  // Typedefs, consts, and enums
  
  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppSequenceMemento(AppExecutable*);
  
  // Destructor
  virtual ~AppSequenceMemento( );
  
  // Modifiers
    
  virtual  void Dump( std::ostream&) const;
  virtual  void Dump_tcl( std::ostream&) const;
  
private:

  // Not implemented.
  AppSequenceMemento( const AppSequenceMemento& );
  AppSequenceMemento& operator=( const AppSequenceMemento& );

protected:
    
  APPList<AppExecutable*>* _theExecList;

};

#endif // APPSEQUENCEMEMENTO_HH





