#ifndef A3D_MATRIX_SHADER_PARAM_INC
#define A3D_MATRIX_SHADER_PARAM_INC

#include "a3dShaderParamBase.h"

class a3dMatrixShaderParam : public a3dShaderParamBase { 
public:
	Matrix4	matrixValue;

	a3dMatrixShaderParam(  shString &paramName ) : a3dShaderParamBase( paramName ) { 
		this->type = SPT_MATRIX;
		originMatrix4( this->matrixValue );
	}
	a3dMatrixShaderParam( shString &paramName, Matrix4 value ) : a3dShaderParamBase( paramName ) { 
		this->type = SPT_MATRIX;
		copyMatrix4( value, this->matrixValue );
	}
	a3dMatrixShaderParam( fxParamInfo *info );  
	a3dShaderParamBase* clone();
	HRESULT setParam( a3dEffectDesc *effect ) { 
		return effect->lpEffect->SetValue( name, this->matrixValue, sizeof(float) * this->row * this->col);
		//return effect->lpEffect->SetMatrix( name, (D3DXMATRIX*)matrixValue ); 
	}
	HRESULT getParam( a3dEffectDesc *effect ) { 
		return effect->lpEffect->GetValue( name, this->matrixValue, sizeof(float) * this->row * this->col);
		//return effect->lpEffect->GetMatrix( name, (D3DXMATRIX*)matrixValue ); 
	}

	int		getHeaderCode() { return a3dHeaders::a3dMatrixShaderParamHeader; }
	int		calcSizeCore();
	bool	saveToMemoryCore( a3dMemoryFile &file );
	bool	loadFromMemoryCore( a3dMemoryFile &file );
};

#endif