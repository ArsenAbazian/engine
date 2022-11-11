#ifndef A3D_VERTEX_INFO_INC
#define A3D_VERTEX_INFO_INC

#include <windows.h>
#include "kernel.h"
#include "vecmat.h"
#include "hstring.h"
#include "headerCodes.h"

typedef enum { 
	VFT_NONE,
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
} a3dVertexFieldType; 

typedef enum { 
	MAP_NONE = 0,
	MAP_TEXTURE1 = 1,
	MAP_BUMP = 2,
	MAP_OPACITY = 3,
	MAP_REFLECTION = 4
}a3dsMapType;

class a3dVertexInfo : public a3dSerializableObject { 
	int							vertexSize;				// размер одной вершины
	int							fieldCount;				// число полей
	int							fieldOffset[32];		// смещение поля в вершине
	a3dVertexFieldType			fieldType[32];			// тип поля
	DWORD						vertexFVF;				// флаги вершины
	int							texCoorSize[9];			// размерность координат текстуры 1
	a3dsMapType					texMappedType[9];		// маппирование на текстурные координаты
public:
	
	a3dVertexInfo() { this->clear(); }
	a3dVertexInfo( DWORD fvf ) { this->initialize( fvf ); }

	DWORD		convertFVF( shString &initString );
	void		initialize( shString &initString );
	void		initialize( DWORD fvf );
	void		initializeTexCoorSize();
	int			getFieldSize( a3dVertexFieldType field );
	void		calcFieldOffsets();
	void		decodeFVF();
	int			calcVertexSize();
	void		setMappedType(int texIndex, a3dsMapType mapType) { this->texMappedType[ texIndex ] = mapType; }
	int			getTexIndexByMapType( a3dsMapType mapType );
	void		copy( a3dVertexInfo *info ) { memcpy( this, info, sizeof( a3dVertexInfo ) ); }
	void		clear();

	BYTE*		getField( BYTE *data, a3dVertexFieldType f );
	bool		hasField( a3dVertexFieldType f );

	float*		getXYZ( BYTE *data );
	float*		getXYZRHW( BYTE *data );
	float*		getXYZB1( BYTE *data );
	float*		getXYZB2( BYTE *data );
	float*		getXYZB3( BYTE *data );
	float*		getXYZB4( BYTE *data );
	float*		getXYZB5( BYTE *data );
	float*		getNormal( BYTE *data );
	float*		getPSIZE( BYTE *data );
	DWORD*		getDIFFUSE( BYTE *data );
	DWORD*		getSPECULAR( BYTE *data );
	float*		getTex0( BYTE *data );
	float*		getTex1( BYTE *data );
	float*		getTex2( BYTE *data );
	float*		getTex3( BYTE *data );
	float*		getTex4( BYTE *data );
	float*		getTex5( BYTE *data );
	float*		getTex6( BYTE *data );
	float*		getTex7( BYTE *data );
	float*		getTex8( BYTE *data );

	void		setXYZ( BYTE *data, float* v);
	void		setXYZRHW( BYTE *data, float* v);
	void		setXYZB1( BYTE *data, float* b1 );
	void		setXYZB2( BYTE *data, float* b2 );
	void		setXYZB3( BYTE *data, float* b3 );
	void		setXYZB4( BYTE *data, float* b4 );
	void		setXYZB5( BYTE *data, float* b5 );
	void		setNormal( BYTE *data, float* n );
	void		setPSIZE( BYTE *data, float* psize );
	void		setDIFFUSE( BYTE *data, DWORD* color );
	void		setSPECULAR( BYTE *data, DWORD* color );
	void		setTex0( BYTE *data, float* );
	void		setTex1( BYTE *data, float* );
	void		setTex2( BYTE *data, float* );
	void		setTex3( BYTE *data, float* );
	void		setTex4( BYTE *data, float* );
	void		setTex5( BYTE *data, float* );
	void		setTex6( BYTE *data, float* );
	void		setTex7( BYTE *data, float* );
	void		setTex8( BYTE *data, float* );

	int			getVertexSize() { return this->vertexSize; }
	DWORD		getFVF() { return this->vertexFVF; }
	
	bool		hasXYZ() { return this->hasField( VFT_XYZ); }
	bool		hasNormal() { return this->hasField( VFT_NORMAL); }
	bool		hasTexCoord0() { return this->hasField( VFT_TEX0 ); }
	bool		hasTexCoord1() { return this->hasField( VFT_TEX1 ); }
	bool		hasTexCoord2() { return this->hasField( VFT_TEX2 ); }
	bool		hasTexCoord3() { return this->hasField( VFT_TEX3 ); }
	bool		hasTexCoord4() { return this->hasField( VFT_TEX4 ); }
	bool		hasTexCoord5() { return this->hasField( VFT_TEX5 ); }
	bool		hasTexCoord6() { return this->hasField( VFT_TEX6 ); }
	bool		hasTexCoord7() { return this->hasField( VFT_TEX7 ); }
	bool		hasTexCoord8() { return this->hasField( VFT_TEX8 ); }
	int			getTexCoordSize( int index ) { return this->texCoorSize[index]; }

	float*		getTex( BYTE *data, int texIndex );
	void		setTex( BYTE *data, int texIndex, float *t );

	int			getHeaderCode() { return a3dHeaders::a3dVertexInfoHeader; }
	int 		calcSizeCore();
	bool		saveToMemoryCore( a3dMemoryFile &file );
	bool		loadFromMemoryCore( a3dMemoryFile &file );
};

#endif