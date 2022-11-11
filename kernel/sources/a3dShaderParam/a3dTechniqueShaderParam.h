#ifndef A3D_TECHNIQUE_SHADER_PARAM_INC
#define A3D_TECHNIQUE_SHADER_PARAM_INC

#include "a3dShaderParamBase.h"

class a3dTechniqueShaderParam : public a3dShaderParamBase { 
	D3DXHANDLE handle;
public:	
	a3dTechniqueShaderParam( shString &paramName ) : a3dShaderParamBase( paramName ) { 
		this->type = SPT_TECHNIQUE;
		this->handle = null;
	}
	a3dTechniqueShaderParam(fxParamInfo *info);

	a3dShaderParamBase* clone();
	HRESULT setParam( a3dEffectDesc *effect ) { return this->handle != null? effect->lpEffect->SetTechnique( this->handle ): D3D_OK; }
	HRESULT getParam( a3dEffectDesc *effect ) { return D3D_OK; }

	int		getHeaderCode() { return a3dHeaders::a3dTechniqueShaderParamHeader; }
	void	initializeHandle( a3dEffectDesc *effect );
};

#endif