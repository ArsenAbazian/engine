#include "memoryFile.h"

void a3dMemoryFile::initialize( int size ) { 

	this->clear();
	this->shouldDeleteMemory = true;

	this->size = size;
	this->buffer = new BYTE[ this->size ];
	this->currPos = this->buffer;

	return;
}

void a3dMemoryFile::clear() { 
	
	if( this->shouldDeleteMemory && this->buffer != NULL ) delete[] this->buffer;
	this->buffer = NULL;
	this->currPos = NULL;
	this->size = 0;

	return ;
}

int a3dMemoryFile::getFileSize(FILE *fp) { 
	
	long pos = ftell( fp );
	fseek( fp, 0, SEEK_END );
	long sz = ftell( fp );
	fseek( fp, pos, SEEK_SET );
	return sz;
} 

bool a3dMemoryFile::setMemory(BYTE* buf, int size ) { 
	
	this->shouldDeleteMemory = false;
	this->buffer = buf;
	this->currPos = buf;
	this->size = size;

	return true;
}	

bool a3dMemoryFile::loadFromFile( shString &fileName ) { 

	FILE *fp = NULL;
	
	_tfopen_s( &fp, fileName.getCString(), TEXT( "rb" ) );
	if( fp == NULL ) return false;
	
	this->clear();
	
	this->initialize( this->getFileSize( fp ) );
	fread( this->buffer, 1, this->size, fp );

	fclose( fp );
	
	return true;
}

bool a3dMemoryFile::saveToFile( shString &fileName ) { 
	
	FILE *fp = NULL;
	
	_tfopen_s( &fp, fileName.getCString(), TEXT( "wb" ) );
	if( fp == NULL ) return false;
	
	fwrite( this->buffer, 1, this->size, fp );
	fclose( fp );
	
	return true;
}

bool a3dMemoryFile::readMemBlock( BYTE *block, int count ) { 

	if( this->currPos + count > this->buffer + this->size ) return false; 
	memcpy( block, this->currPos, count );
	
	this->currPos += count;

	return true;
}

bool a3dMemoryFile::writeMemBlock( BYTE *block, int count ) { 

	if( this->currPos + count > this->buffer + this->size ) return false; 
	memcpy( this->currPos, block, count );
	
	this->currPos += count;

	return true;
}

bool a3dMemoryFile::writeString( shString &string, bool asCharArray ) {

	int charSize = asCharArray? sizeof( BYTE ): sizeof( TCHAR ); 
	int stringLen = string.getCString() != NULL? string.getLength() + 1 : 0;

	if( !this->writeInt( stringLen ) ) return false;
	for( int i = 0; i < stringLen; i ++ ) { 
		if( asCharArray ) {
			if( !this->writeByte( (BYTE)string.getCString()[ i ] ) ) return false;
		}
		else {
			if( !this->writeTchar( string.getCString()[ i ] ) ) return false;
		}
	}

	return true;
}

bool a3dMemoryFile::readString( shString &string, bool asCharArray ) {

	int charSize = asCharArray? sizeof( BYTE ): sizeof( TCHAR ); 
	int stringLen;
	
	if( !this->readInt( &stringLen ) ) return false;
	if( this->currPos + stringLen > this->buffer + this->size ) return false;
	
	if( stringLen == 0 ) { 
		string.clear(); 
		return true;
	}
	if( !asCharArray ) string.initialize( (TCHAR*)this->currPos );
	else string.copyFromChar( (char*) this->currPos );

	this->currPos += stringLen * charSize;

	return true;
}

