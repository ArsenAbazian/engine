#include "a3dFloatShaderParam.h"
#include "memoryFile.h"

a3dShaderParamBase* a3dFloatShaderParam::clone() { 

	a3dFloatShaderParam *param = new a3dFloatShaderParam( this->paramName, this->floatValue );
	param->assign(this);
	return param;
}

void a3dFloatShaderParam::assign( a3dShaderParamBase *p ) { 
	
	a3dShaderParamBase::assign(p);
	a3dFloatShaderParam *fp = dynamic_cast<a3dFloatShaderParam*>(p);
	if(fp == null) return;
	this->floatValue = fp->floatValue;
	this->minValue = fp->minValue;
	this->maxValue = fp->maxValue;
	this->step = fp->step;
	
	if(this->floatValueProperty == null)
		this->registerDependencyProperties();
}

a3dFloatShaderParam::a3dFloatShaderParam(fxParamInfo *info) : a3dShaderParamBase(info) { 
	
	this->type = SPT_FLOAT;
	this->floatValue = 0.0f;
	if( info->floatBuffer != null )
		this->floatValue = *info->floatBuffer;
	this->minValue = info->uiMin;
	this->maxValue = info->uiMax;
	this->step = info->uiStep;
}

int a3dFloatShaderParam::calcSizeCore() { 
	int sz = a3dShaderParamBase::calcSizeCore() + sizeof( float ); 
	if( this->supportVersion( a3dVersion(1,1) ) ) { 
		sz += sizeof(float) +  // minValue;
				sizeof(float) + // maxValue;
				sizeof(float); // step
	}
	
	return sz;
}

bool a3dFloatShaderParam::saveToMemoryCore(a3dMemoryFile &file) { 

	if( !a3dShaderParamBase::saveToMemoryCore( file ) ) return false;
	if( !file.writeFloat( this->floatValue ) ) return false;
	if( this->supportVersion( a3dVersion(1,1) ) ) { 
		if( !file.writeFloat(this->minValue) ) return false;
		if( !file.writeFloat(this->maxValue) ) return false;
		if( !file.writeFloat(this->step) ) return false;
	}

	return true;
}

bool a3dFloatShaderParam::loadFromMemoryCore(a3dMemoryFile &file) { 

	if( !a3dShaderParamBase::loadFromMemoryCore( file ) ) return false;
	if( !file.readFloat( &this->floatValue ) ) return false;
	if( this->supportVersion( a3dVersion(1,1) ) ) { 
		if( !file.readFloat(&this->minValue) ) return false;
		if( !file.readFloat(&this->maxValue) ) return false;
		if( !file.readFloat(&this->step) ) return false;
	}

	this->registerDependencyProperties();

	return true;
}

void a3dFloatShaderParam::registerDependencyProperties() { 
	
	this->floatValueProperty = this->registerDependencyProperty( TEXT("Value"), TEXT("floatValue"), DPROP_FLOAT, &this->floatValue);
	return;
}