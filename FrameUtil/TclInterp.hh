//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: TclInterp.hh 508 2010-01-14 15:18:18Z stroili $
//
// Description:
//	Class TclInterp
//
//      The purpose of this class is to isolate out the interpreter dependence
//      in the Framework.  It is based on the sigleton pattern since any
//      applciation should use only one instance of this class.
//
// Environment:
//	Software developed for the BaBar/CDF Framework
//
// Author List:
//	Liz Sexton-Kennedy		        Original Author
//
// Copyright Information:
//	Copyright (C) 1997		        FNAL
//
//------------------------------------------------------------------------

#ifndef TCLINTERP_HH
#define TCLINTERP_HH

//----------------------
// Base Class Headers --
//----------------------
#include "FrameUtil/AbsInterp.hh"

//------------------------------
// Forward Class Declarations --
//------------------------------

typedef void *ClientData;
#define TCL_ERROR 1
#define TCL_OK 0

class  AbsCommand;

struct Tcl_Interp;
//		---------------------
// 		-- Class Interface --
//		---------------------
 
class TclInterp : public AbsInterp {

  friend class AbsInterp;

public:
  
  // Destructor
  virtual ~TclInterp( );
  
  // member functions 
  virtual AbsInterp::StatusCode runCommandFile( const char* filename );
  virtual void startInterpLoop( );
  
  virtual void createCommand( const char* commandName, 
			      const AbsCommand* command );
  virtual void deleteCommand( const char* commandName );
  virtual bool existsCommand( const char* commandName ) const;
  
  virtual int ascii2Int( const char* intString, int *intPtr ) const;
  virtual bool fileOperation( const char* const fileName,
			      const char* const operation ) const;
  virtual void setResult( const char* result ) const;
  virtual void appendResult( const char* result ) const;
  
  // Static member function for use as tcl call-back
  static int commandHandler( ClientData clientData, Tcl_Interp* tcl_interp,
			     int argc, char* argv[] );
  
private:
  
  Tcl_Interp*    _tclInterpreter;    
  
  // Constructors
  TclInterp(Tcl_Interp* interp);
  TclInterp();
};
#endif // TCLINTERP_HH
