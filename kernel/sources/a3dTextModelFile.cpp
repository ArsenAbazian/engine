#include "a3dTextModelFile.h"
#include "a3dScene.h"
#include "a3dDesc\\a3dDesc.h"
#include "scaner.h"
#include "a3dMaterial\\a3dMultiMaterial.h"
#include "a3dMaterial\\a3dShaderMaterial.h"
#include "a3dMaterial\\a3dStandardMaterial.h"
#include "a3dTextFileTokens.h"

bool a3dTextModelFile ::loadFromFile( shString &fileName ) { 
	
	scriptScaner	scaner;
	scriptRetCode	retCode;
	
	// запретить форматировать строки
	scaner.enableFormatString = false;

	retCode = scaner.openFile( fileName.getCString() );
	if( retCode != SRV_SUCCESS )
		return false;

	// сканируем файл
	retCode = scaner.scan();
	if( retCode != SRV_SUCCESS )
		return false;

	// очищаем ресурсы
	this->clear();

	if( !this->initializeMeshes( scaner.getLexemesList() ) ) return false;
	if( !this->initializeMaterials( scaner.getLexemesList() ) ) return false;
	if( !this->initializeLights( scaner.getLexemesList() ) ) return false;

	return true;
}

void a3dTextModelFile ::clear() { 
	
	if( this->meshList != null ) delete[] this->meshList;
	if( this->materialList != null ) delete[] this->materialList;
	if( this->lightList != null ) delete[] this->lightList;

	this->meshList = null;
	this->materialList = null;
	this->lightList = null;
	this->meshCount = 0;
	this->materialCount = 0;
}

int a3dTextModelFile ::getTokenCount( std::list< scriptLexeme* > &lexList, shString &tokenName ) { 
	
	int tcount = 0;
	
	std::list< scriptLexeme* >::iterator li;
	for( li = lexList.begin(); li != lexList.end(); li++ ) { 
		if( (*li)->isId( tokenName ) ) tcount ++;
	}

	return tcount;
}

int a3dTextModelFile ::getMeshCount( std::list< scriptLexeme* > &lexList ) { 
	
	return this->getTokenCount( lexList, a3dTextFileTokens::a3dEditableMeshString );
}

int a3dTextModelFile ::getMaterialCount( std::list< scriptLexeme* > &lexList ) { 
	
	return this->getTokenCount( lexList, a3dTextFileTokens::a3dMaterialString );
}

int a3dTextModelFile ::getLightCount( std::list< scriptLexeme* > &lexList ) { 
	
	return this->getTokenCount( lexList, a3dTextFileTokens::a3dLightString );
}



void a3dTextModelFile ::findNextMesh( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 
	
	return findNextToken( lexList, li, a3dTextFileTokens::a3dEditableMeshString );
}

void a3dTextModelFile ::findNextMaterial( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 
	
	return findNextToken( lexList, li, a3dTextFileTokens::a3dMaterialString );
}

void a3dTextModelFile ::findNextLight( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 
	
	return findNextToken( lexList, li, a3dTextFileTokens::a3dLightString );
}

a3dMaterialBase* a3dTextModelFile ::getMaterial( shString &name ) { 
	
	int mi;
	for( mi = 0; mi < this->materialCount; mi ++ ) { 
		if( this->materialList[ mi ]->name == name ) return this->materialList[ mi ];
	} 

	return null;
}


bool a3dTextModelFile ::initializeMesh( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, a3dMesh &mesh ) { 
	
	return mesh.import( lexList, li );
}

a3dMesh::a3dMesh( ) { 
	
	name.clear();
	this->vertexCount = 0;
	this->vertexList = null;
	
	this->faceCount = 0;
	this->faceList = null;
	this->faceMatIdList = null;
	
	this->mapChannelCount = 0;
	originMatrix4( this->transform );
	clearVector3( this->pivot );
	clearVector3( this->objectOffsetPos );
	clearVector4( this->objectOffsetRot );
	clearVector3( this->objectOffsetScale );
	originMatrix4( this->objectTransform );
	
	materialName.clear();

	int index;
	for( index = 0; index < 8; index ++ ) { 
		mapChannelVertCount[ index ] = 0;
		mapChannelCoors[ index ] = null;
		mapChannelFaceList[ index ] = null;
	}
}

