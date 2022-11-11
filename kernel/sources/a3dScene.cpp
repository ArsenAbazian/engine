#include "a3dScene.h"
#include <dxerr9.h>

#include "a3dTextSceneFile.h"
#include "a3dTextModelFile.h"
#include "memoryFile.h"
#include "hlslman.h"
#include "landscape.h"
#include "railManager.h"
#include "a3dDesc\\a3dDesc.h"
#include "a3dLight\\a3dLight.h"
#include "a3dModel\\a3dDetail.h"
#include "a3dModelRef\\a3dModelRef.h"
#include "a3dModelRef\\a3dObjectRef.h"
#include "a3dModelRef\\a3dDetailRef.h"
#include "a3dObjectInfo\\a3dObjectsInfo.h"
#include "a3dTextFileTokens.h"
#include "renderHelper.h"
#include "a3dMaterial\\a3dShaderMaterial.h"
#include "a3dGraph\\a3dGraph.h"
#include "a3dParticleSystem\\particleSystemBase.h"
#include "a3dUtils\\a3dTextUtils.h"
 
#ifndef intsize
#define intsize sizeof(int)
#endif

unsigned short checkWord1;
unsigned short checkWord3;
unsigned short checkWord8;
unsigned short checkWord9;
unsigned short checkWord11;
unsigned short checkWord12;

unsigned short checkWord1_1;
unsigned short checkWord3_1;
unsigned short checkWord8_1;
unsigned short checkWord9_1;
unsigned short checkWord11_1;
unsigned short checkWord12_1;

void a3dScene::initializeDefault() { 
	 
	ZeroMemory( this->models, sizeof( this->models ) );
	ZeroMemory( this->lands, sizeof( this->lands ) );
	ZeroMemory( this->particles, sizeof( this->particles ) );
	ZeroMemory( this->rails, sizeof( this->rails ) );
	ZeroMemory( this->effects, sizeof( this->effects ) );
	ZeroMemory( this->textures, sizeof( this->textures ) );
	ZeroMemory( this->fonts, sizeof( this->fonts ) );
	ZeroMemory( this->sounds, sizeof( this->sounds ) );
	ZeroMemory( this->lights, sizeof( this->lights ) );
	ZeroMemory( this->graphs, sizeof( this->graphs ) );

	this->modelDescCount = 0;
	this->landDescCount = 0;
	this->particleDescCount = 0;
	this->railDescCount = 0;
	this->effectDescCount = 0;
	this->textureDescCount = 0;
	this->fontDescCount = 0;
	this->soundDescCount = 0;
	this->lightsCount = 0;
	this->graphCount = 0;

	ZeroMemory( this->modelRef, sizeof( this->modelRef ) );
	this->modelRefCount = 0;
	ZeroMemory( this->modelInfo, sizeof( this->modelInfo ) );
	this->modelInfoCount = 0;

	this->kernel = null;
	this->designMode = false;
	
	this->omniLightRender = null;
	this->dirLightRender = null;
	this->sprayParticleRender = null;
	
	this->defaultVersion = a3dVersion(1, 1);
	this->version = this->defaultVersion;

	this->timeProvider.registerDependencyProperties();

	return ;
}

void a3dScene::clear() { 

	int i;
	
	for( i = 0; i < this->modelDescCount; i++ ) { 
		if( this->models[ i ] != null )
			delete this->models[ i ];
	}
	
	for( i = 0; i < this->landDescCount; i++ ) {
		if( this->lands[ i ] != null )		
			delete this->lands[ i ];
	}

	for( i = 0; i < this->particleDescCount; i++ ) {
		if( this->particles[ i ] != null )		
			delete this->particles[ i ];
	}

	for( i = 0; i < this->railDescCount; i++ ) {
		if( this->rails[ i ] != null )
			delete this->rails[ i ];
	}

	for( i = 0; i < this->effectDescCount; i++ ) {
		if( this->effects[ i ] != null )
			delete this->effects[ i ];
	}

	for( i = 0; i < this->textureDescCount; i++ ) { 
		if( this->textures[ i ] != null )
			delete this->textures[ i ];
	}

	for( i = 0; i < this->fontDescCount; i++ ) { 
		if( this->fonts[ i ] != null )
			delete this->fonts[ i ];
	}

	for( i = 0; i < this->soundDescCount; i++ ) {
		if( this->sounds[ i ] != null )
			delete this->sounds[ i ];
	}

	for( i = 0; i < this->lightsCount; i++ ) {
		if( this->lights[ i ] != null )
			delete this->lights[ i ];
	}

	for( i = 0; i < this->graphCount; i++ ) { 
		if( this->graphs[ i ] != null )
			delete this->graphs[ i ];
	}

	for( i = 0; i < this->modelRefCount; i++ )
		delete this->modelRef[ i ];

	for( i = 0; i < this->modelInfoCount; i++ )
		delete this->modelInfo[ i ];

	if( this->omniLightRender != null )
		delete this->omniLightRender;
	if( this->dirLightRender != null )
		delete this->dirLightRender;
	if( this->sprayParticleRender != null )
		delete this->sprayParticleRender;
	
	Kernel *krn = this->kernel;
	
	this->initializeDefault();
	this->kernel = krn;

	return;
}

a3dModelDesc* a3dScene::getModelDesc( shString &modelFileName ) { 

	if( modelFileName.getLength() > 1 && modelFileName.getCString()[1] == ':' ) { 
		for( int i = 0; i < this->modelDescCount; i++ ) { 
			if( modelFileName == this->models[i]->fullPathName ) 
				return this->models[i];
		}

		return null;
	}
	
	for( int i = 0; i < this->modelDescCount; i++ ) { 
		if( modelFileName == this->models[i]->fileName ) 
			return this->models[i];
	}

	return null;
}

