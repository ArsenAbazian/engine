#include "a3dIntShaderParam.h"
#include "memoryFile.h"

a3dIntShaderParam::a3dIntShaderParam(fxParamInfo *info ) : a3dShaderParamBase(info) { 
	
	this->type = SPT_INT;
	if(info->intBuffer != null)
		this->intValue = *info->intBuffer;
	else 
		this->intValue = 0;
}

a3dShaderParamBase* a3dIntShaderParam::clone() { 

	a3dIntShaderParam *param = new a3dIntShaderParam( this->paramName, this->intValue );
	param->assign(this);
	return param;	
}

int a3dIntShaderParam::calcSizeCore() { 
	return a3dShaderParamBase::calcSizeCore() + sizeof( int ); 
}

bool a3dIntShaderParam::saveToMemoryCore(a3dMemoryFile &file) { 

	if( !a3dShaderParamBase::saveToMemoryCore( file ) ) return false;
	if( !file.writeInt( this->intValue ) ) return false;

	return true;
}

bool a3dIntShaderParam::loadFromMemoryCore(a3dMemoryFile &file) { 

	if( !a3dShaderParamBase::loadFromMemoryCore( file ) ) return false;
	if( !file.readInt( &this->intValue ) ) return false;

	return true;
}