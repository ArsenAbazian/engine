#ifndef A3D_COLOR_SHADER_PARAM
#define A3D_COLOR_SHADER_PARAM

#include "a3dFloatArrayShaderParam.h"

class a3dColorShaderParam : public a3dFloatArrayShaderParam {
public:
	a3dColorShaderParam( shString &paramName ) : a3dFloatArrayShaderParam(paramName) { 
		this->type = SPT_COLOR;
	}
	a3dColorShaderParam( shString &paramName, float *arr, int count ) : a3dFloatArrayShaderParam(paramName, arr, count) { 
		this->type = SPT_COLOR;
	}

	int		getHeaderCode() { return a3dHeaders::a3dColorShaderParamHeader; }
};

#endif