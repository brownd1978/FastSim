//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppSequence.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppSequence. This class describes sequences within the BaBar
//	Application Framework.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//      Marc Turcotte                   Introduced Actions       May '97
//      Marc Turcotte                   Removed dependence on RW
//                                      to allow sharing         June '97
//      Marc Turcotte                   Modified for new style begin/end 2/98
//      Marc Turcotte                   Added support for new style event.
//      Marc Turcotte                   Added execute member function to
//                                      support dynamic dispatch  3/98
//      Marc Turcotte                   Introduced AppStopType 3/20/98
//      Marc Turcotte                   Renamed execute to frame 3/26/98
//      Marc Turcotte                   Let null frame pointer thru 4/8/98
//      Marc Turcotte                   Introduced ActionControllers 4/20/98
//      Marc Turcotte                   Merged in R.Kapur's TK interface 5/15
//      Marc Turcotte                   Modified for AppStatus 6/20/98
//      Marc Turcotte                   Added input/output Frame 6/22/98
//      Marc Turcotte                   Added theDispatchStopType to
//                                      signatures 6/23/98
//      Marc Turcotte                   Changed AppStatus for AppResult 6/23/98
//      Marc Turcotte                   Modified for new sigs 6/24/98
//      Marc Turcotte                   Introduced non const heap temporary
//                                      sequenceName copied into
//                                      for safety reasons. 7/30/98
//      Rainer Bartoldus                Implemented use of before/afterOther()
//                                      actions in other() method. 7/18/2001
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1997, 1998        University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppSequence.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
#include <stddef.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppFramework.hh"
#include "Framework/AppExecNode.hh"
#include "Framework/AppNameNode.hh"
#include "Framework/AppStopType.hh"
class AppFrame;
#include "Framework/AppActionController.hh"
#include "Framework/AppModule.hh"
#include "Framework/AppReportAttributes.hh"
#include "Framework/AppHierarchyReportAttributes.hh"

#include "FrameUtil/APPList.hh"
#include "FrameUtil/APPListIterator.hh"

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppSequence::AppSequence( 
    const char* const theName, 
    const char* const theDescription )
    : AppExecutable( theName, theDescription )
{
    _execType  = APP_sequence;
    _nodes      = new APPList< AppExecNode* >;
}

//--------------
// Destructor --
//--------------

AppSequence::~AppSequence( )
{
    delete _nodes;
}
    
//--------------
// Operations --
//--------------

AppResult
AppSequence::beginJob( AbsEvent* aRecord ) {
   AppExecNode** node;

   APPListIterator<AppExecNode*> theIterator( *_nodes );
   while ( ( node = theIterator( ) ) ) 
   {
      (*node)->target( )->beginJob( aRecord );
      
   }
   return AppResult::OK;
}

AppResult
AppSequence::beginRun( AbsEvent* anEvent ) {

   AppExecNode** node;
   AppExecutable* exec;    
    
   assert( NULL != anEvent );
    
   APPListIterator<AppExecNode*> theIterator( *_nodes );
   while ( ( node = theIterator( ) ) ) {
      exec = (*node)->target( );
      if ( exec->isEnabled( ) ) {

	 exec->beginRun( anEvent );         
	 
      }
   }
   return AppResult::OK;
}

AppResult
AppSequence::inputFrame( AppFrame*& aFrame,
                         AppStopType& theDispatchStopType ) { // Frame input
  return AppResult::OK;
}

AppResult
AppSequence::outputFrame( AppFrame* aFrame ) {                // Frame output
  return AppResult::OK;
}

AppResult
AppSequence::frame( AppFrame* aFrame,
                    const AppStopType& theDispatchStopType ) { // Dynamic dispatch

  AppExecNode** node;
  AppExecutable* exec;
  
  setPassed( true ); // Leave potential for an AppMethod to terminate exection, for now...
  
  APPListIterator<AppExecNode*> theIterator( *_nodes );
  
  AppResult theResult = AppResult::OK;
  while ( ( node = theIterator( ) ) ) { // Run over the nodes...
    exec = (*node)->target();          // ...get that node's executable
    if ( passed( ) ) {
      if(exec->isEnabled()) {theResult=exec->frame( aFrame, theDispatchStopType );}
      setPassed( exec->passed( ) );
    }
  }
  
  return theResult.value();

}

