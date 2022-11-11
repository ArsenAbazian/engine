#ifndef A3D_DETAIL_REF_INC
#define A3D_DETAIL_REF_INC

#include "..\\lightMapInfo.h"
#include "..\\a3dMaterial\\a3dShaderMaterial.h"
#include "..\\delegate.h"

class a3dDetail;
class a3dObjectRef;
class a3dLightMap;

class a3dDetailRef { 
public:
	a3dLightMapInfo		lightMapInfo;
	a3dObjectRef		*objectRef;
	a3dDetail			*detail;
	a3dShaderMaterial	material;

	a3dDetailRef() { this->initializeDefault(); }

	a3dDetailRef( a3dObjectRef *objectRef, a3dDetail *detail ) { 
		this->initializeDefault();
		this->initialize( objectRef, detail ); 
	}

	~a3dDetailRef() { this->clear(); }

	void	initializeDefault();
	void	initialize( a3dObjectRef *objectRef, a3dDetail *detail );
	void	clear();

	// lightmap functions
	bool	hasLightMap() { return lightMapInfo.enabled; }
	a3dLightMap* getLightMap( processLightMapPlaneDeletate fp, void *lpData );
};

#endif