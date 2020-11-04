#include "BaBar/BaBar.hh"
#include "PacEnv/PacConfig.hh"
#include "PacEnv/EdmlParser.hh"
#include "PacEnv/EdmlDetector.hh"
#include "PacEnv/EdmlMeasuringDevice.hh"
#include "Framework/AppFileName.hh"

#include <string.h>
#include <assert.h>

#include <fstream>
#include <iostream>

using namespace std;

/* Global configuration table */
PacConfig gconfig;
bool verbose;


/*******************************************************************************
 * PacConfig
*******************************************************************************/

PacConfig::PacConfig() : _edml_parser(0) {
}

PacConfig::~PacConfig() {
    delete _edml_parser;
    _edml_parser=0;
}

bool PacConfig::has(const pstring& key) const {
    const_iterator iter = entries.find(key);
    return (iter != end());
}

pstring PacConfig::operator[](const pstring& key) const {
    return get(key);
}

pstring PacConfig::get(const pstring& key, pstring def) const {
    const_iterator iter = entries.find(key);
    return (iter != end()) ? iter->second : def;
}

const char* PacConfig::getcstr(const pstring& key, const char* def) const {
    return has(key) ? get(key).c_str() : def;
}

bool PacConfig::getbool(const pstring& key, bool def) const {
    return has(key) ? bool(get(key)) : def;
}

int PacConfig::getint(const pstring& key, int def) const {
    return has(key) ? int(get(key)) : def;
}

long PacConfig::getlong(const pstring& key, long def) const {
    return has(key) ? long(get(key)) : def;
}

float PacConfig::getfloat(const pstring& key, float def) const {
    return has(key) ? float(get(key)) : def;
}

double PacConfig::getdouble(const pstring& key, double def) const {
    return has(key) ? double(get(key)) : def;
}

vector<double> PacConfig::getvector(const pstring& key, const vector<double>& def) const {
    vector<double> v;
    const_iterator iter = entries.find(key);
    if(iter != entries.end()) {
        pstring value = iter->second.strip();
        if(value.startswith('(') && value.endswith(')'))
            value = value.slice(1, -1);
        vector<pstring> ss;
        if(value.contains(','))
            ss = value.split(',');
        else
            ss = value.split();
        v.resize(ss.size());
        for(unsigned i = 0; i < ss.size(); i++)
            v[i] = double(ss[i]);
        return v;
    }
    else
        return def;
    return v;
}
vector<int> PacConfig::getvector(const pstring& key, const vector<int>& def) const {
    vector<int> v;
    const_iterator iter = entries.find(key);
    if(iter != entries.end()) {
        pstring value = iter->second.strip();
        if(value.startswith('(') && value.endswith(')'))
            value = value.slice(1, -1);
        vector<pstring> ss;
        if(value.contains(','))
            ss = value.split(',');
        else
            ss = value.split();
        v.resize(ss.size());
        for(unsigned i = 0; i < ss.size(); i++)
            v[i] = int(ss[i]);
        return v;
    }
    else
        return def;
    return v;
}

vector<unsigned> PacConfig::getvector(const pstring& key, const vector<unsigned>& def) const {
    vector<unsigned> v;
    const_iterator iter = entries.find(key);
    if(iter != entries.end()) {
        pstring value = iter->second.strip();
        if(value.startswith('(') && value.endswith(')'))
            value = value.slice(1, -1);
        vector<pstring> ss;
        if(value.contains(','))
            ss = value.split(',');
        else
            ss = value.split();
        v.resize(ss.size());
        for(unsigned i = 0; i < ss.size(); i++)
            v[i] = int(ss[i]);
        return v;
    }
    else
        return def;
    return v;
}

vector<string>
PacConfig::getvector(const pstring& key, const vector<string>& def) const {
    vector<string> v;
    const_iterator iter = entries.find(key);
    if(iter != entries.end()) {
        pstring value = iter->second.strip();
        if(value.startswith('(') && value.endswith(')'))
            value = value.slice(1, -1);
        vector<pstring> ss;
        if(value.contains(','))
            ss = value.split(',');
        else
            ss = value.split();
        v.resize(ss.size());
        for(unsigned i = 0; i < ss.size(); i++)
            v[i] = string(ss[i]);
        return v;
    }
    else
        return def;
    return v;
}

HepPoint PacConfig::getHepPoint(const pstring& key, const HepPoint& def) const {
    vector<double> v = getvector(key);
    if(v.size() == 0)
        return def;
    else if(v.size() == 3)
        return HepPoint(v[0], v[1], v[2]);
    else {
        cerr << "Error: PacConfig::getHepPoint: invalid value: " << get(key) << endl;
        return HepPoint();
    }
}

Hep3Vector PacConfig::getHep3Vector(const pstring& key, const Hep3Vector& def) const {
    vector<double> v = getvector(key);
    if(v.size() == 0)
        return def;
    else if(v.size() == 3)
        return Hep3Vector(v[0], v[1], v[2]);
    else {
        cerr << "Error: PacConfig::getHep3Vector: invalid value: " << get(key) << endl;
        return Hep3Vector();
    }
}

