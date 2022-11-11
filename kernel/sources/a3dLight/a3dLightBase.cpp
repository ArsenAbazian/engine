#include "a3dLightBase.h"
#include "..\\a3dTextFileTokens.h"
#include "memoryFile.h"

bool a3dLightBase::import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringValue( li, this->name ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readColorParam( lexList, li, a3dTextFileTokens::a3dcolorString, this->color ) ) return false;

	return true;
}

void a3dLightBase::initializeDefault() { 

	this->type = LIGHT_NONE;
	clearVector3( this->color );
}

void a3dLightBase::clear() { 

	this->name.clear();
} 

a3dLightBase* a3dLightBase::clone() { 

	a3dLightBase *res = new a3dLightBase();	
	this->copyTo(res);
	return res;
}

void a3dLightBase::copyTo( a3dLightBase *light ) {

	light->name = this->name;
	light->type = this->type;
	copyVector3(this->color, light->color);
}

int a3dLightBase::calcSizeCore() { 
	return sizeof(int) + this->name.calcSize( ) + sizeof( Vector3 ); 
}

bool a3dLightBase::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !file.writeInt( (int) this->type ) ) return false;
	if( !this->name.saveToMemory( file ) ) return false;
	if( !file.writeVector3( this->color ) ) return false;

	return true;
}

bool a3dLightBase::loadFromMemoryCore( a3dMemoryFile &file ) { 

	int tp;

	if( !file.readInt( &tp ) ) return false;
	this->type = (a3dLightType)tp;

	if( !this->name.loadFromMemory( file ) ) return false;
	if( !file.readVector3( this->color ) ) return false;

	return true;
}
