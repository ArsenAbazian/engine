#ifndef A3D_INT_SHADER_PARAM_INC
#define A3D_INT_SHADER_PARAM_INC

#include "a3dShaderParamBase.h"

class a3dIntShaderParam : public a3dShaderParamBase { 
public:
	int	intValue;

	a3dIntShaderParam(  shString &paramName ) : a3dShaderParamBase( paramName ) { 
		this->type = SPT_INT;
		this->intValue = 0;
	}
	a3dIntShaderParam( shString &paramName, int value ) : a3dShaderParamBase( paramName ) { 
		this->type = SPT_INT;
		this->intValue = value; 
	}
	a3dIntShaderParam( fxParamInfo *info );
	a3dShaderParamBase* clone();
	HRESULT setParam( a3dEffectDesc *effect ) { return effect->lpEffect->SetValue( name, &intValue, sizeof(int) ); }
	HRESULT getParam( a3dEffectDesc *effect ) { return effect->lpEffect->GetValue( name, &intValue, sizeof(int) ); }

	int		getHeaderCode() { return a3dHeaders::a3dIntShaderParamHeader; }
	int		calcSizeCore();
	bool	saveToMemoryCore( a3dMemoryFile &file );
	bool	loadFromMemoryCore( a3dMemoryFile &file );
};

#endif