void a3dMesh::clear() { 

	name.clear();
	this->vertexCount = 0;
	if( this->vertexList != null ) 
		delete[] this->vertexList;
	this->vertexList = null;
	
	this->faceCount = 0;
	if( this->faceList != null )
		delete[] this->faceList;
	this->faceList = null;
	
	if( this->faceMatIdList != null )
		delete[] this->faceMatIdList;
	this->faceMatIdList = null;
	
	this->mapChannelCount = 0;
	originMatrix4( this->transform );
	clearVector3( this->pivot );
	clearVector3( this->objectOffsetPos );
	clearVector4( this->objectOffsetRot );
	clearVector3( this->objectOffsetScale );
	originMatrix4( this->objectTransform );
	
	materialName.clear();

	int index;
	for( index = 0; index < 8; index ++ ) { 
		this->mapChannelVertCount[ index ] = 0;
		
		if( this->mapChannelCoors[ index ] != null )
			delete[] this->mapChannelCoors[ index ];
		this->mapChannelCoors[ index ] = null;
		
		if( this->mapChannelFaceList[ index ] != null )
			delete[] this->mapChannelFaceList[ index ];
		this->mapChannelFaceList[ index ] = null;
	}
	
	return;
}

void a3dMesh::getVertexTextureCoor( int vIndex, int tIndex, Vector3 tcoor ) { 
	
	clearVector3( tcoor );	
	
	// если нет копируем из первой
	if( this->mapChannelCoors[tIndex] == null ) tIndex = 0;
	
	if( this->mapChannelCoors[tIndex] == null || vIndex >= this->vertexCount ) return;	
	
	int fi, tVertex = -1;
	for( fi = 0; fi < this->faceCount; fi++ ) { 
		if( this->faceList[ fi ].aIndex == vIndex ) { 
			tVertex = this->mapChannelFaceList[ tIndex ][ fi ].aIndex;
			break;
		}
		else if( this->faceList[ fi ].bIndex == vIndex ) { 
			tVertex = this->mapChannelFaceList[ tIndex ][ fi ].bIndex;
			break;
		}
		else if( this->faceList[ fi ].cIndex == vIndex ) { 
			tVertex = this->mapChannelFaceList[ tIndex ][ fi ].cIndex;
			break;
		}
	}
	
	if( tVertex >= 0 && tVertex < this->mapChannelVertCount[ tIndex ] ) { 
		copyVector3( this->mapChannelCoors[ tIndex ][ tVertex ], tcoor );
	}
	
	return ;
}

bool a3dMesh::importVertices( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 
	
	// считываем vertexList
	if( !(*li)->isId( a3dTextFileTokens::a3dMeshVertexListString ) ) return false;

	int vIndex;
	for( vIndex = 0; vIndex < this->vertexCount; vIndex ++ ) { 
		if( !safeNextLex( lexList, li ) ) return false;
		if( !(*li)->isNumericConst() ) return false;
		
		if( !safeNextLex( lexList, li ) ) return false;	
		if( !importVertex( lexList, li, this->vertexList[ vIndex ] ) ) return false;
	} 	

	if( !safeNextLex( lexList, li ) ) return false;
	if( !isTokenEnd( lexList, li, a3dTextFileTokens::a3dMeshVertexListString ) ) return false;
	
	return true;
}



