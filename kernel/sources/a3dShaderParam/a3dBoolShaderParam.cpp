#include "a3dBoolShaderParam.h"
#include "memoryFile.h"

int a3dBoolShaderParam::calcSizeCore() { 
	return a3dShaderParamBase::calcSizeCore() + sizeof( BOOL ); 
}

a3dBoolShaderParam::a3dBoolShaderParam(fxParamInfo *info) : a3dShaderParamBase(info) { 
	
	this->type = SPT_BOOL;
	if( info->boolBuffer != null )
		this->boolValue = *info->boolBuffer;
}

bool a3dBoolShaderParam::saveToMemoryCore(a3dMemoryFile &file) { 

	if( !a3dShaderParamBase::saveToMemoryCore( file ) ) return false;
	if( !file.writeBOOL( this->boolValue ) ) return false;

	return true;
}

bool a3dBoolShaderParam::loadFromMemoryCore(a3dMemoryFile &file) { 

	if( !a3dShaderParamBase::loadFromMemoryCore( file ) ) return false;
	if( !file.readBOOL( &this->boolValue ) ) return false;

	return true;
}

a3dShaderParamBase* a3dBoolShaderParam::clone() { 

	a3dBoolShaderParam *param = new a3dBoolShaderParam( this->paramName, this->boolValue );
	param->assign(this);
	return param;
}