#include "a3dRailDesc.h"
#include "..\\a3dObjectInfo\\a3dRailSystemInfo.h"
#include "..\\railManager.h"
#include "..\\a3dScene.h"

a3dRailSystemDesc::~a3dRailSystemDesc() { 
	if( this->rsInfo != null )
		delete this->rsInfo;
	if( this->rSystem != null )
		delete this->rSystem;
}

int a3dRailSystemDesc::calcSizeCore() { 

	return a3dObjectDescBase::calcSizeCore() + this->rsInfo->calcSize();
}

bool a3dRailSystemDesc::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dObjectDescBase::saveToMemoryCore( file ) ) return false;
	if( !this->rsInfo->saveToMemory( file ) ) return false;

	return true;
}

bool a3dRailSystemDesc::loadFromMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dObjectDescBase::loadFromMemoryCore( file ) ) return false;
	if( !this->rsInfo->loadFromMemory( file ) ) return false;

	return true;
}

HRESULT a3dRailSystemDesc::loadRail( a3dScene *scene ) { 

	HRESULT rv;

	this->kernel = scene->kernel;

	rv = this->rsInfo->loadRail( scene );
	this->rSystem = this->rsInfo->rSystem;

	return rv;
}