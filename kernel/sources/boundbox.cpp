#include "boundBox.h"
#include "memoryFile.h"


void a3dBoundBox::getVertex(Vector3 pos, Vector3 v) { 
	
	v[0] = basis[3][0] + basis[0][0] * pos[0] + basis[0][1] * pos[0] + basis[0][2] * pos[0];
	v[1] = basis[3][1] + basis[1][0] * pos[1] + basis[1][1] * pos[1] + basis[1][2] * pos[1];
	v[2] = basis[3][2] + basis[2][0] * pos[0] + basis[2][1] * pos[2] + basis[2][2] * pos[2];
}

void a3dBoundBox::getVertex0( Vector3 v ) { 
	
	Vector3 p = { a[0], a[1], b[2] };
	this->getVertex( p, v );
}

void a3dBoundBox::getVertex1( Vector3 v ) { 
	
	this->getVertex( a, v );
}

void a3dBoundBox::getVertex2( Vector3 v ) { 
	
	Vector3 p = { b[0], a[1], a[2] };
	this->getVertex( p, v );
}

void a3dBoundBox::getVertex3( Vector3 v ) { 
	
	Vector3 p = { b[0], a[1], b[2] };
	this->getVertex( p, v );
}

void a3dBoundBox::getVertex4( Vector3 v ) { 
	
	Vector3 p = { a[0], b[1], b[2] };
	this->getVertex( p, v );
}

void a3dBoundBox::getVertex5( Vector3 v ) { 
	
	Vector3 p = { a[0], b[1], a[2] };
	this->getVertex( p, v );
}

void a3dBoundBox::getVertex6( Vector3 v ) { 
	
	Vector3 p = { b[0], b[1], a[2] };
	this->getVertex( p, v );
}

void a3dBoundBox::getVertex7( Vector3 v ) { 
	
	this->getVertex( b, v );
}

void a3dBoundBox::updateBoundBox() { 
	
	this->getVertex0(this->vertex[0]);
	this->getVertex1(this->vertex[1]);
	this->getVertex2(this->vertex[2]);
	this->getVertex3(this->vertex[3]);
	this->getVertex4(this->vertex[4]);
	this->getVertex5(this->vertex[5]);
	this->getVertex6(this->vertex[6]);
	this->getVertex7(this->vertex[7]);

	invertMatrix4( this->basis, this->invertBasis );
}

void a3dBoundBox::calcBoundBox( int count, Vertex3DS *v ) { 

	setVector3( this->a, FLT_MAX, FLT_MIN, FLT_MIN );
	setVector3( this->b, FLT_MIN, FLT_MAX, FLT_MAX );

	int i;
	for( i = 0; i < count; i ++ ) {
		if( v[i].SCoor.x < a[0] ) a[0] = v[i].SCoor.x;
		if( v[i].SCoor.x > b[0] ) b[0] = v[i].SCoor.x;
		if( v[i].SCoor.y > a[1] ) a[1] = v[i].SCoor.y;
		if( v[i].SCoor.y < b[1] ) b[1] = v[i].SCoor.y;
		if( v[i].SCoor.z > a[2] ) a[2] = v[i].SCoor.z;
		if( v[i].SCoor.z < b[2] ) b[2] = v[i].SCoor.z;
	}
}

void a3dBoundBox::initializeVertices() { 
	
	this->getVertex0(this->vertex[0]);
	this->getVertex1(this->vertex[1]);
	this->getVertex2(this->vertex[2]);
	this->getVertex3(this->vertex[3]);
	this->getVertex4(this->vertex[4]);
	this->getVertex5(this->vertex[5]);
	this->getVertex6(this->vertex[6]);
	this->getVertex7(this->vertex[7]);
}

void a3dBoundBox::calcBoundBox( int count, Vector3 *v ) { 
	
	setVector3( this->a, FLT_MAX, FLT_MIN, FLT_MIN );
	setVector3( this->b, FLT_MIN, FLT_MAX, FLT_MAX );

	int i;
	for( i = 0; i < count; i ++ ) {
		if( v[i][0] < a[0] ) a[0] = v[i][0];
		if( v[i][0] > b[0] ) b[0] = v[i][0];
		if( v[i][1] > a[1] ) a[1] = v[i][1];
		if( v[i][1] < b[1] ) b[1] = v[i][1];
		if( v[i][2] > a[2] ) a[2] = v[i][2];
		if( v[i][2] < b[2] ) b[2] = v[i][2];
	}

}

