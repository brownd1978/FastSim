#ifndef PacBuildEnv_hh
#define PacBuildEnv_hh

#include "BaBar/BaBar.hh"

class TrkEnv;
class GenEnv;
class BField;
class DetMaterial;
class MaterialInfo;
class KalContext;
class TrkVolume;
// must include the following to deal with namespace issues
#include "CLHEP/Random/RandomEngine.h"
#include <string>

// I'm calling this class TrkBuildCoreEnv so that it can exploit
// an existing friendship relationship with TrkEnv.  This needs to be changed
// in future!!
class TrkBuildCoreEnv {
public:
    TrkBuildCoreEnv();
    ~TrkBuildCoreEnv();

		void buildTrk();
		void buildCore();

		const TrkEnv* getTrkEnv() const { return _trkenv;}
		const BField* getBField() const { return _bfield;}
		const KalContext* getKalContext() const { return _context;}
protected:
    TrkEnv* _trkenv;
		GenEnv* _genenv;
		BField* _bfield;
		MaterialInfo* _matinfo;
		KalContext* _context;
    TrkVolume* _inner_volume;
    TrkVolume* _outer_volume;
    HepRandomEngine* _rng;
};

typedef TrkBuildCoreEnv PacBuildEnv;

#endif // PacBuildEnv_hh
