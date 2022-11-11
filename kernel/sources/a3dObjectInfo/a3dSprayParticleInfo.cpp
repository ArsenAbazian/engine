#include "a3dSprayParticleInfo.h"
#include "memoryFile.h"
#include "..\\a3dTextFileTokens.h"
#include "..\\a3dParticleSystem\\particles.h"
#include "..\\a3dScene.h"

void a3dSprayParticleSystemInfo::initializeDefault() { 

	this->viewportCount = 0;
	this->renderCount = 0;
	this->dropSize = 0.0f;
	this->speed = 0.0f;
	this->lifeTime = 0.0f;
	this->birthRate = 0;
	this->emitterHeight = 0.0f;
	this->emitterWidth = 0.0f;

	return;
} 

void a3dSprayParticleSystemInfo::clear() { 

	this->initializeDefault();
}


a3dParticleSystemInfo* a3dSprayParticleSystemInfo::clone() { 

	a3dSprayParticleSystemInfo *res = new a3dSprayParticleSystemInfo();
	this->copyTo( (a3dSprayParticleSystemInfo*)res );
	return res;
}

void a3dSprayParticleSystemInfo::copyTo(a3dParticleSystemInfo *info) { 

	a3dSprayParticleSystemInfo *sinfo = dynamic_cast<a3dSprayParticleSystemInfo*>(info); 
	if( sinfo == null ) return;

	a3dParticleSystemInfo::copyTo( info );

	sinfo->viewportCount = this->viewportCount;
	sinfo->renderCount = this->renderCount;
	sinfo->dropSize = this->dropSize;
	sinfo->speed = this->speed;
	sinfo->lifeTime = this->lifeTime;
	sinfo->birthRate = this->birthRate;
	sinfo->emitterHeight = this->emitterHeight;
	sinfo->emitterWidth = this->emitterWidth;
}

int a3dSprayParticleSystemInfo::calcSizeCore() { 

	return a3dParticleSystemInfo::calcSizeCore() + 
		sizeof( int ) + 
		sizeof( int ) + 
		sizeof( float ) +
		sizeof( float ) + 
		sizeof( float ) +
		sizeof( int ) + 
		sizeof( float ) + 
		sizeof( float );
}

bool a3dSprayParticleSystemInfo::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dParticleSystemInfo::saveToMemoryCore( file ) ) return false;
	if( !file.writeInt( this->viewportCount ) ) return false;
	if( !file.writeInt( this->renderCount ) ) return false;
	if( !file.writeFloat( this->dropSize ) ) return false;
	if( !file.writeFloat( this->speed ) ) return false;
	if( !file.writeFloat( this->lifeTime ) ) return false;
	if( !file.writeInt( this->birthRate ) ) return false;
	if( !file.writeFloat( this->emitterHeight ) ) return false;
	if( !file.writeFloat( this->emitterWidth ) ) return false;

	return true;
}

bool a3dSprayParticleSystemInfo::loadFromMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dParticleSystemInfo::loadFromMemoryCore( file ) ) return false;
	if( !file.readInt( &this->viewportCount ) ) return false;
	if( !file.readInt( &this->renderCount ) ) return false;
	if( !file.readFloat( &this->dropSize ) ) return false;
	if( !file.readFloat( &this->speed ) ) return false;
	if( !file.readFloat( &this->lifeTime ) ) return false;
	if( !file.readInt( &this->birthRate ) ) return false;
	if( !file.readFloat( &this->emitterHeight ) ) return false;
	if( !file.readFloat( &this->emitterWidth ) ) return false;

	return true;
}

bool a3dSprayParticleSystemInfo::readFromLexemeList(std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li) { 

	if( !a3dParticleSystemInfo::readFromLexemeList( lexList, li ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readIntParam( lexList, li, a3dTextFileTokens::a3dviewportcountString, &this->viewportCount ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readIntParam( lexList, li, a3dTextFileTokens::a3drendercountString, &this->renderCount ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatParam( lexList, li, a3dTextFileTokens::a3ddropsizeString, &this->dropSize ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatParam( lexList, li, a3dTextFileTokens::a3dspeedString, &this->speed ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatParam( lexList, li, a3dTextFileTokens::a3dlifetimeString, &this->lifeTime ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readIntParam( lexList, li, a3dTextFileTokens::a3dbirthrateString, &this->birthRate ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatParam( lexList, li, a3dTextFileTokens::a3demitterheightString, &this->emitterHeight ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readFloatParam( lexList, li, a3dTextFileTokens::a3demitterwidthString, &this->emitterWidth ) ) return false;

	if( !a3dParticleSystemInfo::readEnd( lexList, li ) ) return false;

	return true;
}

HRESULT a3dSprayParticleSystemInfo::loadParticle( a3dScene *scene ) { 

	a3dSprayParticleSystem *spray = new a3dSprayParticleSystem();
	this->pSystem = spray;
	this->pSystem->setKernel( scene->kernel );

	spray->maxRenderParticleCount = this->renderCount;
	spray->maxBirthCount = this->birthRate;
	spray->effectFileName = this->effectFileName;

	return D3D_OK;
}
