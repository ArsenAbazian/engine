#include "a3dModelObjectInfo.h"
#include "a3dDetailInfo.h"
#include "..\\a3dModelRef\\a3dObjectRef.h"

void a3dObjectInfo::initializeDefault() { 
	
	this->detailInfo = null;
	this->detailInfoCount = 0;
}

void a3dObjectInfo::clear() { 
	
	if(this->detailInfo != null)
		delete[] this->detailInfo;
	this->detailInfo = null;
	this->detailInfoCount = 0;
}

int a3dObjectInfo::calcSizeCore() { 
	
	int detSize = 0;
	for(int i = 0; i < this->detailInfoCount; i++) 
		detSize += this->detailInfo[i].calcSize();
	
	return a3dVisualObjectInfoBase::calcSizeCore() + sizeof(int) + detSize;
}

bool a3dObjectInfo::saveToMemoryCore(a3dMemoryFile &file) {

	if(!a3dVisualObjectInfoBase::saveToMemoryCore(file)) return false;
	if(!file.writeInt(this->detailInfoCount)) return false;
	for(int i = 0; i < this->detailInfoCount; i++)
		if(!this->detailInfo[i].saveToMemory(file)) return false;

	return true;
}

bool a3dObjectInfo::loadFromMemoryCore(a3dMemoryFile &file) {
	
	if(!a3dVisualObjectInfoBase::loadFromMemoryCore(file)) return false;
	if(!file.readInt(&this->detailInfoCount)) return false;
	
	if(detailInfoCount == 0) return true;
	this->detailInfo = new a3dDetailInfo[this->detailInfoCount];
	for(int i=0; i< this->detailInfoCount; i++) {
		if(!this->detailInfo[i].loadFromMemory(file)) return false;	
	}

	return true;
}

void a3dObjectInfo::initializeReference(a3dObjectRef *objectRef) {
	
	copyMatrix4(this->basis, objectRef->basis);
	objectRef->scaleX = this->scaleX;
	objectRef->scaleY = this->scaleY;
	objectRef->scaleZ = this->scaleZ;

	for( int i = 0; i < this->detailInfoCount; i++ ) {
		this->detailInfo[i].initializeReference( objectRef->detailRef[i] );
	}
}