#ifndef EDML_DEFAULT_TYPE_ID_GENERATOR_HH
#define EDML_DEFAULT_TYPE_ID_GENERATOR_HH

// File and Version Information:
//      $Id: EdmlDefaultTypeIdGenerator.hh,v 1.1 2005/12/04 20:39:47 kelsey Exp $

/// The default generator of unique type identifiers (a utility class)
/**
  * The generator will produce unsigned integer numbers starting with 0 every
  * time the EdmlDefaultTypeIdGenerator::next() method is invoked.
  *
  * The class is used in the unique mapping scheme between C++ types and numbers.
  *
  * IMPORTANT DESIGN NOTE:
  *
  *   That the mapping can't be persistent or it can't be used in any way beyong
  *   the lifespan of a process.
  *
  * @see class EdmlType2Id
  */
class EdmlDefaultTypeIdGenerator {

public:

  /// Produce the next unique number
  /**
    * The first time the method gets called it will produce 0.
    */
    static unsigned int next( )
    {
        static unsigned int current = 0;
        return current++;
    }
};

#endif /* EDML_DEFAULT_TYPE_ID_GENERATOR_HH */
