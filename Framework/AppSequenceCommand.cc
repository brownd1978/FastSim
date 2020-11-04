//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppSequenceCommand.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
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
//	sequence list				List sequence
//	sequence remove <seq> <mod1> [<seq1>..]	Remove module(s)/sequence(s)
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//      Marc Turcotte                   Merged in TK interface 5/15/98
//      Marc Turcotte                   Improved error reporting to
//                                      name already in use. 7/1/98
//      Marc Turcotte                   Added wrap around TK calls. 8/24/98
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1998              University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppSequenceCommand.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
#include <string.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppFramework.hh"
#include "Framework/AppModule.hh"
#include "Framework/AppSequence.hh"
#include "Framework/AppExecNode.hh"
#include "Framework/AppNameNode.hh"
#include "FrameUtil/AbsInterp.hh"

#include "FrameUtil/APPList.hh"
#include "FrameUtil/APPListIterator.hh"
#include "Framework/AppHierarchyReportAttributes.hh"

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppSequenceCommand::AppSequenceCommand( 
   const char* const theCommand,  AppModule* theTarget )
    : AppBasicCommand( theCommand, theTarget )
{
}

//--------------
// Destructor --
//--------------

AppSequenceCommand::~AppSequenceCommand( )
{
}


//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

int
AppSequenceCommand::handle( int argc, char* argv[] )
{
  int result = AbsInterp::ERROR;
  AppFramework* theFrame = (AppFramework*)target( );

  setArgs( argc, argv );

  if ( 2 <= argc ) {
    if ( 0 == strcmp( argv[1], "listMembers" ) ) {
      result = listMembersHandler( );
    } else {
      result = AppBasicCommand::handle(argc, argv);
    }
  }
  else {
    std::string txtmsg("incomplete arguments: ");
    for (int index=0; index < argc; index++ ) {
      txtmsg += " ";
      txtmsg += argv[index];
    }
    theFrame->setError( txtmsg.c_str() );

  }
  return result;
}

int
AppSequenceCommand::listMembersHandler( )
{
  AppFramework*  theFrame = (AppFramework*)target( );
  AppSequence*   theSeq(0);
  char* theName;
  int result = AbsInterp::ERROR;
  std::string errorMsg;
  AppHierarchyReportAttributes attrib;

  int index = firstIndex;
  theName = getArgument( index );
  if ( 0 != theName ) {
    if ( 0 !=  ( theSeq = fetchSeq( theFrame, theName ) ) ) {
      result = AbsInterp::OK;
    }
    else {
      errorMsg += theName;
      errorMsg += ": is unknown";
    }
  } else {
    errorMsg += ": missing name to command";
  }
  if (errorMsg.size() != 0) {
    theFrame->setError(errorMsg.c_str());
    return AbsInterp::ERROR;
  }

  index++;
  while ( 0 != ( theName = getArgument( index ) ) ) {
    if ( 0 == strcmp( theName, "-brief" ) ||
         0 == strcmp( theName, "-b"      ) ) {
      attrib.setBrief(true);
    }
    else if ( 0 == strcmp( theName, "-expand" ) ||
         0 == strcmp( theName, "-e"      ) ) {
      attrib.setExpand(true);
    }
    else if ( 0 == strcmp( theName, "-level" ) ||
         0 == strcmp( theName, "-l"      ) ) {
      index++;
      char* theLevel = getArgument( index );
      if (theLevel != 0) {
	attrib.setLevel( atoi(theLevel) );
      } 
      else {
	errorMsg += theName;
	errorMsg += " is mising argument ";
      }
    }
    else {
      errorMsg += theName;
      errorMsg += " ";
    }
    index++;
  }
  if (errorMsg.size() != 0) {
    errorMsg += ": unknown options";
    theFrame->setError(errorMsg.c_str());
    return AbsInterp::ERROR;
  }

  theSeq->report(&attrib, 0);

  return result;
}

