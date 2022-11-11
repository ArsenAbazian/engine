#include "vertexInfo.h"
#include "memoryFile.h"
#include "3dtypes.h"

// теперь начинаем искать
DWORD fvfArray[] = { D3DFVF_XYZ, 
					D3DFVF_XYZRHW, 
					D3DFVF_XYZB1, 
					D3DFVF_XYZB2, 
					D3DFVF_XYZB3, 
					D3DFVF_XYZB4, 
					D3DFVF_XYZB5, 
					D3DFVF_NORMAL, 
					D3DFVF_PSIZE, 
					D3DFVF_DIFFUSE, 
					D3DFVF_SPECULAR,
					D3DFVF_TEX0, 
					D3DFVF_TEX1,
					D3DFVF_TEX2,
					D3DFVF_TEX3, 
					D3DFVF_TEX4, 
					D3DFVF_TEX5,
					D3DFVF_TEX6,
					D3DFVF_TEX7,
					D3DFVF_TEX8 };

shString fvfString[] = { shString( TEXT( "D3DFVF_XYZ" ) ), 
							shString( TEXT( "D3DFVF_XYZRHW" ) ), 
							shString( TEXT( "D3DFVF_XYZB1" ) ), 
							shString( TEXT( "D3DFVF_XYZB2" ) ), 
							shString( TEXT( "D3DFVF_XYZB3" ) ), 
							shString( TEXT( "D3DFVF_XYZB4" ) ), 
							shString( TEXT( "D3DFVF_XYZB5" ) ), 
							shString( TEXT( "D3DFVF_NORMAL" ) ), 
							shString( TEXT( "D3DFVF_PSIZE" ) ), 
							shString( TEXT( "D3DFVF_DIFFUSE" ) ), 
							shString( TEXT( "D3DFVF_SPECULAR" ) ),
							shString( TEXT( "D3DFVF_TEX0" ) ), 
							shString( TEXT( "D3DFVF_TEX1" ) ),
							shString( TEXT( "D3DFVF_TEX2" ) ),
							shString( TEXT( "D3DFVF_TEX3" ) ), 
							shString( TEXT( "D3DFVF_TEX4" ) ), 
							shString( TEXT( "D3DFVF_TEX5" ) ),
							shString( TEXT( "D3DFVF_TEX6" ) ),
							shString( TEXT( "D3DFVF_TEX7" ) ),
							shString( TEXT( "D3DFVF_TEX8" ) ) 
};

shString texCoorString[] = { 
	shString( TEXT( "D3DFVF_TEXCOORDSIZE1(1)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE2(1)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE3(1)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE4(1)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE1(2)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE2(2)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE3(2)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE4(2)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE1(3)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE2(3)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE3(3)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE4(3)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE1(4)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE2(4)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE3(4)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE4(4)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE1(5)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE2(5)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE3(5)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE4(5)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE1(6)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE2(6)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE3(6)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE4(6)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE1(7)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE2(7)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE3(7)" ) ),
	shString( TEXT( "D3DFVF_TEXCOORDSIZE4(7)" ) )
};

DWORD texCoorSizeArray[] = { 
	D3DFVF_TEXCOORDSIZE1(1),
	D3DFVF_TEXCOORDSIZE2(1),
	D3DFVF_TEXCOORDSIZE3(1),
	D3DFVF_TEXCOORDSIZE4(1),
	D3DFVF_TEXCOORDSIZE1(2),
	D3DFVF_TEXCOORDSIZE2(2),
	D3DFVF_TEXCOORDSIZE3(2),
	D3DFVF_TEXCOORDSIZE4(2),
	D3DFVF_TEXCOORDSIZE1(3),
	D3DFVF_TEXCOORDSIZE2(3),
	D3DFVF_TEXCOORDSIZE3(3),
	D3DFVF_TEXCOORDSIZE4(3),
	D3DFVF_TEXCOORDSIZE1(4),
	D3DFVF_TEXCOORDSIZE2(4),
	D3DFVF_TEXCOORDSIZE3(4),
	D3DFVF_TEXCOORDSIZE4(4),
	D3DFVF_TEXCOORDSIZE1(5),
	D3DFVF_TEXCOORDSIZE2(5),
	D3DFVF_TEXCOORDSIZE3(5),
	D3DFVF_TEXCOORDSIZE4(5),
	D3DFVF_TEXCOORDSIZE1(6),
	D3DFVF_TEXCOORDSIZE2(6),
	D3DFVF_TEXCOORDSIZE3(6),
	D3DFVF_TEXCOORDSIZE4(6),
	D3DFVF_TEXCOORDSIZE1(7),
	D3DFVF_TEXCOORDSIZE2(7),
	D3DFVF_TEXCOORDSIZE3(7),
	D3DFVF_TEXCOORDSIZE4(7)
};

