#ifndef A3D_SHADER_PARAM_TYPE_INC
#define A3D_SHADER_PARAM_TYPE_INC

typedef enum _a3dShaderParamType { 
	SPT_BASE, 
	SPT_BOOL, 
	SPT_BOOL_ARRAY, 
	SPT_FLOAT, 
	SPT_FLOAT_ARRAY, 
	SPT_INT, 
	SPT_INT_ARRAY, 
	SPT_MATRIX, 
	SPT_MATRIX_ARRAY, 
	SPT_TEXTURE, 
	SPT_TECHNIQUE,
	SPT_COLOR,
	SPT_VECTOR
} a3dShaderParamType;

#endif