a3dModelDesc* a3dScene::getModel( shString &fileName ) { 

	a3dModelDesc *desc = this->getModelDesc( fileName );
	if( desc != null ) 
		return desc;

	HRESULT rv;
	
	rv = this->addModelToScene( fileName );
	if( FAILED( rv ) )
		return null;
	
	return this->getModelDesc( fileName );
}

bool a3dScene::addModelDesc( a3dModelInfo *info ) { 

	if( this->modelDescCount == SCENE_MODEL_MAX_COUNT ) return false;
	
	a3dModelDesc *desc = new a3dModelDesc();
	
	desc->name = shString( TEXT( "*" ) );
	desc->fileName = info->fileName;
	this->models[ this->modelDescCount ] = desc;
	this->modelDescCount ++;
	
	return true;
} 

bool a3dScene::importModel( shString &textModelFileName, shString &fileName ) { 

	a3dTextModelFile file;
	
	if(!file.loadFromFile( textModelFileName )) { 
		return false;
	}

	a3dModel model;
	if(!model.import( file )) { 
		return false;
	}

	return model.saveToFile(fileName);
}

bool a3dScene::importModels( a3dTextSceneFile &file ) { 

	a3dModelDesc *model;
	this->modelInfoCount = file.modelCount;
	
	// копируем без модели 
	for( int i = 0; i < this->modelInfoCount; i++ ) { 
		this->modelInfo[i] = file.modelList[i]->clone();
		
		model = this->getModelDesc( this->modelInfo[i]->fileName );
		if( model == null ) { 
			if( !this->addModelDesc( this->modelInfo[i] ) ) 
				return false;
		}
	}

	return true;
}

bool a3dScene::importLands( a3dTextSceneFile &file ) { 

	this->landDescCount = file.landCount;

	for( int i = 0; i < this->landDescCount; i++ ) { 
		this->lands[i] = new a3dLandscapeDesc();
		this->lands[i]->name = file.landList[i]->name;
		this->lands[i]->landInfo = file.landList[i]->clone();
	}

	return true;
}

bool a3dScene::importParticles( a3dTextSceneFile &file ) { 

	this->particleDescCount = file.particleSystemCount;
	
	for( int i = 0; i < this->particleDescCount; i++ ) { 
		
		this->particles[i] = new a3dParticleSystemDesc();
		this->particles[i]->name = file.particleSystemList[i]->name;
		//this->particles[i]->psInfo = file.particleSystemList[i]->clone();
	}

	return true;
}

bool a3dScene::importRails( a3dTextSceneFile &file ) { 

	this->railDescCount = file.railSystemCount;

	for( int i = 0; i < this->railDescCount; i++ ) {
		this->rails[i] = new a3dRailSystemDesc();
		this->rails[i]->name = file.railSystemList[i]->name;
		this->rails[i]->rsInfo = file.railSystemList[i]->clone();
	}

	return true;
}

bool a3dScene::importLights( a3dTextSceneFile &file ) { 
	
	this->lightsCount = file.lightCount;
	
	for( int i = 0; i < this->lightsCount; i++ ) {
		this->lights[i] = file.lightList[i]->clone();	
	}

	return true;
}

bool a3dScene::import( a3dTextSceneFile &file ) { 

	this->textureDirectory = file.textureDirectory; 
	this->modelDirectory = file.modelDirectory;
	this->effectDirectory = file.effectDirectory;
	this->landDirectory = file.landDirectory;
	this->particleDirectory = file.particleDirectory;
	this->railSystemDirectory = file.railSystemDirectory;
	this->fontDirectory = file.fontDirectory;
		
	if( !this->importModels( file ) ) return false;
	if( !this->importLands( file ) ) return false;
	if( !this->importParticles( file ) ) return false;
	if( !this->importRails( file ) ) return false;
	if( !this->importLights( file ) ) return false;

	return true;
}

int a3dScene::calcSizeCore() { 
	
	int totalSize = 0;
	int i;
	
	totalSize += this->textureDirShort.calcSize();
	totalSize += this->modelDirShort.calcSize();
	totalSize += this->effectDirShort.calcSize();
	totalSize += this->landDirShort.calcSize();
	totalSize += this->particleDirShort.calcSize();
	totalSize += this->railSystemDirShort.calcSize();
	totalSize += this->fontDirShort.calcSize();

	totalSize += intsize;
	for( i = 0; i < this->modelDescCount; i++ ) { 
		totalSize += this->models[i]->calcSize();
	}
	
	totalSize += intsize;
	for( i = 0; i < this->landDescCount; i++ ) { 
		totalSize += this->lands[i]->calcSize();
	}

	totalSize += intsize;
	for( i = 0; i < this->particleDescCount; i++ ) { 
		totalSize += this->particles[i]->calcSize();
	}

	totalSize += intsize;
	for( i = 0; i < this->railDescCount; i++ ) { 
		totalSize += this->rails[i]->calcSize();
	}

	totalSize += intsize;
	for( i = 0; i < this->effectDescCount; i++ ) { 
		totalSize += this->effects[i]->calcSize();
	}	

	totalSize += intsize;
	for( i = 0; i < this->textureDescCount; i++ ) { 
		totalSize += this->textures[i]->calcSize();
	}

	totalSize += intsize;
	for( i = 0; i < this->fontDescCount; i++ ) { 
		totalSize += this->fonts[i]->calcSize();
	}

	totalSize += intsize;
	for( i = 0; i < this->soundDescCount; i++ ) { 
		totalSize += this->sounds[i]->calcSize();
	}

	totalSize += intsize;
	for( i = 0; i < this->lightsCount; i++ ) {
		totalSize += this->lights[i]->calcSize();
	}

	if( this->version >= a3dVersion(1, 1) ) {
		totalSize += intsize;
		for( i = 0; i < this->graphCount; i++ ) { 
			totalSize += this->graphs[i]->calcSize();
		}
	}

	totalSize += intsize;
	// считаем объекты в сцене
	for( i = 0; i < this->modelInfoCount; i++ ) { 
		totalSize += this->modelInfo[i]->calcSize();
	}
	
	return totalSize;
}

