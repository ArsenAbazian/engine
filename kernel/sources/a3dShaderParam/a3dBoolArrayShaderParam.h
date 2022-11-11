#ifndef A3D_BOOL_ARRAY_SHADER_PARAM_INC
#define A3D_BOOL_ARRAY_SHADER_PARAM_INC

#include "a3dShaderParamBase.h"

class a3dBoolArrayShaderParam : public a3dShaderParamBase { 
public:
	BOOL	*boolArray;
	int		count;

	a3dBoolArrayShaderParam(  shString &paramName ) : a3dShaderParamBase( paramName ) { 
		this->type = SPT_BOOL_ARRAY;
		this->boolArray = null;
		this->count = 0;
	}
	a3dBoolArrayShaderParam( shString &paramName, BOOL *boolArray, int count ) : a3dShaderParamBase( paramName ) { 
		this->type = SPT_BOOL_ARRAY;
		this->count = count;
		this->boolArray = new BOOL[ count ];
		memcpy( this->boolArray, boolArray, sizeof( bool ) * count );
	}
	a3dBoolArrayShaderParam(fxParamInfo *info);
	~a3dBoolArrayShaderParam() { 
		if( this->boolArray != null ) delete[] this->boolArray;
	}

	a3dShaderParamBase* clone();
	HRESULT setParam( a3dEffectDesc *effect ) { return effect->lpEffect->SetValue( name, boolArray, sizeof(BOOL) * count ); }
	HRESULT getParam( a3dEffectDesc *effect ) { return effect->lpEffect->GetValue( name, boolArray, sizeof(BOOL) * count ); }

	int		getHeaderCode() { return a3dHeaders::a3dBoolArrayShaderParamHeader; }
	int		calcSizeCore();
	bool	saveToMemoryCore( a3dMemoryFile &file );
	bool	loadFromMemoryCore( a3dMemoryFile &file );
};

#endif