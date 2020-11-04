#ifndef STDHEPUTIL_HH
#define STDHEPUTIL_HH 1
// FILE:
// StdHepUtil.hh
//--------------------------------------------------------------------
// PURPOSE: 
// Declare quantities associated with the stdhep common block and the
// GenFwkInt interface to it.
//--------------------------------------------------------------------
// AUTHORS:
// Abi Soffer
//--------------------------------------------------------------------

// stdhep common block flag for an incoming particle:
static const int STDHEP_INCOMING = 3;

// stdhep common block flag for a particle that has no daughters in
// the common block:
static const int STDHEP_STABLE = 1;

// The number added to the status of long-lived particles to flag them
// as long-lived. If this number changes here, you must change it in 
// std_flag_long_lived too:

static const int STDHEP_LONG_LIVED_FLAG = 200;

// stdhep interface routines:  
extern "C" 
{
  void std_flag_long_lived_(const int & entry);
  
  void std_new_event_();
  
  void std_insert_incoming_(const double * p1,
			    const int & id1,
			    const double * p2,
			    const int & id2,
			    const double * v);
  
  void std_remove_incoming_();
  
  void std_get_daughters_(const int & mother, 
			  int & first, 
			  int & last);
  
  int std_primary_(const int & startIndex, 
		   double * momentum, 
		   int & isIncoing);
  
  void std_set_evt_num_(const int & nevhep);
  
  void std_set_num_entries_(const int & numEntries);
  
  void std_set_entry_(const int & entry, 
		      const int & isthep, 
		      const int & idhep, 
		      const int * jmohep, 
		      const int * jdahep, 
		      const double * phep, 
		      const double * vhep);

  void std_get_particle_ids_(int & nParticles, int * particleIds);

  void std_set_p4_(int & entry, const double * p4);
}
#endif
