#ifndef A3D_MATRIX_ARRAY_SHADER_PARAM_INC
#define A3D_MATRIX_ARRAY_SHADER_PARAM_INC

#include "a3dShaderParamBase.h"

class a3dMatrixArrayShaderParam : public a3dShaderParamBase { 
public:
	Matrix4	*matrixArray;
	int	count;

	a3dMatrixArrayShaderParam(  shString &paramName ) : a3dShaderParamBase( paramName ) { 
		this->type = SPT_MATRIX_ARRAY;
		this->matrixArray = null;
		this->count = 0;
	}
	a3dMatrixArrayShaderParam( shString &paramName, Matrix4 *matrixArray, int count ) : a3dShaderParamBase( paramName ) { 
		this->type = SPT_MATRIX_ARRAY;
		this->count = count;
		this->matrixArray = new Matrix4[ count ];
		memcpy( this->matrixArray, matrixArray, sizeof( Matrix4 ) * count );
	}
	a3dMatrixArrayShaderParam( fxParamInfo *info);
	~a3dMatrixArrayShaderParam() { 
		if( this->matrixArray != null ) delete[] this->matrixArray;
	}

	a3dShaderParamBase* clone();
	HRESULT setParam( a3dEffectDesc *effect ) { return effect->lpEffect->SetMatrixArray( name, (D3DXMATRIX*)matrixArray, count ); }
	HRESULT getParam( a3dEffectDesc *effect ) { return effect->lpEffect->GetMatrixArray( name, (D3DXMATRIX*)matrixArray, count ); }

	int		getHeaderCode() { return a3dHeaders::a3dMatrixArrayShaderParamHeader; }
	int		calcSizeCore();
	bool	saveToMemoryCore( a3dMemoryFile &file );
	bool	loadFromMemoryCore( a3dMemoryFile &file );
};

#endif