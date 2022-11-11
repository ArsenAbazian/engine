#ifndef A3D_MEMORY_FILE_INC
#define A3D_MEMORY_FILE_INC

#include "hstring.h"
#include "vecmat.h"

class a3dMemoryFile { 
	int getFileSize( FILE *fp );
	bool shouldDeleteMemory;
public:
	BYTE	*buffer;
	BYTE	*savedPos;
	BYTE	*currPos;
	int		size;

	a3dMemoryFile() { 
		this->shouldDeleteMemory = false;
		this->buffer = NULL;
		this->currPos = NULL;
		this->size = 0;
	}
	~a3dMemoryFile() { this->clear(); }

	void initialize( int size );
	void clear();
	
	int  usedSpace() { return (int)(this->currPos - this->buffer); }
	int  emptySpace() { return this->size - this->usedSpace(); }

	bool saveToFile( shString &fileName );
	bool loadFromFile( shString &fileName );
	
	bool setMemory( BYTE* buf, int size );

	bool writeMemBlock( BYTE *block, int count );
	bool writeByte( BYTE b ) { return this->writeMemBlock( (BYTE*)&b, sizeof( BYTE ) ); }  
	bool writeByteArray( BYTE *b, int count ) { return this->writeMemBlock( b, sizeof( BYTE ) * count ); }
	bool writeBool( bool b ) { return this->writeMemBlock( (BYTE*)&b, sizeof( bool ) ); }
	bool writeBoolArray( bool *b, int count ) { return this->writeMemBlock( (BYTE*)b, sizeof( bool ) * count ); }
	bool writeBOOL( BOOL b ) { return this->writeMemBlock( (BYTE*)&b, sizeof( BOOL ) ); }
	bool writeBOOLArray( BOOL *b, int count ) { return this->writeMemBlock( (BYTE*)b, sizeof( BOOL ) * count ); }
	bool writeTchar( TCHAR ch ) { return this->writeMemBlock( (BYTE*)&ch, sizeof( TCHAR ) ); } 
	bool writeTcharArray( TCHAR *ch, int count ) { return this->writeMemBlock( (BYTE*)ch, sizeof( TCHAR ) * count ); }
	bool writeInt( int value ) { return this->writeMemBlock( (BYTE*) &value, sizeof( int ) ); }
	bool writeIntArray( int *arr, int count ) { return this->writeMemBlock( (BYTE*)arr, sizeof( int ) * count ); }
	bool writeFloat( float value ) { return this->writeMemBlock( (BYTE*) &value, sizeof( float ) ); }
	bool writeFloatArray( float *arr, int count ) { return this->writeMemBlock( (BYTE*)arr, sizeof( float ) * count ); }
	bool writeString( shString &string, bool asCharArray ); 
	bool writeVector3( Vector3 v ) { return this->writeMemBlock( (BYTE*)v, sizeof( Vector3 ) ); }
	bool writeMatrix4( Matrix4 mat ) { return this->writeMemBlock( (BYTE*)mat, sizeof( Matrix4 ) ); }
	bool writeMatrix4Array( Matrix4 *arr, int count ) { return this->writeMemBlock( (BYTE*)arr, sizeof( Matrix4 ) * count ); }

	bool readMemBlock( BYTE *block, int count );
	bool readByte( BYTE *b ) { return this->readMemBlock( b, 1 ); }  
	bool readByteArray( BYTE *b, int count ) { return this->readMemBlock( (BYTE*)b, sizeof( BYTE ) * count ); }
	bool readBool( bool *b ) { return this->readMemBlock( (BYTE*)b, sizeof( bool ) ); }
	bool readBoolArray( bool *b, int count ) { return this->readMemBlock( (BYTE*)b, sizeof( bool ) * count ); }
	bool readBOOL( BOOL *b ) { return this->readMemBlock( (BYTE*)b, sizeof( BOOL ) ); }
	bool readBOOLArray( BOOL *b, int count ) { return this->readMemBlock( (BYTE*)b, sizeof( BOOL ) * count ); }
	bool readTchar( TCHAR *ch ) { return this->readMemBlock( (BYTE*)ch, sizeof( TCHAR ) ); } 
	bool readTcharArray( TCHAR *ch, int count ) { return this->readMemBlock( (BYTE*)ch, sizeof( TCHAR ) * count ); }
	bool readInt( int *value ) { return this->readMemBlock( (BYTE*) value, sizeof( int ) ); }
	bool readIntArray( int *arr, int count ) { return this->readMemBlock( (BYTE*)arr, sizeof( int ) * count ); }
	bool readFloat( float *value ) { return this->readMemBlock( (BYTE*) value, sizeof( float ) ); }
	bool readFloatArray( float *arr, int count ) { return this->readMemBlock( (BYTE*)arr, sizeof( float ) * count ); }
	bool readString( shString &string, bool asCharArray );
	bool readVector3( Vector3 v ) { return this->readMemBlock( (BYTE*)v, sizeof( Vector3 ) ); }
	bool readMatrix4( Matrix4 mat ) { return this->readMemBlock( (BYTE*)mat, sizeof( Matrix4 ) ); }
	bool readMatrix4Array( Matrix4 *arr, int count ) { return this->readMemBlock( (BYTE*)arr, sizeof( Matrix4 ) * count ); }
	
	void savePos() { this->savedPos = this->currPos; }
	void restorePos() { this->currPos = this->savedPos; }
	int	 delta() { return (int)(this->currPos - this->savedPos); }
};

#endif