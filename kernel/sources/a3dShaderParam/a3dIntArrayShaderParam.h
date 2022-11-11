#ifndef A3D_INT_ARRAY_SHADER_PARAM_INC
#define A3D_INT_ARRAY_SHADER_PARAM_INC

#include "a3dShaderParamBase.h"

class a3dIntArrayShaderParam : public a3dShaderParamBase { 
public:
	int		*intArray;
	int		count;

	a3dIntArrayShaderParam(  shString &paramName ) : a3dShaderParamBase( paramName ) { 
		this->type = SPT_INT_ARRAY;
		this->intArray = null;
		this->count = 0;
	}
	a3dIntArrayShaderParam( shString &paramName, int *intArray, int count ) : a3dShaderParamBase( paramName ) { 
		this->type = SPT_INT_ARRAY;
		this->count = count;
		this->intArray = new int[ count ];
		memcpy( this->intArray, intArray, sizeof( int ) * count );
	}
	a3dIntArrayShaderParam(fxParamInfo *info);
	~a3dIntArrayShaderParam() { 
		if( this->intArray != null ) delete[] this->intArray;
	}

	a3dShaderParamBase* clone();
	HRESULT setParam( a3dEffectDesc *effect ) { return effect->lpEffect->SetValue( name, intArray, sizeof(int) * count ); }
	HRESULT getParam( a3dEffectDesc *effect ) { return effect->lpEffect->GetValue( name, intArray, sizeof(int) * count ); }

	int		getHeaderCode() { return a3dHeaders::a3dIntArrayShaderParamHeader; }
	int		calcSizeCore();
	bool	saveToMemoryCore( a3dMemoryFile &file );
	bool	loadFromMemoryCore( a3dMemoryFile &file );
};

#endif