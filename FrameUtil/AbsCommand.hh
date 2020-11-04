//--------------------------------------------------------------------------
// File and Version Information:
//
// Description:
//      Class AbsCommand. The abstract parent of all command classes. 
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Asoka De Silva
//
// Copyright Information:
//
//------------------------------------------------------------------------

#ifndef ABSCOMMAND_HH
#define ABSCOMMAND_HH

class AbsCommand {

public:

  AbsCommand();
  virtual ~AbsCommand();

  // Handler
  virtual int handle( int argc, char* argv[]) = 0;        

private:

};

#endif
