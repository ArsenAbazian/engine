#include "a3dDescBase.h"

int a3dObjectDescBase::calcSizeCore() { 

	return this->name.calcSize();
}

bool a3dObjectDescBase::saveToMemoryCore(a3dMemoryFile &file) { 

	if( !this->name.saveToMemory( file ) ) return false;
	return true;
}

bool a3dObjectDescBase::loadFromMemoryCore(a3dMemoryFile &file) { 

	if( !this->name.loadFromMemory( file ) ) return false;
	return true;
}
