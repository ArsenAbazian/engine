#include "a3dRailSystemInfo.h"
#include "..\\railManager.h"
#include "..\\a3dScene.h"
#include "..\\a3dTextFileTokens.h"
#include "memoryFile.h"

a3dRailSystemInfo::~a3dRailSystemInfo() { 
	if( this->rSystem != null )
		delete this->rSystem;
}

HRESULT a3dRailSystemInfo::loadRail( a3dScene *scene ) { 

	this->rSystem = new a3dRailManager();

	shString railFile = this->railFileName;
	railFile.appendPath( scene->railSystemDirectory );

	if( !this->rSystem->loadFromFile( railFile ) ) { 
		_stprintf_s( scene->kernel->tempBuffer, scene->kernel->bufferSize, TEXT( "Error: can't load rail from file '%s'" ), railFile.getCString() );
		scene->kernel->ErrorBox( TEXT( "a3dRailSystemInfo" ) );
		return D3DERR_INVALIDCALL;
	}

	this->rSystem->setOrigin( this->basis[3][0], this->basis[3][1], this->basis[3][2] );

	a3dModelDesc *lineModelDesc = scene->getModel( this->lineModelFileName );
	a3dModelDesc *roundModelDesc = scene->getModel( this->roundModelFileName );

	if( lineModelDesc == null ) { 
		scene->kernel->ErrorBox( TEXT( "Error: can't load model '%s'" ), this->lineModelFileName.getCString()); 	
		return D3DERR_INVALIDCALL;
	}

	if( roundModelDesc == null ) { 
		scene->kernel->ErrorBox( TEXT( "Error: can't load model '%s'" ), this->roundModelFileName.getCString()); 	
		return D3DERR_INVALIDCALL;
	}

	a3dEffectDesc *lineEffectDesc = scene->getEffect( this->lineEffectFileName );
	a3dEffectDesc *roundEffectDesc = scene->getEffect( this->roundEffectFileName );
	a3dEffectDesc *round2EffectDesc = scene->getEffect( this->round2EffectFileName );

	if( lineEffectDesc == null ) { 
		scene->kernel->ErrorBox( TEXT( "Error: can't load effect '%s'" ), this->lineEffectFileName.getCString()); 	
		return D3DERR_INVALIDCALL;
	}

	if( roundEffectDesc == null ) { 
		scene->kernel->ErrorBox( TEXT( "Error: can't load effect '%s'" ), this->roundEffectFileName.getCString()); 	
		return D3DERR_INVALIDCALL;
	}

	if( round2EffectDesc == null ) { 
		scene->kernel->ErrorBox( TEXT( "Error: can't load effect '%s'" ), this->round2EffectFileName.getCString()); 	
		return D3DERR_INVALIDCALL;
	}

	this->rSystem->setRailModels( lineModelDesc, roundModelDesc );
	this->rSystem->setRailEffects( lineEffectDesc, roundEffectDesc, round2EffectDesc );

	return D3D_OK;
}

bool a3dRailSystemInfo::readFromLexemeList(std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 

	if( !a3dVisualObjectInfoBase::readFromLexemeList( lexList, li ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringParam( lexList, li, a3dTextFileTokens::a3drailFileNameString, this->railFileName ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringParam( lexList, li, a3dTextFileTokens::a3dlineModelFileNameString, this->lineModelFileName ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringParam( lexList, li, a3dTextFileTokens::a3droundModelFileNameString, this->roundModelFileName ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringParam( lexList, li, a3dTextFileTokens::a3dlineEffectFileNameString, this->lineEffectFileName ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringParam( lexList, li, a3dTextFileTokens::a3droundEffectFileNameString, this->roundEffectFileName ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringParam( lexList, li, a3dTextFileTokens::a3dround2EffectFileNameString, this->round2EffectFileName ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringParam( lexList, li, a3dTextFileTokens::a3dlandNameString, this->landName ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readIntParam( lexList, li, a3dTextFileTokens::a3dlandHeightPixelXString, &this->landHeightPixelX ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readIntParam( lexList, li, a3dTextFileTokens::a3dlandHeightPixelYString, &this->landHeightPixelY ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, a3dTextFileTokens::a3dEndString ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, a3dTextFileTokens::a3da3dRailSystemString ) ) return false;	

	return true;
}

a3dRailSystemInfo* a3dRailSystemInfo::clone() { 

	a3dRailSystemInfo* res = new a3dRailSystemInfo();
	this->copyTo( res );	
	return res;
}

void a3dRailSystemInfo::copyTo(a3dVisualObjectInfoBase *obj) { 

	a3dRailSystemInfo *info = dynamic_cast<a3dRailSystemInfo*>(obj);
	if( info == null ) return;

	a3dVisualObjectInfoBase::copyTo( obj );

	info->railFileName = this->railFileName;
	info->lineModelFileName = this->lineModelFileName;
	info->roundModelFileName = this->roundModelFileName;
	info->lineEffectFileName = this->lineEffectFileName;
	info->roundEffectFileName = this->roundEffectFileName;
	info->round2EffectFileName = this->round2EffectFileName;
	info->landName = this->landName;
	info->landHeightPixelX = this->landHeightPixelX;
	info->landHeightPixelY = this->landHeightPixelY;
}

int a3dRailSystemInfo::calcSizeCore() { 

	return a3dVisualObjectInfoBase::calcSizeCore() +
		this->railFileName.calcSize() + 
		this->lineModelFileName.calcSize() + 
		this->roundModelFileName.calcSize() + 
		this->lineEffectFileName.calcSize() + 
		this->roundEffectFileName.calcSize() + 
		this->round2EffectFileName.calcSize() + 
		this->landName.calcSize() + 
		sizeof( int ) + 
		sizeof( int );
}

bool a3dRailSystemInfo::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dVisualObjectInfoBase::saveToMemoryCore( file ) ) return false;
	if( !this->railFileName.saveToMemory( file ) ) return false;
	if( !this->lineModelFileName.saveToMemory( file ) ) return false;
	if( !this->roundModelFileName.saveToMemory( file ) ) return false;
	if( !this->lineEffectFileName.saveToMemory( file ) ) return false;
	if( !this->roundEffectFileName.saveToMemory( file ) ) return false;
	if( !this->round2EffectFileName.saveToMemory( file ) ) return false;
	if( !this->landName.saveToMemory( file ) ) return false;
	if( !file.writeInt( this->landHeightPixelX ) ) return false;
	if( !file.writeInt( this->landHeightPixelY ) ) return false;

	return true;
}

bool a3dRailSystemInfo::loadFromMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dVisualObjectInfoBase::loadFromMemoryCore( file ) ) return false;
	if( !this->railFileName.loadFromMemory( file ) ) return false;
	if( !this->lineModelFileName.loadFromMemory( file ) ) return false;
	if( !this->roundModelFileName.loadFromMemory( file ) ) return false;
	if( !this->lineEffectFileName.loadFromMemory( file ) ) return false;
	if( !this->roundEffectFileName.loadFromMemory( file ) ) return false;
	if( !this->round2EffectFileName.loadFromMemory( file ) ) return false;
	if( !this->landName.loadFromMemory( file ) ) return false;
	if( !file.readInt( &this->landHeightPixelX ) ) return false;
	if( !file.readInt( &this->landHeightPixelY ) ) return false;

	return true;
}