bool a3dScene::loadFromMemoryCore(a3dMemoryFile &file) { 

	int i;
	
	if( !this->textureDirShort.loadFromMemory( file ) ) return false;
	if( !this->modelDirShort.loadFromMemory( file ) ) return false;
	if( !this->effectDirShort.loadFromMemory( file ) ) return false;
	if( !this->landDirShort.loadFromMemory( file ) ) return false;
	if( !this->particleDirShort.loadFromMemory( file ) ) return false;
	if( !this->railSystemDirShort.loadFromMemory( file ) ) return false;
	if( !this->fontDirShort.loadFromMemory( file ) ) return false;

	this->textureDirectory = this->textureDirShort;
	this->modelDirectory = this->modelDirShort;
	this->effectDirectory = this->effectDirShort;
	this->landDirectory = this->landDirShort;
	this->particleDirectory = this->particleDirShort;
	this->railSystemDirectory = this->railSystemDirShort;
	this->fontDirectory = this->fontDirShort;

	if( !file.readInt( &this->modelDescCount ) ) return false;
	for( i = 0; i < this->modelDescCount; i++ ) { 
		this->models[i] = new a3dModelDesc();
		if( !this->models[i]->loadFromMemory( file ) ) return false;
	}

	if( !file.readInt( &this->landDescCount ) ) return false;
	for( i = 0; i < this->landDescCount; i++ ) { 
		this->lands[i] = new a3dLandscapeDesc();
		this->lands[i]->landInfo = new a3dLandscapeInfo();
		if( !this->lands[i]->loadFromMemory( file ) ) return false;
	}

	if( !file.readInt( &this->particleDescCount ) ) return false;
	for( i = 0; i < this->particleDescCount; i++ ) { 
		this->particles[i] = new a3dParticleSystemDesc();
		//this->particles[i]->psInfo = new a3dParticleSystemInfo();
		if( !this->particles[i]->loadFromMemory( file ) ) return false;
	}

	if( !file.readInt( &this->railDescCount ) ) return false;
	for( i = 0; i < this->railDescCount; i++ ) { 
		this->rails[i] = new a3dRailSystemDesc();
		this->rails[i]->rsInfo = new a3dRailSystemInfo();
		if( !this->rails[i]->loadFromMemory( file ) ) return false;
	}

	if( !file.readInt( &this->effectDescCount ) ) return false;
	for( i = 0; i < this->effectDescCount; i++ ) { 
		this->effects[i] = new a3dEffectDesc();
		if( !this->effects[i]->loadFromMemory( file ) ) return false;
	}
	
	if( !file.readInt( &this->textureDescCount ) ) return false;
	for( i = 0; i < this->textureDescCount; i++ ) { 
		this->textures[i] = new a3dTextureDesc();
		if( !this->textures[i]->loadFromMemory( file ) ) return false;
	}

	if( !file.readInt( &this->fontDescCount ) ) return false;
	for( i = 0; i < this->fontDescCount; i++ ) { 
		this->fonts[i] = new a3dFontDesc();
		if( !this->fonts[i]->loadFromMemory( file ) ) return false;
	}

	if( !file.readInt( &this->soundDescCount ) ) return false;
	for( i = 0; i < this->soundDescCount; i++ ) { 
		this->sounds[i] = new a3dSoundDesc();
		if( !this->sounds[i]->loadFromMemory( file ) ) return false;
	}

	if( !file.readInt( &this->lightsCount ) ) return false;
	for( i = 0; i < this->lightsCount; i++ ) { 
		this->lights[i] = this->createLight( file );
		if( this->lights[i] == null ) return false;
		if( !this->lights[i]->loadFromMemory( file ) ) return false;
	}

	if( !file.readInt( &this->modelInfoCount ) ) return false;
	for( i = 0; i < this->modelInfoCount; i++ ) { 
		this->modelInfo[i] = new a3dModelInfo();
		if( !this->modelInfo[i]->loadFromMemory( file ) ) return false; 
	}

	if( this->version >= a3dVersion(1, 1) ) { 
		if(!file.readInt( &this->graphCount ) ) return false;
		for( i = 0; i < this->graphCount; i++ ) {
			this->graphs[i] = new a3dGraph();
			if( !this->graphs[i]->loadFromMemory( file ) ) return false;
		}
	}
	
	return true;
}

a3dLightBase* a3dScene::createLight( a3dMemoryFile &file ) { 

	int		type;
	
	file.savePos();
	if( !file.readInt( &type ) ) return null;
	file.restorePos();
	
	switch( type ) { 
		case a3dHeaders::a3dOmniLightHeader: return new a3dOmniLight();
		case a3dHeaders::a3dDirectionalLightHeader: return new a3dDirectionalLight();
	}
	
	return null;
}

