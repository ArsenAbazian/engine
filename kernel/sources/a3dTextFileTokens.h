#ifndef A3D_TEXT_MODEL_FILE_TOKENS_INC
#define A3D_TEXT_MODEL_FILE_TOKENS_INC

#include "hstring.h"
#include "lexeme.h"
#include "3dtypes.h"
#include "vecmat.h"

class a3dTextFileTokens { 

public:
	static shString a3dSceneString;
	static shString a3dSelectionString;
	static shString a3dNodeString;
	static shString a3dNameString;
	static shString a3dUndefinedString;
	static shString a3dcolorString;
	static shString a3dMatrix3String;
	static shString a3dQuatString;
	static shString a3dFilenameString;
	static shString a3dEndString;
	static shString a3dStringString;
	static shString a3dIntegerString;
	static shString a3dBooleanClassString;
	static shString a3dFloatString;
	static shString a3dColorString;
	static shString a3dTrueString;
	static shString a3dFalseString;
	static shString a3dBitMapString;
	static shString a3dStandardMaterialString;

	static shString a3dEditableMeshString;
	static shString a3dMeshVertexCountString;
	static shString a3dMeshFacesCountString;
	static shString a3dMeshVertexFVFString;
	static shString a3dMeshMapChannelsCountString;
	static shString a3dMeshTransformString;
	static shString a3dMeshPivotString;
	static shString a3dMeshPositionOffsetString;
	static shString a3dMeshRotationOffsetString;
	static shString a3dMeshScaleOffsetString;
	static shString a3dMeshOffsetTransformString;
	static shString a3dMeshMaterialNameString;
	static shString a3dMeshMap1ChannelVertCountString;
	static shString a3dMeshMap2ChannelVertCountString;
	static shString a3dMeshMap3ChannelVertCountString;
	static shString a3dMeshMap4ChannelVertCountString;
	static shString a3dMeshMap5ChannelVertCountString;
	static shString a3dMeshMap6ChannelVertCountString;
	static shString a3dMeshMap7ChannelVertCountString;
	static shString a3dMeshMap8ChannelVertCountString;
	static shString a3dMeshVertexListString;
	static shString a3dMeshFacesListString;
	static shString a3dMeshFaceMatIdListString;
	static shString a3dMeshMap1VertexListString;
	static shString a3dMeshMap2VertexListString;
	static shString a3dMeshMap3VertexListString;
	static shString a3dMeshMap4VertexListString;
	static shString a3dMeshMap5VertexListString;
	static shString a3dMeshMap6VertexListString;
	static shString a3dMeshMap7VertexListString;
	static shString a3dMeshMap8VertexListString;
	static shString a3dMeshMap1FaceListString;
	static shString a3dMeshMap2FaceListString;
	static shString a3dMeshMap3FaceListString;
	static shString a3dMeshMap4FaceListString;
	static shString a3dMeshMap5FaceListString;
	static shString a3dMeshMap6FaceListString;
	static shString a3dMeshMap7FaceListString;
	static shString a3dMeshMap8FaceListString;

	static shString a3dMaterialString;
	static shString a3dMaterialCountString;
	static shString a3dMaterialStandardString;
	static shString a3dMaterialShaderTypeString;
	static shString a3dMaterialShaderNameString;
	static shString a3dMaterialWireString;
	static shString a3dMaterialTwoSidedString;
	static shString a3dMaterialFacetedString;
	static shString a3dMaterialOpacityString;
	static shString a3dMaterialAmbientString;
	static shString a3dMaterialDiffuseString;
	static shString a3dMaterialSpecularString;
	static shString a3dMaterialSelfIllumString;
	static shString a3dMaterialSpecularLevelString;
	static shString a3dMaterialGlossinessString;
	static shString a3dMaterialSoftenString;

	static shString a3dMaterialMapEnablesListString;
	static shString a3dMaterialMapAmountsListString;
	static shString a3dMaterialMapAmbientString;
	static shString a3dMaterialMapDiffuseString;
	static shString a3dMaterialMapSpecularString;
	static shString a3dMaterialMapSpecularLevelString;
	static shString a3dMaterialMapGlossinessString;
	static shString a3dMaterialMapSelfIlluminationString;
	static shString a3dMaterialMapOpacityString;
	static shString a3dMaterialMapFilterColorString;
	static shString a3dMaterialMapBumpString;
	static shString a3dMaterialMapReflectionString;
	static shString a3dMaterialMapRefractionString;
	static shString a3dMaterialMapDisplacementString;

	static shString a3dMapClipUString;
	static shString a3dMapClipVString;
	static shString a3dMapClipWString;
	static shString a3dMapClipHString;
	static shString a3dMapUMirrorString;
	static shString a3dMapVMirrorString;
	static shString a3dMapUTileString;
	static shString a3dMapVTileString;
	static shString a3dMapUVWTypeString;
	static shString a3dMapChannelString;
	static shString a3dMapUOffsetString;
	static shString a3dMapVOffsetString;
	static shString a3dMapUTilingString;
	static shString a3dMapVTilingString;

