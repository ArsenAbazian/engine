#include "a3dMaterialBase.h"
#include "memoryFile.h"

a3dMaterialBase* a3dMaterialBase::clone() { return null; }

int a3dMaterialBase::calcSizeCore() { 
	return sizeof(int) + this->name.calcSize(); 
}

bool a3dMaterialBase::loadFromMemoryCore( a3dMemoryFile &file ) { 

	int tp = 0;

	if( !file.readInt( &tp ) ) return false;

	if( !this->name.loadFromMemory( file ) ) return false;
	return true;
}

bool a3dMaterialBase::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !file.writeInt( (int) this->type ) ) return false;
	if( !this->name.saveToMemory( file ) ) return false;

	return true; 
}

void a3dMaterialBase::clear() { 

	this->name.clear();
}

bool a3dMaterialBase::import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 

	return true;
}

bool a3dMaterialBase::initializeTextures( a3dTextureDesc* textures, int count ) {	
	return true; 
}

bool a3dMaterialBase::initializeEffects( a3dEffectDesc* effects, int count ) { 
	return true;
}

HRESULT a3dMaterialBase::initializeData(a3dScene *scene) { 

	return D3D_OK;
}
