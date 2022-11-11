#include "a3dTextureShaderParam.h"

a3dTextureShaderParam::a3dTextureShaderParam(fxParamInfo *info) : a3dShaderParamBase(info) { 
	this->type = SPT_TEXTURE;
	this->fileName = info->resourceName;
}

a3dShaderParamBase* a3dTextureShaderParam::clone() { 

	a3dTextureShaderParam *param = new a3dTextureShaderParam( this->paramName, this->fileName );
	param->assign(this);
	return param;	
}

void a3dTextureShaderParam::assign(a3dShaderParamBase *p) { 
	
	a3dShaderParamBase::assign(p);
	a3dTextureShaderParam *tp = dynamic_cast<a3dTextureShaderParam*>(p);
	if(tp == null) return;
	this->textureDesc = tp->textureDesc;
	return;
}

int a3dTextureShaderParam::calcSizeCore() { 
	return a3dShaderParamBase::calcSizeCore() + this->fileName.calcSize(); 
}

bool a3dTextureShaderParam::saveToMemoryCore(a3dMemoryFile &file) { 

	if( !a3dShaderParamBase::saveToMemoryCore( file ) ) return false;
	if( !this->fileName.saveToMemory( file ) ) return false; 	

	return true;
}

bool a3dTextureShaderParam::loadFromMemoryCore(a3dMemoryFile &file) { 

	if( !a3dShaderParamBase::loadFromMemoryCore( file ) ) return false;
	if( !this->fileName.loadFromMemory( file ) ) return false; 	

	return true;
}