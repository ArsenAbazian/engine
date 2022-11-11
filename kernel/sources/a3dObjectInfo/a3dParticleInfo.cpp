#include "a3dParticleInfo.h"
#include "..\\a3dTextFileTokens.h"
#include "..\\a3dParticleSystem\\particleSystemBase.h"
#include "..\\a3dParticleSystem\\particles.h"

a3dParticleSystemInfo::~a3dParticleSystemInfo() { 
	if( this->pSystem != null )
		delete this->pSystem;
}

bool a3dParticleSystemInfo::readFromLexemeList(std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li) { 

	if( !a3dVisualObjectInfoBase::readFromLexemeList( lexList, li ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringParam( lexList, li, a3dTextFileTokens::a3deffectFileNameString, this->effectFileName ) ) return false;

	return true;
}

HRESULT a3dParticleSystemInfo::loadParticle( a3dScene *scene ) { 

	return D3D_OK;
}

bool a3dParticleSystemInfo::readEnd(std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li ) { 

	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, a3dTextFileTokens::a3dEndString ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, a3dTextFileTokens::a3dParticleSystemString ) ) return false;

	return true;
}

a3dParticleSystemInfo* a3dParticleSystemInfo::clone() { 

	a3dParticleSystemInfo *res = new a3dParticleSystemInfo();
	this->copyTo( res );
	return res;
}

particleSystemBase* a3dParticleSystemInfo::createParticle() { 
	
	if( this->type == ptSmoke )
		return new a3dSmokeParticleSystem();

	return new particleSystemBase();
}

int a3dParticleSystemInfo::calcSizeCore() { 

	return a3dVisualObjectInfoBase::calcSizeCore() + this->effectFileName.calcSize();
}

bool a3dParticleSystemInfo::saveToMemoryCore(a3dMemoryFile &file) { 

	if( !a3dVisualObjectInfoBase::saveToMemoryCore( file ) ) return false;
	if( !file.writeInt( (int)this->type ) ) return false;
	if( !this->effectFileName.saveToMemory( file ) ) return false;

	return true;
}

bool a3dParticleSystemInfo::loadFromMemoryCore(a3dMemoryFile &file) { 
	
	int tp;
	if( !a3dVisualObjectInfoBase::loadFromMemoryCore( file ) ) return false;
	if( !file.readInt( &tp ) ) return false;
	this->type = (a3dParticleType)tp;
	if( !this->effectFileName.loadFromMemory( file ) ) return false;
	
	this->pSystem = this->createParticle();
	
	return true;
}

void a3dParticleSystemInfo::copyTo(a3dParticleSystemInfo *info) { 

	info->effectFileName = this->effectFileName;
}
