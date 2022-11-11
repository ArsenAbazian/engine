#include "a3dTextModelFileTokens.h"

void a3dTextModelFileTokens::Initialize() { 

	if(a3dSceneString.getLength() == 0) return;

	a3dSceneString.initialize( TEXT( "a3dScene" ) );
	a3dSelectionString.initialize( TEXT( "a3dSelection" ) );
	a3dNodeString.initialize( TEXT( "a3dNode" ) );
	a3dNameString.initialize( TEXT( "name" ) );
	a3dUndefinedString.initialize( TEXT( "undefined" ) );
	a3dcolorString.initialize( TEXT( "color" ) );
	a3dMatrix3String.initialize( TEXT( "matrix3" ) );
	a3dQuatString.initialize( TEXT( "quat" ) );
	a3dFilenameString.initialize( TEXT( "filename" ) );
	a3dEndString.initialize( TEXT( "end" ) );
	a3dTrueString.initialize( TEXT( "true" ) );
	a3dFalseString.initialize( TEXT( "false" ) );
	a3dBitMapString.initialize( TEXT( "BitMap" ) );
	a3dStandardMaterialString.initialize ( TEXT( "Standardmaterial" ) );

	a3dStringString.initialize( TEXT( "String" ) );
	a3dIntegerString.initialize( TEXT( "Integer" ) );
	a3dBooleanClassString.initialize( TEXT( "BooleanClass" ) );
	a3dFloatString.initialize( TEXT( "Float" ) );
	a3dColorString.initialize( TEXT( "Color" ) );

	a3dEditableMeshString.initialize( TEXT( "Editable_mesh" ) );
	a3dMeshVertexCountString.initialize( TEXT( "vertexCount" ) );
	a3dMeshFacesCountString.initialize( TEXT( "facesCount" ) );
	a3dMeshVertexFVFString.initialize( TEXT( "vertexFVF" ) ); 
	a3dMeshMapChannelsCountString.initialize( TEXT( "mapChannelCount" ) );
	a3dMeshTransformString.initialize( TEXT( "transform" ) );
	a3dMeshPivotString.initialize( TEXT( "pivot" ) );
	a3dMeshPositionOffsetString.initialize( TEXT( "objectOffsetPos" ) );
	a3dMeshRotationOffsetString.initialize( TEXT( "objectOffsetRot" ) );
	a3dMeshScaleOffsetString.initialize( TEXT( "objectOffsetScale" ) );
	a3dMeshOffsetTransformString.initialize( TEXT( "objectTransform" ) );
	a3dMeshMaterialNameString.initialize( TEXT( "materialName" ) );
	a3dMeshMap1ChannelVertCountString.initialize( TEXT( "map1ChannelVertCount" ) );
	a3dMeshMap2ChannelVertCountString.initialize( TEXT( "map2ChannelVertCount" ) );
	a3dMeshMap3ChannelVertCountString.initialize( TEXT( "map3ChannelVertCount" ) );
	a3dMeshMap4ChannelVertCountString.initialize( TEXT( "map4ChannelVertCount" ) );
	a3dMeshMap5ChannelVertCountString.initialize( TEXT( "map5ChannelVertCount" ) );
	a3dMeshMap6ChannelVertCountString.initialize( TEXT( "map6ChannelVertCount" ) );
	a3dMeshMap7ChannelVertCountString.initialize( TEXT( "map7ChannelVertCount" ) );
	a3dMeshMap8ChannelVertCountString.initialize( TEXT( "map8ChannelVertCount" ) );
	a3dMeshVertexListString.initialize( TEXT( "vertexList" ) );
	a3dMeshFacesListString.initialize( TEXT( "faceList" ) );
	a3dMeshFaceMatIdListString.initialize( TEXT( "faceMatIDList" ) );
	a3dMeshMap1VertexListString.initialize( TEXT( "map1VertexList" ) );
	a3dMeshMap2VertexListString.initialize( TEXT( "map2VertexList" ) );
	a3dMeshMap3VertexListString.initialize( TEXT( "map3VertexList" ) );
	a3dMeshMap4VertexListString.initialize( TEXT( "map4VertexList" ) );
	a3dMeshMap5VertexListString.initialize( TEXT( "map5VertexList" ) );
	a3dMeshMap6VertexListString.initialize( TEXT( "map6VertexList" ) );
	a3dMeshMap7VertexListString.initialize( TEXT( "map7VertexList" ) );
	a3dMeshMap8VertexListString.initialize( TEXT( "map8VertexList" ) );

	a3dMeshMap1FaceListString.initialize( TEXT( "map1FaceList" ) );
	a3dMeshMap2FaceListString.initialize( TEXT( "map2FaceList" ) );
	a3dMeshMap3FaceListString.initialize( TEXT( "map3FaceList" ) );
	a3dMeshMap4FaceListString.initialize( TEXT( "map4FaceList" ) );
	a3dMeshMap5FaceListString.initialize( TEXT( "map5FaceList" ) );
	a3dMeshMap6FaceListString.initialize( TEXT( "map6FaceList" ) );
	a3dMeshMap7FaceListString.initialize( TEXT( "map7FaceList" ) );
	a3dMeshMap8FaceListString.initialize( TEXT( "map8FaceList" ) );

	a3dMaterialString.initialize( TEXT( "a3dMaterial" ) );
	a3dMaterialCountString.initialize( TEXT( "materialCount" ) );
	a3dMaterialStandardString.initialize( TEXT( "StandardMaterial" ) );
	a3dMaterialShaderTypeString.initialize( TEXT( "shaderType" ) );
	a3dMaterialShaderNameString.initialize( TEXT( "shaderName" ) );
	a3dMaterialWireString.initialize( TEXT( "wire" ) );
	a3dMaterialTwoSidedString.initialize( TEXT( "twoSided" ) );
	a3dMaterialFacetedString.initialize( TEXT( "faceted" ) );
	a3dMaterialOpacityString.initialize( TEXT( "opacity" ) );
	a3dMaterialAmbientString.initialize( TEXT( "ambient" ) );
	a3dMaterialDiffuseString.initialize( TEXT( "diffuse" ) );
	a3dMaterialSpecularString.initialize( TEXT( "specular" ) );
	a3dMaterialSelfIllumString.initialize( TEXT( "selfIllum" ) );
	a3dMaterialSpecularLevelString.initialize( TEXT( "specularLevel" ) );
	a3dMaterialGlossinessString.initialize( TEXT( "glossiness" ) );
	a3dMaterialSoftenString.initialize( TEXT( "soften" ) );

	a3dMaterialMapEnablesListString.initialize( TEXT( "mapEnablesList" ) );
	a3dMaterialMapAmountsListString.initialize( TEXT( "mapAmountsList" ) );

	a3dMaterialMapAmbientString.initialize( TEXT( "Ambient" ) );
	a3dMaterialMapDiffuseString.initialize( TEXT( "Diffuse" ) );
	a3dMaterialMapSpecularString.initialize( TEXT( "Specular" ) );
	a3dMaterialMapSpecularLevelString.initialize( TEXT( "SpecularLevel" ) );
	a3dMaterialMapGlossinessString.initialize( TEXT( "Glossiness" ) );
	a3dMaterialMapSelfIlluminationString.initialize( TEXT( "SelfIllumination" ) );
	a3dMaterialMapOpacityString.initialize( TEXT( "Opacity" ) );
	a3dMaterialMapFilterColorString.initialize( TEXT( "FilterColor" ) );
	a3dMaterialMapBumpString.initialize( TEXT( "Bump" ) );
	a3dMaterialMapReflectionString.initialize( TEXT( "Reflection" ) );
	a3dMaterialMapRefractionString.initialize( TEXT( "Refraction" ) );
	a3dMaterialMapDisplacementString.initialize( TEXT( "Displacement" ) );	

	a3dMapClipUString.initialize( TEXT( "clipu" ) );
	a3dMapClipVString.initialize( TEXT( "clipv" ) );
	a3dMapClipWString.initialize( TEXT( "clipw" ) );
	a3dMapClipHString.initialize( TEXT( "cliph" ) );
	a3dMapUMirrorString.initialize( TEXT( "umirror" ) );
	a3dMapVMirrorString.initialize( TEXT( "vmirror" ) );
	a3dMapUTileString.initialize( TEXT( "utile" ) );
	a3dMapVTileString.initialize( TEXT( "vtile" ) );
	a3dMapUVWTypeString.initialize( TEXT( "uvwType" ) );
	a3dMapChannelString.initialize( TEXT( "mapChannel" ) );
	a3dMapUOffsetString.initialize( TEXT( "uOffset" ) );
	a3dMapVOffsetString.initialize( TEXT( "vOffset" ) );
	a3dMapUTilingString.initialize( TEXT( "uTiling" ) );
	a3dMapVTilingString.initialize( TEXT( "vTiling" ) );

	a3dMultiMaterialString.initialize( TEXT( "MultiMaterial" ) );
	a3dMMSubMtlCountString.initialize( TEXT( "subMtlCount" ) );

	a3dMMTypeListString.initialize( TEXT( "materialTypeList" ) );
	a3dMMEnabledListString.initialize( TEXT( "mapEnabledList" ) );
	a3dMMNameListString.initialize( TEXT( "materialNameList" ) );
	a3dMMIdListString.initialize( TEXT( "materialIdList" ) );

	a3dSubMaterialCountString.initialize( TEXT( "subMaterialCount" ) );

	a3dDirectX_9_ShaderString.initialize( TEXT( "DirectX_9_Shader" ) );
	a3dDXEffectFileString.initialize( TEXT( "effectFile" ) );
	a3dDXTechniqueString.initialize( TEXT( "technique" ) );

	a3dLightString.initialize( TEXT( "a3dLight" ) );
	a3dOmnilightString.initialize( TEXT( "Omnilight" ) );
	a3dPositionString.initialize( TEXT( "position" ) );
	a3dFarAttenStartString.initialize( TEXT( "farAttenStart" ) );
	a3dFarAttenEndString.initialize( TEXT( "farAttenEnd" ) );
	a3dDirectionallightString.initialize( TEXT( "Directionallight" ) );
	a3dDirectionString.initialize( TEXT( "direction" ) );
}

