#ifndef A3DHEADER_CODES
#define A3DHEADER_CODES

#include "serialization.h"

#define MAKE_HEADER(a, b, c, d) ((((int)d) << 24) | (((int)c) << 16) | (((int)b) << 8) | (a))
class a3dHeaders { 
public:

	static const int a3dLightBaseHeader =					MAKE_HEADER( 'l', 'b', ' ', ' ' );
	static const int a3dOmniLightHeader =					MAKE_HEADER( 'o', 'l', ' ', ' ' );
	static const int a3dDirectionalLightHeader =			MAKE_HEADER( 'd', 'l', ' ', ' ' );
	static const int a3dMaterialBaseHeader =				MAKE_HEADER( 'm', 'b', ' ', ' ' );
	static const int a3dShaderMaterialHeader =				MAKE_HEADER( 's', 'h', 'm', ' ' );
	static const int a3dDetailHeader =						MAKE_HEADER( 'd', 't', 'l', ' ' );
	static const int a3dMathObjectHeader =					MAKE_HEADER( 'm', 'o', 'b', ' ' );
	static const int a3dObjectHeader =						MAKE_HEADER( 'o', 'b', 'j', ' ' );
	static const int a3dModelHeader =						MAKE_HEADER( 'm', 'd', 'l', ' ' );
	static const int a3dObjectInfoHeader =					MAKE_HEADER( 'o', 'i', 'h', ' ' );
	static const int a3dEffectInfoHeader =					MAKE_HEADER( 'e', 'i', 'h', ' ' );
	static const int a3dTextureInfoHeader =					MAKE_HEADER( 't', 'i', 'h', ' ' );
	static const int a3dVisualObjectInfoBaseHeader =		MAKE_HEADER( 'v', 'i', 'b', 'h' );
	static const int a3dModelInfoHeader =					MAKE_HEADER( 'm', 'i', 'h', ' ' );
	static const int a3dModelObjectInfoHeader =				MAKE_HEADER( 'm', 'o', 'i', 'h' );
	static const int a3dModelDetailInfoHeader =				MAKE_HEADER( 'm', 'd', 'i', 'h' );
	static const int a3dLandPassInfoHeader =				MAKE_HEADER( 'l', 'p', 'i', 'h' );
	static const int a3dLandscapeInfoHeader =				MAKE_HEADER( 'l', 'i', 'h', ' ' );
	static const int a3dParticleSystemInfo =				MAKE_HEADER( 'p', 's', 'i', 'h' );
	static const int a3dSprayParticleSystemInfoHeader =		MAKE_HEADER( 's', 'p', 'i', 'h' );
	static const int a3dRailSystemInfoHeader =				MAKE_HEADER( 'r', 's', 'i', 'h' );
	static const int a3dVertexInfoHeader =					MAKE_HEADER( 'v', 'i', 'h', ' ' );
	static const int a3dShaderParamBaseHeader =				MAKE_HEADER( 's', 'p', 'b', ' ' );
	static const int a3dBoolShaderParamHeader =				MAKE_HEADER( 'b', 's', 'p', ' ' );
	static const int a3dBoolArrayShaderParamHeader =		MAKE_HEADER( 'b', 'a', 's', 'p ' );
	static const int a3dFloatShaderParamHeader =			MAKE_HEADER( 'f', 's', 'p', ' ' );
	static const int a3dFloatArrayShaderParamHeader =		MAKE_HEADER( 'f', 'a', 's', 'p ' );
	static const int a3dColorShaderParamHeader =			MAKE_HEADER( 'c', 's', 'p', ' ' );
	static const int a3dVectorShaderParamHeader =			MAKE_HEADER( 'v', 's', 'p', ' ' );
	static const int a3dIntShaderParamHeader =				MAKE_HEADER( 'i', 's', 'p', ' ' );
	static const int a3dIntArrayShaderParamHeader =			MAKE_HEADER( 'i', 'a', 's', 'p ' );
	static const int a3dMatrixShaderParamHeader =			MAKE_HEADER( 'm', 's', 'p', ' ' );
	static const int a3dMatrixArrayShaderParamHeader =		MAKE_HEADER( 'm', 'a', 's', 'p ' );
	static const int a3dTextureShaderParamHeader =			MAKE_HEADER( 't', 's', 'p', ' ' );
	static const int a3dTechniqueShaderParamHeader =		MAKE_HEADER( 't', 'h', 's', 'p' );
	static const int a3dBoundBoxHeader =					MAKE_HEADER( 'b', 'b', 'h', ' ' );
	static const int a3dSceneHeader =						MAKE_HEADER( 'a', 's', 'c', 'h' );
	static const int a3dObjectDescBaseHeader =				MAKE_HEADER( 'o', 'd', 'b', ' ' );
	static const int a3dModelDescHeader =					MAKE_HEADER( 'm', 'd', 'h', ' ' );
	static const int a3dLandDescHeader =					MAKE_HEADER( 'l', 'd', 'h', ' ' );
	static const int a3dParticleSystemDescHeader =			MAKE_HEADER( 'p', 's', 'd', 'h' );
	static const int a3dRailSystemDescHeader =				MAKE_HEADER( 'r', 's', 'd', 'h' );
	static const int a3dEffectDescHeader =					MAKE_HEADER( 'e', 'd', 'h', ' ' );
	static const int a3dTextureDescHeader =					MAKE_HEADER( 't', 'd', 'h', ' ' );
	static const int a3dFontDescHeader =					MAKE_HEADER( 'f', 'd', 'h', ' ' );
	static const int a3dSoundDescHeader =					MAKE_HEADER( 's', 'd', 'h', ' ' );
	static const int a3dLightMapInfoHeader =				MAKE_HEADER( 'l', 'm', 'i', ' ' );
	static const int a3dGraphNodeHeader =					MAKE_HEADER( 'g', 'n', 'h', ' ' );
	static const int a3dGraphHeader =						MAKE_HEADER( 'g', 'h', ' ', ' ' );
	static const int a3dDependencyPropertyHeader =			MAKE_HEADER ( 'd', 'p', 'h', ' ' );
	static const int particleSystemBaseHeader =				MAKE_HEADER ( 'p', 's', 'b', 'h' );
	static const int a3dSmokeParticleSystemHeader =			MAKE_HEADER ( 's', 'p', 's', 'h' );
	static const int a3dDependencyPropertyOwnerHeader =		MAKE_HEADER ( 'd', 'p', 'o', 'h' );

	static const int header =								MAKE_HEADER( 's', 'c', '3', 'a' );
	static const int model =								MAKE_HEADER( 'm', 'd', '3', 'a' );
	static const int landscape =							MAKE_HEADER( 'l', 'd', '3', 'a' );
	static const int particle =								MAKE_HEADER( 'p', 't', '3', 'a' );
	static const int railSystem =							MAKE_HEADER( 'r', 's', '3', 'a' );
	static const int light =								MAKE_HEADER( 'l', 't', '3', 'a' );
};

#endif