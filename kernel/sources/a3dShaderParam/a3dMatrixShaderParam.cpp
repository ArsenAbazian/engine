#include "a3dMatrixShaderParam.h"
#include "memoryFile.h"

a3dMatrixShaderParam::a3dMatrixShaderParam(fxParamInfo *info ) : a3dShaderParamBase(info) { 
	
	this->type = SPT_MATRIX;
	if(info->matrixBuffer != null)
		memcpy(this->matrixValue, info->matrixBuffer, info->calcItemSize());
}

a3dShaderParamBase* a3dMatrixShaderParam::clone() { 

	a3dMatrixShaderParam *param = new a3dMatrixShaderParam( this->paramName, this->matrixValue );
	param->assign(this);
	return param;
}

int a3dMatrixShaderParam::calcSizeCore() { 
	return a3dShaderParamBase::calcSizeCore() + sizeof( Matrix4 ); 
}

bool a3dMatrixShaderParam::saveToMemoryCore(a3dMemoryFile &file) { 

	if( !a3dShaderParamBase::saveToMemoryCore( file ) ) return false;
	if( !file.writeMatrix4( this->matrixValue ) ) return false;

	return true;
}

bool a3dMatrixShaderParam::loadFromMemoryCore(a3dMemoryFile &file) { 

	if( !a3dShaderParamBase::loadFromMemoryCore( file ) ) return false;
	if( !file.readMatrix4( this->matrixValue ) ) return false;

	return true;
}