bool a3dMesh::importFaces( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 
	
	// считываем vertexList
	if( !(*li)->isId( a3dTextFileTokens::a3dMeshFacesListString ) ) return false;

	int fIndex;
	for( fIndex = 0; fIndex < this->faceCount; fIndex ++ ) { 
		if( !safeNextLex( lexList, li ) ) return false;
		if( !(*li)->isNumericConst() ) return false;
		
		if( !safeNextLex( lexList, li ) ) return false;	
		if( !importFace( lexList, li, this->faceList[ fIndex ] ) ) return false;
	} 	
	
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isTokenEnd( lexList, li, a3dTextFileTokens::a3dMeshFacesListString ) ) return false;

	return true;
}

bool a3dMesh::importMapsChannelCount( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 
	
	if( !readIntParam( lexList, li, a3dTextFileTokens::a3dMeshMap1ChannelVertCountString, &this->mapChannelVertCount[0] ) ) return false; 
	
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readIntParam( lexList, li, a3dTextFileTokens::a3dMeshMap2ChannelVertCountString, &this->mapChannelVertCount[1] ) ) return false; 
	
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readIntParam( lexList, li, a3dTextFileTokens::a3dMeshMap3ChannelVertCountString, &this->mapChannelVertCount[2] ) ) return false; 
	
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readIntParam( lexList, li, a3dTextFileTokens::a3dMeshMap4ChannelVertCountString, &this->mapChannelVertCount[3] ) ) return false; 
	
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readIntParam( lexList, li, a3dTextFileTokens::a3dMeshMap5ChannelVertCountString, &this->mapChannelVertCount[4] ) ) return false; 
	
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readIntParam( lexList, li, a3dTextFileTokens::a3dMeshMap6ChannelVertCountString, &this->mapChannelVertCount[5] ) ) return false; 
	
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readIntParam( lexList, li, a3dTextFileTokens::a3dMeshMap7ChannelVertCountString, &this->mapChannelVertCount[6] ) ) return false; 
	
	if( !safeNextLex( lexList, li ) ) return false;	
	if( !readIntParam( lexList, li, a3dTextFileTokens::a3dMeshMap8ChannelVertCountString, &this->mapChannelVertCount[7] ) ) return false; 
	
	return true;
}

bool a3dMesh::importFaceMatIdList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 

	if( !isToken( li, a3dTextFileTokens::a3dMeshFaceMatIdListString ) ) return false;

	int faceMatIdIndex;
	int index;
	for( faceMatIdIndex = 0; faceMatIdIndex < this->faceCount; faceMatIdIndex ++ ) { 
		if( !safeNextLex( lexList, li ) ) return false;	
		if( !readIntValue( lexList, li, &index ) ) return false;
		if( !safeNextLex( lexList, li ) ) return false;	
		if( !readIntValue( lexList, li, &this->faceMatIdList[ faceMatIdIndex ] ) ) return false;
	}
	
	if( !safeNextLex( lexList, li ) ) return false;	
	if( !isTokenEnd( lexList, li, a3dTextFileTokens::a3dMeshFaceMatIdListString ) ) return false;

	return true;
} 

int a3dMesh::getSubMaterialFaceCount(int id) { 
	
	int fc = 0;
	for( int i = 0; i < this->faceCount; i ++ ) { 
		if( this->faceMatIdList[i] == id ) fc ++;
	}
	
	return fc;
}

void a3dMesh::fillSubMaterialFaceList(a3dFace *face, int id) { 
	
	int fc = 0;
	for( int i = 0; i < this->faceCount; i ++ ) { 
		if( this->faceMatIdList[i] == id ) { 
			memcpy( &face[fc], &this->faceList[i], sizeof(a3dFace) );
			fc ++;
		}
	}
	return ;
}

