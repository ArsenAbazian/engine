#ifndef A3D_RAIL_DESC_INC
#define A3D_RAIL_DESC_INC

#include "a3dDescBase.h"
#include "3dtypes.h"

class a3dRailSystemInfo;
class a3dRailManager;
class a3dScene;
class a3dRailSystemDesc : public a3dObjectDescBase { 
public:
	a3dRailSystemInfo		*rsInfo;
	a3dRailManager			*rSystem;

	a3dRailSystemDesc() { 
		this->rsInfo = null;
		this->rSystem = null;
	}
	~a3dRailSystemDesc();

	int		getHeaderCount() { return a3dHeaders::a3dRailSystemDescHeader; }
	int		calcSizeCore();
	bool	loadFromMemoryCore( a3dMemoryFile &file );
	bool	saveToMemoryCore( a3dMemoryFile &file );

	HRESULT loadRail( a3dScene *scene );
};



#endif