#include "serialization.h"
#include "memoryFile.h"

bool a3dSerializableObject::saveToMemory( a3dMemoryFile &file ) { 
	
	if( !file.writeInt( this->getHeaderCode() ) ) return false;
	if( !file.writeInt( this->calcSizeCore() ) ) return false;
	if( !file.writeInt( this->version ) ) return false;
	if( !this->saveToMemoryCore( file ) ) return false;

	return true;
}

bool a3dSerializableObject::loadFromMemory( a3dMemoryFile &file ) { 
	
	int hdr, sz;
	
	if( !file.readInt( &hdr ) || hdr != this->getHeaderCode() ) return false;
	if( !file.readInt( &sz ) ) return false;
	if( !file.readInt( &this->version ) ) return false;
	if( !this->loadFromMemoryCore( file ) ) return false;
	if( sz != this->calcSizeCore() ) return false;
 
	return true;
}

bool a3dSerializableObject::saveToMemoryCore( a3dMemoryFile &file ) { 
	
	return true;
}

bool a3dSerializableObject::loadFromMemoryCore( a3dMemoryFile &file ) {
	
	return true;
}  