bool a3dScene::saveToMemoryCore(a3dMemoryFile &file) { 
	
	int i;
	
	if( !this->textureDirShort.saveToMemory( file ) ) return false;
	if( !this->modelDirShort.saveToMemory( file ) ) return false;
	if( !this->effectDirShort.saveToMemory( file ) ) return false;
	if( !this->landDirShort.saveToMemory( file ) ) return false;
	if( !this->particleDirShort.saveToMemory( file ) ) return false;
	if( !this->railSystemDirShort.saveToMemory( file ) ) return false;
	if( !this->fontDirShort.saveToMemory( file ) ) return false;
	
	if( !file.writeInt( this->modelDescCount ) ) return false;
	for( i = 0; i < this->modelDescCount; i++ ) { 
		file.savePos();
		if( !this->models[i]->saveToMemory( file ) ) return false;
		if( file.delta() != this->models[i]->calcSize() ) return false;
	}
	
	if( !file.writeInt( this->landDescCount ) ) return false;
	for( i = 0; i < this->landDescCount; i++ ) { 
		file.savePos();
		if( !this->lands[i]->saveToMemory( file ) ) return false;
 		if( file.delta() != this->lands[i]->calcSize() ) return false;
	}
	
	if( !file.writeInt( this->particleDescCount ) ) return false;
	for( i = 0; i < this->particleDescCount; i++ ) { 
		file.savePos();
		if( !this->particles[i]->saveToMemory( file ) ) return false;
		if( file.delta() != this->particles[i]->calcSize() ) return false;
	}

	if( !file.writeInt( this->railDescCount ) ) return false;
	for( i = 0; i < this->railDescCount; i++ ) { 
		file.savePos();
		if( !this->rails[i]->saveToMemory( file ) ) return false;
		if( file.delta() != this->rails[i]->calcSize() ) return false;
	}

	if( !file.writeInt( this->effectDescCount ) ) return false;
	for( i = 0; i < this->effectDescCount; i++ ) { 
		file.savePos();
		if( !this->effects[i]->saveToMemory( file ) ) return false;
		if( file.delta() != this->effects[i]->calcSize() ) return false;
	}	

	if( !file.writeInt( this->textureDescCount ) ) return false;
	for( i = 0; i < this->textureDescCount; i++ ) { 
		file.savePos();
		if( !this->textures[i]->saveToMemory( file ) ) return false;
		if( file.delta() != this->textures[i]->calcSize() ) return false;
	}

	if( !file.writeInt( this->fontDescCount ) ) return false;
	for( i = 0; i < this->fontDescCount; i++ ) { 
		file.savePos();
		if( !this->fonts[i]->saveToMemory( file ) ) return false;
		if( file.delta() != this->fonts[i]->calcSize() ) return false;
	}

	if( !file.writeInt( this->soundDescCount ) ) return false;
	for( i = 0; i < this->soundDescCount; i++ ) { 
		file.savePos();
		if( !this->sounds[i]->saveToMemory( file ) ) return false; 
		if( file.delta() != this->sounds[i]->calcSize() ) return false;
	}

	if( !file.writeInt( this->lightsCount ) ) return false;
	for( i = 0; i < this->lightsCount; i++ ) {
		file.savePos();
		if( !this->lights[i]->saveToMemory( file ) ) return false;
		if( file.delta() != this->lights[i]->calcSize() ) return false;
	}

	if( !file.writeInt( this->modelInfoCount ) ) return false;
	for( i = 0; i < this->modelInfoCount; i++ ) { 
		file.savePos();
		if( !this->modelInfo[i]->saveToMemory( file ) ) return false;
		if( file.delta() != this->modelInfo[i]->calcSize() ) return false;
	}

	if( !file.writeInt( this->graphCount ) ) return false;
	for( i = 0; i < this->graphCount; i++ ) { 
		file.savePos();
		if( !this->graphs[i]->saveToMemory( file ) ) return false;
		if( file.delta() != this->graphs[i]->calcSize() ) return false;
	}
	
	return true;
}

void a3dScene::clearModelsInfo() { 

	for( int i = 0; i < this->modelInfoCount; i++ ) {
		delete this->modelInfo[i];
	}
	this->modelInfoCount = 0;
}

void a3dScene::updateModelsInfo() { 
	
	a3dModelInfo *info;

	this->clearModelsInfo();
	for( int i = 0; i < this->modelRefCount; i++ ) { 
		info = new a3dModelInfo();
		info->initialize(this->modelRef[i]->desc);
		info->assign( this->modelRef[i] );

		this->modelInfo[i] = info;
		this->modelInfoCount++;
	}
}

bool a3dScene::loadFromFile(shString &fileName) { 

	a3dMemoryFile file;
	
	this->fullPathName = fileName;
	this->fileName = this->fullPathName;
	this->fileName.extractFileName();
	
	a3dOutput::Default->outInfo( TEXT( "loading scene " ) );
	a3dOutput::Default->outInfo( this->fileName.getCString() );
	a3dOutput::Default->outInfo( TEXT( " ...\n" ) );

	if( !file.loadFromFile( fileName ) ) { 
		a3dOutput::Default->outInfo( TEXT( "error: can't load file.\n" ) );
		return false;
	}
	if( !this->loadFromMemory( file ) ) { 
		a3dOutput::Default->outInfo( TEXT( "error: while read scene from memory.\n" ) );
		return false;	
	}
	if( file.emptySpace() != 0 ) { 
		a3dOutput::Default->outInfo( TEXT( "error: invalid checksum after read scene from memory.\n" ) );
		return false;
	}

	// восстанавливаем версию
	this->version = this->defaultVersion;
	a3dOutput::Default->outInfo( this->fileName.getCString() );
	a3dOutput::Default->outInfo( TEXT( "\nscene sucessuflly loaded\n" ) );

	return true;
}

