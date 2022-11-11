#ifndef A3D_OBJECT_REF_INC
#define A3D_OBJECT_REF_INC

#include "..\\vecmat.h"
#include "..\\delegate.h"

class a3dModelRef;
class a3dObject;
class a3dDetailRef;
class a3dLightMap;

class a3dObjectRef { 
public:
	a3dModelRef		*modelRef;
	a3dObject		*object;

	a3dDetailRef	**detailRef;
	int				detailCount;

	Matrix4		basis;
	Matrix4		invertBasis;
	float		scaleX;
	float		scaleY;
	float		scaleZ;

	a3dObjectRef() { this->initializeDefault(); }

	a3dObjectRef( a3dModelRef *modelRef, a3dObject *object ) { 
		this->initializeDefault();
		this->initialize( modelRef, object ); 
	}

	~a3dObjectRef() { this->clear(); }

	void initializeDefault();
	void initialize( a3dModelRef *modelRef, a3dObject *object );
	void clear();

	// apply basis before rendering
	void applyBasis();

	// lightMap functions
	int getLightMapCount();
	void getLightMaps( a3dLightMap** llList, processLightMapPlaneDeletate fp, void *lpData );
};

#endif