void PacConfig::set(const pstring& key, pstring s) {
    entries[key] = s;
}

void PacConfig::set(const pstring& key, bool b) {
    set(key, str(b));
}

void PacConfig::set(const pstring& key, long l) {
    set(key, str(l));
}

void PacConfig::set(const pstring& key, double d) {
    set(key, str(d));
}

void PacConfig::set(const pstring& key, const vector<double>& v) {
    pstring s;
    unsigned int N = v.size();
    for(unsigned int i = 0; i < N; i++)
        s += str(v[i]) + ((i < N - 1) ? ", " : "");
    set(key, s);
}

void PacConfig::set(const pstring& key, const HepPoint& p) {
    pstring s = str(p.x()) + ", " + str(p.y()) + ", " + str(p.z());
    set(key, s);
}

void PacConfig::set(const pstring& key, const Hep3Vector& v) {
    pstring s = str(v.x()) + ", " + str(v.y()) + ", " + str(v.z());
    set(key, s);
}

PacConfig PacConfig::getconfig(const pstring& prefix, bool stripprefix) const {
    PacConfig c;
    for(const_iterator iter = begin(); iter != end(); iter++) {
        const pstring& key = iter->first;
        const pstring& value = iter->second;
        if(key.startswith(prefix)) {
            if(stripprefix)
                c.set(key.slice(prefix.length()), value);
            else
                c.set(key, value);
        }
    }
    return c;
}

const EdmlDetector* PacConfig::edml_detector() const {
    if(!_edml_parser) return 0; // no EDML parsing attempt made before calling the method
    return _edml_parser->detector();
}

void PacConfig::edml_devices(std::vector<EdmlMeasuringDevice >& theDevices) const
{
    assert(_edml_parser);   // no EDML parsing attempt made before calling the method
    _edml_parser->devices(theDevices);
}

void PacConfig::parseline(pstring line) {
    line.strip(true);

    /* First check for a comment line */
    if(line.empty() || line.startswith('#'))
        return;

    /* Secondly, check if it's an include statement */
    if(line.startswith("include")) {      
        pstring files = line.slice(8).strip().stripquotes();
        static const char* sep(",");
        std::vector<pstring> fvect = files.split(sep[0]);
        for(int ifile=0;ifile<fvect.size();ifile++)
          parsefile(fvect[ifile]);
        return;
    }

    /* Next look for an equals sign, indicating an assignment statement */
    int n = line.find('=');
    if(n != string::npos) {
		if(_verbose) {
			cout << line << endl;
		}
        pstring key = groupprefix + line.slice(0, n).strip();
        pstring value = line.slice(n + 1).strip().stripquotes();
        set(key, value);
        return;
    }

    /* Otherwise, see if this is a group declaration */
    if(line.endswith('{')) {
		if(_verbose) {
			cout << line << endl;
		}
        pstring newgroup = line.slice(0, -1).strip();
        groupstack.push_back(newgroup);
        groupprefix += newgroup + '.';
        return;
    }

    /* Or the end of a group */
    if(line.startswith('}') && line.length() == 1) {
        if(groupstack.empty()) {
            cerr << "Warning: PacConfig.parseline(): not in a group: " << line << '\n';
		} else {
			if(_verbose) {
				cout << line << endl;
			}
            groupstack.pop_back();
		}

        /* Update the working group prefix */
        groupprefix = "";
        for(unsigned n = 0; n < groupstack.size(); n++)
            groupprefix += groupstack[n] + '.';
        return;
    }

    /* If we make it here, the line is invalid */
    cerr << "Error: PacConfig::parseline(): invalid config line: " << line << '\n';
}

void PacConfig::parsefile(pstring filename) {
      
    ifstream fin(AppFileName(filename.c_str()).pathname().c_str());
    if(!fin) {
        cerr << "Error: PacConfig::parsefile(): could not open file: " << filename << '\n';
        return;
    }

    // Check to see if an EDML file is requested
    //
    const char* ext=strrchr(filename, '.');
    if((0 != ext ) && !strcmp(ext, ".xml")) {
        this->parsefile_edml(filename);
        return;
    }

    // Otherwise proceed with the old style configuration file format

    /* Read the file line by line */
    pstring line;
    while(!fin.eof()) {
        getline(fin, line);
        parseline(line);
    }

    /* Make sure all the groups were properly closed */
    if(!groupstack.empty())
        cerr << "Warning: PacConfig::parsefile(): not all groups were closed: " << groupprefix << '\n';
}