bool a3dScene::saveToFile(shString &fileName) { 

	a3dMemoryFile file;
	
	this->updateModelsInfo();
	file.initialize( this->calcSize() );

	if( !this->saveToMemory( file ) ) return false;
	if( !file.saveToFile( fileName ) ) return false;
	if( file.emptySpace() != 0 ) return false;

	return true;
}

HRESULT a3dScene::loadTextures( ) { 
	
	HRESULT		rv;
	int			i;

	if( this->kernel == null ) return D3DERR_INVALIDCALL;
	for( i = 0; i < this->textureDescCount; i++ ) { 
		rv = this->textures[i]->loadTexture( this );
		if( FAILED( rv ) )
			return rv;
	}

	return D3D_OK;
}

HRESULT a3dScene::loadEffects( ) { 
	
	HRESULT		rv;
	int			i;

	if( this->kernel == null ) return D3DERR_INVALIDCALL;
	for( i = 0; i < this->effectDescCount; i++ ) { 
		rv = this->effects[i]->loadEffect( this );
		if( FAILED( rv ) )
			return rv;
	}

	return D3D_OK;
}

HRESULT a3dScene::loadModels() { 

	HRESULT		rv;
	int			i;

	if( this->kernel == null ) return D3DERR_INVALIDCALL;
	for( i = 0; i < this->modelDescCount; i++ ) { 
		rv = this->models[i]->loadModel( this );
		if( FAILED( rv ) )
			return rv;
	}
	
	return D3D_OK;
}

HRESULT a3dScene::addModelToScene( shString &fileName ) { 
	
	HRESULT rv;
	
	this->models[ this->modelDescCount ] = new a3dModelDesc();
	this->models[ this->modelDescCount ]->kernel = this->kernel;
	this->models[ this->modelDescCount ]->fileName = fileName;
	this->models[ this->modelDescCount ]->fileName.removePath( this->models[ this->modelDescCount ]->fileName, this->dataDirectory, this->modelDirShort );	

	rv = this->models[ this->modelDescCount ]->loadModel( this );
	this->modelDescCount ++;

	return rv;
}

HRESULT a3dScene::loadLands() { 
	
	HRESULT		rv;
	int			i;

	if( this->kernel == null ) return D3DERR_INVALIDCALL;
	for( i = 0; i < this->landDescCount; i++ ) { 
		rv = this->lands[i]->loadLand( this );
		if( FAILED( rv ) )
			return rv;
	}
	
	return D3D_OK;
}

HRESULT a3dScene::loadParticles() { 

	HRESULT		rv;
	int			i;

	if( this->kernel == null ) return D3DERR_INVALIDCALL;
	for( i = 0; i < this->particleDescCount; i++ ) { 
		rv = this->particles[i]->loadParticle( this );
		if( FAILED( rv ) )
			return rv;
	}
	
	return D3D_OK;
}

HRESULT a3dScene::loadRails() { 
	
	HRESULT		rv;
	int			i;

	if( this->kernel == null ) return D3DERR_INVALIDCALL;
	for( i = 0; i < this->railDescCount; i++ ) { 
		rv = this->rails[i]->loadRail( this );
		if( FAILED( rv ) )
			return rv;
	}
	
	return D3D_OK;
}

HRESULT a3dScene::loadData( shString &dataDirectory ) { 
	
	HRESULT		rv;

	this->dataDirectory = dataDirectory;
	this->textureDirectory.appendPath( this->dataDirectory ); 
	this->modelDirectory.appendPath( this->dataDirectory );
	this->effectDirectory.appendPath( this->dataDirectory );
	this->landDirectory.appendPath( this->dataDirectory );
	this->particleDirectory.appendPath( this->dataDirectory );
	this->railSystemDirectory.appendPath( this->dataDirectory );
	this->fontDirectory.appendPath( this->dataDirectory );

	if( this->kernel == null ) return D3DERR_INVALIDCALL;
	
	rv = this->loadTextures();
	if( FAILED( rv ) )
		return rv;
	
	rv = this->loadEffects();
	if( FAILED( rv ) )
		return rv;
	
	rv = this->loadModels();
	if( FAILED( rv ) )
		return rv;

	rv = this->loadLands();
	if( FAILED( rv ) )
		return rv;

	rv = this->loadParticles();
	if( FAILED( rv ) )
		return rv;

	rv = this->loadRails();
	if( FAILED( rv ) )
		return rv;

	rv = this->initializeModelInfos();
	if( FAILED( rv ) )
		return rv;
	
	rv = this->createModelRef();
	if( FAILED( rv ) )
		return rv;

	if( this->designMode && this->omniLightRender == null ) { 
		rv = this->initializeDesignModeHelpers();
		if( FAILED( rv ) )
			return rv;
	}

	return D3D_OK;
}

HRESULT a3dScene::initializeModelInfos() { 
	
	a3dModelDesc *desc;
	
	int mi;
	for( mi = 0; mi < this->modelInfoCount; mi++ ) { 
		desc = this->getModelDesc( this->modelInfo[mi]->fileName );
		if( desc == null ) 
			return D3DERR_INVALIDCALL;
		
		this->modelInfo[mi]->initialize( desc );
		if( this->modelInfo[mi]->desc->model == null )
			return D3DERR_INVALIDCALL;
	}
	
	return D3D_OK;
}