int 
AppSequenceCommand::appendHandler( )
{
    AppFramework*  theFrame = (AppFramework*)target( );
    AppNameNode*   theNode = NULL;
    AppNameNode*   otherNode = NULL;
    char*          theName;
    AppExecutable* theExec;
    AppExecutable* theOther = NULL;
    AppSequence*   theSeq;
    int index  = firstIndex;
    int result = AbsInterp::OK;
    
    if ( NULL != ( theName = getArgument( index ) ) ) {
      if ( NULL != ( theSeq = fetchSeq( theFrame, theName ) ) ) {
	index++;
	std::string unknownArg;
	while( NULL != ( theName = getArgument( index ) ) ) {
	  if ( 0 == strcmp( theName, "-after" ) || 
	       0 == strcmp( theName, "-a"     ) ) {
	    index++;
	    if ( NULL != otherNode ) {
	      delete otherNode;
	    }
	    otherNode = getNameNode( &index );
	    if ( NULL != otherNode ) {
	      theOther = theFrame->execFromName( otherNode->name( ) );
	    }
	  }
	  if ( NULL != theNode) {
	    delete theNode;
	  }
	  if ( NULL != ( theNode = getNameNode( &index ) ) ) { 
	    theExec = theFrame->execFromName( theNode->name( ) );
	    if ( NULL != theExec ) {
	      if ( NULL == theOther ) {
		theSeq->append( theExec, theNode->mode( ) );
	      } else {
		theSeq->append( 
			       theExec, 
			       theNode->mode( ),
			       theOther, otherNode->mode( ) );
		// Setup the just appended node to be append after
		delete otherNode;
		otherNode = theNode;
		theNode = 0;
		theOther  = theExec;
	      }
	    } else {
	      unknownArg += theName;
	      unknownArg += " ";
	    }
	  }
	}
	if (unknownArg.size() != 0) {
	  unknownArg += ": unknown name";
	  theFrame->setError(unknownArg.c_str());
	  result = AbsInterp::ERROR;
	}
	if ( NULL != theNode ) {
	  delete theNode;
	}
        if ( NULL != otherNode ) {
	  delete otherNode;
	}
      } else {
	std::string  theSeqName(theName);
	std::string theTrailer(": unknown name");
	std::string theMessage=theSeqName+theTrailer;
	theFrame->setError( theMessage.c_str() );	   
	result = AbsInterp::ERROR;
      }
    } else {
      theFrame->setError( "no name specified" );
      result = AbsInterp::ERROR;
    }
    return result;
}

int 
AppSequenceCommand::createHandler( )
{
    AppFramework*  theFrame = (AppFramework*)target( );
    AppNameNode*   theNode;
    char*          theName;
    AppExecutable* theExec;
    AppSequence*       theSeq = NULL;
    int index  = firstIndex;
    int result = AbsInterp::OK;
    
    if ( NULL != ( theName = getArgument( index ) ) ) {
       if ( NULL != ( theSeq = newSeq( theFrame, theName ) ) ) {
	    index++;
	    std::string unknownName;
	    while( NULL != ( theNode = getNameNode( &index ) ) ) { // new happens
	        theExec = theFrame->execFromName( theNode->name( ) );
		if ( NULL != theExec ) {
		    theSeq->append( theExec, theNode->mode( ) );
		} else {
		  unknownName += theNode->name();
		  unknownName += " ";
		}
		delete theNode;
	    }
	    if (unknownName.size() != 0) {
	      unknownName += ": unknown name(s)";
	      theFrame->setError(unknownName.c_str());
	      result = AbsInterp::ERROR;
	    }
	} else {
	   std::string theSeqName(theName);
	   std::string theTrailer(": name already in use");
	   std::string theMessage=theSeqName+theTrailer;
	   theFrame->setError( theMessage.c_str() );
	   result = AbsInterp::ERROR;
	}
    } else {
	theFrame->setError( "no name specified" );
	result = AbsInterp::ERROR;
    }
    return result;
}

