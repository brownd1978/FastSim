/* Class: PacMeasurementFactory
 *
 * A factory utility class to produce transient measurement devices
 * for the current configuration.
 */
#ifndef PacMeasurementFactory_HH
#define PacMeasurementFactory_HH

#include <map>
#include <string>

class PacMeasurement;

class PacMeasurementFactory {

public:

    /// Get a measurement for the specified name

    static PacMeasurement* get( const std::string& name );

private:

    /// The singleton

    static PacMeasurementFactory& instance();

    /// No public c-tor because it's a singleton

    PacMeasurementFactory();

    /// No public d-tor because it's a singleton

    virtual ~PacMeasurementFactory();

    /// Get a measurement for the specified name (Actuall implmentation)

    PacMeasurement* doGet( const std::string& name );

    /// Do the actual initialization separatedly from the c-tor

    void init();

private:

    bool _tried2init;
    std::map<std::string, PacMeasurement* > _name2measure;
};

#endif // PacMeasurementFactory_HH