void a3dMesh::initializeData( ) { 

	this->vertexList = new Vector3[ this->vertexCount ];
	this->faceList = new a3dFace[ this->faceCount ];
	this->faceMatIdList = new int[ this->faceCount ];
	
	if( this->mapChannelVertCount[0] != 0) {
		this->mapChannelCoors[0] = new Vector3[ this->mapChannelVertCount[0] ];
		this->mapChannelFaceList[0] = new a3dFace[ this->faceCount ];
	}
	if( this->mapChannelVertCount[1] != 0) {
		this->mapChannelCoors[1] = new Vector3[ this->mapChannelVertCount[1] ];
		this->mapChannelFaceList[1] = new a3dFace[ this->faceCount ];
	}
	if( this->mapChannelVertCount[2] != 0) {
		this->mapChannelCoors[2] = new Vector3[ this->mapChannelVertCount[2] ];
		this->mapChannelFaceList[2] = new a3dFace[ this->faceCount ];
	}
	if( this->mapChannelVertCount[3] != 0) {
		this->mapChannelCoors[3] = new Vector3[ this->mapChannelVertCount[3] ];
		this->mapChannelFaceList[3] = new a3dFace[ this->faceCount ];
	}
	if( this->mapChannelVertCount[4] != 0) {
		this->mapChannelCoors[4] = new Vector3[ this->mapChannelVertCount[4] ];
		this->mapChannelFaceList[4] = new a3dFace[ this->faceCount ];
	}
	if( this->mapChannelVertCount[5] != 0) {
		this->mapChannelCoors[5] = new Vector3[ this->mapChannelVertCount[5] ];
		this->mapChannelFaceList[5] = new a3dFace[ this->faceCount ];
	}
	if( this->mapChannelVertCount[6] != 0) {
		this->mapChannelCoors[6] = new Vector3[ this->mapChannelVertCount[6] ];
		this->mapChannelFaceList[6] = new a3dFace[ this->faceCount ];
	}
	if( this->mapChannelVertCount[7] != 0) {
		this->mapChannelCoors[7] = new Vector3[ this->mapChannelVertCount[7] ];
		this->mapChannelFaceList[7] = new a3dFace[ this->faceCount ];
	}

	return ;
}