void a3dBoundBox::calcBoundBox( int count, a3dBoundBox **box ) { 

	setVector3( this->a, FLT_MAX, FLT_MIN, FLT_MIN );
	setVector3( this->b, FLT_MIN, FLT_MAX, FLT_MAX );

	int i, j;
	for( i = 0; i < count; i ++ ) {
		box[i]->initializeVertices();
		for( j = 0; j < 8; j ++ ) {
			if( box[i]->vertex[j][0] < a[0] ) a[0] = box[i]->vertex[j][0];
			if( box[i]->vertex[j][0] > b[0] ) b[0] = box[i]->vertex[j][0];
			if( box[i]->vertex[j][1] > a[1] ) a[1] = box[i]->vertex[j][1];
			if( box[i]->vertex[j][1] < b[1] ) b[1] = box[i]->vertex[j][1];
			if( box[i]->vertex[j][2] > a[2] ) a[2] = box[i]->vertex[j][2];
			if( box[i]->vertex[j][3] < b[2] ) b[2] = box[i]->vertex[j][2];
		}
	}
}

void a3dBoundBox::calcBoundBox( int count, a3dBoundBox *box ) { 
	
	setVector3( this->a, FLT_MAX, FLT_MIN, FLT_MIN );
	setVector3( this->b, FLT_MIN, FLT_MAX, FLT_MAX );

	int i, j;
	for( i = 0; i < count; i ++ ) {
		for( j = 0; j < 8; j ++ ) {
			box[i].initializeVertices();
			if( box[i].vertex[j][0] < a[0] ) a[0] = box[i].vertex[j][0];
			if( box[i].vertex[j][0] > b[0] ) b[0] = box[i].vertex[j][0];
			if( box[i].vertex[j][1] > a[1] ) a[1] = box[i].vertex[j][1];
			if( box[i].vertex[j][1] < b[1] ) b[1] = box[i].vertex[j][1];
			if( box[i].vertex[j][2] > a[2] ) a[2] = box[i].vertex[j][2];
			if( box[i].vertex[j][3] < b[2] ) b[2] = box[i].vertex[j][2];
		}
	}
}

void a3dBoundBox::calcBoundBox( int count, BYTE *data, a3dVertexInfo *info ) { 
	
	setVector3( this->a, FLT_MAX, FLT_MIN, FLT_MIN );
	setVector3( this->b, FLT_MIN, FLT_MAX, FLT_MAX );

	if( !info->hasXYZ() ) return;

	float *v;
	int i;
	for( i = 0; i < count; i ++, data += info->getVertexSize() ) {
		v = info->getXYZ( data );
		if( v[0] < a[0] ) a[0] = v[0];
		if( v[0] > b[0] ) b[0] = v[0];
		if( v[1] > a[1] ) a[1] = v[1];
		if( v[1] < b[1] ) b[1] = v[1];
		if( v[2] > a[2] ) a[2] = v[2];
		if( v[2] < b[2] ) b[2] = v[2];
 	} 
}

void a3dBoundBox::prepareRenderBasis( Matrix4 mat ) { 

	Matrix4 tmp;
	copyMatrix4( this->basis, tmp );
	
	tmp[3][0] = this->a[0];
	tmp[3][1] = this->a[1];
	tmp[3][2] = this->b[2];
	
	tmp[0][0] *= this->getWidth();
	tmp[1][0] *= this->getWidth();
	tmp[2][0] *= this->getWidth();
	
	tmp[0][1] *= this->getHeight();
	tmp[1][1] *= this->getHeight();
	tmp[2][1] *= this->getHeight();

	tmp[0][2] *= this->getDepth();
	tmp[1][2] *= this->getDepth();
	tmp[2][2] *= this->getDepth();

	invertMatrix4( tmp, mat );
	return ;
}

int a3dBoundBox::calcSizeCore() { 

	//          basis           invertBasis          a                  b                     vertex
	return sizeof( Matrix4 ) + sizeof( Matrix4 ) + sizeof( Vector3 ) + sizeof( Vector3 ) + 8 * sizeof( Vector3 );
}

bool a3dBoundBox::loadFromMemoryCore( a3dMemoryFile &file ) { 
	
	if( !file.readMatrix4( this->basis ) ) return false;
	if( !file.readMatrix4( this->invertBasis ) ) return false;
	if( !file.readVector3( this->a ) ) return false;
	if( !file.readVector3( this->b ) ) return false;
	
	for( int i = 0; i < 8; i ++ )
		if( !file.readVector3( this->vertex[i] ) ) return false;

	return true;
}

bool a3dBoundBox::saveToMemoryCore( a3dMemoryFile &file ) { 
	
	if( !file.writeMatrix4( this->basis ) ) return false;
	if( !file.writeMatrix4( this->invertBasis ) ) return false;
	if( !file.writeVector3( this->a ) ) return false;
	if( !file.writeVector3( this->b ) ) return false;
	
	for( int i = 0; i < 8; i ++ )
		if( !file.writeVector3( this->vertex[i] ) ) return false;
	
	return true;
}