int 
AppSequenceCommand::deleteHandler( )
{
    AppFramework* theFrame = (AppFramework*)target( );
    char*         theName;
    AppSequence*      theSeq;
    int index  = firstIndex;
    int result = AbsInterp::OK;
    std::string unknownName;
    
    while ( NULL != ( theName = getArgument( index ) ) ) {
        if ( NULL != ( theSeq = fetchSeq( theFrame, theName ) ) ) {
	    theFrame->remove( theSeq );
	    delete theSeq; // ownership is given up by AppFramework
	    result = AbsInterp::OK;
        } else {
	  unknownName += theName;
	  unknownName += " ";
	}
	index++;
    }
    if (unknownName.size() != 0) {
      unknownName += ": unknown name(s)";
      theFrame->setError(unknownName.c_str());
      result = AbsInterp::ERROR;
    }

    return result;
}

int 
AppSequenceCommand::disableHandler( )
{
  AppFramework* theFrame = (AppFramework*)target( );
  char*         theName;
  AppSequence*      theSeq;
  int index  = firstIndex;
  int result = AbsInterp::OK;
  std::string unknownName;
  
  while ( NULL != ( theName = getArgument( index ) ) ) {

    if ( NULL != ( theSeq = fetchSeq( theFrame, theName ) ) ) {
      theSeq->setEnabled( false );
      
    } else if ( 0 == strcmp(theName, "all") ) {
      APPList< AppSequence* >* sequences;
      sequences = fetchSeqs( theFrame );
      if ( ! sequences->isEmpty( ) ) {
	AppSequence** aSeq;
	APPListIterator<AppSequence*> theIterator( *sequences );
	while ( ( aSeq = theIterator( ) ) ) {
	  (*aSeq)->setEnabled( false );
	}
      }
      
    } else {
      unknownName += theName;
      unknownName += " ";
    }
	
    index++;  
  }

  if (unknownName.size() != 0) {
    unknownName += ": unknown name";
    theFrame->setError(unknownName.c_str());
    result = AbsInterp::ERROR;
  }

  return result;
}

int 
AppSequenceCommand::enableHandler( )
{
  AppFramework* theFrame = (AppFramework*)target( );
  char*         theName;
  AppSequence*      theSeq;
  int index  = firstIndex;
  int result = AbsInterp::OK;
  std::string unknownName;

  while ( NULL != ( theName = getArgument( index ) ) ) {
    
    if ( NULL != ( theSeq = fetchSeq( theFrame, theName ) ) ) {
      theSeq->setEnabled( true );
      
    } else if ( 0 == strcmp(theName, "all") ) {
      APPList< AppSequence* >* sequences;
      sequences = fetchSeqs( theFrame );
      if ( ! sequences->isEmpty( ) ) {
	APPListIterator<AppSequence*> theIterator( *sequences );
	AppSequence** aSeq;
	while ( ( aSeq = theIterator( ) ) ) {
	  (*aSeq)->setEnabled( true );
	}	  
      }
      
    } else {
      unknownName += theName;
      unknownName += " ";
    }
    index++;
  }
  if (unknownName.size() != 0) {
    unknownName += ": unknown name";
    theFrame->setError(unknownName.c_str());
    result = AbsInterp::ERROR;
  }
  return result;
}