a3dVertexFieldType vftArray[] = {  
					VFT_XYZ, 
					VFT_XYZRHW, 
					VFT_XYZB1, 
					VFT_XYZB2, 
					VFT_XYZB3, 
					VFT_XYZB4, 
					VFT_XYZB5, 
					VFT_NORMAL, 
					VFT_PSIZE, 
					VFT_DIFFUSE, 
					VFT_SPECULAR,
					VFT_TEX0, 
					VFT_TEX1,
					VFT_TEX2,
					VFT_TEX3, 
					VFT_TEX4, 
					VFT_TEX5,
					VFT_TEX6,
					VFT_TEX7,
					VFT_TEX8 
}; 

void a3dVertexInfo::clear() { 
	
	this->vertexSize = 0;
	this->fieldCount;
	ZeroMemory( this->fieldOffset, sizeof( this->fieldOffset ) );
	ZeroMemory( this->fieldType, sizeof( this->fieldType ) );
	this->vertexFVF = 0;
	ZeroMemory( this->texCoorSize, sizeof( this->texCoorSize ) );
	ZeroMemory( this->texMappedType, sizeof( this->texMappedType ) );
}

void a3dVertexInfo::initialize( DWORD fvf ) { 
	
	this->clear();
	this->vertexFVF = fvf;
	
	// декодируем FVF
	this->decodeFVF();
	
	// начинаем строить смещения
	this->calcFieldOffsets();

	// посчитаем размер вершины
	this->vertexSize = this->calcVertexSize();
}

void a3dVertexInfo::decodeFVF() {
	
	// инициализируем размерность текстуры
	this->initializeTexCoorSize();
	
	this->fieldCount = 0;
	
	int arraySize = sizeof(fvfArray) / sizeof(DWORD);
	
	int i, texIndex;
	for( i = 0; i < arraySize; i++ ) {
		if( fvfArray[i] == 0 ) continue;
		if( fvfArray[i] == D3DFVF_TEX1 ) { 
			texIndex = i;
			break; 
		}
		if( ( fvfArray[ i ] & this->vertexFVF ) == fvfArray[ i ] ) { 
			this->fieldType[ this->fieldCount ] = vftArray[i];
			this->fieldCount ++;
		}
	}

	// now texture
	int fc = 8;
	for( i = arraySize - 1; i >= texIndex; i--, fc-- ) { 
		if( ( fvfArray[i] & this->vertexFVF ) == fvfArray[ i ] ) { 
			for( int ft = 0; ft < fc; ft++ ) { 
				this->fieldType[ this->fieldCount + ft ] = vftArray[texIndex + ft];
			}
			this->fieldCount += fc;
			break;
		}
	} 
}

void a3dVertexInfo::initializeTexCoorSize() { 
	
	DWORD fvf = this->vertexFVF;
	fvf >>= 16;
	for( int i = 0; i < 8; i++, fvf >>= 2 ) { 
		if( (fvf & 0x03) == 0)
			this->texCoorSize[i] = 2 * sizeof(float);
		else if( (fvf & 0x03) == 1) 
			this->texCoorSize[i] = 3 * sizeof(float);
		else if( (fvf & 0x03) == 2) 
			this->texCoorSize[i] = 4 * sizeof(float);
		else if( (fvf & 0x03) == 3) 
			this->texCoorSize[i] = sizeof(float);
	}
}

