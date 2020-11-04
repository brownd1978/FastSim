//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsParmFilename.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AbsParmFilename
//
//      Command handler for the "parm" command for
//	the module.  Provides a "filename" value. Valid commands are
//
//	parm set <x>                            set variable to x
//      parm list                               print parms
//
//
// AbsParmFilename is the standard way to put a filename parameter in a 
// module.  The value() is the name itself, the pathname() member returns
// the pathname of a located file using a specified search path.  The
// default search path, DEFAULTBFRECOPATH, is ".:RELEASE:PARENT", with the
// same syntax as the Unix "PATH" variable.  See AbsParmFilename.cc.
//
// In the user's work area, RELEASE is normally a symbolic link pointing
// the BaBar software release to which the work area belongs (i.e, the
// "test release" directory created with |newrel|), often "..".  PARENT
// is a symbolic link to the "parent release" (i.e., the _production_
// release) upon which the user's test release is based.
//  
// See FrameExample/ParmExample for a sample.
//
//
// Eventually, the AbsEnv class should provide a factory for
// these so they can be subtyped as needed to various database
// classes, etc.  
//
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Bob Jacobsen		Original Author
//      Marc Turcotte           University of TExas at Dallas 5/15
//      Luca Lista              INFN Napoli : changed to use class AppFileName
//
// Copyright Information:
//	Copyright (C) 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1998      University of Texas at Dallas
//      Copyright (C) 1999      INFN
//
//------------------------------------------------------------------------

#ifndef ABSPARMFILENAME_HH
#define ABSPARMFILENAME_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AbsParm.hh"
#include "Framework/AppFileName.hh"
#include "Framework/AppConfigRecorder.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AbsParmFilename : public AbsParm {
      
//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AbsParmFilename( const char* const theCommand, AppModule* theTarget, 
		     std::string def );
  AbsParmFilename( const AbsParmFilename& theOther, AppModule* theTarget);

  // Destructor
  virtual ~AbsParmFilename( );
  
  // Handler is inherited

  // value accessor
  virtual const std::string& value() const { return _fileName.value(); }
  std::string type () const { return "Filename";}

  std::string classDescription () const { 
    return "filename parameter: set or list."; 
  }

  // get the pathname 
  virtual std::string pathname() const;
  
  // set the value
  void set(const std::string v) { _fileName.set( v ); }

  void dumpState(AppConfigRecorder* r){r->store(value(), "set");}
  void restoreState(char* [] );
  
protected:
  
  virtual int setHandler    ( );
  virtual int listHandler   ( ) const;
  virtual int getHandler    ( );
   
private:

  // Copy and assignment not implemented.
  AbsParmFilename( const AbsParmFilename& );
  AbsParmFilename& operator=( const AbsParmFilename& );

  AppFileName _fileName;

};

#endif
