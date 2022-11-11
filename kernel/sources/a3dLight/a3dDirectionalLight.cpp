#include "a3dDirectionalLight.h"
#include "memoryFile.h"
#include "..\\a3dTextFileTokens.h"

void a3dDirectionalLight::initializeDefault() { 

	a3dLightBase::initializeDefault();
	this->type = LIGHT_DIRECTIONAL;
	clearVector3( this->dir );
}

void a3dDirectionalLight::clear() { 

	a3dLightBase::clear();
	this->type = LIGHT_DIRECTIONAL;
	clearVector3( this->dir );
}

int a3dDirectionalLight::calcSizeCore() { 
	return a3dLightBase::calcSizeCore() + sizeof( Vector3 );
}

bool a3dDirectionalLight::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dLightBase::saveToMemoryCore( file ) ) return false;
	if( !file.writeVector3( this->dir ) ) return false;
	return true;
}

bool a3dDirectionalLight::loadFromMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dLightBase::loadFromMemoryCore( file ) ) return false;
	if( !file.readVector3( this->dir ) ) return false;
	return true;
}

bool a3dDirectionalLight::import(std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 

	if( !a3dLightBase::import( lexList, li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readVertexParam( lexList, li, a3dTextFileTokens::a3dDirectionString, this->dir ) ) return false;

	return true;
}

a3dLightBase* a3dDirectionalLight::clone() { 

	a3dDirectionalLight *res = new a3dDirectionalLight();	
	this->copyTo(res);
	return res;
}

void a3dDirectionalLight::copyTo( a3dLightBase *light ) {

	a3dDirectionalLight *dirLight = dynamic_cast<a3dDirectionalLight*>(light); 
	a3dLightBase::copyTo(light);
	copyVector3(this->dir, dirLight->dir);
}