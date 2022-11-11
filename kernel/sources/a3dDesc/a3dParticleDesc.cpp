#include "a3dParticleDesc.h"
#include "..\\a3dObjectInfo\\a3dParticleInfo.h"
#include "..\\a3dScene.h"
#include "..\\a3dParticleSystem\\particles.h"

a3dParticleSystemDesc::~a3dParticleSystemDesc() { 
	/*if( this->psInfo != null ) 
		delete this->psInfo;*/
	if( this->pSystem != null )
		delete this->pSystem;
}

particleSystemBase* a3dParticleSystemDesc::createParticle(	a3dParticleType type ) { 
	
	switch( type ) { 
		case ptSmoke:
			return new a3dSmokeParticleSystem();
	}

	return null;
}

int a3dParticleSystemDesc::calcSizeCore() { 

	return a3dObjectDescBase::calcSizeCore() + this->pSystem->calcSize();
	//return a3dObjectDescBase::calcSizeCore() + this->psInfo->calcSize();
}

bool a3dParticleSystemDesc::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dObjectDescBase::saveToMemoryCore( file ) ) return false;
	if( !this->pSystem->saveToMemory(file) ) return false;
	//if( !this->psInfo->saveToMemory( file ) ) return false;

	return true;
}

bool a3dParticleSystemDesc::loadFromMemoryCore( a3dMemoryFile &file ) { 

	int tp;

	if( !a3dObjectDescBase::loadFromMemoryCore( file ) ) return false;
	file.savePos();
	if( !file.readInt(&tp) ) return false;
	this->pSystem = this->createParticle( (a3dParticleType)tp );
	this->pSystem->registerDependencyProperties();
	file.restorePos();
	if( !this->pSystem->loadFromMemory(file) ) return false;
	//if( !this->psInfo->loadFromMemory( file ) ) return false;

	return true;
}

HRESULT a3dParticleSystemDesc::loadParticle( a3dScene *scene ) { 

	HRESULT rv;

	this->kernel = scene->kernel;
	rv = this->pSystem->initialize();
	//rv = this->psInfo->loadParticle( scene );
	//this->pSystem = this->psInfo->pSystem;

	return rv;
}