AppResult
AppSequence::event( AbsEvent* anEvent )
{
  assert( NULL != anEvent );
  
  setPassed( true );

  AppExecutable* exec;
  AppExecNode** node;
  APPListIterator<AppExecNode*> theIterator( *_nodes );
  APPListIterator<AppActionController*> 
    itr1(*(framework()->actionControllers()));
  AppActionController** anActionController;

  while ( ( node = theIterator( ) ) ) {
    exec = (*node)->target(); // Get that node's executable
    if ( passed( ) ) {
      if ( ! (*node)->isExecuted( ) ) { 
	//  The node has not been executed yet; do it now.
	if ( exec->isEnabled( ) ) {

	  if ( exec->execType() == AppExecutable::APP_module 
	       || exec->execType() == AppExecutable::APP_filter
	       || exec->execType() == AppExecutable::APP_input
	       || exec->execType() == AppExecutable::APP_output
	       || exec->execType() == AppExecutable::APP_noclone
	       ) { // poor man's dynamic cast:
	    itr1.rewind();
	    while ( ( anActionController=itr1.next() ) ) {
	      (*anActionController)->beforeEvent(exec,anEvent);
	    }
	  }
	  
	  exec->event( anEvent );
	  
	  if ( exec->execType() == AppExecutable::APP_module 
	       || exec->execType() == AppExecutable::APP_filter
	       || exec->execType() == AppExecutable::APP_input
	       || exec->execType() == AppExecutable::APP_output
	       || exec->execType() == AppExecutable::APP_noclone
	       ) { // poor man's dynamic cast:
	    itr1.rewind();
	    while ( ( anActionController=itr1.next() ) ) {
	      (*anActionController)->afterEvent(exec,anEvent);
	    }
	  }
	  
	  // This gives the container exec node the pass value of the 
	  // last executed containee exec node. If the pass value was reset to
	  // false, the execution path would then terminate.
	  //
	  // Comment by M.Turcotte 12/3/97
	  setPassed( exec->passed( ) );	
  
	}
	
	(*node)->setExecuted( true ); // set node as being executed.
	
      } else { 
	//                   
	// The node has been executed previously. Do not repeat any of
	// the execution. But set the container exec's pass value anyway
	// since you may still want to terminate execution on the fly...
	//
	//               Marc Turcotte, 12/3/97

	if ( exec->isEnabled( ) ) { setPassed( exec->passed( ) ); } 
	
      }  
    }
  }
  return AppResult::OK;
}

AppResult
AppSequence::inputEvent( AbsEvent*& anEvent ) {
  return AppResult::OK;
}

AppResult
AppSequence::outputEvent ( AbsEvent* anEvent ) {
  return AppResult::OK;
}

AppResult
AppSequence::other( AbsEvent* anOther )
{
    assert( NULL != anOther );
    
    AppExecNode** node;
    AppExecutable* exec;
    APPListIterator<AppExecNode*> theIterator( *_nodes );
    APPListIterator<AppActionController*> 
      itr1(*(framework()->actionControllers()));
    AppActionController** anActionController;

    while ( ( node = theIterator( ) ) ) {
       if ( ! (*node)->isExecuted( ) ) {
	  exec = (*node)->target( );
	  if ( exec->isEnabled( ) ) {

            if ( exec->execType() == AppExecutable::APP_module 
                 || exec->execType() == AppExecutable::APP_filter
                 || exec->execType() == AppExecutable::APP_input
                 || exec->execType() == AppExecutable::APP_output
                 || exec->execType() == AppExecutable::APP_noclone
                 ) {
              itr1.rewind();
              while ( ( anActionController=itr1.next() ) ) {
                (*anActionController)->beforeOther(exec,anOther);
              }
            }
	  
            exec->other( anOther );

            if ( exec->execType() == AppExecutable::APP_module 
                 || exec->execType() == AppExecutable::APP_filter
                 || exec->execType() == AppExecutable::APP_input
                 || exec->execType() == AppExecutable::APP_output
                 || exec->execType() == AppExecutable::APP_noclone
                 ) {
              itr1.rewind();
              while ( ( anActionController=itr1.next() ) ) {
                (*anActionController)->afterOther(exec,anOther);
              }
            }
	  }
	  (*node)->setExecuted( true );
       }
    }
    return AppResult::OK;
}