void a3dVertexInfo::setTex( BYTE *data, int texIndex, float *t ) {
	
	switch( texIndex ) { 
		case 0: this->setTex0( data, t );
		case 1: this->setTex1( data, t );
		case 2: this->setTex2( data, t );
		case 3: this->setTex3( data, t );
		case 4: this->setTex4( data, t );
		case 5: this->setTex5( data, t );
		case 6: this->setTex6( data, t );
		case 7: this->setTex7( data, t );
		case 8: this->setTex8( data, t );
	}
}

float* a3dVertexInfo::getTex( BYTE *data, int texIndex ) { 
	
	switch( texIndex ) { 
		case 0: return this->getTex0( data );
		case 1: return this->getTex1( data );
		case 2: return this->getTex2( data );
		case 3: return this->getTex3( data );
		case 4: return this->getTex4( data );
		case 5: return this->getTex5( data );
		case 6: return this->getTex6( data );
		case 7: return this->getTex7( data );
		case 8: return this->getTex8( data );
	}

	return null;
}

int a3dVertexInfo::getTexIndexByMapType( a3dsMapType mapType ) { 
	
	for( int i = 0; i < 9; i ++ ) { 
		if( this->texMappedType[ i ] == mapType ) return i;
	}
	return -1;
}

bool a3dVertexInfo::hasField( a3dVertexFieldType field ) { 
	
	int i;
	for( i = 0; i < this->fieldCount; i ++ ) { 
		if( this->fieldType[i] == field ) return true;
	}
	return false;
}

int a3dVertexInfo::getFieldSize( a3dVertexFieldType field ) { 
	
	switch(field) { 
		case VFT_XYZ: return sizeof(float) * 3;
		case VFT_XYZRHW: return sizeof(float) * 4;
		case VFT_XYZB1: 
		case VFT_XYZB2:
		case VFT_XYZB3:
		case VFT_XYZB4:
		case VFT_XYZB5: return sizeof(float);	
		case VFT_NORMAL: return sizeof(float) * 3;
		case VFT_PSIZE: return sizeof(float);
		case VFT_DIFFUSE: return sizeof(DWORD);
		case VFT_SPECULAR: return sizeof(DWORD);
		case VFT_TEX0: return this->texCoorSize[0];
		case VFT_TEX1: return this->texCoorSize[1];
		case VFT_TEX2: return this->texCoorSize[2];
		case VFT_TEX3: return this->texCoorSize[3];
		case VFT_TEX4: return this->texCoorSize[4];
		case VFT_TEX5: return this->texCoorSize[5];
		case VFT_TEX6: return this->texCoorSize[6];
		case VFT_TEX7: return this->texCoorSize[7];
		case VFT_TEX8: return this->texCoorSize[8];
	}
	
	return 0;
}

void a3dVertexInfo::calcFieldOffsets() { 

	int i, offset = 0;
	for( i = 0; i < this->fieldCount; i++ ) { 
		this->fieldOffset[ i ] = offset;
		offset += this->getFieldSize( this->fieldType[ i ] );
	}

	return ;
} 

int a3dVertexInfo::calcVertexSize() { 
	return this->fieldOffset[ this->fieldCount - 1 ] + this->getFieldSize( this->fieldType[ this->fieldCount - 1 ] );
}

BYTE* a3dVertexInfo::getField( BYTE *data, a3dVertexFieldType f ) { 
	
	int i;
	for( i = 0; i < this->fieldCount; i ++ ) { 
		if( this->fieldType[ i ] == f ) return data + this->fieldOffset[ i ];
	}
	
	return null;
}

float*	a3dVertexInfo::getXYZ( BYTE *data ) { return (float*)this->getField( data, VFT_XYZ ); }

float*	a3dVertexInfo::getXYZRHW( BYTE *data ) { return (float*)this->getField( data, VFT_XYZRHW ); }

float* a3dVertexInfo::getXYZB1( BYTE *data ) { return (float*)this->getField( data, VFT_XYZB1 ); }

float* a3dVertexInfo::getXYZB2( BYTE *data ) { return (float*)this->getField( data, VFT_XYZB2 ); }

