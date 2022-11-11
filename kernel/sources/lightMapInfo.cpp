#include "lightMapInfo.h"

void a3dLightMapInfo::initializeDefault() {
	
	this->enabled = false;
	this->lightMapWidth = 0;
	this->lightMapHeight = 0;
	this->textureCoorIndex = 0;
	this->isAtlasTextureCoords = false;
	this->u0 = 0.0f;
	this->v0 = 0.0f;
	this->uSize = 0.0f;
	this->vSize = 0.0f;
}

int a3dLightMapInfo::calcSizeCore() { 
	
	return sizeof(bool) + this->lightMapFileName.calcSize() +
			this->lightMapTextureParamName.calcSize() +
			sizeof(int) * 3 + sizeof(bool) + sizeof(float) * 4;
}

bool a3dLightMapInfo::saveToMemoryCore(a3dMemoryFile &file ) { 
	
	if(!file.writeBool(this->enabled)) return false;
	if(!this->lightMapFileName.saveToMemory(file)) return false;
	if(!this->lightMapTextureParamName.saveToMemory(file)) return false;
	if(!file.writeInt(this->lightMapWidth)) return false;
	if(!file.writeInt(this->lightMapHeight)) return false;
	if(!file.writeInt(this->textureCoorIndex)) return false;
	if(!file.writeBool(this->isAtlasTextureCoords)) return false;
	if(!file.writeFloat(this->u0)) return false;
	if(!file.writeFloat(this->v0)) return false;
	if(!file.writeFloat(this->uSize)) return false;
	if(!file.writeFloat(this->vSize)) return false;

	return true;
}

bool a3dLightMapInfo::loadFromMemoryCore(a3dMemoryFile &file ) { 

	if(!file.readBool(&this->enabled)) return false;
	if(!this->lightMapFileName.loadFromMemory(file)) return false;
	if(!this->lightMapTextureParamName.loadFromMemory(file)) return false;
	if(!file.readInt(&this->lightMapWidth)) return false;
	if(!file.readInt(&this->lightMapHeight)) return false;
	if(!file.readInt(&this->textureCoorIndex)) return false;
	if(!file.readBool(&this->isAtlasTextureCoords)) return false;
	if(!file.readFloat(&this->u0)) return false;
	if(!file.readFloat(&this->v0)) return false;
	if(!file.readFloat(&this->uSize)) return false;
	if(!file.readFloat(&this->vSize)) return false;

	return true;
}

void a3dLightMapInfo::assign(a3dLightMapInfo &lm ) {
	
	this->enabled = lm.enabled;
	this->lightMapFileName = lm.lightMapFileName;
	this->lightMapTextureParamName = lm.lightMapTextureParamName;
	this->lightMapWidth = lm.lightMapWidth;
	this->lightMapHeight = lm.lightMapHeight;
	this->textureCoorIndex = lm.textureCoorIndex;
	this->isAtlasTextureCoords = lm.isAtlasTextureCoords;
	this->u0 = lm.u0;
	this->v0 = lm.v0;
	this->uSize = lm.uSize;
	this->vSize = lm.vSize;
}