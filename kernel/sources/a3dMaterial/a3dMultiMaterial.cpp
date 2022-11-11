#include "a3dMultiMaterial.h"
#include "memoryFile.h"
#include "..\\a3dTextFileTokens.h"

void a3dMultiMaterial::clear() { 

	if( this->subMaterialNameList != null ) delete[] this->subMaterialNameList;
	if( this->subMaterialEnablesList != null ) delete[] this->subMaterialEnablesList;
	if( this->subMaterialIdList != null ) delete[] this->subMaterialIdList;

	this->subMaterialNameList = null;
	this->subMaterialEnablesList = null;
	this->subMaterialIdList = null;
	this->subMaterialCount = 0;

	a3dMaterialBase::clear();
}

a3dMaterialBase* a3dMultiMaterial::clone() { 

	a3dMultiMaterial *mat = new a3dMultiMaterial();

	mat->name = this->name;
	mat->subMaterialCount = this->subMaterialCount;

	mat->subMaterialEnablesList = new bool[ this->subMaterialCount ];
	mat->subMaterialIdList = new int[ this->subMaterialCount ];
	mat->subMaterialNameList = new shString[ this->subMaterialCount ];

	int i;
	for( i = 0; i < this->subMaterialCount; i ++ ) {
		mat->subMaterialEnablesList[ i ] = this->subMaterialEnablesList[ i ];
		mat->subMaterialIdList[ i ] = this->subMaterialIdList[ i ];
		mat->subMaterialNameList[ i ] = this->subMaterialNameList[ i ];
	}

	return mat;
}

bool a3dMultiMaterial::import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 

	if( !readStringParam( lexList, li, a3dTextFileTokens::a3dNameString, this->name ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !readIntParam( lexList, li, a3dTextFileTokens::a3dMMSubMtlCountString, &this->subMaterialCount ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	this->subMaterialNameList = new shString[ this->subMaterialCount ];
	this->subMaterialEnablesList = new bool[ this->subMaterialCount ];
	this->subMaterialIdList = new int[ this->subMaterialCount ];

	if( !this->importSubMaterialNameList( lexList, li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !this->importSubMaterialIdList( lexList, li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !this->importSubMaterialEnablesList( lexList, li ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;

	if( !isToken( li, a3dTextFileTokens::a3dEndString ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, a3dTextFileTokens::a3dMultiMaterialString ) ) return false;	

	return true;
}

bool a3dMultiMaterial::importSubMaterialNameList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 

	if( !isToken( li, a3dTextFileTokens::a3dMMNameListString ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !importStringList( lexList, li, this->subMaterialCount, this->subMaterialNameList ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, a3dTextFileTokens::a3dEndString ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, a3dTextFileTokens::a3dMMNameListString ) ) return false;

	return true;
}

bool a3dMultiMaterial::importSubMaterialEnablesList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 

	if( !isToken( li, a3dTextFileTokens::a3dMMEnabledListString ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !importBooleanList( lexList, li, this->subMaterialCount, this->subMaterialEnablesList ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, a3dTextFileTokens::a3dEndString ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, a3dTextFileTokens::a3dMMEnabledListString ) ) return false;

	return true;
}

bool a3dMultiMaterial::importSubMaterialIdList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 

	if( !isToken( li, a3dTextFileTokens::a3dMMIdListString ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !importIntList( lexList, li, this->subMaterialCount, this->subMaterialIdList ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, a3dTextFileTokens::a3dEndString ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, a3dTextFileTokens::a3dMMIdListString ) ) return false;

	return true;
}