float* a3dVertexInfo::getXYZB3( BYTE *data ) { return (float*)this->getField( data, VFT_XYZB3 ); }

float* a3dVertexInfo::getXYZB4( BYTE *data ) { return (float*)this->getField( data, VFT_XYZB4 ); }

float* a3dVertexInfo::getXYZB5( BYTE *data ) { return (float*)this->getField( data, VFT_XYZB5 ); }

float* a3dVertexInfo::getNormal( BYTE *data ) { return (float*)this->getField( data, VFT_NORMAL ); }

float* a3dVertexInfo::getPSIZE( BYTE *data ) { return (float*)this->getField( data, VFT_PSIZE ); }

DWORD* a3dVertexInfo::getDIFFUSE( BYTE *data ) { return (DWORD*)this->getField( data, VFT_DIFFUSE ); }

DWORD* a3dVertexInfo::getSPECULAR( BYTE *data ) { return (DWORD*)this->getField( data, VFT_SPECULAR ); }

float* a3dVertexInfo::getTex0( BYTE *data ) { return (float*)this->getField( data, VFT_TEX0 ); }

float* a3dVertexInfo::getTex1( BYTE *data ) { return (float*)this->getField( data, VFT_TEX1 ); }

float* a3dVertexInfo::getTex2( BYTE *data ) { return (float*)this->getField( data, VFT_TEX2 ); }

float* a3dVertexInfo::getTex3( BYTE *data ) { return (float*)this->getField( data, VFT_TEX3 ); }

float* a3dVertexInfo::getTex4( BYTE *data ) { return (float*)this->getField( data, VFT_TEX4 ); }

float* a3dVertexInfo::getTex5( BYTE *data ) { return (float*)this->getField( data, VFT_TEX5 ); }

float* a3dVertexInfo::getTex6( BYTE *data ) { return (float*)this->getField( data, VFT_TEX6 ); }

float* a3dVertexInfo::getTex7( BYTE *data ) { return (float*)this->getField( data, VFT_TEX7 ); }

float* a3dVertexInfo::getTex8( BYTE *data ) { return (float*)this->getField( data, VFT_TEX8 ); }


void a3dVertexInfo::setXYZ( BYTE *data, float *v ) { 
	float* f = (float*)this->getField( data, VFT_XYZ ); 
	if( f == null ) return ;

	f[0] = v[0]; f[1] = v[1]; f[2] = v[2];
}

void a3dVertexInfo::setXYZRHW( BYTE *data, float *v ) { 
	float* f = (float*)this->getField( data, VFT_XYZRHW ); 
	if( f == null ) return ;

	f[0] = v[0]; f[1] = v[1]; f[2] = v[2]; f[3] = v[3];
}

void a3dVertexInfo::setXYZB1( BYTE *data, float *b1 ) { 
	float* f = (float*)this->getField( data, VFT_XYZB1 ); 
	if( f == null ) return ;

	*f = *b1;
}

void a3dVertexInfo::setXYZB2( BYTE *data, float *b2 ) { 
	float* f = (float*)this->getField( data, VFT_XYZB2 ); 
	if( f == null ) return ;

	*f = *b2;
}

void a3dVertexInfo::setXYZB3( BYTE *data, float *b3 ) { 
	float* f = (float*)this->getField( data, VFT_XYZB3 ); 
	if( f == null ) return ;

	*f = *b3;
}

void a3dVertexInfo::setXYZB4( BYTE *data, float *b4 ) { 
	float* f = (float*)this->getField( data, VFT_XYZB4 ); 
	if( f == null ) return ;

	*f = *b4;
}

void a3dVertexInfo::setXYZB5( BYTE *data, float *b5 ) { 
	float* f = (float*)this->getField( data, VFT_XYZB5 ); 
	if( f == null ) return ;

	*f = *b5;
}

void a3dVertexInfo::setNormal( BYTE *data, float *v ) { 
	float* f = (float*)this->getField( data, VFT_NORMAL ); 
	if( f == null ) return ;

	f[0] = v[0]; f[1] = v[1]; f[2] = v[2];
}