HRESULT a3dScene::createModelRef() { 
	
	if( this->modelInfoCount == 0 ) 
		return D3D_OK;
	
	this->modelRefCount = this->modelInfoCount;
	
	int mi;
	for( mi = 0; mi < this->modelInfoCount; mi++ ) { 
		this->modelRef[mi] = this->modelInfo[mi]->createReference();
	}
	
	return D3D_OK;
}

void a3dScene::updateTexturePath( shString &fileName ) { 
	
	if( fileName.getCString()[1] == ':' )
		fileName.replacePath( fileName, this->textureDirectory, this->textureDirShort );
	else 
		fileName.appendPath( this->textureDirectory );

	return ;
}

void a3dScene::makeLongPathNames() { 
	
	this->textureDirectory = this->textureDirShort;
	this->effectDirectory = this->effectDirShort;
	this->modelDirectory = this->modelDirShort;
	this->landDirectory = this->landDirShort;
	this->particleDirectory = this->particleDirShort;
	this->railSystemDirectory = this->railSystemDirShort;
	this->fontDirectory = this->fontDirShort;
	this->soundDirectory = this->soundDirShort;
	
	this->textureDirectory.appendPath( this->dataDirectory );
	this->effectDirectory.appendPath( this->dataDirectory );
	this->modelDirectory.appendPath( this->dataDirectory );
	this->landDirectory.appendPath( this->dataDirectory );
	this->particleDirectory.appendPath( this->dataDirectory );
	this->railSystemDirectory.appendPath( this->dataDirectory );
	this->fontDirectory.appendPath( this->dataDirectory );
	this->soundDirectory.appendPath( this->dataDirectory );
}

a3dTextureDesc* a3dScene::getTexture(shString &fileName) { 

	if(fileName.getLength() < 1) return null;

	shString textureFile = fileName;
	if( textureFile.getCString() == null ) return null;  
	if( textureFile.getCString()[1] == ':' )
		textureFile.replacePath( textureFile, this->textureDirectory, this->textureDirShort );
	else 
		textureFile.appendPath( this->textureDirectory );
	
	int i;
	for( i = 0; i < this->textureDescCount; i++ ) { 
		if( textureFile == this->textures[i]->fullPathName ) return this->textures[i];
	}
	
	HRESULT rv = this->loadTexture( textureFile );
	if( FAILED( rv ) ) 
		return null;
	
	return this->textures[ this->textureDescCount - 1 ];
} 

HRESULT a3dScene::loadTexture( shString &fileName ) { 
	
	HRESULT rv;
	
	Texture *texture = new Texture();
	texture->SetKernel( this->kernel );
	rv = texture->loadFromFile( fileName, true );
	if( FAILED( rv ) ) { 
		this->kernel->ErrorBox( TEXT( "a3dScene" ), TEXT( "Error: can't load texture from file '%s'. returnValue = %s" ), fileName.getCString(), DXGetErrorString9( rv ) ); 
		return rv;
	}
	
	a3dTextureDesc *desc = new a3dTextureDesc();
	desc->kernel = this->kernel;
	desc->fullPathName = fileName;
	desc->fileName = fileName;
	desc->texture = texture;
	desc->used = true;
	
	this->textures[ this->textureDescCount ] = desc;
	this->textureDescCount ++;
	
	return D3D_OK;
}
 
a3dEffectDesc* a3dScene::getEffect(shString &fileName) { 

	if(fileName.getLength() < 1) return null;
	
	shString effectFile = fileName;
	if( effectFile.getCString()[1] == ':' )
		effectFile.replacePath( effectFile, this->effectDirectory, this->effectDirShort );
	else 
		effectFile.appendPath( this->textureDirectory );

	int i;
	for( i = 0; i < this->effectDescCount; i++ ) { 
		if( effectFile == this->effects[i]->fullPathName ) return this->effects[i];
	}
	
	HRESULT rv = this->loadEffect( effectFile );
	if( FAILED( rv ) ) 
		return null;
	
	return this->effects[ this->effectDescCount - 1 ];
}

HRESULT a3dScene::loadEffect( shString &fileName ) { 

	HLSLManager manager;
	HRESULT rv;

	manager.result = TEXT( "unavailable" );
	rv = manager.createEffectFromFile( this->kernel->lpDevice, fileName );
	if( FAILED( rv ) ) { 
		this->kernel->ErrorBox( TEXT( "a3dScene" ), TEXT( "Error: can't load effect from file '%s'. returnValue = %s. Detail: %s" ), fileName.getCString(), DXGetErrorString9( rv ), manager.result.getCString() );
		return rv;
	}
	
	a3dEffectDesc *desc = new a3dEffectDesc();
	desc->fullPathName = fileName;
	desc->fileName = fileName;
	desc->fileName.extractFileName();
	desc->lpEffect = manager.lpEffect;
	desc->kernel = this->kernel;
	desc->used = true;

	this->effects[ this->effectDescCount ] = desc;
	this->effectDescCount ++;

	return D3D_OK;
}

int a3dScene::getLightMapCount() { 
	
	int lightMapCount = 0, i;
	
	for( i = 0; i < this->modelRefCount; i++ ) { 
		lightMapCount += this->modelRef[i]->getLightMapCount();
	}
	for( i = 0; i < this->landDescCount; i++ ) {
		lightMapCount += this->lands[i]->land->getLightMapCount();
	}
	for( i = 0; i < this->railDescCount; i++ ) {
		lightMapCount += this->rails[i]->rSystem->getLightMapCount();
	}

	return lightMapCount;
}

