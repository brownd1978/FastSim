/*******************************************************************************
* PacElemFactory
*******************************************************************************/

#include "BaBar/BaBar.hh"

#include "PacDetector/PacElemFactory.hh"
#include "PacDetector/PacMeasurementFactory.hh"
#include "PacDetector/PacTypeFactory.hh"

#include "PacGeom/PacCylDetElem.hh"
#include "PacGeom/PacCylDetType.hh"
#include "PacGeom/PacConeDetElem.hh"
#include "PacGeom/PacConeDetType.hh"
#include "PacGeom/PacPlaneDetElem.hh"
#include "PacGeom/PacRingDetType.hh"
#include "PacGeom/PacRectDetType.hh"
#include "PacGeom/PacMeasurement.hh"

#include "PacEnv/EdmlCylDetElement.hh"
#include "PacEnv/EdmlRingDetElement.hh"
#include "PacEnv/EdmlRectDetElement.hh"
#include "PacEnv/EdmlConeDetElement.hh"

#include <iostream>
#include <boost/tokenizer.hpp>

using namespace std;

#include <assert.h>

DetElem*
PacElemFactory::get( const EdmlDetElement* theEdmlElement )
{
    return PacElemFactory::instance().doGet( theEdmlElement );
}

PacElemFactory&
PacElemFactory::instance()
{
    static PacElemFactory factory;
    return factory;
}

PacElemFactory::PacElemFactory() 
{}

PacElemFactory::~PacElemFactory()
{
}

DetElem*
PacElemFactory::doGet( const EdmlDetElement* theEdmlElement )
{
  assert( theEdmlElement );

    // find the measuring devices (if any)
    //
  const std::string& device = theEdmlElement->device();
  std::vector<const PacMeasurement*> mdevs;

  if(!device.empty()){
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep(",;:| ");
    tokenizer tokens(device, sep);
    for (tokenizer::iterator titer = tokens.begin();titer != tokens.end(); ++titer){
      PacMeasurement* meas = PacMeasurementFactory::get(*titer);
      if(meas != 0){
        mdevs.push_back(meas);
      } else {
        ErrMsg(fatal)  << "Can't find measurement device " << *titer << endmsg;
      }
    }
  }

    // Build a subdetector of the given type
    //
  if( theEdmlElement->type().find("Cylinder") != string::npos) {

    const EdmlCylDetElement* cyl = dynamic_cast<const EdmlCylDetElement*>(theEdmlElement);
    if(ErrLogging(routine)){
      cout << "Building " << cyl->type() << " " << cyl->name() << " " << cyl->id() << " at radius " 
        << cyl->radius();
      printElem(cout,theEdmlElement);
    }
    PacCylDetType* type = PacTypeFactory::get(cyl);
    assert(type);
    return new PacCylDetElem( type, cyl->radius(), cyl->name().c_str(), cyl->id(), mdevs,
      cyl->gap(), cyl->overlap() );

  }
  if(theEdmlElement->type().find("Ring") != string::npos) {

    const EdmlRingDetElement* ring = dynamic_cast<const EdmlRingDetElement*>(theEdmlElement);

    if(ErrLogging(routine)){
      cout << "Building " << ring->type() << " " << ring->name() << " " << ring->id() << " at z "
        << ring->z();
      printElem(cout,theEdmlElement);
    }
    PacRingDetType* type = PacTypeFactory::get(ring);
    assert(type);
    return new PacPlaneDetElem( type, PacPlaneDetElem::zparallel,Hep3Vector(0.0,0.0,ring->z()), 0.0,
      ring->name().c_str(), ring->id(), mdevs,
      ring->gap(), ring->overlap() );
  }
  if(theEdmlElement->type().find("Rect") != string::npos) {

    const EdmlRectDetElement* rect = dynamic_cast<const EdmlRectDetElement*>(theEdmlElement);

    if(ErrLogging(routine)){
      cout << "Building " << rect->type() << " " << rect->name() << " " << rect->id() 
        << " orientation " << rect->orientation()
        << " center rho " << rect->centerRho() 
        << " center z " << rect->centerZ()
        << " center phi "  << rect->centerPhi()
        << " axis phi "  << rect->axisPhi() ;
      printElem(cout,theEdmlElement);
    }
    PacRectDetType* type = PacTypeFactory::get(rect);
    assert(type);
    return new PacPlaneDetElem( type, (PacPlaneDetElem::orientation)rect->orientation(),
      Hep3Vector(rect->centerRho()*cos(rect->centerPhi()),rect->centerRho()*sin(rect->centerPhi()),rect->centerZ()), 
      rect->axisPhi(),
      rect->name().c_str(), rect->id(), mdevs,
      rect->gap(), rect->overlap() );
  }
  if( theEdmlElement->type().find("Cone") != string::npos ) {

    const EdmlConeDetElement* cone = dynamic_cast<const EdmlConeDetElement*>(theEdmlElement);

    if(ErrLogging(routine)){
      cout << "Building " << cone->type() << " " << cone->name() << " " << cone->id() << " tan(angle) "
        << cone->tanth() << " z vertex " << cone->zvert();
      printElem(cout,theEdmlElement);
    }
    PacConeDetType* type = PacTypeFactory::get(cone);
    assert(type);
    return new PacConeDetElem( type, cone->tanth(), cone->zvert(), cone->name().c_str(), theEdmlElement->id(),
      mdevs, cone->gap(), cone->overlap() );

  }
  ErrMsg(fatal) << "PacElemFactory::get(): unsupported EDML detector type: '" << theEdmlElement->type() << "'" << endmsg;
  return 0;
}

void
PacElemFactory::printElem(ostream& str, const EdmlDetElement* theEdmlElement ) const {
  str << " measurement " << theEdmlElement->device();
  if(theEdmlElement->gap() != 0.0)
    str << " gap " << theEdmlElement->gap();
  if(theEdmlElement->overlap() != 0.0)
    str << " overlap " << theEdmlElement->overlap();
  return;
}