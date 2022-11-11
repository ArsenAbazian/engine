#include "a3dOmniLight.h"
#include "memoryFile.h"
#include "..\\a3dTextFileTokens.h"

a3dLightBase* a3dOmniLight::clone() { 

	a3dOmniLight *res = new a3dOmniLight();	
	this->copyTo(res);
	return res;
}

void a3dOmniLight::copyTo( a3dLightBase *light ) {

	a3dOmniLight *omni = dynamic_cast<a3dOmniLight*>(light); 
	a3dLightBase::copyTo(light);
	copyVector3( this->pos, omni->pos );
	omni->farAttenStart = this->farAttenStart;
	omni->farAttenEnd = this->farAttenEnd;
}

int a3dOmniLight::calcSizeCore() { 
	return a3dLightBase::calcSizeCore() + sizeof( Vector3 ) + sizeof( float ) + sizeof( float ); 
}

bool a3dOmniLight::saveToMemoryCore(a3dMemoryFile &file) { 

	if( !a3dLightBase::saveToMemoryCore( file ) ) return false;
	if( !file.writeVector3( this->pos ) ) return false;
	if( !file.writeFloat( this->farAttenStart ) ) return false;
	if( !file.writeFloat( this->farAttenEnd ) ) return false;

	return true;
}

bool a3dOmniLight::loadFromMemoryCore(a3dMemoryFile &file) { 

	if( !a3dLightBase::loadFromMemoryCore( file ) ) return false;
	if( !file.readVector3( this->pos ) ) return false;
	if( !file.readFloat( &this->farAttenStart ) ) return false;
	if( !file.readFloat( &this->farAttenEnd ) ) return false;

	return true;
}

void a3dOmniLight::initializeDefault() { 

	a3dLightBase::initializeDefault();
	this->type = LIGHT_OMNI;
	clearVector3( this->pos );
	this->farAttenStart = 0.0f;
	this->farAttenEnd = 0.0f;
}

void a3dOmniLight::clear() { 

	a3dLightBase::clear();
	this->type = LIGHT_OMNI;
	clearVector3( this->pos );
	this->farAttenStart = 0.0f;
	this->farAttenEnd = 0.0f;
}

bool a3dOmniLight::import(std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 

	if( !a3dLightBase::import( lexList, li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readVertexParam( lexList, li, a3dTextFileTokens::a3dPositionString, this->pos ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatParam( lexList, li, a3dTextFileTokens::a3dFarAttenStartString, &this->farAttenStart ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatParam( lexList, li, a3dTextFileTokens::a3dFarAttenEndString, &this->farAttenEnd ) ) return false;

	// swap y and z
	swapFloat( this->pos[1], this->pos[2] );

	return true;
}