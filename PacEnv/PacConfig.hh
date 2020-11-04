#ifndef PacConfig_hh
#define PacConfig_hh

#include <map>
#include <vector>
#include <iosfwd>
#include <string>

#include "CLHEP/Geometry/HepPoint.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "PacEnv/pstring.hh"

class EdmlParser;
class EdmlDetector;
class EdmlMeasuringDevice;

/* Class: PacConfig
 *
 * Maintains a table of configuration entries. */

class PacConfig {
public:
    PacConfig();
    ~PacConfig();

    /* Basic accessor functions */
    bool has(const pstring& key) const;
    pstring operator[](const pstring& key) const;
    pstring get(const pstring& key, pstring def = "") const;
    void set(const pstring& key, pstring value);
	void verbose(bool verb = false){_verbose = verb;}

    /* Type-specific accessor functions */
    const char*         getcstr(const pstring& key, const char* def = "") const;
    bool                getbool(const pstring& key, bool def = false) const;
    int                 getint(const pstring& key, int def = 0) const;
    long                getlong(const pstring& key, long def = 0) const;
    float               getfloat(const pstring& key, float def = 0.0) const;
    double              getdouble(const pstring& key, double def = 0.0) const;
    std::vector<double> getvector(const pstring& key, const std::vector<double>& def = std::vector<double>()) const;
    std::vector<int> getvector(const pstring& key, const std::vector<int>& def) const;
    std::vector<unsigned> getvector(const pstring& key, const std::vector<unsigned>& def) const;
    std::vector<std::string> getvector(const pstring& key, const std::vector<std::string>& def) const;
    HepPoint            getHepPoint(const pstring& key, const HepPoint& def = HepPoint(0,0,0)) const;
    Hep3Vector          getHep3Vector(const pstring& key, const Hep3Vector& def = Hep3Vector(0,0,0)) const;
    void                set(const pstring& key, bool value);
    void                set(const pstring& key, long value);
    void                set(const pstring& key, double value);
    void                set(const pstring& key, const std::vector<double>& value);
    void                set(const pstring& key, const HepPoint& value);
    void                set(const pstring& key, const Hep3Vector& value);

    /* EDML-specific accessor functions */
    const EdmlDetector* edml_detector() const;

    void edml_devices(std::vector<EdmlMeasuringDevice >& theDevices) const;

    /* Extract a subset of the configuration table (those whose keys begin with 'prefix'). */
    PacConfig getconfig(const pstring& prefix, bool stripprefix = true) const;

    /* Parse a single configuration line */
	void parseline(pstring line);
    
    /* Parse a configuration file */
    void parsefile(pstring filename);

    /* Parse command line arguments, leaving behind any unrecognized options */
	void parseargs(int& argc, char** argv);

    /* Extend this configuration table by merging it with another */
    void merge(const PacConfig& other, const pstring& prefix = "");

    /* Clear a single configuration entry */
    void unset(const pstring& key);

    /* Clear the entire configuration table */
    void clear();
    
    void dump() const;
    
    /* Iteration support */
    typedef std::map<pstring, pstring>::iterator iterator;
    typedef std::map<pstring, pstring>::const_iterator const_iterator;
    iterator       begin() { return entries.begin(); }
    const_iterator begin() const { return entries.begin(); }
    iterator       end()   { return entries.end(); }
    const_iterator end()   const { return entries.end(); }

private:
    /* Parse an EDML configuration file */
    void parsefile_edml(pstring filename);

protected:
    /* PacConfiguration entries */
    std::map<pstring, pstring> entries;

    /* EDML container */
    EdmlParser* _edml_parser;

    /* Temporary variables for parsing */
    std::vector<pstring> groupstack;
    pstring groupprefix;
	bool _verbose;
};

/* Global configuration table */
extern PacConfig gconfig;

/* Print the entire table to file */
std::ostream& operator<<(std::ostream& os, const PacConfig& config);

#endif // CONFIG_HH
