//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: GEvent.cc 430 2010-01-14 15:20:24Z stroili $
//
// Description:
//      Class GEvent implementation file
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      ??
//      Ed Frank                     University of Pennsylvania
//
// History:
//      ?? ??? ??   ??               create
//      23 Jan 96   Ed Frank         add *Store.dcc C++->FORTRAN copyback(dbio)
//      22 Jan 97   Stephen J. Gowdy add osteam argument to printAll
//      18 May 00   DCW              remove dbio dependence, rw persistence
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

#include <iostream>
#include <iomanip>

#include "G3Data/GEvent.hh"
using std::ostream;
using std::setw;


GEvent::GEvent()
  : _idrun(0),
    _idevt(0),
    _nrndm1(0),
    _nrndm2(0),
    _ntrack(0),
    _nvertx(0),
    _iday(0),
    _itime(0),
    _t0(0),
    _bx0(0),
    _platform(0),
    _partition(0),
    _bdbepoch(0),
    _bdboffset(0)
{}

    
bool
GEvent::operator==( const GEvent &otherEvent ) const
{
  bool answer=false;

  if ( _idrun  == otherEvent.idrun()  &&
       _idevt  == otherEvent.idevt()  &&
       _nrndm1 == otherEvent.nrndm1() &&
       _nrndm2 == otherEvent.nrndm2() &&
       _ntrack == otherEvent.ntrack() &&
       _nvertx == otherEvent.nvertx() &&
       _iday   == otherEvent.iday()   &&
       _itime  == otherEvent.itime()  &&
       _t0     == otherEvent.t0()     &&
       _bx0    == otherEvent.bx0()    &&
       _platform == otherEvent.platform()  &&
       _partition == otherEvent.partition() &&
       _bdbepoch == otherEvent.bdbepoch() &&
       _bdboffset == otherEvent.bdboffset()
       ) answer=true;
  return answer;
}

void GEvent::printAll( ostream &o ) const 
{
    o << *this;
}

ostream &operator<<(ostream &stream, const GEvent* pc)
{
    stream << *pc;
    return stream;
}

ostream &operator<<(ostream &stream, const GEvent& pc)
{
    stream << "GEvent:" << setw(8) << "run" << setw(7) << "event"
           << setw(11) << "nrndm1" << setw(11) << "nrndm2"
           << setw(7) << "day" << setw(7) << "time" << setw(7) << "t0" 
	   << setw(7) << "bx0" << setw(7) << "platf." << setw(7)
	   << "part." << setw(7) << "Epoch" << setw(7) << "Offset";
    stream << "\n";
    stream << "       " << setw(8) << pc.idrun() << setw(7) << pc.idevt()
           << setw(11) << pc.nrndm1() << setw(11) << pc.nrndm2()
           << setw(7)  << pc.iday()   << setw(7)  << pc.itime()
	   << setw(7)  << pc.t0()     << setw(7)  << pc.bx0()
           << setw(7)  << pc.platform() << setw(7) << pc.partition()
           << setw(7)  << pc.bdbepoch() << setw(7) << pc.bdboffset();
    
    return stream;
}
