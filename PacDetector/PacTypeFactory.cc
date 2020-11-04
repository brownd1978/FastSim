/*******************************************************************************
* PacTypeFactory
*******************************************************************************/

#include "BaBar/BaBar.hh"

#include "PacDetector/PacTypeFactory.hh"

#include "PacGeom/PacCylDetType.hh"
#include "PacGeom/PacConeDetType.hh"
#include "PacGeom/PacRingDetType.hh"
#include "PacGeom/PacRectDetType.hh"

#include "PacEnv/EdmlCylDetElement.hh"
#include "PacEnv/EdmlRingDetElement.hh"
#include "PacEnv/EdmlRectDetElement.hh"
#include "PacEnv/EdmlConeDetElement.hh"

#include "PacEmc/PacEmcCylType.hh"

#include "DetectorModel/DetMaterial.hh"

#include "GenEnv/GenEnv.hh"
#include "AbsEnv/AbsEnv.hh"

#include <iostream>
using namespace std;

#include <assert.h>

PacCylDetType*
PacTypeFactory::get( const EdmlCylDetElement* theEdmlElement )
{
  return PacTypeFactory::instance().doGet( theEdmlElement );
}

PacRingDetType*
PacTypeFactory::get( const EdmlRingDetElement* theEdmlElement )
{
  return PacTypeFactory::instance().doGet( theEdmlElement );
}

PacRectDetType*
PacTypeFactory::get( const EdmlRectDetElement* theEdmlElement )
{
  return PacTypeFactory::instance().doGet( theEdmlElement );
}

PacConeDetType*
PacTypeFactory::get( const EdmlConeDetElement* theEdmlElement )
{
  return PacTypeFactory::instance().doGet( theEdmlElement );
}

PacTypeFactory&
PacTypeFactory::instance()
{
  static PacTypeFactory factory;
  return factory;
}

PacTypeFactory::PacTypeFactory(): _icyl(0),_iring(0),_irect(0),_icone(0)
{}

PacTypeFactory::~PacTypeFactory()
{
}

PacCylDetType*
PacTypeFactory::doGet( const EdmlCylDetElement* cyl )
{
  assert( cyl );
//
// interpret empty material string as 'no material'
//
  const DetMaterial* detmat = 0;
  if( !cyl->material().empty()){
    detmat = gblEnv->getGen()->findDetMaterial( cyl->material());
    assert( detmat );
  }
  double zends[2] = { cyl->zmin(), cyl->zmax()};
  if(ErrLogging(routine)){
    cout << " z ends " << zends[0] << " , " 
      << zends[1] << " thickness " << cyl->thick();
    if(detmat)
      cout << " material " << *detmat->name();
    cout << endl;
  }
// specialize for constant-thickness Emc barrel
  if(cyl->type() == "EmcCylinder")
    return new PacEmcCylType(cyl->radius(),cyl->thick(), zends, detmat, _icyl++);
  else
    return new PacCylDetType( cyl->thick(), zends, detmat, _icyl++ );
}



PacRingDetType*
PacTypeFactory::doGet( const EdmlRingDetElement* ring )
{
  assert( ring );
//
// interpret empty material string as 'no material'
//
  const DetMaterial* detmat = 0;
  if( !ring->material().empty()){
    detmat = gblEnv->getGen()->findDetMaterial( ring->material());
    assert( detmat );
  }
  if(ErrLogging(routine)){
    cout << " lower radius " 
      << ring->lowradius() << " high radius " << ring->hiradius()
      << " thickness " << ring->thick();
    if(detmat)
      cout << " material " << *detmat->name();
    cout << endl;
  }
  return  new PacRingDetType( ring->thick(), ring->lowradius(), ring->hiradius(), detmat, _iring++ );
}

PacRectDetType*
PacTypeFactory::doGet( const EdmlRectDetElement* rect )
{
  assert( rect );
//
// interpret empty material string as 'no material'
//
  const DetMaterial* detmat = 0;
  if( !rect->material().empty()){
    detmat = gblEnv->getGen()->findDetMaterial( rect->material());
    assert( detmat );
  }
  if(ErrLogging(routine)){
    cout << " u size " << rect->uSize() << " v size " << rect->vSize()
      << " thickness " << rect->thick();
    if(detmat)
      cout << " material " << *detmat->name();
    cout << endl;
  }
  return  new PacRectDetType( rect->thick(), rect->uSize(), rect->vSize(), detmat, _irect++ );
}


PacConeDetType*
PacTypeFactory::doGet( const EdmlConeDetElement* cone )
{
  assert( cone );
//
// interpret empty material stcone as 'no material'
//
  const DetMaterial* detmat = 0;
  if( !cone->material().empty()){
    detmat = gblEnv->getGen()->findDetMaterial( cone->material());
    assert( detmat );
  }
  
  double rends[2] = { cone->rmin(), cone->rmax()};
  if(ErrLogging(routine)){
    cout << " r ends " << rends[0] << " , " << rends[1]
      << " thickness " << cone->thick();
    if(detmat)
      cout << " material " << *detmat->name();
    cout << endl;
  }

  return new PacConeDetType( cone->thick(), rends, detmat, _icone++ );
}
