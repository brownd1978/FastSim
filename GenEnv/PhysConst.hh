//-----------------------------------------------------------------------------
// File and Version Information
//      $Id: PhysConst.hh 510 2010-01-14 15:28:09Z stroili $
//
// Description:
//      Class PhysConst 
//      This header file is derived from the Geant4 header file 
//      PhysicalConstants.h
// 
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Mossadek Talby     (SLAC - CPPM/IN2P3 University of Aix-Marseille II)
//
// Modification History:
//   June 18, 1998 - Talby : created
//-----------------------------------------------------------------------------
#ifndef PHYSCONST_HH
#define PHYSCONST_HH 1

//----------------------
// Base Class Headers --
//----------------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
// Mandatory include file

#include <math.h>
#ifndef BABAR_HH
#include "BaBar/BaBar.hh"
#endif


// some units 
static const double mm = 1.;
static const double mm3 = mm*mm*mm;
static const double cm = 10.*mm;
static const double cm3 = cm*cm*cm;
static const double m = 1000.*mm;
static const double ns = 1.;
static const double s = 1.e+9*ns; 
static const double eV = 1.e-6; 
static const double keV = 1.e-3; 
static const double MeV = 1.; 
static const double joule = 6.24150e+12 * MeV;
static const double kg = joule*s*s/(m*m);   
static const double g = 1.e-3*kg;
static const double mg = 1.e-3*g;
static const double eplus = 1.; 
static const double henry = 1.60217733e-7*MeV*(ns*ns/eplus*eplus);
static const double kelvin = 1.;
static const double mole = 1.;
static const double atmosphere = 6.32420e+8 * MeV/mm3;
static const double STP_Temperature = 273.15*kelvin;
static const double STP_Pressure    = 1.*atmosphere;
static const double GasThreshold    = 10.*mg/cm3;
static const double universe_mean_density = 1.e-25*g/cm3;

// some constants
static const double     pi  = 3.14159265358979323846;
static const double  twopi  = 2*pi;
static const double c_light   = 2.99792458e+8 * m/s;
static const double c_squared = c_light * c_light;
static const double electron_charge = - eplus; 
static const double e_squared = eplus * eplus;
static const double Avogadro = 6.0221367e+23/mole;
static const double h_Planck      = 6.6260755e-34 * joule*s;
static const double hbar_Planck   = h_Planck/twopi;
static const double hbarc         = hbar_Planck * c_light;
static const double hbarc_squared = hbarc * hbarc;
static const double k_Boltzmann = 8.617385e-11 * MeV/kelvin;
static const double electron_mass_c2 = 0.51099906 * MeV;
static const double   proton_mass_c2 = 938.27231 * MeV;
static const double mu0      = 4*pi*1.e-7 * henry/m;
static const double epsilon0 = 1./(c_squared*mu0);
static const double elm_coupling           = e_squared/(4*pi*epsilon0);
static const double classic_electr_radius  = elm_coupling/electron_mass_c2;
static const double fine_structure_const   = elm_coupling/hbarc;
static const double classic_electr_radius_squared  = classic_electr_radius
                                                    *classic_electr_radius;
static const double electron_Compton_length = hbarc/electron_mass_c2;
static const double alpha_rcl2 = fine_structure_const
                                   *classic_electr_radius_squared;
static const double twopi_mc2_rcl2 = twopi*electron_mass_c2
                                    *classic_electr_radius_squared;


#endif


