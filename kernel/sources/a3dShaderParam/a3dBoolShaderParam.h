#ifndef A3D_BOOL_SHADER_PARAM_INC
#define A3D_BOOL_SHADER_PARAM_INC

#include "a3dShaderParamBase.h"

class a3dBoolShaderParam : public a3dShaderParamBase { 
public:
	BOOL	boolValue;

	a3dBoolShaderParam(  shString &paramName ) : a3dShaderParamBase( paramName ) { 
		this->type = SPT_BOOL;
		this->boolValue = false;
	}
	a3dBoolShaderParam( shString &paramName, BOOL value ) : a3dShaderParamBase( paramName ) { 
		this->type = SPT_BOOL;
		this->boolValue = value; 
	}
	a3dBoolShaderParam(fxParamInfo *info);

	a3dShaderParamBase* clone();
	HRESULT setParam( a3dEffectDesc *effect ) { return effect->lpEffect->SetValue( name, &boolValue, sizeof(BOOL) ); }
	HRESULT getParam( a3dEffectDesc *effect ) { return effect->lpEffect->GetValue( name, &boolValue, sizeof(BOOL) ); }

	int		getHeaderCode() { return a3dHeaders::a3dBoolShaderParamHeader; }
	int		calcSizeCore();
	bool	saveToMemoryCore( a3dMemoryFile &file );
	bool	loadFromMemoryCore( a3dMemoryFile &file );
};

#endif