void a3dVertexInfo::setPSIZE( BYTE *data, float *p ) { 
	float* f = (float*)this->getField( data, VFT_PSIZE ); 
	if( f == null ) return ;

	*f = *p;
}

void a3dVertexInfo::setDIFFUSE( BYTE *data , DWORD *color) { 
	DWORD* d = (DWORD*)this->getField( data, VFT_DIFFUSE ); 
	if( d == null ) return ;

	*d = *color;
}

void a3dVertexInfo::setSPECULAR( BYTE *data, DWORD *color ) { 
	DWORD* d = (DWORD*)this->getField( data, VFT_DIFFUSE ); 
	if( d == null ) return ;

	*d = *color;
}

void a3dVertexInfo::setTex0( BYTE *data, float *t ) { 
	float* f = (float*)this->getField( data, VFT_TEX0 ); 
	if( f == null ) return ;

	*f = *t;
	if( this->texCoorSize[0] >= 8 ) f[1] = t[1];
	if( this->texCoorSize[0] >= 12 ) f[2] = t[2];
	if( this->texCoorSize[0] >= 16 ) f[2] = t[2];
}

void a3dVertexInfo::setTex1( BYTE *data, float*t ) { 
	float* f = (float*)this->getField( data, VFT_TEX1 ); 
	if( f == null ) return ;

	*f = *t;
	if( this->texCoorSize[1] >= 8 ) f[1] = t[1];
	if( this->texCoorSize[1] >= 12 ) f[2] = t[2];
	if( this->texCoorSize[1] >= 16 ) f[2] = t[2];
}

void a3dVertexInfo::setTex2( BYTE *data, float*t ) { 
	float* f = (float*)this->getField( data, VFT_TEX2 ); 
	if( f == null ) return ;

	*f = *t;
	if( this->texCoorSize[2] >= 8 ) f[1] = t[1];
	if( this->texCoorSize[2] >= 12 ) f[2] = t[2];
	if( this->texCoorSize[2] >= 16 ) f[2] = t[2];
}

void a3dVertexInfo::setTex3( BYTE *data, float *t ) { 
	float* f = (float*)this->getField( data, VFT_TEX3 ); 
	if( f == null ) return ;

	*f = *t;
	if( this->texCoorSize[3] >= 8 ) f[1] = t[1];
	if( this->texCoorSize[3] >= 12 ) f[2] = t[2];
	if( this->texCoorSize[3] >= 16 ) f[2] = t[2];
}

void a3dVertexInfo::setTex4( BYTE *data, float *t ) { 
	float* f = (float*)this->getField( data, VFT_TEX4 ); 
	if( f == null ) return ;

	*f = *t;
	if( this->texCoorSize[4] >= 8 ) f[1] = t[1];
	if( this->texCoorSize[4] >= 12 ) f[2] = t[2];
	if( this->texCoorSize[4] >= 16 ) f[2] = t[2];
}

void a3dVertexInfo::setTex5( BYTE *data, float *t ) { 
	float* f = (float*)this->getField( data, VFT_TEX5 ); 
	if( f == null ) return ;

	*f = *t;
	if( this->texCoorSize[5] >= 8 ) f[1] = t[1];
	if( this->texCoorSize[5] >= 12 ) f[2] = t[2];
	if( this->texCoorSize[5] >= 16 ) f[2] = t[2];
}

void a3dVertexInfo::setTex6( BYTE *data, float *t ) { 
	float* f = (float*)this->getField( data, VFT_TEX6 ); 
	if( f == null ) return ;

	*f = *t;
	if( this->texCoorSize[6] >= 8 ) f[1] = t[1];
	if( this->texCoorSize[6] >= 12 ) f[2] = t[2];
	if( this->texCoorSize[6] >= 16 ) f[2] = t[2];
}

void a3dVertexInfo::setTex7( BYTE *data, float *t ) { 
	float* f = (float*)this->getField( data, VFT_TEX7 ); 
	if( f == null ) return ;

	*f = *t;
	if( this->texCoorSize[7] >= 8 ) f[1] = t[1];
	if( this->texCoorSize[7] >= 12 ) f[2] = t[2];
	if( this->texCoorSize[7] >= 16 ) f[2] = t[2];
}

