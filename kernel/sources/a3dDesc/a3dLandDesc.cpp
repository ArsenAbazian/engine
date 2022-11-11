#include "a3dLandDesc.h"
#include "..\\a3dObjectInfo\\a3dLandscapeInfo.h"
#include "..\\a3dScene.h"
#include "..\\landscape.h"

a3dLandscapeDesc::~a3dLandscapeDesc() { 
	if( this->landInfo != null ) 
		delete landInfo;
}

int a3dLandscapeDesc::calcSizeCore() { 

	return a3dObjectDescBase::calcSizeCore() + this->landInfo->calcSize();
}

bool a3dLandscapeDesc::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dObjectDescBase::saveToMemoryCore( file ) ) return false;
	if( !this->landInfo->saveToMemory( file ) ) return false;

	return true;
}

bool a3dLandscapeDesc::loadFromMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dObjectDescBase::loadFromMemoryCore( file ) ) return false;
	if( !this->landInfo->loadFromMemory( file ) ) return false;

	return true;
}

HRESULT a3dLandscapeDesc::loadLand( a3dScene *scene ) { 

	HRESULT rv;

	this->kernel = scene->kernel;

	rv = landInfo->loadLand( scene );
	this->land = this->landInfo->land;
	this->land->setScene( scene );

	return rv;
}