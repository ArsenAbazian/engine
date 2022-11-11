#ifndef A3D_VECTOR_SHADER_PARAM
#define A3D_VECTOR_SHADER_PARAM

#include "a3dFloatArrayShaderParam.h"

class a3dVectorShaderParam : public a3dFloatArrayShaderParam {
public:
	a3dVectorShaderParam( shString &paramName ) : a3dFloatArrayShaderParam( paramName ) { 
		this->type = SPT_VECTOR;
	}
	a3dVectorShaderParam( shString &paramName, float *arr, int count ) : a3dFloatArrayShaderParam(paramName, arr, count) { 
		this->type = SPT_VECTOR;
	}

	int		getHeaderCode() { return a3dHeaders::a3dVectorShaderParamHeader; }
};

#endif