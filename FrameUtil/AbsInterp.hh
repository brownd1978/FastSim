//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsInterp.hh 508 2010-01-14 15:18:18Z stroili $
//
// Description:
//	Class AbsInterp
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
//
// Copyright Information:
//	Copyright (C) 1997		        FNAL
//
//------------------------------------------------------------------------

#ifndef ABSINTERP_HH
#define ABSINTERP_HH

#include "BaBar/BaBar.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include <string>

//------------------------------
// Forward Class Declarations --
//------------------------------
class  AbsCommand;
struct Tcl_Interp; // ads05May00 - need this until we migrate other packages

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AbsInterp {

public:
  // Destructor
  virtual ~AbsInterp( );

  enum StatusCode
  {
    OK = 0, ERROR = 1, CONTINUE = 2
  };
  
  // member functions      
  static AbsInterp* theInterpreter( );
  // ads 05May00 - need the following ctor until we migrate other packages
  static AbsInterp* theInterpreter(Tcl_Interp* interp);
  bool isInitialized();
  void setCommandPrompt( const char* const thePrompt );
  char* commandPrompt( ) const;
  
  virtual void sendPartialMessage( const char* const aMessage ) const;
  virtual void sendFullMessage( const char* const aMessage ) const;
  virtual void flushMessage( ) const;
  
  virtual StatusCode runCommandFile( const char* filename ) = 0;
  virtual void startInterpLoop( ) = 0;
  virtual void simulateInput( std::string strInput );
  virtual void deleteCommand( const char* commandName ) = 0;
  virtual void createCommand( const char* commandName, 
			      const AbsCommand* command ) = 0;
  virtual bool existsCommand( const char* commandName ) const;
   
  virtual StatusCode parseArgs( const std::string formalCmd, 
				int argc, char *argv[] ) const;
  virtual bool present( const std::string Key ) const;
  virtual int ascii2Int( const char* intString, int *intPtr ) const = 0;
  virtual StatusCode getInt( const std::string intKey, int &intPtr ) const;
  virtual StatusCode getUnsigned( const std::string uintKey, 
				  unsigned int &uintPtr ) const;
  virtual StatusCode getDouble( const std::string doubleKey, 
				double &doublePtr ) const;
  virtual StatusCode getString( const std::string strKey, std::string* strPtr ) const;
  virtual StatusCode getCstring( const char* charKey, char* *charPtr ) const;
  
  virtual bool fileOperation( const char* const fileName,
			      const char* const operation ) const;
  virtual void setResult( const char* result ) const;
  virtual void appendResult (const char* result ) const;  

  void setDoLoopInterp( bool stayInLoop );
  bool getDoLoopInterp() const;

protected:
  
  // Constructor
  AbsInterp();
  
private:
  
  static AbsInterp* _theInterpreter;
  char*             _commandPrompt;  // owned pointer to the current
                                     // value of the command prompt
  bool              _doLoopInterp;   // flag to exit Interpreter loop
  
};
#endif // ABSENV_HH


