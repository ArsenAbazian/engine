#ifndef A3D_FLOAT_ARRAY_SHADER_PARAM_INC
#define A3D_FLOAT_ARRAY_SHADER_PARAM_INC

#include "a3dShaderParamBase.h"

class a3dFloatArrayShaderParam : public a3dShaderParamBase { 
public:
	float	*floatArray;
	int		count;

	a3dFloatArrayShaderParam(  shString &paramName ) : a3dShaderParamBase( paramName ) { 
		this->type = SPT_FLOAT_ARRAY;
		this->floatArray = null;
		this->count = 0;
	}
	a3dFloatArrayShaderParam( shString &paramName, float *floatArray, int count ) : a3dShaderParamBase( paramName ) { 
		this->type = SPT_FLOAT_ARRAY;
		this->count = count;
		this->floatArray = new float[ count ];
		memcpy( this->floatArray, floatArray, sizeof( float ) * count );
	}
	a3dFloatArrayShaderParam(fxParamInfo *info);
	~a3dFloatArrayShaderParam() { 
		if( this->floatArray != null ) delete[] this->floatArray;
	}

	a3dShaderParamBase* clone();
	HRESULT setParam( a3dEffectDesc *effect ) { return effect->lpEffect->SetValue( name, floatArray, sizeof(float) * count ); }
	HRESULT getParam( a3dEffectDesc *effect ) { return effect->lpEffect->GetValue( name, floatArray, sizeof(float) * count ); }

	int		getHeaderCode() { return a3dHeaders::a3dFloatArrayShaderParamHeader; }
	int		calcSizeCore();
	bool	saveToMemoryCore( a3dMemoryFile &file );
	bool	loadFromMemoryCore( a3dMemoryFile &file );
};

#endif