AppResult
AppSequence::endRun( AbsEvent* anEvent ) {

   AppExecNode** node;
   AppExecutable* exec;
    
   assert( NULL != anEvent );
    
   APPListIterator<AppExecNode*> theIterator( *_nodes );
   while ( ( node = theIterator( ) ) ) {
      exec = (*node)->target( );
      if ( exec->isEnabled( ) ) {
	 
	 exec->endRun(anEvent);
	 
      }
   }
   return AppResult::OK;
}

AppResult
AppSequence::endJob( AbsEvent* aRecord ) {

   AppExecNode** node;
    
   APPListIterator<AppExecNode*> theIterator( *_nodes );
   while ( ( node = theIterator( ) ) ) {

      (*node)->target( )->endJob( aRecord );  

   }
   return AppResult::OK;
}

AppResult
AppSequence::abortJob( AbsEvent* anEvent ) {

   AppExecNode** node;
    
   APPListIterator<AppExecNode*> theIterator( *_nodes );
   while ( ( node = theIterator( ) ) ) {

      (*node)->target( )->abortJob( anEvent );
   }
   return AppResult::OK;
}

//-------------
// Selectors --
//-------------
    
AppExecNode*
AppSequence::head( ) const
{
  if (_nodes->first( ) == 0)
    return 0;
  else {
    return (*_nodes->first( ));
  }

}

void 
AppSequence::addToReport( AppReportAttributes* attrib, int mode ) const
{
  AppHierarchyReportAttributes* attribH 
    = dynamic_cast<AppHierarchyReportAttributes*>(attrib);
  if ( (! attribH->listNextLevel()) || (! attribH->listExpand(this)) ) {
    partialReport(" (Compressed) ");
  }
}

void 
AppSequence::report( AppReportAttributes* attrib, int mode ) const
{
  AppExecNode** node;
  AppHierarchyReportAttributes* attribH 
    = dynamic_cast<AppHierarchyReportAttributes*>(attrib);
  
  AppExecutable::report(attribH);

  if (attribH->listNextLevel() && attribH->listExpand(this) ) {
    attribH->increaseLevel(this);
    if ( ! _nodes->isEmpty( ) ) {
      APPListIterator<AppExecNode*> theIterator( *_nodes );
      while ( ( node = theIterator( ) ) ) {
	(*node)->target( )->report( attribH, (*node)->mode( ) );
      }
    }
    attribH->decreaseLevel();
  }

}

bool
AppSequence::passed( ) const
{
    return _passed;
}

//-------------
// Modifiers --
//-------------

void
AppSequence::append( 
    const AppNameNode* const aNode, 
    const AppNameNode* const anOther )
{
   APPList< AppExecNode* >* theList;
   AppExecNode** p_otherNode = NULL;
   AppExecNode** p_useNode;
   AppExecNode*  useNode;

   AppExecutable* theExec;
   AppExecutable* theOther;
   
   assert( NULL != framework( ) );
   assert( NULL != aNode );
   
   if ( NULL != ( theExec = framework( )->execFromName( aNode->name( ) ) ) ) {
      theList = framework( )->nodes( );
      APPListIterator<AppExecNode*> iter1( *theList );
      while ( ( p_useNode = iter1( ) ) ) {
	 if ( (*p_useNode)->target( ) == theExec && 
	      (*p_useNode)->mode( )   == aNode->mode( ) ) {
	    break;
	 }
      }
      if ( NULL == p_useNode ) {
	 useNode = new AppExecNode( theExec, aNode->mode( ) );
	 p_useNode = &useNode;
	 framework( )->add( useNode );
      }
      if ( NULL != anOther ) {
	 if ( NULL != ( theOther = 
			framework( )->execFromName( anOther->name( ) ) ) ) {
	    APPListIterator<AppExecNode*> iter2( *_nodes );
	    while ( ( p_otherNode = iter2( ) ) ) {
	       if ( (*p_otherNode)->target( ) == theOther && 
		    (*p_otherNode)->mode( )   == anOther->mode( ) ) {
		  break;
	       }
	    }
	 }
      }
      _nodes->append( p_useNode, p_otherNode );
   }
}

