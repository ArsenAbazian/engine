#include "a3dTextSceneFile.h"
#include "a3dScene.h"
#include "a3dTextFileTokens.h"
#include "scaner.h"
#include "a3dObjectInfo\\a3dObjectsInfo.h"
#include "a3dLight\\a3dLight.h"

void a3dTextSceneFile::initializeDefault() { 

	ZeroMemory( this->modelList, sizeof( this->modelList ) );
	ZeroMemory( this->landList, sizeof( this->landList ) );
	ZeroMemory( this->particleSystemList, sizeof( this->particleSystemList ) );
	ZeroMemory( this->railSystemList, sizeof( this->railSystemList ) );
	ZeroMemory( this->lightList, sizeof( this->lightList ) );

	this->modelCount = 0;
	this->landCount = 0;
	this->particleSystemCount = 0;
	this->railSystemCount = 0;
	this->lightCount = 0;

	return ;
}

void a3dTextSceneFile::clear() { 

	int i;

	if( this->modelList != null ) { 
		for( i = 0; i < this->modelCount; i++ ) {
			if( this->modelList[i] != null ) delete this->modelList[i];
		}
	}

	if( this->landList != null ) { 
		for( i = 0; i < this->landCount; i++ ) {
			if( this->landList[i] != null ) delete this->landList[i];
		}
	}

	if( this->particleSystemList != null ) { 
		for( i = 0; i < this->particleSystemCount; i++ ) {
			if( this->particleSystemList[i] != null ) delete this->particleSystemList[i];
		}
	}

	if( this->railSystemList != null ) { 
		for( i = 0; i < this->railSystemCount; i++ ) {
			if( this->railSystemList[i] != null ) delete this->railSystemList[i];
		}
	}

	if( this->lightList != null ) { 
		for( i = 0; i < this->lightCount; i++ ) {
			if( this->lightList[i] != null ) delete this->lightList[i];
		}
	}

	this->initializeDefault();

	return ;
}

