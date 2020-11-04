//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: FtclInterp.hh 508 2010-01-14 15:18:18Z stroili $
//
// Description:
//	Class FtclInterp
//
//      The purpose of this class is to isolate the interpreter dependence
//      in the Framework.  It is based on the singleton pattern since any
//      application should use only one instance of this class.
//
// Environment:
//	Software developed for the BaBar/CDF Framework
//
// Author List:
//	Liz Sexton-Kennedy		        Original Author
//	Glenn Cooper				Some ftcl mods
//
// Copyright Information:
//	Copyright (C) 1997		        FNAL
//
//------------------------------------------------------------------------

#ifndef FTCLINTERP_HH
#define FTCLINTERP_HH

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
 
class FtclInterp : public AbsInterp {
  
  friend class AbsInterp;
  
public:
  
  // Destructor
  virtual ~FtclInterp( );

  // member functions      
  virtual AbsInterp::StatusCode runCommandFile( const char* filename );
  virtual void startInterpLoop( );
  virtual void simulateInput( std::string strInput );
  virtual void createCommand( const char* commandName, 
			      const AbsCommand* command );
  virtual void deleteCommand( const char* commandName );
  virtual bool existsCommand( const char* commandName ) const;
  
  virtual AbsInterp::StatusCode parseArgs( const std::string formalCmd, 
					   int argc, char *argv[] ) const;
  virtual bool present( const std::string Key ) const;
  virtual int ascii2Int( const char* intString, int *intPtr ) const;
  virtual AbsInterp::StatusCode getInt( const std::string intKey, 
					int &intPtr ) const;
  virtual AbsInterp::StatusCode getUnsigned( const std::string uintKey, 
					     unsigned int &uintPtr ) const;
  virtual AbsInterp::StatusCode getDouble( const std::string doubleKey, 
					   double &doublePtr ) const;
  virtual AbsInterp::StatusCode getString( const std::string strKey, 
					   std::string* strPtr ) const;
  
  virtual bool fileOperation( const char* const fileName,
			      const char* const operation ) const;
  virtual void setResult( const char* result ) const;
  virtual void appendResult( const char* result) const;
  
  // Static member function for use as tcl call-back
  static int commandHandler( ClientData clientData, Tcl_Interp* tcl_interp,
			     int argc, char* argv[] );
  
private:
  
  Tcl_Interp*    _tclInterpreter;    

  // Constructor
  FtclInterp(Tcl_Interp* interp);  // need this until we migrate other packages
  FtclInterp();
  
};
#endif // FTCLINTERP_HH
