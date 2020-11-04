#include "BaBar/BaBar.hh"
#include "FrameUtil/TclInterp.hh"
#include "FrameUtil/AbsCommand.hh"
extern "C" {
#include "tcl.h"
}
#include <assert.h>
//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------
int
TclInterp::commandHandler( 
    ClientData clientData, 
    Tcl_Interp* tcl_interp, 
    int argc, 
    char* argv[] )
{
    AbsCommand* theCommand;
    
    assert( NULL != clientData );
    
    theCommand = (AbsCommand*)clientData;
    return theCommand->handle( argc, argv );
}