void
AppSequence::append( 
    const AppExecutable* const anExec, 
    int theMode,
    const AppExecutable* const theOther,
    int otherMode )
{
   APPList< AppExecNode* >* theList;
   AppExecNode** p_aNode = NULL;
   AppExecNode** p_useNode;
   AppExecNode*  useNode;
   assert( NULL != framework( ) );
   assert( NULL != anExec );
   
   theList = framework( )->nodes( );
   APPListIterator<AppExecNode*> iter1( *theList );
   while ( ( p_useNode = iter1( ) ) ) {
      if ( (*p_useNode)->target( ) == anExec && (*p_useNode)->mode( ) == theMode ) {
	 break;
      }
   }
   if ( NULL == p_useNode ) {
      useNode = new AppExecNode( anExec, theMode );
      p_useNode = &useNode;
      framework( )->add( useNode );
   }
   if ( NULL != theOther ) {
      APPListIterator<AppExecNode*> iter2( *_nodes );
      while ( ( p_aNode = iter2( ) ) ) {
	 if ( (*p_aNode)->target( ) == theOther && 
	      (*p_aNode)->mode( )   == otherMode ) {
	    break;
	 }
      }
   }
    _nodes->append( p_useNode, p_aNode );
}

void
AppSequence::append( 
    const char* const theName, 
    int theMode,
    const char* const theOther,
    int otherMode )
{
    APPList< AppExecNode* >* theList;
    AppExecNode** p_aNode = NULL;
    AppExecNode** p_useNode;
    AppExecNode*  useNode;
    AppExecutable* theExec;

    assert( NULL != framework( ) );
    assert( NULL != theName );
    
    if ( NULL != ( theExec = framework( )->execFromName( theName ) ) ) {
		theList = framework( )->nodes( );
 		APPListIterator<AppExecNode*> iter1( *theList );
		while ( ( p_useNode = iter1( ) ) ) {
			if ( (*p_useNode)->target( ) == theExec && 
				 (*p_useNode)->mode( )   == theMode ) {
			break;
			}
		}
		if ( NULL == p_useNode ) {
			useNode = new AppExecNode( theExec, theMode );
			p_useNode = &useNode;
			framework( )->add( useNode );
		}
		if ( NULL != theOther ) {
 			APPListIterator<AppExecNode*> iter2( *_nodes );
			while ( ( p_aNode = iter2( ) ) ) {
			if ( (*p_aNode)->target( )->name( ) == theOther &&
			     (*p_aNode)->mode( )            == otherMode ) {
					break;
				}
			}
		}
		_nodes->append( p_useNode, p_aNode );
    }
}

void
AppSequence::insert( 
    const AppNameNode* const aNode, 
    const AppNameNode* const anOther )
{
   APPList< AppExecNode* >* theList;
   AppExecNode**  p_otherNode = NULL;
   AppExecNode**  p_useNode;
   AppExecNode*   useNode;
   AppExecutable* theExec;
   AppExecutable* theOther;
   
   assert( NULL != framework( ) );
   assert( NULL != aNode );
   
   if ( NULL != ( theExec = framework( )->execFromName( aNode->name( ) ) ) ) {
      theList = framework( )->nodes( );
      APPListIterator<AppExecNode*> iter1( *theList );
      while ( ( p_useNode = iter1( ) ) ) {
	 if ( (*p_useNode)->target( ) == theExec &&
	      (*p_useNode)->mode( )   == aNode->mode( ) ) {
	    break;
	 }
      }
      if ( NULL == p_useNode ) {
	 useNode = new AppExecNode( theExec, aNode->mode( ) );
	 p_useNode = &useNode;
	 framework( )->add( useNode );
      }
      if ( NULL != anOther ) {
	 if ( NULL != ( theOther = 
			framework( )->execFromName( anOther->name( ) ) ) ) {
	    APPListIterator<AppExecNode*> iter2( *_nodes );
	    while ( ( p_otherNode = iter2( ) ) ) {
	       if ( (*p_otherNode)->target( ) == theOther && 
		    (*p_otherNode)->mode( )   == anOther->mode( ) ) {
		  break;
	       }
	    }
	 }
      }
      _nodes->insert( p_useNode, p_otherNode );
   }
}

