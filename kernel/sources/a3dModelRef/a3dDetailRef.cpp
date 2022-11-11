#include "a3dDetailRef.h"
#include "..\\a3dModel\\a3dDetail.h"
#include "..\\a3dModel\\a3dObject.h"
#include "..\\kernel.h"
#include "3dtypes.h"
#include "..\\a3dModelRef\\a3dObjectRef.h"

void a3dDetailRef::clear() { 

	this->objectRef = null;
	this->detail = null;
}

void a3dDetailRef::initializeDefault() { 

	this->objectRef = null;
	this->detail = null;
}

void a3dDetailRef::initialize( a3dObjectRef *objectRef, a3dDetail *detail ) { 

	this->objectRef = objectRef;
	this->detail = detail;
	this->lightMapInfo.assign( this->detail->lightMapInfo );
	this->material.assign( this->detail->material );
}

a3dLightMap* a3dDetailRef::getLightMap( processLightMapPlaneDeletate fp, void *lpData ) { 
	
	return this->detail->getLightMap( fp, lpData, &this->lightMapInfo, this->objectRef->basis );
}
