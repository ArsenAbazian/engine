#include "a3dObjectInfoBase.h"
#include "memoryFile.h"

void a3dObjectInfoBase::copyTo( a3dObjectInfoBase *obj ) { 

	obj->name = this->name;
}

int a3dObjectInfoBase::calcSizeCore() { 

	return this->name.calcSize();
}

bool a3dObjectInfoBase::saveToMemoryCore(a3dMemoryFile &file) { 

	return this->name.saveToMemory( file );
}

bool a3dObjectInfoBase::loadFromMemoryCore(a3dMemoryFile &file) {

	return this->name.loadFromMemory( file );
}
