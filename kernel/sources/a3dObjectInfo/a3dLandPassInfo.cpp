#include "a3dLandPassInfo.h"
#include "memoryFile.h"

void a3dLandPassInfo::copyTo( a3dLandPassInfo* info ) { 

	info->enabled = this->enabled;
	info->animated = this->animated;
	info->mapFileName = this->mapFileName;		
	info->maskFileName = this->maskFileName;
	info->detailFileName = this->detailFileName;

	info->mapUSize = this->mapUSize;
	info->mapVSize = this->mapVSize;

	info->detUSize = this->detUSize;
	info->detVSize = this->detVSize;

	info->minDetailDist = this->minDetailDist;
	info->maxDetailDist = this->maxDetailDist;
	info->maxDetailAlphaValue = this->maxDetailAlphaValue;

	return ;
}

int a3dLandPassInfo::calcSizeCore() { 

	return sizeof( bool ) + sizeof( bool ) + 
		this->name.calcSize() + this->mapFileName.calcSize() + this->maskFileName.calcSize() + this->detailFileName.calcSize() + 
		sizeof( float ) + sizeof( float ) + 
		sizeof( float ) + sizeof( float ) + 
		sizeof( float ) + sizeof( float ) + sizeof( float );
}

bool a3dLandPassInfo::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !file.writeBool( this->enabled ) ) return false;
	if( !file.writeBool( this->animated ) ) return false;
	if( !this->name.saveToMemory( file ) ) return false;
	if( !this->mapFileName.saveToMemory( file ) ) return false;
	if( !this->maskFileName.saveToMemory( file ) ) return false;
	if( !this->detailFileName.saveToMemory( file ) ) return false;
	if( !file.writeFloat( this->mapUSize ) ) return false; 
	if( !file.writeFloat( this->mapVSize ) ) return false; 
	if( !file.writeFloat( this->detUSize ) ) return false; 
	if( !file.writeFloat( this->detVSize ) ) return false; 
	if( !file.writeFloat( this->minDetailDist ) ) return false;
	if( !file.writeFloat( this->maxDetailDist ) ) return false;
	if( !file.writeFloat( this->maxDetailAlphaValue ) ) return false;

	return true;
}

bool a3dLandPassInfo::loadFromMemoryCore( a3dMemoryFile &file ) { 

	if( !file.readBool( &this->enabled ) ) return false;
	if( !file.readBool( &this->animated ) ) return false;
	if( !this->name.loadFromMemory( file ) ) return false;
	if( !this->mapFileName.loadFromMemory( file ) ) return false;
	if( !this->maskFileName.loadFromMemory( file ) ) return false;
	if( !this->detailFileName.loadFromMemory( file ) ) return false;
	if( !file.readFloat( &this->mapUSize ) ) return false; 
	if( !file.readFloat( &this->mapVSize ) ) return false; 
	if( !file.readFloat( &this->detUSize ) ) return false; 
	if( !file.readFloat( &this->detVSize ) ) return false; 
	if( !file.readFloat( &this->minDetailDist ) ) return false;
	if( !file.readFloat( &this->maxDetailDist ) ) return false;
	if( !file.readFloat( &this->maxDetailAlphaValue ) ) return false;

	return true;
}
