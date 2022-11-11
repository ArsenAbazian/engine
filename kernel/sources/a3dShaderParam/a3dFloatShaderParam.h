#ifndef A3D_FLOAT_SHADER_PARAM_INC
#define A3D_FLOAT_SHADER_PARAM_INC

#include "a3dShaderParamBase.h"

class a3dFloatShaderParam : public a3dShaderParamBase { 
	// current version is 1.1
public:
	float	floatValue;
	float	minValue;	// included since 1.1
	float	maxValue;	// included since 1.1
	float	step;		// included since 1.1

	a3dDependencyProperty	*floatValueProperty;

	a3dFloatShaderParam(  shString &paramName ) : a3dShaderParamBase( paramName ) { 
		this->type = SPT_FLOAT;
		this->floatValue = 0.0f;
		this->minValue = -100000.0f;
		this->maxValue = 100000.0f;
		this->step = 1.0f;
		this->version = a3dVersion(1,1);
		this->floatValueProperty = null;
	}
	a3dFloatShaderParam( shString &paramName, float value ) : a3dShaderParamBase( paramName ) { 
		this->type = SPT_FLOAT;
		this->floatValue = value; 
		this->minValue = -100000.0f;
		this->maxValue = 100000.0f;
		this->step = 1.0f;
		this->version = a3dVersion(1,1);
		this->floatValueProperty = null;
	}
	a3dFloatShaderParam(fxParamInfo *info);

	a3dShaderParamBase* clone();
	void	assign(a3dShaderParamBase*);
	HRESULT setParam( a3dEffectDesc *effect ) { return effect->lpEffect->SetValue( name, &floatValue, sizeof(float) ); }
	HRESULT getParam( a3dEffectDesc *effect ) { return effect->lpEffect->GetValue( name, &floatValue, sizeof(float) ); }

	int		getHeaderCode() { return a3dHeaders::a3dFloatShaderParamHeader; }
	int		calcSizeCore();
	bool	saveToMemoryCore( a3dMemoryFile &file );
	bool	loadFromMemoryCore( a3dMemoryFile &file );

	void	registerDependencyProperties();
};

#endif