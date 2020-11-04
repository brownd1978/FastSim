#ifndef EDML_TYPE_2_ID_HH
#define EDML_TYPE_2_ID_HH

// File and Version Information:
//      $Id: EdmlType2Id.hh,v 1.2 2005/12/04 20:39:53 kelsey Exp $

#include "PacEnv/EdmlDefaultTypeIdGenerator.hh"

/// Translate the type into its identifier
/**
  * This class will use the specified (or the default) generator of identifiers.
  * A reson why we may want to parameterize the current class with different
  * generators is that we may want to have a better control over actual number
  * of identifiers associated with transient types. For example, we may consider
  * using predefined type numbers rather than automatically generated ones. Or,
  * we may want to have two independent collections of transient types whose
  * identifiers both start with 0.
  */
template< class T,
          class GENERATOR = EdmlDefaultTypeIdGenerator >
class EdmlType2Id {
public:

  /// Get the identifier corresponding to the type

  static unsigned int id( );
};

#ifdef BABAR_COMP_INST
#include "PacEnv/EdmlType2Id.icc"
#endif

#endif  // EDML_TYPE_2_ID_HH