bool isString( std::list< scriptLexeme* >::iterator &li ) { 

	return (*li)->type == LT_STRING;
}

bool importStringList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, int count, shString *strings ) { 

	for( int i = 0; i < count; i++ ) { 
		if( !isString( li ) ) return false;
		strings[ i ] = (*li)->name;

		if( i == count - 1 ) break;
		if( !safeNextLex( lexList, li ) ) return false;
	}

	return true;
}

bool importBooleanList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, int count, bool *values ) {

	for( int i = 0; i < count; i++ ) { 
		if( !readBoolValue( li, &values[ i ] ) ) return false;

		if( i == count - 1 ) break;
		if( !safeNextLex( lexList, li ) ) return false;
	}

	return true;
}

bool importIntList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, int count, int *values  ) {

	for( int i = 0; i < count; i++ ) { 
		if( !(*li)->isNumericConst() ) return false;
		values[ i ] = (*li)->longValue;

		if( i == count - 1 ) break;
		if( !safeNextLex( lexList, li ) ) return false;
	}

	return true;
}

bool importFloatList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, int count, float *values ) { 

	for( int i = 0; i < count; i++ ) { 
		if( !readFloatValue( lexList, li, &values[ i ] ) ) return false;

		if( i == count - 1 ) break;
		if( !safeNextLex( lexList, li ) ) return false;
	}

	return true;
}

