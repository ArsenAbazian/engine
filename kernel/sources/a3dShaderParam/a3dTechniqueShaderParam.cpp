#include "a3dTechniqueShaderParam.h"

a3dTechniqueShaderParam::a3dTechniqueShaderParam(fxParamInfo *info) : a3dShaderParamBase(info) { 
	this->type = SPT_TECHNIQUE;
	this->handle = null;
}

a3dShaderParamBase* a3dTechniqueShaderParam::clone() { 

	a3dTechniqueShaderParam *param = new a3dTechniqueShaderParam( this->paramName );
	param->assign(this);
	return param;
}

void a3dTechniqueShaderParam::initializeHandle( a3dEffectDesc *effect ) { 
	
	this->handle = effect->lpEffect->GetTechniqueByName( this->name );
}