	static shString a3dMultiMaterialString;
	static shString a3dMMSubMtlCountString;
	static shString a3dMMTypeListString;
	static shString a3dMMEnabledListString;
	static shString a3dMMNameListString;
	static shString a3dMMIdListString;

	static shString a3dSubMaterialCountString;

	static shString a3dDirectX_9_ShaderString;
	static shString a3dDXEffectFileString;
	static shString a3dDXTechniqueString;

	static shString a3dLightString;
	static shString	a3dOmnilightString;
	static shString a3dPositionString;
	static shString a3dFarAttenStartString;
	static shString a3dFarAttenEndString;
	static shString a3dDirectionallightString;
	static shString a3dDirectionString;
	
	static shString a3dModelString;
	static shString a3dfileNameString;
	static shString a3deffectFileNameString;
	static shString a3dParticleSystemString;

	static shString a3drailFileNameString;
	static shString a3dlineModelFileNameString;
	static shString a3droundModelFileNameString;
	static shString a3dlineEffectFileNameString;
	static shString a3droundEffectFileNameString;
	static shString a3dround2EffectFileNameString;
	static shString a3dlandNameString;
	static shString a3dlandHeightPixelXString;
	static shString a3dlandHeightPixelYString;
	static shString a3da3dRailSystemString;

	static shString a3dviewportcountString;
	static shString a3drendercountString;
	static shString a3ddropsizeString;
	static shString a3dspeedString;
	static shString a3dlifetimeString;
	static shString a3dbirthrateString;
	static shString a3demitterheightString;
	static shString a3demitterwidthString;

	static shString a3dmodelDirectoryString;
	static shString a3dtextureDirectoryString;
	static shString a3deffectDirectoryString;
	static shString a3dlandscapeDirectoryString;
	static shString a3dparticleDirectoryString;
	static shString a3drailDirectoryString;
	static shString a3dfontDirectoryString;
	static shString a3dSprayString;
	static shString a3dLandString;
	static shString a3dOmniLightString;
	static shString a3dDirectionalLightString;
};

void	findNextToken( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &tokenName );
bool	safeNextLex( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
bool	isEnd( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
bool	isToken( std::list< scriptLexeme* >::iterator &li, shString &tokenName );
bool	isTokenEnd( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString tokenName );
bool	readIntValue( std::list< scriptLexeme* >::iterator &li, int *intVal );
bool	readIntValue( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, int *intVal );
bool	readBoolValue( std::list< scriptLexeme* >::iterator &li, bool *boolVal );
bool	readFloatValue( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, float *floatVal );
bool	readFloatValue( std::list< scriptLexeme* >::iterator &li, float *floatVal );
bool	readStringValue( std::list< scriptLexeme* >::iterator &li, shString &val );
bool	readParser( std::list< scriptLexeme* >::iterator &li, lexParserType pars );
bool	readParamAssignment( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName );
bool	readIntParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, int *intVal );
bool	readFloatParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, float *floatVal );
bool	readBoolParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, bool *boolVal );
bool	readMatrixParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, Matrix4 mat );
bool	readVertexParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, Vector3 v );
bool	readQuaternionParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, Vector4 v );
bool	readColorValue( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, Vector4 v );
bool	readColorParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, Vector3 v );
bool	readStringParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, shString &stringVal );
bool	importVertex(  std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, Vector3 v );
bool	importFace(  std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, a3dFace &face );
bool	importStringList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, int count, shString *strings );
bool	importBooleanList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, int count, bool *values );
bool	importFloatList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, int count, float *values );
bool	importIntList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, int count, int *values  );
bool	isString( std::list< scriptLexeme* >::iterator &li );
bool	isBooleanClass( std::list< scriptLexeme* >::iterator &li );
bool    isFloatClass( std::list< scriptLexeme* >::iterator &li );
bool	isIntegerClass( std::list< scriptLexeme* >::iterator &li );
bool	isStringClass( std::list< scriptLexeme* >::iterator &li );
bool    isColorClass( std::list< scriptLexeme* >::iterator &li );
bool	isBitMapClass( std::list< scriptLexeme* >::iterator &li );
bool	isStandardMaterialClass( std::list< scriptLexeme* >::iterator &li );
bool	isTokenClass( std::list< scriptLexeme* >::iterator &li );
bool	findNextParserUntilParser( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, lexParserType findParser, lexParserType endParser );
bool	findNextParserUntilParser( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, lexParserType findParser, lexParserType endParser, lexParserType endParser2 );
bool	findNextConstantUntilParser( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, lexParserType endParser );

#endif