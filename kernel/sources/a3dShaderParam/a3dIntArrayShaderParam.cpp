#include "a3dIntArrayShaderParam.h"
#include "memoryFile.h"

a3dIntArrayShaderParam::a3dIntArrayShaderParam(fxParamInfo *info) : a3dShaderParamBase(info) { 

	this->type = SPT_INT_ARRAY;
	this->count = info->calcItemCount() * info->row * info->col;
	if( this->count != 0 ) { 
		this->intArray = new int[this->count];
		ZeroMemory(this->intArray, sizeof(int) * this->count);
		if(info->intBuffer != null)
			memcpy(this->intArray, info->intBuffer, info->calcItemCount() * info->calcItemSize());
	}
}

a3dShaderParamBase* a3dIntArrayShaderParam::clone() { 

	a3dIntArrayShaderParam *param = new a3dIntArrayShaderParam( this->paramName, this->intArray, this->count );
	param->assign(this);
	return param;
}

int a3dIntArrayShaderParam::calcSizeCore() { 
	return a3dShaderParamBase::calcSizeCore() + sizeof( int ) + sizeof( int ) * this->count; 
}

bool a3dIntArrayShaderParam::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dShaderParamBase::saveToMemoryCore( file ) ) return false;
	if( !file.writeInt( this->count ) ) return false;
	if( !file.writeIntArray( this->intArray, this->count ) ) return false;

	return true;
}

bool a3dIntArrayShaderParam::loadFromMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dShaderParamBase::loadFromMemoryCore( file ) ) return false;
	if( !file.readInt( &this->count ) ) return false;

	this->intArray = new int[ this->count ];
	if( !file.readIntArray( this->intArray, this->count ) ) return false;

	return true;
}