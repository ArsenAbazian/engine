#include "a3dFloatArrayShaderParam.h"
#include "memoryFile.h"

a3dFloatArrayShaderParam::a3dFloatArrayShaderParam(fxParamInfo *info) : a3dShaderParamBase(info) { 
	
	this->type = SPT_FLOAT_ARRAY;
	this->count = info->calcItemCount() * info->row * info->col;
	if( this->count != null) { 
		this->floatArray = new float[this->count];
		for(int i = 0; i < this->count; i++) this->floatArray[i] = 0.0f;
		if( info->floatBuffer != null )
			memcpy(this->floatArray, info->floatBuffer, info->calcItemSize() * info->calcItemCount());
	}
}

a3dShaderParamBase* a3dFloatArrayShaderParam::clone() { 

	a3dFloatArrayShaderParam *param = new a3dFloatArrayShaderParam( this->paramName, this->floatArray, this->count );
	param->assign(this);
	return param;
}

int a3dFloatArrayShaderParam::calcSizeCore() { 
	return a3dShaderParamBase::calcSizeCore() + sizeof( int ) + sizeof( float ) * this->count; 
}

bool a3dFloatArrayShaderParam::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dShaderParamBase::saveToMemoryCore( file ) ) return false;
	if( !file.writeInt( this->count ) ) return false;
	if( !file.writeFloatArray( this->floatArray, this->count ) ) return false;

	return true;
}

bool a3dFloatArrayShaderParam::loadFromMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dShaderParamBase::loadFromMemoryCore( file ) ) return false;
	if( !file.readInt( &this->count ) ) return false;

	this->floatArray = new float[ this->count ];
	if( !file.readFloatArray( this->floatArray, this->count ) ) return false;

	return true;
}