bool a3dMesh::importMapChannelsCoors( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 

	if( !this->importMapChannelCoors( lexList, li, a3dTextFileTokens::a3dMeshMap1VertexListString, this->mapChannelVertCount[0], this->mapChannelCoors[0] ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !this->importMapChannelCoors( lexList, li, a3dTextFileTokens::a3dMeshMap2VertexListString, this->mapChannelVertCount[1], this->mapChannelCoors[1] ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !this->importMapChannelCoors( lexList, li, a3dTextFileTokens::a3dMeshMap3VertexListString, this->mapChannelVertCount[2], this->mapChannelCoors[2] ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !this->importMapChannelCoors( lexList, li, a3dTextFileTokens::a3dMeshMap4VertexListString, this->mapChannelVertCount[3], this->mapChannelCoors[3] ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !this->importMapChannelCoors( lexList, li, a3dTextFileTokens::a3dMeshMap5VertexListString, this->mapChannelVertCount[4], this->mapChannelCoors[4] ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !this->importMapChannelCoors( lexList, li, a3dTextFileTokens::a3dMeshMap6VertexListString, this->mapChannelVertCount[5], this->mapChannelCoors[5] ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !this->importMapChannelCoors( lexList, li, a3dTextFileTokens::a3dMeshMap7VertexListString, this->mapChannelVertCount[6], this->mapChannelCoors[6] ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !this->importMapChannelCoors( lexList, li, a3dTextFileTokens::a3dMeshMap8VertexListString, this->mapChannelVertCount[7], this->mapChannelCoors[7] ) ) return false;

	return true;
}

bool a3dMesh::importMapChannelFaceList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &tokenName, int fCount, a3dFace *mapFaces ) {

	if( !(*li)->isId( tokenName ) ) return false;

	int fIndex;
	for( fIndex = 0; fIndex < fCount; fIndex ++ ) { 
		if( !safeNextLex( lexList, li ) ) return false;
		if( !(*li)->isNumericConst() ) return false;
		
		if( !safeNextLex( lexList, li ) ) return false;	
		if( !importFace( lexList, li, mapFaces[ fIndex ] ) ) return false;
	} 	

	if( !safeNextLex( lexList, li ) ) return false;
	if( !isTokenEnd( lexList, li, tokenName ) ) return false;

	return true;
}

bool a3dMesh::importMapChannelsFaceList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) {

	if( !this->importMapChannelFaceList( lexList, li, a3dTextFileTokens::a3dMeshMap1FaceListString, this->getMapFaceCount(0), this->mapChannelFaceList[0] ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !this->importMapChannelFaceList( lexList, li, a3dTextFileTokens::a3dMeshMap2FaceListString, this->getMapFaceCount(1), this->mapChannelFaceList[1] ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !this->importMapChannelFaceList( lexList, li, a3dTextFileTokens::a3dMeshMap3FaceListString, this->getMapFaceCount(2), this->mapChannelFaceList[2] ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !this->importMapChannelFaceList( lexList, li, a3dTextFileTokens::a3dMeshMap4FaceListString, this->getMapFaceCount(3), this->mapChannelFaceList[3] ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !this->importMapChannelFaceList( lexList, li, a3dTextFileTokens::a3dMeshMap5FaceListString, this->getMapFaceCount(4), this->mapChannelFaceList[4] ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !this->importMapChannelFaceList( lexList, li, a3dTextFileTokens::a3dMeshMap6FaceListString, this->getMapFaceCount(5), this->mapChannelFaceList[5] ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !this->importMapChannelFaceList( lexList, li, a3dTextFileTokens::a3dMeshMap7FaceListString, this->getMapFaceCount(6), this->mapChannelFaceList[6] ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !this->importMapChannelFaceList( lexList, li, a3dTextFileTokens::a3dMeshMap8FaceListString, this->getMapFaceCount(7), this->mapChannelFaceList[7] ) ) return false;
	
	return true;
} 

bool a3dMesh::importMapChannelCoors( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, shString &tokenName, int vCount, Vector3 *mapCoors ) { 

	// считываем vertexList
	if( !(*li)->isId( tokenName ) ) return false;

	int vIndex;
	for( vIndex = 0; vIndex < vCount; vIndex ++ ) { 
		if( !safeNextLex( lexList, li ) ) return false;
		if( !(*li)->isNumericConst() ) return false;
		
		if( !safeNextLex( lexList, li ) ) return false;	
		if( !importVertex( lexList, li, mapCoors[ vIndex ] ) ) return false;
	} 	

	if( !safeNextLex( lexList, li ) ) return false;
	if( !isTokenEnd( lexList, li, tokenName ) ) return false;
	
	return true;
}

bool a3dMesh::import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 
	
	if( !safeNextLex( lexList, li ) ) return false;
	
	// считываем имя
	if( (*li)->type != LT_STRING ) return false;
	this->name = (*li)->name;
	this->name.cutSpacesFromTheEnds();

	// считываем vertexCount
	if( !safeNextLex( lexList, li ) || !readIntParam( lexList, li, a3dTextFileTokens::a3dMeshVertexCountString, &this->vertexCount ) ) { 
		_tprintf_s( TEXT("error: can't read a3dMeshVertexCountString from mesh '%s'\n"), this->name.getCString() );
		return false;
	}
	if( !safeNextLex( lexList, li ) || !readIntParam( lexList, li, a3dTextFileTokens::a3dMeshFacesCountString, &this->faceCount ) ) { 
		_tprintf_s( TEXT("error: can't read a3dMeshFacesCountString from mesh '%s'\n"), this->name.getCString() );
		return false;
	}
	if( !safeNextLex( lexList, li ) || !readStringParam( lexList, li, a3dTextFileTokens::a3dMeshVertexFVFString, this->vertexFVF ) ) { 
		_tprintf_s( TEXT("error: can't read a3dMeshVertexFVFString from mesh '%s'\n"), this->name.getCString() );
		return false;	
	}
	if( !safeNextLex( lexList, li ) || !readIntParam( lexList, li, a3dTextFileTokens::a3dMeshMapChannelsCountString, &this->mapChannelCount ) ) { 
		_tprintf_s( TEXT("error: can't read a3dMeshMapChannelsCountString from mesh '%s'\n"), this->name.getCString() );
		return false;
	}
	if( !safeNextLex( lexList, li ) || !readMatrixParam( lexList, li, a3dTextFileTokens::a3dMeshTransformString, this->transform ) ) { 
		_tprintf_s( TEXT("error: can't read a3dMeshTransformString from mesh '%s'\n"), this->name.getCString() );
		return false;
	}
	if( !safeNextLex( lexList, li ) || !readVertexParam( lexList, li, a3dTextFileTokens::a3dMeshPivotString , this->pivot) ) { 
		_tprintf_s( TEXT("error: can't read a3dMeshPivotString from mesh '%s'\n"), this->name.getCString() );
		return false;
	}
	if( !safeNextLex( lexList, li ) || !readVertexParam( lexList, li, a3dTextFileTokens::a3dMeshPositionOffsetString , this->objectOffsetPos ) ) { 
		_tprintf_s( TEXT("error: can't read a3dMeshPositionOffsetString from mesh '%s'\n"), this->name.getCString() );
		return false;
	}
	if( !safeNextLex( lexList, li ) || !readQuaternionParam( lexList, li, a3dTextFileTokens::a3dMeshRotationOffsetString, this->objectOffsetRot ) ) { 
		_tprintf_s( TEXT("error: can't read a3dMeshRotationOffsetString from mesh '%s'\n"), this->name.getCString() );
		return false;
	}
	if( !safeNextLex( lexList, li ) || !readVertexParam( lexList, li, a3dTextFileTokens::a3dMeshScaleOffsetString , this->objectOffsetScale ) ) { 
		_tprintf_s( TEXT("error: can't read a3dMeshScaleOffsetString from mesh '%s'\n"), this->name.getCString() );
		return false;
	}
	if( !safeNextLex( lexList, li ) || !readMatrixParam( lexList, li, a3dTextFileTokens::a3dMeshOffsetTransformString, this->objectTransform ) ) { 
		_tprintf_s( TEXT("error: can't read a3dMeshOffsetTransformString from mesh '%s'\n"), this->name.getCString() );
		return false;
	}
	if( !safeNextLex( lexList, li ) || !readStringParam( lexList, li, a3dTextFileTokens::a3dMeshMaterialNameString, this->materialName ) ) { 
		_tprintf_s( TEXT("error: can't read a3dMeshMaterialNameString from mesh '%s'\n"), this->name.getCString() );
		return false;
	}
	this->materialName.cutSpacesFromTheEnds();

	if( !safeNextLex( lexList, li ) || !importMapsChannelCount( lexList, li ) ) { 
		_tprintf_s( TEXT("error: importMapsChannelCount from mesh '%s'\n"), this->name.getCString() );
		return false;
	}
	
	this->initializeData();

	if( !safeNextLex( lexList, li ) || !this->importVertices( lexList, li ) ) { 
		_tprintf_s( TEXT("error: importVertices from mesh '%s'\n"), this->name.getCString() );
		return false;
	}
	
	if( !safeNextLex( lexList, li ) || !this->importFaces( lexList, li ) ) { 
		_tprintf_s( TEXT("error: importFaces from mesh '%s'\n"), this->name.getCString() );
		return false;
	}
	
	if( !safeNextLex( lexList, li ) || !this->importFaceMatIdList( lexList, li ) ) { 
		_tprintf_s( TEXT("error: importFaceMatIdList from mesh '%s'\n"), this->name.getCString() );
		return false;
	}

	if( !safeNextLex( lexList, li ) || !this->importMapChannelsCoors( lexList, li ) ) { 
		_tprintf_s( TEXT("error: importMapChannelsCoors from mesh '%s'\n"), this->name.getCString() );
		return false;
	}

	if( !safeNextLex( lexList, li ) || !this->importMapChannelsFaceList( lexList, li ) ) { 
		_tprintf_s( TEXT("error: importMapChannelsFaceList from mesh '%s'\n"), this->name.getCString() );
		return false;
	}

	return true;
}

bool a3dTextModelFile ::initializeMeshes( std::list< scriptLexeme* > &lexList ) { 
	
	this->meshCount = this->getMeshCount( lexList );
	if( this->meshCount == 0 ) return true;
	
	this->meshList = new a3dMesh[ this->meshCount ];
	
	std::list< scriptLexeme* >::iterator li = lexList.begin();
	
	int meshIndex = 0;
	while( true ) { 
		this->findNextMesh( lexList, li );	
		if( li == lexList.end() ) break;		
		if( this->initializeMesh( lexList, li, this->meshList[ meshIndex ] ) == false ) return false;
		_tprintf_s( TEXT("mesh %s initialized.\n"), this->meshList[meshIndex].name.getCString() );
		meshIndex ++;
	}

	return true;
}

bool a3dTextModelFile ::initializeMaterials( std::list< scriptLexeme* > &lexList ) { 

	this->materialCount = this->getMaterialCount( lexList );
	if( this->materialCount == 0 ) return true;

	this->materialList = new a3dMaterialBase*[ this->materialCount ];
	std::list< scriptLexeme* >::iterator li = lexList.begin();

	int matIndex = 0;
	while( true ) { 
		this->findNextMaterial( lexList, li );	
		if( li == lexList.end() ) break;		
		if( !this->initializeMaterial( lexList, li, &this->materialList[ matIndex ] ) ) return false;

		matIndex ++;
	}

	return true;
}

bool a3dTextModelFile ::initializeLights( std::list< scriptLexeme* > &lexList ) { 

	this->lightCount = this->getLightCount( lexList );
	if( this->lightCount == 0 ) return true;

	this->lightList = new a3dLightBase*[ this->lightCount ];
	std::list< scriptLexeme* >::iterator li = lexList.begin();

	int ltIndex = 0;
	while( true ) { 
		this->findNextLight( lexList, li );	
		if( li == lexList.end() ) break;		
		if( !this->initializeLight( lexList, li, &this->lightList[ ltIndex ] ) ) return false;

		ltIndex ++;
	}

	return true;
}

bool a3dTextModelFile ::initializeMaterial( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, a3dMaterialBase **material ) { 

	if( !safeNextLex( lexList, li ) ) return false;
	// считываем тип
	if( isToken( li, a3dTextFileTokens::a3dMultiMaterialString ) ) (*material) = (a3dMaterialBase*)new a3dMultiMaterial();
	else if( isToken( li, a3dTextFileTokens::a3dMaterialStandardString ) ) (*material) = (a3dMaterialBase*)new a3dStandardMaterial();
	else if( isToken( li, a3dTextFileTokens::a3dDirectX_9_ShaderString ) ) (*material) = (a3dMaterialBase*)new a3dShaderMaterial();
	else return false;

	if( !safeNextLex( lexList, li ) ) return false;
	return (*material)->import( lexList, li );
}

bool a3dTextModelFile ::initializeLight( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, a3dLightBase **light ) { 

	if( !safeNextLex( lexList, li ) ) return false;
	// считываем тип
	if( isToken( li, a3dTextFileTokens::a3dOmnilightString ) ) (*light) = (a3dLightBase*)new a3dOmniLight();
	else if( isToken( li, a3dTextFileTokens::a3dDirectionallightString ) ) (*light) = (a3dLightBase*)new a3dDirectionalLight();
	else return false;

	if( !safeNextLex( lexList, li ) ) return false;
	return (*light)->import( lexList, li );
}