void
AppSequence::insert( 
    const AppExecutable* const anExec, 
    int theMode,
    const AppExecutable* const theOther,
    int otherMode )
{
   APPList< AppExecNode* >* theList;
   AppExecNode** p_aNode = NULL;
   AppExecNode** p_useNode;
   AppExecNode* useNode;
   assert( NULL != framework( ) );
   assert( NULL != anExec );
   
   theList = framework( )->nodes ( );
   APPListIterator<AppExecNode*> iter1( *theList );
   while ( ( p_useNode = iter1( ) ) ) {
      if ( (*p_useNode)->target( ) == anExec && 
	   (*p_useNode)->mode( )   == theMode ) {
	 break;
      }
   }
   if ( NULL == p_useNode ) {
      useNode = new AppExecNode( anExec, theMode );
      p_useNode = &useNode;
      framework( )->add( useNode );
   }    
   if ( NULL != theOther ) {
      APPListIterator<AppExecNode*> iter2( *_nodes );
      while ( (  p_aNode = iter2( ) ) ) {
	 if ( (*p_aNode)->target( ) == theOther && 
	      (*p_aNode)->mode( )   == otherMode ) {
	    break;
	 }
      }
   }
   _nodes->insert( p_useNode, p_aNode );
}

void
AppSequence::insert( 
    const char* const theName, 
    int theMode,
    const char* const theOther,
    int otherMode )
{
   APPList< AppExecNode* >* theList;
   AppExecNode** p_aNode = NULL;
   AppExecNode** p_useNode;
   AppExecNode*  useNode;
   AppExecutable* theExec;
   assert( NULL != framework( ) );
   assert( NULL != theName );
    
   if ( NULL != ( theExec = framework( )->execFromName( theName ) ) ) {
      theList = framework( )->nodes ( );
      APPListIterator<AppExecNode*> iter1( *theList );
      while ( ( p_useNode = iter1( ) ) ) {
	 if ( (*p_useNode)->target( ) == theExec && 
	      (*p_useNode)->mode( )   == theMode ) {
	    break;
	 }
      }
      if ( NULL == p_useNode ) {
	 useNode = new AppExecNode( theExec, theMode );
	 p_useNode = &useNode;
	 framework( )->add( useNode );
      }    
      if ( NULL != theOther ) {
	 APPListIterator<AppExecNode*> iter2( *_nodes );
	 while ( ( p_aNode = iter2( ) ) ) {
	    if ( (*p_aNode)->target( )->name( ) == theOther &&
		 (*p_aNode)->mode( )            == otherMode ) {
	       break;
	    }
	 }
      }
      _nodes->insert( p_useNode, p_aNode );
   }
}

void
AppSequence::remove( const AppExecutable* const anExec, int mode )
{
    AppExecNode** node;

    assert( NULL != anExec );
    
 	APPListIterator<AppExecNode*> theIterator( *_nodes );
	while ( ( node = theIterator( ) ) ) {
        if ( (*node)->target( ) == anExec && (*node)->mode( ) == mode ) {
			_nodes->remove( (*node) );
			break;
		}
    }
}

void
AppSequence::remove( const char* const theName, int mode  )
{
    AppExecNode** node;

    assert( NULL != theName );
    
 	APPListIterator<AppExecNode*> theIterator( *_nodes );
	while ( ( node = theIterator( ) ) ) {
		if ( (*node)->target( )->name( ) == theName &&
                     (*node)->mode( )            == mode ) {
			_nodes->remove( (*node) );
			break;
		}
    }
}

void
AppSequence::setPassed( bool flag )
{
    _passed = flag;
}

void
AppSequence::buildNodeList(APPList<AppExecNode*>& destination, 
			   APPList<AppExecNode*>& source){

    APPListIterator<AppExecNode*> itr( source );
    AppExecNode** n;
    APPList<AppExecNode*> node;      
    while( ( n = itr() ) ){
      switch( (*n)->target()->execType() ){
      default :
	break;
      case AppExecutable::APP_module:
      case AppExecutable::APP_filter:
	destination.append( *n );
	break;
      case AppExecutable::APP_sequence:
	APPList<AppExecNode*>* snode = 
	  ((AppSequence*)( (*n)->target() ))->nodes();      
	((AppSequence*) *n)->buildNodeList( node, *snode );
	destination.append( node );
	break;
      }
    }
}