bool	isBooleanClass( std::list< scriptLexeme* >::iterator &li ) {
	return isToken( li, a3dTextModelFileTokens::a3dBooleanClassString );
}

bool    isFloatClass( std::list< scriptLexeme* >::iterator &li ) { 
	return isToken( li, a3dTextModelFileTokens::a3dFloatString );
}

bool	isIntegerClass( std::list< scriptLexeme* >::iterator &li ) { 
	return isToken( li, a3dTextModelFileTokens::a3dIntegerString );
}

bool	isStringClass( std::list< scriptLexeme* >::iterator &li ) { 
	return isToken( li, a3dTextModelFileTokens::a3dStringString );
}

bool    isColorClass( std::list< scriptLexeme* >::iterator &li ) { 
	return isToken( li, a3dTextModelFileTokens::a3dColorString );
}

bool	isBitMapClass( std::list< scriptLexeme* >::iterator &li ) { 
	return isToken( li, a3dTextModelFileTokens::a3dBitMapString );
}

bool	isStandardMaterialClass( std::list< scriptLexeme* >::iterator &li ) { 
	return isToken( li, a3dTextModelFileTokens::a3dStandardMaterialString );
} 

bool	isToken( std::list< scriptLexeme* >::iterator &li, shString &tokenName ) { return (*li)->isId( tokenName ); }

