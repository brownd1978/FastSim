//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppPathCommand.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppPathCommand. Command handler for the "path" command for
//	the BaBar Application Framework. Valid subcommands are:
//
//	path append  <path> <mod1> [<seq1>..]	Append module(s)/sequence(s)
//	path create  <path> [<mod1> [<seq2> ..]	Create a new path
//	path delete  <path>              	Delete a path
//	path disable <path1> [<path2> ..]	Disable path(s)
//	path enable  <path1> [<path2> ..]	Enable path(s)
//	path help				Show help for this command
//	path insert  <path> <mod1> [<seq1>..]	Insert module(s)/sequence(s)
//	path list				List paths
//	path remove  <path> <mod1> [<seq1>..]	Remove module(s)/sequence(s)
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------

#ifndef APPPATHCOMMAND_HH
#define APPPATHCOMMAND_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppSequenceCommand.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppPathCommand : public AppSequenceCommand {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AppPathCommand( const char* const theCommand, AppModule* theTarget );
  
  // Destructor
  virtual ~AppPathCommand( );
  virtual std::string description( ) const;
  virtual void show( ) const;
  virtual bool isShowable( ) const;
protected:
  
  virtual int deleteHandler ( );
  virtual int helpHandler   ( );
  
  virtual AppSequence* fetchSeq( 
				AppFramework* theFrame, 
				const char* const theName );
  virtual AppSequence* newSeq(
			      AppFramework* theFrame, 
			      const char* const theName );
  virtual APPList< AppSequence* >* fetchSeqs( AppFramework* theFrame );

private:

  // Not implemented.
  AppPathCommand( const AppPathCommand& );
  AppPathCommand& operator=( const AppPathCommand& );

};

#endif
