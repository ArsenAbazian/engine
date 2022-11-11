#include "a3dMatrixArrayShaderParam.h"
#include "memoryFile.h"

a3dMatrixArrayShaderParam::a3dMatrixArrayShaderParam(fxParamInfo *info) : a3dShaderParamBase(info) { 

	this->type = SPT_MATRIX_ARRAY;
	this->count = info->calcItemCount();
	if( this->count != 0 ) { 
		this->matrixArray = new Matrix4[this->count];
		for(int i = 0; i < this->count; i++) originMatrix4(this->matrixArray[i]);
		if(info->matrixBuffer != null) 
			memcpy(this->matrixArray, info->matrixBuffer, info->calcItemCount() * info->calcItemSize());
	}
}

a3dShaderParamBase* a3dMatrixArrayShaderParam::clone() { 

	a3dMatrixArrayShaderParam *param = new a3dMatrixArrayShaderParam( this->paramName, this->matrixArray, this->count );
	param->assign(this);
	return param;
}

int a3dMatrixArrayShaderParam::calcSizeCore() { 
	return a3dShaderParamBase::calcSizeCore() + sizeof( int ) + sizeof( Matrix4 ) * this->count; 
}

bool a3dMatrixArrayShaderParam::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dShaderParamBase::saveToMemoryCore( file ) ) return false;
	if( !file.writeInt( this->count ) ) return false;
	if( !file.writeMatrix4Array( this->matrixArray, this->count ) ) return false;

	return true;
}

bool a3dMatrixArrayShaderParam::loadFromMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dShaderParamBase::loadFromMemoryCore( file ) ) return false;
	if( !file.readInt( &this->count ) ) return false;

	this->matrixArray = new Matrix4[ this->count ];
	if( !file.readMatrix4Array( this->matrixArray, this->count ) ) return false;

	return true;
}