int 
AppSequenceCommand::helpHandler( )
{
  AppBasicCommand::helpHandler( );
  
  target()->partialReport( "  append <seq> [-a(fter) <mod/seq>] "); 
  target()->fullReport("<mod1/seq1> [<mod2/seq2> ..]");
  target()->partialReport( "                                        " ); 
  target()->fullReport("Append module(s)/sequence(s)");  
  
  target()->partialReport( "  create <seq> [<mod1/seq1> ..]         " );
  target()->fullReport("Create a new sequence");
  
  target()->partialReport( "  delete <seq1> [<seq2> ..]             " );
  target()->fullReport("Delete sequence(s)");
  
  target()->partialReport( "  disable <seq1> [<seq2> ..]            " );
  target()->fullReport("Disable sequence(s)*");
  
  target()->partialReport( "  enable <seq1> [<seq2> ..]             " );
  target()->fullReport("Enable sequence(s)*");
  
  target()->partialReport( "  help                                  " );
  target()->fullReport("Show available commands");
  
  target()->partialReport( "  insert <seq> [-b(efore) <mod/seq>] " );
  target()->fullReport("<mod1/seq1> [<mod2/seq2> ..]");
  target()->partialReport( "                                        " ); 
  target()->fullReport("Insert module(s)/sequence(s)");
  
  target()->fullReport( "  list [-b(rief)] -e(xpand) -l(evel) <level> ] " );
  target()->partialReport( "                                        " ); 
  target()->fullReport("List sequence(s)");

  target()->fullReport( "  listMembers <seq> [-b(rief)] -e(xpand) -l(evel) <level> ] " );
  target()->partialReport( "                                        " ); 
  target()->fullReport("List members of a sequence");
  
  target()->fullReport( "  remove <seq> <mod1/seq1> [<mod2/seq2> ..] " );
  target()->partialReport( "                                        " ); 
  target()->fullReport("Remove module(s)/sequence(s)");
  
  target()->fullReport(" ");
  target()->fullReport("* = 'all' can be used in place of <seq1>.");

  return AbsInterp::OK;
  
}

int 
AppSequenceCommand::insertHandler( )
{
  AppFramework*  theFrame = (AppFramework*)target( );
  AppNameNode*   theNode=0;
  AppNameNode*   otherNode =0;
  char*          theName=0;
  AppExecutable* theExec=0;
  AppExecutable* theOther =0;
  AppSequence*   theSeq=0;
  AppExecNode*   first=0;
  int index  = firstIndex;
  int result = AbsInterp::OK;
  
  if ( 0 != ( theName = getArgument( index ) ) ) {
    if ( 0 != ( theSeq = fetchSeq( theFrame, theName ) ) ) {
      index++;
      std::string unknownName;
      while( 0 != ( theName = getArgument( index ) ) ) {
	if ( 0 == strcmp( theName, "-before" ) || 
	     0 == strcmp( theName, "-b"      ) ) {
	  index++;
	  if ( 0 != otherNode ) {
	    delete otherNode;
	  }
	  otherNode = getNameNode( &index );
	  if ( 0 != otherNode ) {
	    theOther = theFrame->execFromName( otherNode->name( ) );
	  }
	}
	if ( 0 != ( theNode = getNameNode( &index ) ) ) {
	  theExec = theFrame->execFromName( theNode->name( ) );
	  if ( 0 != theExec ) {
	    first = theSeq->head( );
	    if ( 0 == theOther ) {
	      if (0 != first) { 
		theOther  =first->target( );
		otherNode = new AppNameNode (
					     theOther->name( ),
					     first->mode( ) );
	      }
	    }
	    if (0 != first) {
	      theSeq->insert( 
			     theExec, 
			     theNode->mode( ),
			     theOther, otherNode->mode( ) );
	    }
	    else {
	      theSeq->insert(theExec, theNode->mode());
	    }
	  } else {
	    unknownName += theName;
	    unknownName += " ";
	  }
	}
	delete theNode;
      }
      if (unknownName.size() != 0) {
	unknownName += ": unknown name";
	theFrame->setError(unknownName.c_str());
	result = AbsInterp::ERROR;
      }

      if ( 0 != otherNode ) {
	delete otherNode;
      }
    } else {
      std::string theSeqName(theName);
      std::string theTrailer(": unknown name");
      std::string theMessage=theSeqName+theTrailer;
      theFrame->setError( theMessage.c_str() );		
      result = AbsInterp::ERROR;
    }
  } else {
    theFrame->setError( "no name specified" );
    result = AbsInterp::ERROR;
  }
  return result;
}

