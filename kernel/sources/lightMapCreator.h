#ifndef A3D_LIGHTMAP_CREATOR
#define A3D_LIGHTMAP_CREATOR

#include "3dtypes.h"

class a3dLightMap;
class a3dLightMapCreator { 

public:
	virtual a3dLightMap** getLightMaps() { return null; }
	virtual int			  getLightMapCount() { return 0; }
};

#endif