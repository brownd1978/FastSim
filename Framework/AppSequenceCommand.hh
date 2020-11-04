//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppSequenceCommand.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppSequenceCommand. Sequence command Class. 
//
// Environment:
//	Class AppSequenceCommand. Command handler for the "sequence" command 
//	for the BaBar Application Framework. Valid subcommands are:
//
//	sequence append <seq> <mod1> [<seq1>..]	Append module(s)/sequence(s)
//	sequence create <seq> <mod1> [<seq2>..]	Create a new sequence
//	sequence delete  <seq>              	Delete a sequence
//	sequence disable <seq1> [<path2> ..]	Disable sequence(s)
//	sequence enable  <seq1> [<path2> ..]	Enable sequence(s)
//	sequence help				Show help for this command
//	sequence insert <seq> <mod1> [<seq1>..]	Insert module(s)/sequence(s)
//	sequence list				List current sequence(s)
//	sequence remove <seq> <mod1> [<seq1>..]	Remove module(s)/sequence(s)
//
// Author List:
//	David R. Quarrie		Original Author
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------

#ifndef APPSEQUENCECOMMAND_HH
#define APPSEQUENCECOMMAND_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppBasicCommand.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class AppFramework;
class AppSequence;
template<class Item> class APPList;


//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppSequenceCommand : public AppBasicCommand {

  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppSequenceCommand( const char* const theCommand, AppModule* theTarget );
  
  // Destructor
  virtual ~AppSequenceCommand( );
  virtual std::string description( ) const;
  virtual void show( ) const;    
  virtual bool isShowable( ) const;
  virtual int handle( int argc, char* argv[] );

protected:
  
  virtual int appendHandler ( );
  virtual int createHandler ( );
  virtual int deleteHandler ( );
  virtual int disableHandler( );
  virtual int enableHandler ( );
  virtual int helpHandler   ( );
  virtual int insertHandler ( );
  virtual int listHandler   ( );
  virtual int removeHandler ( );
  virtual int listMembersHandler ( );
  
  virtual AppSequence* fetchSeq( 
				AppFramework* theFrame, 
				const char* const theName );
  virtual AppSequence* newSeq(
			      AppFramework* theFrame, 
			      const char* const theName );
  virtual APPList< AppSequence* >* fetchSeqs( AppFramework* theFrame );

private:

  // Not implemented.
  AppSequenceCommand( const AppSequenceCommand& );
  AppSequenceCommand& operator=( const AppSequenceCommand& );

};

#endif