void findNextToken( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &tokenName ) { 

	for( ; li != lexList.end(); li ++ ) { 
		if( (*li)->isId( tokenName ) ) return ;
	}
	return ;
}

bool importFace(  std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, a3dFace &face ) {

	if( !readParser( li, LPT_LEFT_SQUARE_BRACKET ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;	
	if( !readIntValue( lexList, li, &face.aIndex ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readParser( li, LPT_COMMA ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readIntValue( lexList, li, &face.bIndex ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readParser( li, LPT_COMMA ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readIntValue( lexList, li, &face.cIndex ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;	
	if( !readParser( li, LPT_RIGHT_SQUARE_BRACKET ) ) return false;

	// in max index starts from 1
	face.aIndex --;
	face.bIndex --;
	face.cIndex --;

	return true;
}

bool readFloatValue( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, float *floatVal ) { 

	float val = 1.0f;
	if( readParser( li, LPT_SUBTRACTION ) ) { 
		val = -1.0f;
		if( !safeNextLex( lexList, li ) ) return false;
	}

	if( (*li)->type != LT_CONST ) return false;
	if( (*li)->isNumericConst() ) *floatVal = (float)(*li)->longValue;
	else if( (*li)->constType == LCT_FLOAT ) *floatVal = (*li)->floatValue;
	else if( (*li)->constType == LCT_DOUBLE ) *floatVal = (*li)->doubleValue;
	// учитываем знак
	*floatVal *= val;

	return true;
}

bool readIntValue( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, int *intVal ) { 

	int val = 1;
	if( readParser( li, LPT_SUBTRACTION ) ) { 
		val = -1;
		if( !safeNextLex( lexList, li ) ) return false;
	}

	if( !(*li)->isNumericConst() ) return false;
	*intVal = (*li)->longValue;

	*intVal *= val;

	return true;
}

bool readBoolValue( std::list< scriptLexeme* >::iterator &li, bool *boolVal ) { 

	if( isToken( li, a3dTextModelFileTokens::a3dTrueString ) ) *boolVal = true;
	else if( isToken( li, a3dTextModelFileTokens::a3dFalseString ) ) *boolVal = false;
	else return false;

	return true;
}

bool readParser( std::list< scriptLexeme* >::iterator &li, lexParserType pars ) { 

	if( (*li)->type != LT_PARSER || (*li)->parser != pars ) return false;

	return true;
}

bool importVertex( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, Vector3 v ) {

	if( !readParser( li, LPT_LEFT_SQUARE_BRACKET ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;	
	if( !readFloatValue( lexList, li, &(v[0]) ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readParser( li, LPT_COMMA ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatValue( lexList, li, &(v[1]) ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readParser( li, LPT_COMMA ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatValue( lexList, li, &(v[2]) ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;	
	if( !readParser( li, LPT_RIGHT_SQUARE_BRACKET ) ) return false;

	return true;
}

bool isTokenEnd( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString tokenName ) { 

	if( !(*li)->isId( a3dTextModelFileTokens::a3dEndString ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !(*li)->isId( tokenName ) ) return false;

	return true;
}

bool isEnd( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 
	return li == lexList.end();
}

bool safeNextLex( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 

	li ++;
	std::list< scriptLexeme* >::iterator end = lexList.end();
	if( li == lexList.end() ) return false;

	return true;
}

bool readParamAssignment( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName ) { 

	if( !(*li)->isId( valName ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readParser( li, LPT_ASSIGNMENT ) ) return false;

	return true;
}

bool readBoolParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, bool *boolVal ) { 

	if( !readParamAssignment( lexList, li, valName ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readBoolValue( li, boolVal ) ) return false;

	return true;
}

bool readFloatParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, float *floatVal ) { 

	if( !readParamAssignment( lexList, li, valName ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatValue( lexList, li, floatVal ) ) return false;

	return true;
}

bool readIntParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, int *intVal ) { 

	if( !readParamAssignment( lexList, li, valName ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !(*li)->isNumericConst() ) return false;

	*intVal = (*li)->longValue;

	return true;
}

bool readMatrixParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, Matrix4 mat ) { 

	Vector3 v;
	originMatrix4( mat );

	if( !readParamAssignment( lexList, li, valName ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !readParser( li, LPT_LEFT_BRACKET ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !(*li)->isId( a3dTextModelFileTokens::a3dMatrix3String ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !importVertex( lexList, li, v ) ) return false;
	mat[0][0] = v[0]; mat[0][1] = v[1]; mat[0][2] = v[2];

	if( !safeNextLex( lexList, li ) ) return false;
	if( !importVertex( lexList, li, v ) ) return false;
	mat[1][0] = v[0]; mat[1][1] = v[1]; mat[1][2] = v[2];

	if( !safeNextLex( lexList, li ) ) return false;
	if( !importVertex( lexList, li, v ) ) return false;
	mat[2][0] = v[0]; mat[2][1] = v[1]; mat[2][2] = v[2];

	if( !safeNextLex( lexList, li ) ) return false;
	if( !importVertex( lexList, li, v ) ) return false;
	mat[3][0] = v[0]; mat[3][1] = v[1]; mat[3][2] = v[2];

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readParser( li, LPT_RIGHT_BRACKET ) ) return false;

	return true;
}

bool readVertexParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, Vector3 v ) { 

	if( !readParamAssignment( lexList, li, valName ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !importVertex( lexList, li, v ) ) return false;

	return true;
}

bool readColorValue( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, Vector4 v ) { 

	if( !readParser( li, LPT_LEFT_BRACKET ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !(*li)->isId( a3dTextModelFileTokens::a3dcolorString ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !readFloatValue( lexList, li, &(v[0])) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatValue( lexList, li, &(v[1])) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatValue( lexList, li, &(v[2])) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;

	scaleVector3( v, 1.0f / 255.0f );
	if( readParser( li, LPT_RIGHT_BRACKET ) ) return true;

	if( !readFloatValue( lexList, li, &(v[3])) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readParser( li, LPT_RIGHT_BRACKET ) ) return false;
	v[3] *= 1.0f / 255.0f; // if 4d color

	return true;
}

bool readColorParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, Vector4 v ) { 

	if( !readParamAssignment( lexList, li, valName ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readColorValue( lexList, li, v ) ) return false;	

	return true;
}

bool readQuaternionParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, Vector4 v ) { 

	if( !readParamAssignment( lexList, li, valName ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !readParser( li, LPT_LEFT_BRACKET ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !(*li)->isId( a3dTextModelFileTokens::a3dQuatString ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !readFloatValue( lexList, li, &(v[0])) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatValue( lexList, li, &(v[1])) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatValue( lexList, li, &(v[2])) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatValue( lexList, li, &(v[3])) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readParser( li, LPT_RIGHT_BRACKET ) ) return false;

	return true;
}

bool readStringValue( std::list< scriptLexeme* >::iterator &li, shString &val ) {

	if( (*li)->type != LT_STRING ) return false;
	val = (*li)->name;
	val.cutSpacesFromTheEnds();

	return true;
}

bool readStringParam( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &valName, shString &stringVal ) { 

	if( !readParamAssignment( lexList, li, valName ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !readStringValue( li, stringVal ) ) return false; 

	return true;
}


bool isTokenClass( std::list< scriptLexeme* >::iterator &li ) { 

	return (*li)->type == LT_ID;
}