int 
AppSequenceCommand::listHandler( )
{

  AppFramework* theFrame = (AppFramework*)target( );
  AppHierarchyReportAttributes attrib;
  char* theName = 0;
  int index = firstIndex;
  std::string unknownName;
  while ( 0 != ( theName = getArgument( index ) ) ) {
    if ( 0 == strcmp( theName, "-brief" ) ||
         0 == strcmp( theName, "-b"      ) ) {
      attrib.setBrief(true);
    }
    else if ( 0 == strcmp( theName, "-expand" ) ||
         0 == strcmp( theName, "-e"      ) ) {
      attrib.setExpand(true);
    }
    else if ( 0 == strcmp( theName, "-level" ) ||
         0 == strcmp( theName, "-l"      ) ) {
      index++;
      char* theLevel = getArgument( index );
      if (theLevel != 0) {
	attrib.setLevel( atoi(theLevel) );
      } 
      else {
	unknownName += theName;
	unknownName += " is mising argument ";
      }
    }
    else {
      unknownName += theName;
      unknownName += " ";
    }
    index++;
  }
  
  if (unknownName.size() != 0) {
    unknownName += ": unknown options";
    theFrame->setError(unknownName.c_str());
    return AbsInterp::ERROR;
  }

  AppSequence**  seq;
  APPList< AppSequence* >* sequences;
  
  sequences = fetchSeqs( theFrame );
  if ( ! sequences->isEmpty( ) ) {
    APPListIterator<AppSequence*> theIterator( *sequences );
    while ( ( seq = theIterator( ) ) ) {
      (*seq)->report(&attrib, 0);
    }
  } else {
    target()->fullReport( "     None defined" );
  }
  return AbsInterp::OK;
}

int 
AppSequenceCommand::removeHandler( )
{
    AppFramework*  theFrame = (AppFramework*)target( );
    AppNameNode*   theNode;
    char*          theName;
    AppExecutable* theExec;
    AppSequence*   theSeq;
    int index  = firstIndex;
    int result = AbsInterp::OK;
    std::string unknownName;

    if ( NULL != ( theName = getArgument( index ) ) ) {
        if ( NULL != ( theSeq = fetchSeq( theFrame, theName ) ) ) {
	   index++;
	   while( NULL != ( theNode = getNameNode( &index ) ) ) {
	      theExec = theFrame->execFromName( theNode->name( ) );
	      if ( NULL != theExec ) {
		 theSeq->remove( theExec, theNode->mode( ) );
	      } else {
		unknownName += theNode->name();
		unknownName += " ";
	      }
	      delete theNode;
	   }
	   if (unknownName.size() != 0) {
	     unknownName += ": unknown name";
	     theFrame->setError(unknownName.c_str());
	     result = AbsInterp::ERROR;
	   }
	} else {
	   std::string theSeqName(theName);
	   std::string theTrailer(": unknown");
	   std::string theMessage=theSeqName+theTrailer;
	   theFrame->setError( theMessage.c_str() );		
	   result = AbsInterp::ERROR;
	}
    } else {
	theFrame->setError( "no name specified" );
	result = AbsInterp::ERROR;
    }
    return result;
}

AppSequence* 
AppSequenceCommand::fetchSeq( 
    AppFramework* theFrame, 
    const char* const theName )
{
    return theFrame->fetchSequence( theName );
}

AppSequence* 
AppSequenceCommand::newSeq( 
    AppFramework* theFrame, 
    const char* const theName )
{
    AppSequence* result = NULL;
    if ( ! theFrame->has( theName ) ) {
		result = new AppSequence( theName, NULL );
		theFrame->add( result );
    }
    return result;
}

APPList< AppSequence* >* 
AppSequenceCommand::fetchSeqs( AppFramework* theFrame )
{
    return theFrame->sequences( );
}

std::string 
AppSequenceCommand::description( ) const
{
   return "Command used to create, modify and control sequences";
}

void
AppSequenceCommand::show( ) const 
{
}
bool
AppSequenceCommand::isShowable( ) const 
{
   return false; 
}
