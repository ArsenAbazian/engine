#include "a3dLandscapeInfo.h"
#include "memoryFile.h"
#include "..\\landscape.h"
#include "..\\a3dDesc\\a3dDesc.h"
#include "..\\a3dTextFileTokens.h"
#include "..\\a3dScene.h"

#define HSTRING(x) shString( TEXT ( x ) )

a3dLandscapeInfo::~a3dLandscapeInfo() { 
	if( this->land != null )
		delete this->land;
}

a3dLandscapeInfo* a3dLandscapeInfo::clone() { 

	a3dLandscapeInfo* res = new a3dLandscapeInfo();

	a3dVisualObjectInfoBase::copyTo( (a3dVisualObjectInfoBase*) res );
	res->heightMapFileName = this->heightMapFileName;
	res->effectFileName = this->effectFileName;
	res->pixelError = this->pixelError;
	res->patchPerSide = this->patchPerSide;

	for( int i = 0; i < 16; i++ ) {
		this->pass[i].copyTo( &res->pass[i] );
	}

	return res;
}

void a3dLandscapeInfo::initialize(a3dLandscape *land ) { 

	this->name = name;
	copyMatrix4( land->basis, this->basis );
	this->heightMapFileName = land->heightMapFileName;
	this->effectFileName = land->effectFileName;
	if(land->lightMapTextureDesc != null)
		this->lightMapFileName = land->lightMapTextureDesc->fileName;
	this->pixelError = land->pixelError;
	this->patchPerSide = land->patchCount;
	this->width = land->sideScale;
	this->height = land->heightScale;
	this->lightMapWidth = land->lightMapWidth;
	this->lightMapHeight = land->lightMapHeight;
	this->land = land;

	return ;
}

int a3dLandscapeInfo::calcSizeCore() { 

	int totalSize = a3dVisualObjectInfoBase::calcSizeCore() + 
		this->name.calcSize() +
		this->heightMapFileName.calcSize() + 
		this->effectFileName.calcSize() + 
		this->lightMapFileName.calcSize() +
		sizeof( float ) + 
		sizeof( float ) + 
		sizeof( float ) + 
		sizeof( int ) + 
		sizeof( int ) + 
		sizeof( int );

	for( int i = 0; i < 16; i++ ) { 
		totalSize += this->pass[i].calcSize();
	}

	return totalSize;
}

bool a3dLandscapeInfo::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dVisualObjectInfoBase::saveToMemoryCore( file ) ) return false;
	if( !this->name.saveToMemory( file ) ) return false;
	if( !this->heightMapFileName.saveToMemory( file ) ) return false;
	if( !this->effectFileName.saveToMemory( file ) ) return false;
	if( !this->lightMapFileName.saveToMemory( file ) ) return false;
	if( !file.writeFloat( this->width ) ) return false;
	if( !file.writeFloat( this->height ) ) return false;
	if( !file.writeFloat( this->pixelError ) ) return false;
	if( !file.writeInt( this->patchPerSide ) ) return false;
	if( !file.writeInt( this->lightMapWidth ) ) return false;
	if( !file.writeInt( this->lightMapHeight ) ) return false;

	for( int i = 0; i < 16; i++ ) {
		if( !this->pass[i].saveToMemory( file ) ) return false;
	}

	return true;
}

bool a3dLandscapeInfo::loadFromMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dVisualObjectInfoBase::loadFromMemoryCore( file ) ) return false;
	if( !this->name.loadFromMemory( file ) ) return false;
	if( !this->heightMapFileName.loadFromMemory( file ) ) return false;
	if( !this->effectFileName.loadFromMemory( file ) ) return false;
	if( !this->lightMapFileName.loadFromMemory( file ) ) return false;
	if( !file.readFloat( &this->width ) ) return false;
	if( !file.readFloat( &this->height ) ) return false;
	if( !file.readFloat( &this->pixelError ) ) return false;
	if( !file.readInt( &this->patchPerSide ) ) return false;
	if( !file.readInt( &this->lightMapWidth ) ) return false;
	if( !file.readInt( &this->lightMapHeight ) ) return false;


	for( int i = 0; i < 16; i++ ) {
		if( !this->pass[i].loadFromMemory( file ) ) return false;
	}

	return true;
}