void a3dVertexInfo::setTex8( BYTE *data, float *t ) { 
	float* f = (float*)this->getField( data, VFT_TEX8 ); 
	if( f == null ) return ;

	*f = *t;
	if( this->texCoorSize[8] >= 8 ) f[1] = t[1];
	if( this->texCoorSize[8] >= 12 ) f[2] = t[2];
	if( this->texCoorSize[8] >= 16 ) f[2] = t[2];
}

void a3dVertexInfo::initialize( shString &initString ) { 
	
	DWORD fvf = 0;
	
	// убиваем пробелы
	initString.removeSymbol( ' ' );
	
	// просто тупо инициализируем
	unsigned long i, bi;
	TCHAR buffer[512], symb;

	for( i = 0, bi = 0; i < initString.getLength(); i ++ ) { 
		symb = initString.getCString()[ i ];
		if( symb == '|' || symb == ';' || symb == ',' ) { 
			buffer[bi] = 0;
			
			fvf |= this->convertFVF( shString( buffer ) );
			
			bi = 0;
			continue;
		}
		buffer[bi] = symb;
		bi ++;
	}

	// последний
	buffer[bi] = 0;
	fvf |= this->convertFVF( shString( buffer ) );

	this->initialize( fvf );
}

DWORD a3dVertexInfo::convertFVF( shString &initString ) {
	
	int arraySize = sizeof(fvfArray) / sizeof(DWORD);
	int i;
	for( i = 0; i < arraySize; i ++ ) { 
		if( fvfString[ i ] == initString ) return fvfArray[ i ];
	}
	
	// проверка на разрядность текстур
	int texCoorSizeCount = 4 * 8;
	for( i = 0; i < texCoorSizeCount; i ++ ) { 
		if( texCoorString[i] == initString ) return texCoorSizeArray[i];
	}
	
	return 0;
}

int a3dVertexInfo::calcSizeCore() { 
	
	return sizeof( int ) 
		+ sizeof( int )
		+ sizeof( int ) * 32
		+ sizeof( int ) * 32
		+ sizeof( DWORD )
		+ sizeof( int ) * 9
		+ sizeof( a3dsMapType ) * 9; 
}

bool a3dVertexInfo::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !file.writeInt( this->vertexSize ) ) return false;
	if( !file.writeInt( this->fieldCount ) ) return false;
	
	for( int i = 0; i < 32; i ++ )
		if( !file.writeInt( this->fieldOffset[i] ) ) return false;

	for( int i = 0; i < 32; i ++ )
		if( !file.writeInt( this->fieldType[i] ) ) return false;

	if( !file.writeInt( this->vertexFVF ) ) return false;
	
	for( int i = 0; i < 9; i++ )
		if( !file.writeInt( this->texCoorSize[i] ) ) return false;

	for( int i = 0; i < 9; i++ )
		if( !file.writeInt( this->texMappedType[i] ) ) return false;
 
	return true;
} 

bool a3dVertexInfo::loadFromMemoryCore( a3dMemoryFile &file ) {

	if( !file.readInt( &this->vertexSize ) ) return false;
	if( !file.readInt( &this->fieldCount ) ) return false;
	
	for( int i = 0; i < 32; i ++ )
		if( !file.readInt( &this->fieldOffset[i] ) ) return false;

	int val;
	for( int i = 0; i < 32; i ++ ) {
		if( !file.readInt( &val ) ) return false;
		this->fieldType[i] = (a3dVertexFieldType) val;
	}

	if( !file.readInt( (int*)&this->vertexFVF ) ) return false;
	
	for( int i = 0; i < 9; i++ )
		if( !file.readInt( &this->texCoorSize[i] ) ) return false;

	for( int i = 0; i < 9; i++ ) {
		if( !file.readInt( &val ) ) return false;
		this->texMappedType[i] = ( a3dsMapType ) val;
	}
	
	return true;
}