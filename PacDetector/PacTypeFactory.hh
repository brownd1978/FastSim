/* Class: PacTypeFactory
*
  * A factory utility class to produce transient sub-detector Types
  * from the corresponding EDML elements
 */
#ifndef PacTypeFactory_HH
#define PacTypeFactory_HH

class EdmlCylDetElement;
class EdmlRingDetElement;
class EdmlRectDetElement;
class EdmlConeDetElement;
class PacCylDetType;
class PacRingDetType;
class PacRectDetType;
class PacConeDetType;

class PacTypeFactory {

public:
      /// Get a sub-detector Element for the specified EDML Element
       /**
  * OWNERSHIP NOTE: The method will return the ownership over the object.
         */
  static PacCylDetType* get( const EdmlCylDetElement* theEdmlelement );
  static PacRingDetType* get( const EdmlRingDetElement* theEdmlelement );
  static PacRectDetType* get( const EdmlRectDetElement* theEdmlelement );
  static PacConeDetType* get( const EdmlConeDetElement* theEdmlelement );

private:

       /// The singleton

  static PacTypeFactory& instance();

       /// No public c-tor because it's a singleton

  PacTypeFactory();

       /// No public d-tor because it's a singleton

  virtual ~PacTypeFactory();

       /// Get a sub-detector Type for the specified EDML Element (actuall implmentation)

  PacCylDetType* doGet( const EdmlCylDetElement* theEdmlElement );
  PacRingDetType* doGet( const EdmlRingDetElement* theEdmlElement );
  PacRectDetType* doGet( const EdmlRectDetElement* theEdmlElement );
  PacConeDetType* doGet( const EdmlConeDetElement* theEdmlElement );

private:
// generate unique type numbers
  unsigned int _icyl;  // for class PacCylDetType
  unsigned int _iring;    // for class PacRingDetType
  unsigned int _irect;    // for class PacRectDetType
  unsigned int _icone; // for class PacConeDetType

};

#endif // PacTypeFactory_HH
