#include "a3dBoolArrayShaderParam.h"
#include "memoryFile.h"

a3dShaderParamBase* a3dBoolArrayShaderParam::clone() { 

	a3dBoolArrayShaderParam *param = new a3dBoolArrayShaderParam( this->paramName, this->boolArray, this->count );
	param->assign(this);
	return param;
}

a3dBoolArrayShaderParam::a3dBoolArrayShaderParam(fxParamInfo *info) : a3dShaderParamBase(info) { 
	
	this->type = SPT_BOOL_ARRAY;
	this->count = info->calcItemCount() * info->row * info->col;
	if( this->count != 0 ) { 
		this->boolArray = new BOOL[this->count];
		ZeroMemory(this->boolArray, sizeof(BOOL) * this->count);
		if(info->boolBuffer != null)
			memcpy(this->boolArray, info->boolBuffer, info->calcItemSize() * info->calcItemCount());
	}
}

int a3dBoolArrayShaderParam::calcSizeCore() { 
	return a3dShaderParamBase::calcSizeCore() + sizeof( int ) + sizeof( BOOL ) * this->count; 
}

bool a3dBoolArrayShaderParam::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dShaderParamBase::saveToMemoryCore( file ) ) return false;
	if( !file.writeInt( this->count ) ) return false;
	if( !file.writeBOOLArray( this->boolArray, this->count ) ) return false;

	return true;
}

bool a3dBoolArrayShaderParam::loadFromMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dShaderParamBase::loadFromMemoryCore( file ) ) return false;
	if( !file.readInt( &this->count ) ) return false;

	this->boolArray = new BOOL[ this->count ];
	if( !file.readBOOLArray( this->boolArray, this->count ) ) return false;

	return true;
}