#include "a3dSoundDesc.h"
#include "..\\wave.h"

a3dSoundDesc::~a3dSoundDesc() { 
	if( this->sound != null )
		delete this->sound;
}

int a3dSoundDesc::calcSizeCore() { 

	return a3dObjectDescBase::calcSizeCore() + this->fileName.calcSize(); 
}

bool a3dSoundDesc::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dObjectDescBase::saveToMemoryCore( file ) ) return false;
	if( !this->fileName.saveToMemory( file ) ) return false;

	return true;
}

bool a3dSoundDesc::loadFromMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dObjectDescBase::loadFromMemoryCore( file ) ) return false;
	if( !this->fileName.loadFromMemory( file ) ) return false;

	return true;
}