bool a3dLandPassInfo::readFromLexemeList(std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li, int index) { 

	TCHAR buf[256];

	_stprintf_s( buf, 256, TEXT( "pass%dEnabled" ), index );
	if( !readBoolParam( lexList, li, shString( buf ), &this->enabled ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	_stprintf_s( buf, 256, TEXT( "pass%dAnimated" ), index );
	if( !readBoolParam( lexList, li, shString( buf ), &this->animated ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	_stprintf_s( buf, 256, TEXT( "pass%dMapFileName" ), index );
	if( !readStringParam( lexList, li, shString( buf ), this->mapFileName ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	_stprintf_s( buf, 256, TEXT( "pass%dMaskFileName" ), index );
	if( !readStringParam( lexList, li, shString( buf ), this->maskFileName ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	_stprintf_s( buf, 256, TEXT( "pass%dDetailFileName" ), index );
	if( !readStringParam( lexList, li, shString( buf ), this->detailFileName ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	_stprintf_s( buf, 256, TEXT( "pass%dMapUSize" ), index );
	if( !readFloatParam( lexList, li, shString( buf ), &this->mapUSize ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	_stprintf_s( buf, 256, TEXT( "pass%dMapVSize" ), index );
	if( !readFloatParam( lexList, li, shString( buf ), &this->mapVSize ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	_stprintf_s( buf, 256, TEXT( "pass%dDetailUSize" ), index );
	if( !readFloatParam( lexList, li, shString( buf ), &this->detUSize ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	_stprintf_s( buf, 256, TEXT( "pass%dDetailVSize" ), index );
	if( !readFloatParam( lexList, li, shString( buf ), &this->detVSize ) ) return false;

	return true;
}

bool a3dLandscapeInfo::readFromLexemeList(std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) {

	if( !a3dVisualObjectInfoBase::readFromLexemeList( lexList, li ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringParam( lexList, li, HSTRING( "hegithMapFileName" ), this->heightMapFileName ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringParam( lexList, li, HSTRING( "effectFileName" ), this->effectFileName ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringParam( lexList, li, HSTRING( "lightMapFileName" ), this->lightMapFileName ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatParam( lexList, li, HSTRING( "pixelError" ), &this->pixelError ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readIntParam( lexList, li, HSTRING( "patchPerSide" ), &this->patchPerSide ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readIntParam( lexList, li, HSTRING( "lightMapWidth" ), &this->lightMapWidth ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readIntParam( lexList, li, HSTRING( "lightMapHeight" ), &this->lightMapHeight ) ) return false;

	for( int i = 0; i < 16; i ++ ) { 

		if( !safeNextLex( lexList, li ) ) return false;
		if( !this->pass[i].readFromLexemeList( lexList, li, i ) ) return false;
	}

	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, HSTRING( "end" ) ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, HSTRING( "a3dLand" ) ) ) return false;

	return true;
}

void a3dLandscapeInfo::initializeDefault() { 

	this->land = null;
	this->width = 0.0f;
	this->height = 0.0f;
	this->pixelError = 0.0f;
	this->patchPerSide = 0;
	this->lightMapWidth = 0;
	this->lightMapHeight = 0;


	for( int i = 0; i < 16; i ++ ) {
		this->pass[i].initializeDefault();
		this->pass[i].landInfo = this;
		this->pass[i].passIndex = i;
	}
}

HRESULT a3dLandscapeInfo::loadLand( a3dScene *scene ) { 

	shString file;
	a3dTextureDesc	*map, *mask, *detail, *lightMap;

	this->land = new a3dLandscape();
	this->land->setKernel( scene->kernel );

	file = this->heightMapFileName;
	file.appendPath( scene->landDirectory );
	HRESULT rv = land->loadHeightMap( file, this->width, this->height, this->patchPerSide );
	if( FAILED( rv ) ) { 
		_stprintf_s(scene->kernel->tempBuffer, scene->kernel->bufferSize, TEXT( "Error: can't load heightmap from file '%s'" ), file.getCString() );
		scene->kernel->ErrorBox( TEXT( "a3dLandscapeInfo" ) );
		return D3DERR_INVALIDCALL;
	}

	land->setTextureCoors();
	land->setPixelError( this->pixelError );
	land->lightMapWidth = this->lightMapWidth;
	land->lightMapHeight = this->lightMapHeight;

	land->effectFileName = this->effectFileName;

	lightMap = scene->getTexture( this->lightMapFileName );
	if( lightMap == null )
		return D3DERR_INVALIDCALL;
	land->setLightMapTexture( lightMap );

	for( int i = 0; i < 16; i++ ) { 
		if( this->pass[i].enabled == false ) continue;

		map = scene->getTexture( this->pass[i].mapFileName );
		if( map == null )
			return D3DERR_INVALIDCALL;

		detail = scene->getTexture( this->pass[i].detailFileName ); 
		if( map == null )
			return D3DERR_INVALIDCALL;

		mask = scene->getTexture( this->pass[i].maskFileName );
		if( mask == null )
			return D3DERR_INVALIDCALL;

		landMapPassInfo *ps = new landMapPassInfo( land, map, mask, detail, this->pass[i].mapUSize, this->pass[i].mapVSize, this->pass[i].detUSize, this->pass[i].detUSize, this->pass[i].minDetailDist, this->pass[i].maxDetailDist, this->pass[i].maxDetailAlphaValue );
		ps->enabled = this->pass[i].enabled;
		ps->animated = this->pass[i].animated;
		land->addPass( ps );
	}

	copyMatrix4( this->basis, this->land->basis );

	land->effectFileName = this->effectFileName;
	a3dEffectDesc *effect = scene->getEffect( this->effectFileName );
	if( effect == null )
		return D3DERR_INVALIDCALL;

	land->lpEffect = effect->lpEffect;

	return D3D_OK;
}

void a3dLandPassInfo::initializeDefault() { 

	this->enabled = false;
	this->animated = false;

	this->mapUSize = 0.0f;
	this->mapVSize = 0.0f;
	this->detUSize = 0.0f;
	this->detVSize = 0.0f;
	this->minDetailDist = 10.0f;
	this->maxDetailDist = 1.0f;
	this->maxDetailAlphaValue = 0.6f;
}