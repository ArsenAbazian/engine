#ifndef A3D_LANDSCAPE_DESC_INC
#define A3D_LANDSCAPE_DESC_INC

#include "a3dDescBase.h"
#include "3dtypes.h"

class a3dLandscape;
class a3dLandscapeInfo;
class a3dScene;

class a3dLandscapeDesc : public a3dObjectDescBase { 
public:
	a3dLandscapeInfo		*landInfo;
	a3dLandscape			*land;

	a3dLandscapeDesc() { 
		this->land = null;
		this->landInfo = null;
	}
	~a3dLandscapeDesc();

	int		getHeaderCount() { return a3dHeaders::a3dLandDescHeader; }
	int		calcSizeCore();
	bool	loadFromMemoryCore( a3dMemoryFile &file );
	bool	saveToMemoryCore( a3dMemoryFile &file );

	HRESULT loadLand( a3dScene *scene );
};


#endif