bool a3dTextSceneFile::initializeDirectories( std::list<scriptLexeme*> &lexList ) { 

	std::list< scriptLexeme* >::iterator li = lexList.begin();

	findNextToken( lexList, li, a3dTextFileTokens::a3dSceneString );
	if( li == lexList.end() ) return false;
	
	if( !safeNextLex( lexList, li ) ) return false;

	if( !readStringParam( lexList, li, a3dTextFileTokens::a3dmodelDirectoryString, this->modelDirectory ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringParam( lexList, li, a3dTextFileTokens::a3dtextureDirectoryString, this->textureDirectory ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringParam( lexList, li, a3dTextFileTokens::a3deffectDirectoryString, this->effectDirectory ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringParam( lexList, li, a3dTextFileTokens::a3dlandscapeDirectoryString, this->landDirectory ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringParam( lexList, li, a3dTextFileTokens::a3dparticleDirectoryString, this->particleDirectory ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringParam( lexList, li, a3dTextFileTokens::a3drailDirectoryString, this->railSystemDirectory ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringParam( lexList, li, a3dTextFileTokens::a3dfontDirectoryString, this->fontDirectory ) ) return false;

	this->textureDirectory.cutSpacesFromTheEnds();
	this->modelDirectory.cutSpacesFromTheEnds();
	this->effectDirectory.cutSpacesFromTheEnds();
	this->landDirectory.cutSpacesFromTheEnds();
	this->particleDirectory.cutSpacesFromTheEnds();
	this->railSystemDirectory.cutSpacesFromTheEnds();
	this->fontDirectory.cutSpacesFromTheEnds();

	return true;
}

bool a3dTextSceneFile::loadFromFile( shString &fileName ) { 
	
	scriptScaner scaner;
	scriptRetCode	retCode;
	
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

	if( !this->initializeDirectories( scaner.getLexemesList() ) ) return false;
	
	if( !this->initializeModels( scaner.getLexemesList() ) ) return false;
	if( !this->initializeLands( scaner.getLexemesList() ) ) return false;
	if( !this->initializeParticleSystems( scaner.getLexemesList() ) ) return false;
	if( !this->initializeRailSystems( scaner.getLexemesList() ) ) return false;
	if( !this->initializeLights( scaner.getLexemesList() ) ) return false;

	return true;
} 

bool a3dTextSceneFile::initializeModel( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 

	a3dModelInfo *info = new a3dModelInfo();

	if( !info->readFromLexemeList( lexList, li ) ) return false;
	
	this->modelList[ this->modelCount ] = info;
	this->modelCount ++;

	return true;
}

bool a3dTextSceneFile::initializeLand( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 
	
	a3dLandscapeInfo *info = new a3dLandscapeInfo();
	
	if( !info->readFromLexemeList( lexList, li ) ) return false;
	
	this->landList[ this->landCount ] = info;
	this->landCount ++;
	
	return true;
}

a3dParticleSystemInfo* a3dTextSceneFile::createParticleInfo( shString &type ) { 

	if( type == a3dTextFileTokens::a3dSprayString ) return new a3dSprayParticleSystemInfo();
	return null;
}


bool a3dTextSceneFile::initializeParticleSystem( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 

	a3dParticleSystemInfo	*info;
	
	if( !safeNextLex( lexList, li ) ) return false;
	info = this->createParticleInfo( (*li)->name );
	li --;

	if( info == null ) return false;
	if( !info->readFromLexemeList( lexList, li ) ) return false;
	
	this->particleSystemList[ this->particleSystemCount ] = info;
	this->particleSystemCount ++;

	return true;
}

bool a3dTextSceneFile::initializeRailSystem(std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 
	
	a3dRailSystemInfo *info = new a3dRailSystemInfo();

	if( !info->readFromLexemeList( lexList, li ) ) return false;
	this->railSystemList[ this->railSystemCount ] = info;
	this->railSystemCount ++;
	
	return true;
}

bool a3dTextSceneFile::initializeModels( std::list< scriptLexeme* > &lexList ) { 

	std::list< scriptLexeme* >::iterator li = lexList.begin();

	while( true ) { 
		findNextToken( lexList, li, a3dTextFileTokens::a3dModelString );
		if( li == lexList.end() ) break;
		if( !this->initializeModel( lexList, li ) ) return false;		
		li++;
		if( isEnd( lexList, li ) ) break;
	}

	return true;
}

bool a3dTextSceneFile::initializeLands( std::list< scriptLexeme* > &lexList ) { 

	std::list< scriptLexeme* >::iterator li = lexList.begin();

	while( true ) { 
		findNextToken( lexList, li, a3dTextFileTokens::a3dLandString );
		if( li == lexList.end() ) break;
		if( !this->initializeLand( lexList, li ) ) return false;		
		li++;
		if( isEnd( lexList, li ) ) break;
	}

	return true;
}

bool a3dTextSceneFile::initializeParticleSystems( std::list< scriptLexeme* > &lexList ) { 

	std::list< scriptLexeme* >::iterator li = lexList.begin();

	while( true ) { 
		findNextToken( lexList, li, a3dTextFileTokens::a3dParticleSystemString );
		if( li == lexList.end() ) break;
		if( !this->initializeParticleSystem( lexList, li ) ) return false;		
		li++;
		if( isEnd( lexList, li ) ) break;
	}
	
	return true;
}

bool a3dTextSceneFile::initializeRailSystems( std::list< scriptLexeme* > &lexList ) { 

	std::list< scriptLexeme* >::iterator li = lexList.begin();

	while( true ) { 
		findNextToken( lexList, li, a3dTextFileTokens::a3da3dRailSystemString );
		if( li == lexList.end() ) break;
		if( !this->initializeRailSystem( lexList, li ) ) return false;		
		li++;
		if( isEnd( lexList, li ) ) break;
	}
	
	return true;
}

bool a3dTextSceneFile::initializeLight( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li, a3dLightBase *light ) { 

	if( !light->import( lexList, li ) ) return false;
		
	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, a3dTextFileTokens::a3dEndString ) ) return false;
	if( !safeNextLex( lexList, li ) ) return false;
	
	this->lightList[ this->lightCount ] = light;
	this->lightCount ++;
	
	return true;
}

bool a3dTextSceneFile::initializeLights( std::list< scriptLexeme* > &lexList ) { 
	
	std::list< scriptLexeme* >::iterator li = lexList.begin();

	while( true ) { 
		findNextToken( lexList, li, a3dTextFileTokens::a3dOmniLightString );
		if( li == lexList.end() ) break;
		
		if( !this->initializeLight( lexList, li, new a3dOmniLight() ) ) return false;
		li++;
		if( isEnd( lexList, li ) ) break;
	}

	li = lexList.begin();
	while( true ) { 
		findNextToken( lexList, li, a3dTextFileTokens::a3dDirectionalLightString );
		if( li == lexList.end() ) break;
		
		if( !this->initializeLight( lexList, li, new a3dDirectionalLight() ) ) return false;
		li++;
		if( isEnd( lexList, li ) ) break;
	}

	return true;
}