a3dLightMap** a3dScene::getLightMaps() { 
	
	a3dLightMap** lmList = new a3dLightMap*[this->getLightMapCount()];
	a3dLightMap** lmTemp;

	int i, ti, lmIndex = 0, tempCount;
	for( i = 0; i < this->modelRefCount; i++ ) { 
		tempCount = this->modelRef[i]->getLightMapCount();
		lmTemp = this->modelRef[i]->getLightMaps();
		for( ti = 0; ti < tempCount; ti++, lmIndex++ ) { 
			lmList[lmIndex] = lmTemp[ti];
		}
		delete[] lmTemp;
	}

	for( i = 0; i < this->landDescCount; i++ ) { 
		tempCount = this->lands[i]->land->getLightMapCount();
		lmTemp = this->lands[i]->land->getLightMaps();
		for( ti = 0; ti < tempCount; ti++, lmIndex ) { 
			lmList[lmIndex] = lmTemp[ti];
		}
		delete[] lmTemp;
	}
	
	for( i = 0; i < this->railDescCount; i++ ) { 
		tempCount = this->rails[i]->rSystem->getLightMapCount();
		lmTemp = this->rails[i]->rSystem->getLightMaps();
		for( ti = 0; ti < tempCount; ti++, lmIndex ) { 
			lmList[lmIndex] = lmTemp[ti];
		}
		delete[] lmTemp;
	}

	return lmList;
}

HRESULT a3dScene::update( camera *cam ) { 

	int index;
	for( index = 0; index < this->landDescCount; index++ ) { 
		this->lands[index]->land->setCamera(cam);
		this->lands[index]->land->prepareConvValues();
		this->lands[index]->land->tesselate();
	}
	
	return D3D_OK;
}

HRESULT a3dScene::renderLights( camera *cam ) { 

	HRESULT rv;
	Vector3 y = { 0.0f, 1.0f, 0.0f };

	int index;
	for( index = 0; index < this->lightsCount; index++ ) { 
		if( this->lights[index]->type == LIGHT_OMNI ) {
			this->omniLightRender->model->origin();
			this->omniLightRender->model->position( ((a3dOmniLight*)this->lights[index])->pos );
			this->omniLightRender->setCamera(cam);
			rv = this->omniLightRender->render();
			if( FAILED( rv ) )
				return rv;
		}
		else if( this->lights[index]->type == LIGHT_DIRECTIONAL ) { 
			this->dirLightRender->model->origin();
			this->dirLightRender->model->position(0.0f, 0.0f, 0.0f);
			this->dirLightRender->setCamera(cam);
			this->dirLightRender->model->direction(((a3dDirectionalLight*)this->lights[index])->dir, y);
			rv = this->dirLightRender->render();
			if( FAILED( rv ) )
				return rv;
		}
	}
	
	return D3D_OK;
} 

HRESULT a3dScene::render( camera *cam ) { 

	HRESULT rv;

	rv = this->renderLand( cam );
	if( FAILED( rv ) )
		return rv;

	rv = this->renderRail( cam );
	if( FAILED( rv ) )
		return rv;
	
	rv = this->renderModelRef( cam );
	if( FAILED( rv ) )
		return rv;

	rv = this->renderLights( cam );
	if( FAILED( rv ) )
		return rv;

	return D3D_OK;
}

HRESULT a3dScene::renderRail( camera *cam ) { 

	HRESULT rv;

	int index;
	for( index = 0; index < this->railDescCount; index++ ) { 
		rv = this->rails[index]->rSystem->render(cam);
		if( FAILED( rv ) )
			return rv;
	}
	
	return D3D_OK;
}

HRESULT a3dScene::renderLand( camera *cam ) {

	HRESULT rv;

	int index;
	for( index = 0; index < this->landDescCount; index++ ) { 
		this->lands[index]->land->setCamera( cam );
		rv = this->lands[index]->land->render();
		if( FAILED( rv ) )
			return rv;
	}
	
	return D3D_OK;
} 

HRESULT a3dScene::renderModelRef( camera *cam ) { 

	HRESULT rv;

	int index;
	for( index = 0; index < this->modelRefCount; index++ ) { 
		this->modelRef[index]->applyBasises();
		rv = this->modelRef[index]->render( cam );
		if( FAILED( rv ) )
			return rv;
	}

	return D3D_OK;
}

bool a3dScene::addModelRef( a3dModelDesc *desc ) { 

	this->modelRef[ this->modelRefCount ] = desc->createReference();
	this->modelRefCount ++;	

	return true;
}

int a3dScene::getModelRefIndex( a3dModelRef *mr ) { 
	
	for( int index = 0; index < this->modelRefCount; index++ ) { 
		if( this->modelRef[index] == mr ) return index;
	}
	return -1;
}	

bool a3dScene::removeModelRef( a3dModelRef *mr ) { 
	
	int index = this->getModelRefIndex( mr );
	if( index == -1 ) return false;

	// deleting ref
	delete this->modelRef[index];
	if( index == this->modelRefCount - 1 ) { 
		this->modelRefCount--;
		return true;
	}
	
	memcpy( &this->modelRef[index], &this->modelRef[index+1], sizeof( a3dModelRef* ) * ( this->modelRefCount - index - 1 ) );
	this->modelRefCount--;
	return false;
}

