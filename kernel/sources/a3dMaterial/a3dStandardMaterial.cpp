#include "a3dStandardMaterial.h"
#include "memoryFile.h"
#include "..\\a3dTextFileTokens.h"

bool a3dStandardMaterial::importMapEnablesList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 

	if( !isToken( li, a3dTextFileTokens::a3dMaterialMapEnablesListString ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !importBooleanList( lexList, li, 12, this->mapEnablesList ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, a3dTextFileTokens::a3dEndString ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, a3dTextFileTokens::a3dMaterialMapEnablesListString ) ) return false;

	return true;
}

bool a3dStandardMaterial::importMapAmountsList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 

	if( !isToken( li, a3dTextFileTokens::a3dMaterialMapAmountsListString ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !importFloatList( lexList, li, 12, this->mapAmountsList ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, a3dTextFileTokens::a3dEndString ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, a3dTextFileTokens::a3dMaterialMapAmountsListString ) ) return false;

	return true;
}

bool a3dStandardMaterial::import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 

	if( !readStringParam( lexList, li, a3dTextFileTokens::a3dNameString, this->name ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !readIntParam( lexList, li, a3dTextFileTokens::a3dMaterialShaderTypeString, &this->shaderType ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !readStringParam( lexList, li, a3dTextFileTokens::a3dMaterialShaderNameString, this->shaderName ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !readBoolParam( lexList, li, a3dTextFileTokens::a3dMaterialWireString, &this->wire ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !readBoolParam( lexList, li, a3dTextFileTokens::a3dMaterialTwoSidedString, &this->twoSided ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !readBoolParam( lexList, li, a3dTextFileTokens::a3dMaterialFacetedString, &this->faceted ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !readFloatParam( lexList, li, a3dTextFileTokens::a3dMaterialOpacityString, &this->opacity ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !readColorParam( lexList, li, a3dTextFileTokens::a3dMaterialAmbientString, this->ambient ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !readColorParam( lexList, li, a3dTextFileTokens::a3dMaterialDiffuseString, this->diffuse ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !readColorParam( lexList, li, a3dTextFileTokens::a3dMaterialSpecularString, this->specular ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !readColorParam( lexList, li, a3dTextFileTokens::a3dMaterialSelfIllumString, this->selfIllum ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !readFloatParam( lexList, li, a3dTextFileTokens::a3dMaterialSpecularLevelString, &this->specularLevel ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !readFloatParam( lexList, li, a3dTextFileTokens::a3dMaterialGlossinessString, &this->glossiness ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !readFloatParam( lexList, li, a3dTextFileTokens::a3dMaterialSoftenString, &this->soften ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !this->importMapEnablesList( lexList, li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !this->importMapAmountsList( lexList, li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !this->importMaps( lexList, li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !isToken( li, a3dTextFileTokens::a3dEndString ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !isToken( li, a3dTextFileTokens::a3dMaterialStandardString ) ) return false;

	return true;
}

bool a3dStandardMaterial::importMaps( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 

	if( !this->importMap( a3dTextFileTokens::a3dMaterialMapAmbientString, &this->maps[ 0 ], lexList, li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !this->importMap( a3dTextFileTokens::a3dMaterialMapDiffuseString, &this->maps[ 1 ], lexList, li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !this->importMap( a3dTextFileTokens::a3dMaterialMapSpecularString, &this->maps[ 2 ], lexList, li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !this->importMap( a3dTextFileTokens::a3dMaterialMapSpecularLevelString, &this->maps[ 3 ], lexList, li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !this->importMap( a3dTextFileTokens::a3dMaterialMapGlossinessString, &this->maps[ 4 ], lexList, li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !this->importMap( a3dTextFileTokens::a3dMaterialMapSelfIlluminationString, &this->maps[ 5 ], lexList, li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !this->importMap( a3dTextFileTokens::a3dMaterialMapOpacityString, &this->maps[ 6 ], lexList, li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !this->importMap( a3dTextFileTokens::a3dMaterialMapFilterColorString, &this->maps[ 7 ], lexList, li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !this->importMap( a3dTextFileTokens::a3dMaterialMapBumpString, &this->maps[ 8 ], lexList, li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !this->importMap( a3dTextFileTokens::a3dMaterialMapReflectionString, &this->maps[ 9 ], lexList, li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !this->importMap( a3dTextFileTokens::a3dMaterialMapRefractionString, &this->maps[ 10 ], lexList, li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !this->importMap( a3dTextFileTokens::a3dMaterialMapDisplacementString, &this->maps[ 11 ], lexList, li ) ) return false;

	return true;
}

bool a3dStandardMaterial::importMap( shString &mapName, a3dBitmapMap **map, std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 

	if( !readParamAssignment(lexList, li, mapName) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( isToken( li, a3dTextFileTokens::a3dUndefinedString ) ) { 
		*map = null;
		return true;
	}

	(*map) = new a3dBitmapMap( mapName );
	if( !(*map)->import( lexList, li ) ) return false;

	return true;
}

bool a3dBitmapMap::import( std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li ) { 

	if( !isString( li ) ) return false;
	this->name = (*li)->name;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringParam( lexList, li, a3dTextFileTokens::a3dFilenameString, this->fileName ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatParam( lexList, li, a3dTextFileTokens::a3dMapClipUString, &this->clipu ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatParam( lexList, li, a3dTextFileTokens::a3dMapClipVString, &this->clipv ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatParam( lexList, li, a3dTextFileTokens::a3dMapClipWString, &this->clipw ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatParam( lexList, li, a3dTextFileTokens::a3dMapClipHString, &this->cliph ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readBoolParam( lexList, li, a3dTextFileTokens::a3dMapUMirrorString, &this->umirror ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readBoolParam( lexList, li, a3dTextFileTokens::a3dMapVMirrorString, &this->vmirror ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readBoolParam( lexList, li, a3dTextFileTokens::a3dMapUTileString, &this->utile ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readBoolParam( lexList, li, a3dTextFileTokens::a3dMapVTileString, &this->vtile ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readIntParam( lexList, li, a3dTextFileTokens::a3dMapUVWTypeString, &this->uvwType ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readIntParam( lexList, li, a3dTextFileTokens::a3dMapChannelString, &this->mapChannel ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatParam( lexList, li, a3dTextFileTokens::a3dMapUOffsetString, &this->uOffset ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatParam( lexList, li, a3dTextFileTokens::a3dMapVOffsetString, &this->vOffset ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatParam( lexList, li, a3dTextFileTokens::a3dMapUTilingString, &this->uTiling ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatParam( lexList, li, a3dTextFileTokens::a3dMapVTilingString, &this->vTiling ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, a3dTextFileTokens::a3dEndString ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, this->mapType ) ) return false;

	return true;
}

a3dMapBase* a3dMapBase::clone() { return null; }

a3dMapBase* a3dBitmapMap::clone() { 

	a3dBitmapMap *map = new a3dBitmapMap( this->mapType );

	map->fileName = this->fileName;
	map->name = this->name;
	map->clipu = this->clipu;
	map->clipv = this->clipv;
	map->clipw = this->clipw;
	map->cliph = this->cliph;
	map->umirror = this->umirror;
	map->vmirror = this->vmirror;
	map->utile = this->utile;
	map->vtile = this->vtile;
	map->uvwType = this->uvwType;
	map->mapChannel = this->mapChannel;
	map->uOffset = this->uOffset;
	map->vOffset = this->vOffset;
	map->uTiling = this->uTiling;
	map->vTiling = this->vTiling;

	return map;
}

void a3dStandardMaterial::clear() { 

	for( int i = 0; i < 12; i ++ ) { 
		if( this->maps[ i ] != null ) delete this->maps[i];
	}
}

a3dMaterialBase* a3dStandardMaterial::clone() {

	a3dStandardMaterial *mat = new a3dStandardMaterial();

	mat->name = this->name;

	mat->shaderType = this->shaderType;
	mat->wire = this->wire;
	mat->twoSided = this->twoSided;
	mat->faceted = this->faceted;
	mat->opacity = this->opacity;
	copyVector3( this->ambient, mat->ambient );
	copyVector3( this->diffuse, mat->diffuse );
	copyVector3( this->specular, mat->specular );
	copyVector3( this->selfIllum, mat->selfIllum );
	mat->specularLevel = this->specularLevel;
	mat->glossiness = this->glossiness;
	mat->soften = this->soften;

	int i;
	for( i = 0; i < 12; i ++ ) { 
		mat->mapEnablesList[ i ] = this->mapEnablesList[ i ];
		mat->mapAmountsList[ i ] = this->mapAmountsList[ i ];
		if( this->maps[ i ] == null ) mat->maps[ i ] = null;
		else mat->maps[ i ] = (a3dBitmapMap*) this->maps[ i ]->clone();
	}

	return mat;
}