void PacConfig::parsefile_edml(pstring filename) {
    if(!_edml_parser)
        _edml_parser = new EdmlParser(_verbose);

    if (!_edml_parser->parse(AppFileName(filename.c_str()).pathname().c_str())) {
        cerr << "Error: PacConfig::parsefile_edml(): failed to parse file: " << filename << '\n';
        return;
    }

    // Also transfer configuration parameters (if any) into the local dictionary
    //
    std::map<std::string,                           // name
             std::pair<std::string,                 // type
                       std::string > > parameters;  // value
    _edml_parser->config(parameters);

    for( std::map<std::string,
                  std::pair<std::string,
                            std::string > >::const_iterator itr=parameters.begin(); itr != parameters.end(); ++itr) {
        pstring key = itr->first;
        key.strip();
        pstring value = itr->second.second;
        value.strip().stripquotes();
        this->set(key, value);
    }
}

void PacConfig::parseargs(int& argc, char** argv) {    
		/* Parse all arguments that look like configuration statements */
	int newargc = 1;
	char* newargv[argc];

    for(int i = 1; i < argc; i++) {
		bool argpass = true;
		pstring arg = argv[i];
        if(!arg.startswith('-')) {						
			if(arg == "include") {//check if include command is called
				if(i+1 == argc) {
					cerr << "Error: PacConfig::parseargs(): do not end argument with include" << endl;
					argpass = false;
				} else {
					arg.append(" ");
					arg.append(argv[i+1]);
					i++; //skip entry after "include"
				}
			}
			//else if(arg.endswith('.cfg')) {//check for .cfg file, add "include "
			//	arg.insert(0,"include ");			
			//}		
			else if(arg.contains('=')) {//check for '=' sign, combine entry before and after '=' sign
				if(arg.endswith('=')) {// "=" or "***="
					if(arg.startswith('=')) { //"="
						arg.insert(0," ");
						arg.insert(0,argv[i-1]);
						newargc--;
					}
					if(i+1 == argc) {//Check what comes after "*** ="
						cerr << "Error: PacConfig::parseargs(): do not end argument with = " << endl;
						argpass = false;
					} else {
						arg.append(" ");
						arg.append(argv[i+1]);	
						i++;						
						bool entry = true;
						while(entry) {
							if(arg.endswith(',')) {
								if(i+1 == argc) {
									cerr << "Error: PacConfig::parseargs(): do not end argument with , " << endl;
									argpass = false;
								} else {
									arg.append(" ");
									arg.append(argv[i+1]);	
									i++;
								}
							} else {
								entry = false;
							}
						}//while(entry)					
					}
				} else { // "=***" or "***=***"
					if(arg.startswith('=')) { //"=***"
						arg.insert(0," ");
						arg.insert(0,argv[i-1]);
						newargc--;
					}
					//Check what comes after "*** = ***"
					bool entry = true;
					while(entry) {
						if(arg.endswith(',')) {
							if(i+1 == argc) {
								cerr << "Error: PacConfig::parseargs(): do not end argument with , " << endl;
								argpass = false;
							} else {
								arg.append(" ");
								arg.append(argv[i+1]);	
								i++;
							}
						} else {
							entry = false;
						}
					}//while(entry)
				}
			}//arg.contains('=')			
			else if(arg.contains('{')) {//Check for "{" for beginning of grouped entry
				if(arg.endswith('{')) {// "{" or "***{"
					if(arg.startswith('{')) {// "{"
						arg.insert(0," ");
						arg.insert(0,argv[i-1]);
						newargc--;
					}
				} else {// "{***" or "***{***"
					if(arg.startswith('{')) {// "{***"
						arg.insert(0," ");
						arg.insert(0,argv[i-1]);
						newargc--;
					}
				}
				argpass = true;
			}
			else if(arg.contains('}')) {//Check for "}" for beginning of grouped entry
				argpass = true;
			}
			else {//If it gets here, its not likely config argument
				argpass = false;
				newargv[newargc] = argv[i];
				newargc++;
			}			
			if(argpass) {//Pass only argument that passes check
				parseline(arg);			
			}
        }//!arg.startswith('-')
		
		//entry with '-'
		else {
			newargv[newargc] = argv[i];
			newargc++;
		}
    }//for()

    /* Now update the argument list */
    for(int j = 1; j < newargc; j++) {
		argv[j] = newargv[j];
    }
	argc = newargc;
	for(int i = newargc; i < argc; i++) {
		argv[i] = 0;
	}
	
	return;
}

void PacConfig::merge(const PacConfig& c, const pstring& prefix) {
    for(const_iterator iter = c.begin(); iter != c.end(); iter++)
        set(iter->first, prefix + iter->second);
}

void PacConfig::unset(const pstring& key) {
    entries.erase(key);
}

void PacConfig::clear() {
    entries.clear();
}

ostream& operator<<(ostream& os, const PacConfig& config) {
    PacConfig::const_iterator iter = config.begin();
    while(iter != config.end()) {
        os << iter->first << " = " << iter->second << '\n';
        iter++;
    }
    return os;
}

void
PacConfig::dump() const {
  if(_edml_parser != 0) _edml_parser->dump();  
}