HRESULT a3dScene::addLandscape( shString &name, shString &hmFile, shString &lmFile, shString &effectFile, float pixelError, int patchPerSide, float heightScale, float sideScale, int lightMapWidth, int lightMapHeight ) { 

	HRESULT rv;
	a3dLandscape *land = new a3dLandscape();
	
	land->setScene( this );
	land->setKernel( this->kernel );
	land->setPixelError( pixelError );
	land->lightMapWidth = lightMapWidth;
	land->lightMapHeight = lightMapHeight;
	
	land->lightMapTextureDesc = this->getTexture( lmFile );

	rv = land->loadHeightMap( hmFile, sideScale, heightScale, patchPerSide );
	if( FAILED( rv ) ) {
		delete land;
		return rv;
	}
	
	land->effectFileName = effectFile;
	rv = land->createEffect();
	if( FAILED( rv ) ) { 
		delete land;
		return rv;
	}

	a3dLandscapeDesc *desc = new a3dLandscapeDesc();
	desc->land = land;
	desc->name = name;

	desc->landInfo = new a3dLandscapeInfo();
	desc->landInfo->initialize( land );
	
	this->lands[ this->landDescCount ] = desc;
	this->landDescCount ++;	

	return D3D_OK;
}

HRESULT a3dScene::addRailSystem(shString &name, shString &railFileName, shString &lineModelFileName, shString &roundModelFileName, shString &lineEffectFileName, shString &roundEffectFileName, shString &round2EffectFileName) { 

	HRESULT rv;

	a3dRailSystemInfo *info = new a3dRailSystemInfo();
	
	info->name = name;
	info->railFileName = railFileName;
	info->lineModelFileName = lineModelFileName;
	info->roundModelFileName = roundModelFileName;
	info->lineEffectFileName = lineEffectFileName;
	info->roundEffectFileName = roundEffectFileName;
	info->round2EffectFileName = round2EffectFileName;
	originMatrix4( info->basis );

	rv = info->loadRail( this );
	if( FAILED( rv ) ) {
		delete info;
		return rv;
	}

	this->addRailSystem( info );

	return D3D_OK;
}

void a3dScene::addRailSystem(a3dRailSystemInfo *info ) { 
	
	a3dRailSystemDesc *desc = new a3dRailSystemDesc();
	
	desc->kernel = kernel;
	desc->name = info->name;
	desc->rsInfo = info;
	desc->rSystem = info->rSystem;
	
	this->rails[this->railDescCount] = desc;
	this->railDescCount ++;

	return ;
}

bool a3dScene::addOmniLight( shString &name, Vector3 color, float farAttenStart, float farAttenEnd, Vector3 position ) { 
	
	if( this->lightsCount >= SCENE_LIGHT_MAX_COUNT ) return false;
	
	a3dOmniLight *light = new a3dOmniLight();
	light->name = name;
	copyVector3(color, light->color);
	light->farAttenStart = farAttenStart;
	light->farAttenEnd = farAttenEnd;
	copyVector3(position, light->pos);
	
	this->lights[this->lightsCount] = light;
	this->lightsCount++;

	return true;
}

bool a3dScene::addParticleSystem(shString &name, particleSystemBase *psys ) { 
	
	if(this->particleDescCount >= SCENE_PS_MAX_COUNT) return false;
	a3dParticleSystemDesc *desc = new a3dParticleSystemDesc();
	//desc->psInfo = new a3dParticleSystemInfo();
	desc->pSystem = psys;
	psys->name = name;

	//desc->psInfo->name = name;
	desc->name = name;
	this->particles[this->particleDescCount] = desc;
	this->particleDescCount ++;

	return true;
}

bool a3dScene::addDirectionalLight( shString &name, Vector3 color, Vector3 direction ) { 
	
	if( this->lightsCount >= SCENE_LIGHT_MAX_COUNT ) return false;
	
	a3dDirectionalLight *light = new a3dDirectionalLight();
	light->name = name;
	copyVector3(color, light->color);
	copyVector3(direction, light->dir);
	
	this->lights[this->lightsCount] = light;
	this->lightsCount++;

	return true;
}

HRESULT a3dScene::initializeDesignModeHelpers() { 
	
	HRESULT rv;
	
	this->omniLightRender = new a3dOmniLightRender();
	this->omniLightRender->setKernel(this->kernel);

	rv = this->omniLightRender->initialize();
	if( FAILED( rv ) )
		return rv;

	this->dirLightRender = new a3dDirectionalLightRender();
	this->dirLightRender->setKernel(this->kernel);
	
	rv = this->dirLightRender->initialize();
	if( FAILED( rv ) )
		return rv;
	
	this->sprayParticleRender = new a3dSprayParticleRender();
	this->sprayParticleRender->setKernel(this->kernel);
	if( FAILED( rv ) )
		return rv;

	return D3D_OK;
}

void a3dScene::changeEffect(a3dShaderMaterial *material, a3dEffectDesc *effect) { 
	
	if( material == null || effect == null ) return;
	
	material->setNewEffect( this, effect );
	
	int mi, oi, di;
	
	for( mi = 0; mi < this->modelDescCount; mi++ ) {
		for( di = 0; di < this->models[mi]->model->materialCount; di++ ) { 
			a3dShaderMaterial *mat = dynamic_cast<a3dShaderMaterial *>(this->models[mi]->model->material[di]);
			if(mat != material) continue;
			mat->setNewEffect(this, effect);
		}
	}
	
	for(mi = 0; mi < this->modelRefCount; mi++) { 
		for(oi = 0; oi < this->modelRef[mi]->objCount; oi++ ) {
			for(di = 0; di < this->modelRef[mi]->objectRef[oi]->detailCount; di++ ) { 
				if(this->modelRef[mi]->objectRef[oi]->detailRef[di]->detail->material != material) continue;
				this->modelRef[mi]->objectRef[oi]->detailRef[di]->material.assign(material); //.setNewEffect(this, effect);
				this->modelRef[mi]->objectRef[oi]->detailRef[di]->material.initializeData(this);
			}
		}
	}

	return ;
}

#undef intsize