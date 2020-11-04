/* Class: PacElemFactory
 *
 * A factory utility class to produce transient sub-detector elements
 * from the corresponding EDML elements.
 */
#ifndef PacElemFactory_HH
#define PacElemFactory_HH

#include <vector>
#include <iostream>

class EdmlDetElement;
class DetElem;
class DetType;
class PacElemFactory {

public:

    /// Get a sub-detector element for the specified EDML element
    /**
      * OWNERSHIP NOTE: The method will return the ownership over the object.
      */
    static DetElem* get( const EdmlDetElement* theEdmlElement );

private:

    /// The singleton

    static PacElemFactory& instance();

    /// No public c-tor because it's a singleton

    PacElemFactory();

    /// No public d-tor because it's a singleton

    virtual ~PacElemFactory();

    /// Get a sub-detector element for the specified EDML element (actuall implmentation)

    DetElem* doGet( const EdmlDetElement* theEdmlElement );

    void printElem(std::ostream& str, const EdmlDetElement* theEdmlElement ) const;

private:

};

#endif // PacElemFactory_HH
