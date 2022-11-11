#include "a3dDetailInfo.h"
#include "..\\lightMapInfo.h"
#include "..\\a3dMaterial\\a3dShaderMaterial.h"
#include "..\\a3dModelRef\\a3dDetailRef.h"
#include "..\\a3dModel\\a3dDetail.h"

void a3dDetailInfo::initializeDefault() { 
}

void a3dDetailInfo::clear() {
}

int a3dDetailInfo::calcSizeCore() {

	return a3dObjectInfoBase::calcSizeCore() + this->lightMapInfo.calcSize() + this->material.calcSize();
}

bool a3dDetailInfo::saveToMemoryCore(a3dMemoryFile &file) {
	
	if(!a3dObjectInfoBase::saveToMemoryCore(file)) return false;
	if(!this->lightMapInfo.saveToMemory(file)) return false;
	if(!this->material.saveToMemory(file)) return false; 

	return true;
}

bool a3dDetailInfo::loadFromMemoryCore(a3dMemoryFile &file) {
	
	if(!a3dObjectInfoBase::loadFromMemoryCore(file)) return false;
	if(!this->lightMapInfo.loadFromMemory(file)) return false;
	if(!this->material.loadFromMemory(file)) return false;

	return true;
}

void a3dDetailInfo::initializeReference(a3dDetailRef *detailRef) { 
	
	detailRef->lightMapInfo.assign( this->lightMapInfo );
	detailRef->material.assign( &this->material );
}

void a3dDetailInfo::assign(a3dDetailRef *detailRef) { 
	
	this->lightMapInfo.assign( detailRef->lightMapInfo );
	this->material.assign( &detailRef->material );
}

void a3dDetailInfo::initialize( a3dDetail *det ) { 
	
	this->lightMapInfo.assign(det->lightMapInfo);
	this->material.assign(det->material);
}