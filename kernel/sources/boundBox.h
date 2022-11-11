#ifndef A3D_BOUNDBOX_INC
#define A3D_BOUNDBOX_INC

#include "vecmat.h"
#include "vertexInfo.h"
#include "n3dsfile.h"
#include "headerCodes.h"

//   a
//
//
//	     b

class a3dBoundBox : public a3dSerializableObject{ 

	static const int header = (((int)'b') << 24) | (((int)'b') << 16) | (((int)'3') << 8) | ('a');
public:
	Matrix4		basis;
	Matrix4		invertBasis;
	Vector3		a;
	Vector3		b;
	
	Vector3		vertex[8];			// вершины куба	

	a3dBoundBox() { this->clear(); }
	~a3dBoundBox() { this->clear(); }

	void	clear() { 
		originMatrix4( this->basis );
		originMatrix4( this->invertBasis );
		setVector3( this->a, 0.0f, 0.0f, 0.0f );
		setVector3( this->b, 0.0f, 0.0f, 0.0f );  
	}
	
	void getVertex( Vector3 pos, Vector3 v );
	void getVertex0( Vector3 v );
	void getVertex1( Vector3 v );
	void getVertex2( Vector3 v );
	void getVertex3( Vector3 v );
	void getVertex4( Vector3 v );
	void getVertex5( Vector3 v );
	void getVertex6( Vector3 v );
	void getVertex7( Vector3 v );

	void calcBoundBox( int count, BYTE *data, a3dVertexInfo *info );
	void calcBoundBox( int count, Vertex3DS *v );
	void calcBoundBox( int count, Vector3 *v );
	void calcBoundBox( int count, a3dBoundBox *b );
	void calcBoundBox( int count, a3dBoundBox **b );
	void updateBoundBox();
	void copyWithoutBasis( a3dBoundBox &src ) {
		copyVector3(src.a, this->a); 
		copyVector3(src.b, this->b); 
		updateBoundBox();
	}
	void copy( a3dBoundBox &src ) { 
		copyVector3(src.a, this->a); 
		copyVector3(src.b, this->b); 
		copyMatrix4(src.basis, this->basis);
		copyMatrix4(src.invertBasis, this->invertBasis);
		updateBoundBox();
	}
	float getHeight() { return this->a[1] - this->b[1]; }
	float getWidth() { return this->b[0] - this->a[0]; }
	float getDepth() { return this->a[2] - this->b[2]; }

	void  prepareRenderBasis( Matrix4 mat );
	void  initializeVertices();

	int		getHeaderCode() { return a3dHeaders::a3dBoundBoxHeader; } 
	int		calcSizeCore();
	bool	saveToMemoryCore( a3dMemoryFile &file );
	bool	loadFromMemoryCore( a3dMemoryFile &file );
};

#endif