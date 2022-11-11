#include "a3dMathObject.h"
#include "memoryFile.h"

int a3dMathObject::calcSizeCore() { 

	return sizeof( Matrix4 ) + this->bVolume.calcSize() + sizeof( float ) * 3;
}

bool a3dMathObject::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !file.writeMatrix4( this->basis ) ) return false;
	if( !this->bVolume.saveToMemory( file ) ) return false;
	if( !file.writeFloat( this->scaleX ) ) return false;
	if( !file.writeFloat( this->scaleY ) ) return false;
	if( !file.writeFloat( this->scaleZ ) ) return false;

	return true;
}

bool a3dMathObject::loadFromMemoryCore( a3dMemoryFile &file ) { 

	if( !file.readMatrix4( this->basis ) ) return false;
	if( !this->bVolume.loadFromMemory( file ) ) return false;
	if( !file.readFloat( &this->scaleX ) ) return false;
	if( !file.readFloat( &this->scaleY ) ) return false;
	if( !file.readFloat( &this->scaleZ ) ) return false;

	return true;
}

void a3dMathObject::position(Vector3 v) { 

	this->basis[3][0] = v[0];
	this->basis[3][1] = v[1];
	this->basis[3][2] = v[2];
}

void a3dMathObject::position(float x, float y, float z) { 

	this->basis[3][0] = x;
	this->basis[3][1] = y;
	this->basis[3][2] = z;
}

void a3dMathObject::moveX(float f){

	this->basis[3][0] += this->basis[0][0] * f;
	this->basis[3][1] += this->basis[1][0] * f;
	this->basis[3][2] += this->basis[2][0] * f;

}

void a3dMathObject::moveY(float f){

	this->basis[3][0] += this->basis[0][1] * f;
	this->basis[3][1] += this->basis[1][1] * f;
	this->basis[3][2] += this->basis[2][1] * f;

}

void a3dMathObject::moveZ(float f){

	this->basis[3][0] += this->basis[0][2] * f;
	this->basis[3][1] += this->basis[1][2] * f;
	this->basis[3][2] += this->basis[2][2] * f;

}

void a3dMathObject::moveV(Vector3 v){

	this->basis[3][0] += v[0];
	this->basis[3][1] += v[1];
	this->basis[3][2] += v[2];

}

void a3dMathObject::moveV(float x,float y,float z){

	this->basis[3][0] += x;
	this->basis[3][1] += y;
	this->basis[3][2] += z;

}

void a3dMathObject::rotateAroundX(float angle){

	Vector3 nz;
	float ca,sa;

	ca = cos(angle);
	sa = sin(angle);

	nz[0] = this->basis[0][2] * ca + this->basis[0][1] * sa;
	nz[1] = this->basis[1][2] * ca + this->basis[1][1] * sa;
	nz[2] = this->basis[2][2] * ca + this->basis[2][1] * sa;

	this->basis[0][1] = this->basis[0][1] * ca - this->basis[0][2] * sa;
	this->basis[1][1] = this->basis[1][1] * ca - this->basis[1][2] * sa;
	this->basis[2][1] = this->basis[2][1] * ca - this->basis[2][2] * sa;

	this->basis[0][2] = nz[0];
	this->basis[1][2] = nz[1];
	this->basis[2][2] = nz[2];

}

void a3dMathObject::rotateAroundY(float angle){

	Vector3 nx;
	float ca,sa;

	ca = cos(angle);
	sa = sin(angle);

	nx[0] = this->basis[0][0] * ca + this->basis[0][2] * sa;
	nx[1] = this->basis[1][0] * ca + this->basis[1][2] * sa;
	nx[2] = this->basis[2][0] * ca + this->basis[2][2] * sa;

	this->basis[0][2] = this->basis[0][2] * ca - this->basis[0][0] * sa;
	this->basis[1][2] = this->basis[1][2] * ca - this->basis[1][0] * sa;
	this->basis[2][2] = this->basis[2][2] * ca - this->basis[2][0] * sa;

	this->basis[0][0] = nx[0];
	this->basis[1][0] = nx[1];
	this->basis[2][0] = nx[2];

}

void a3dMathObject::rotateAroundZ(float angle){

	Vector3 nx;
	float ca,sa;

	ca = cos(angle);
	sa = sin(angle);

	nx[0] = this->basis[0][0] * ca + this->basis[0][1] * sa;
	nx[1] = this->basis[1][0] * ca + this->basis[1][1] * sa;
	nx[2] = this->basis[2][0] * ca + this->basis[2][1] * sa;

	this->basis[0][1] = this->basis[0][1] * ca - this->basis[0][0] * sa;
	this->basis[1][1] = this->basis[1][1] * ca - this->basis[1][0] * sa;
	this->basis[2][1] = this->basis[2][1] * ca - this->basis[2][0] * sa;

	this->basis[0][0] = nx[0];
	this->basis[1][0] = nx[1];
	this->basis[2][0] = nx[2];

}

void a3dMathObject::rotateAroundV(Vector3 v,float angle){

	Vector3 tmp[3],dir;
	float ca,sa;

	ca = cos(angle);
	sa = sin(angle);

	normalizeVector( v, dir );

	tmp[0][0] = this->basis[0][0];
	tmp[0][1] = this->basis[1][0];
	tmp[0][2] = this->basis[2][0];

	tmp[1][0] = this->basis[0][1];
	tmp[1][1] = this->basis[1][1];
	tmp[1][2] = this->basis[2][1];

	tmp[2][0] = this->basis[0][2];
	tmp[2][1] = this->basis[1][2];
	tmp[2][2] = this->basis[2][2];

	rotateVectorsArrayAroundNormVector( tmp, 3, dir, tmp, ca, sa );

	this->basis[0][0] = tmp[0][0];
	this->basis[1][0] = tmp[0][1];
	this->basis[2][0] = tmp[0][2];

	this->basis[0][1] = tmp[1][0];
	this->basis[1][1] = tmp[1][1];
	this->basis[2][1] = tmp[1][2];

	this->basis[0][2] = tmp[2][0];
	this->basis[1][2] = tmp[2][1];
	this->basis[2][2] = tmp[2][2];
}

void a3dMathObject::rotateAroundA(Vector3 p,Vector3 a,float angle){

	Vector3 tmp[4],dir;
	float ca,sa;

	ca = cos(angle);
	sa = sin(angle);

	normalizeVector( a, dir );

	tmp[0][0] = this->basis[0][0];
	tmp[0][1] = this->basis[1][0];
	tmp[0][2] = this->basis[2][0];

	tmp[1][0] = this->basis[0][1];
	tmp[1][1] = this->basis[1][1];
	tmp[1][2] = this->basis[2][1];

	tmp[2][0] = this->basis[0][2];
	tmp[2][1] = this->basis[1][2];
	tmp[2][2] = this->basis[2][2];

	tmp[3][0] = this->basis[3][0];
	tmp[3][1] = this->basis[3][1];
	tmp[3][2] = this->basis[3][2];

	addVector3(tmp[0],tmp[3],tmp[0]);
	addVector3(tmp[1],tmp[3],tmp[1]);
	addVector3(tmp[2],tmp[3],tmp[2]);

	rotateVectorsArrayAroundNormAxis( tmp, 4, dir, p, tmp, ca, sa );

	subVector3( tmp[0], tmp[3], tmp[0]);
	subVector3( tmp[1], tmp[3], tmp[1]);
	subVector3( tmp[2], tmp[3], tmp[2]);

	this->basis[0][0] = tmp[0][0];
	this->basis[1][0] = tmp[0][1];
	this->basis[2][0] = tmp[0][2];

	this->basis[0][1] = tmp[1][0];
	this->basis[1][1] = tmp[1][1];
	this->basis[2][1] = tmp[1][2];

	this->basis[0][2] = tmp[2][0];
	this->basis[1][2] = tmp[2][1];
	this->basis[2][2] = tmp[2][2];

	this->basis[3][0] = tmp[3][0];
	this->basis[3][1] = tmp[3][1];
	this->basis[3][2] = tmp[3][2];
}

void a3dMathObject::direction( Vector3 newZDir, Vector3 newYDir ) {

	calcDirectionMatrix( this->basis, newZDir, newYDir );
	return ;
}

void a3dMathObject::scale( float fx, float fy, float fz ) {

	float inv_scx, inv_scy, inv_scz;

	inv_scx = fx / this->scaleX;
	inv_scy = fy / this->scaleY;
	inv_scz = fz / this->scaleZ;

	this->scaleX = fx;
	this->scaleY = fy;
	this->scaleZ = fz;

	this->basis[0][0] *= inv_scx;
	this->basis[1][0] *= inv_scx;
	this->basis[2][0] *= inv_scx;

	this->basis[0][1] *= inv_scy;
	this->basis[1][1] *= inv_scy;
	this->basis[2][1] *= inv_scy;

	this->basis[0][2] *= inv_scz;
	this->basis[1][2] *= inv_scz;
	this->basis[2][2] *= inv_scz;
}

void a3dMathObject::origin(){

	this->scaleX = this->scaleY = this->scaleZ = 1.0f;

	this->basis[0][0] = this->basis[1][1] = this->basis[2][2] = this->basis[3][3] = 1.0f;
	this->basis[0][1] = this->basis[0][2] = this->basis[0][3] = 0.0f;
	this->basis[1][0] = this->basis[1][2] = this->basis[1][3] = 0.0f;
	this->basis[2][0] = this->basis[2][1] = this->basis[2][3] = 0.0f;
	this->basis[3][0] = this->basis[3][1] = this->basis[3][2] = 0.0f;
	return ;
}

void a3dMathObject::correct( Matrix4 mbas ) {

	Matrix3	bas, fbas;	
	Matrix4 rbas;
	Vector3	dist,tmp;
	int	i, j;

	dist[0] = this->basis[3][0] - mbas[3][0];
	dist[1] = this->basis[3][1] - mbas[3][1];
	dist[2] = this->basis[3][2] - mbas[3][2];

	tmp[0] = mbas[0][0] * dist[0];
	tmp[1] = mbas[1][0] * dist[0];
	tmp[2] = mbas[2][0] * dist[0];

	tmp[0] += mbas[0][1] * dist[1];
	tmp[1] += mbas[1][1] * dist[1];
	tmp[2] += mbas[2][1] * dist[1];

	tmp[0] += mbas[0][2] * dist[2];
	tmp[1] += mbas[1][2] * dist[2];
	tmp[2] += mbas[2][2] * dist[2];

	this->basis[3][0] = mbas[3][0] + tmp[0];
	this->basis[3][1] = mbas[3][1] + tmp[1];
	this->basis[3][2] = mbas[3][2] + tmp[2];

	tmp[0] = mbas[0][0] * this->basis[0][0];
	tmp[1] = mbas[1][0] * this->basis[0][0];
	tmp[2] = mbas[2][0] * this->basis[0][0];

	tmp[0] += mbas[0][1] * this->basis[1][0];
	tmp[1] += mbas[1][1] * this->basis[1][0];
	tmp[2] += mbas[2][1] * this->basis[1][0];

	tmp[0] += mbas[0][2] * this->basis[2][0];
	tmp[1] += mbas[1][2] * this->basis[2][0];
	tmp[2] += mbas[2][2] * this->basis[2][0];

	rbas[0][0] = tmp[0];
	rbas[1][0] = tmp[1];
	rbas[2][0] = tmp[2];

	tmp[0] = mbas[0][0] * this->basis[0][1];
	tmp[1] = mbas[1][0] * this->basis[0][1];
	tmp[2] = mbas[2][0] * this->basis[0][1];

	tmp[0] += mbas[0][1] * this->basis[1][1];
	tmp[1] += mbas[1][1] * this->basis[1][1];
	tmp[2] += mbas[2][1] * this->basis[1][1];

	tmp[0] += mbas[0][2] * this->basis[2][1];
	tmp[1] += mbas[1][2] * this->basis[2][1];
	tmp[2] += mbas[2][2] * this->basis[2][1];

	rbas[0][1] = tmp[0];
	rbas[1][1] = tmp[1];
	rbas[2][1] = tmp[2];

	tmp[0] = mbas[0][0] * this->basis[0][2];
	tmp[1] = mbas[1][0] * this->basis[0][2];
	tmp[2] = mbas[2][0] * this->basis[0][2];

	tmp[0] += mbas[0][1] * this->basis[1][2];
	tmp[1] += mbas[1][1] * this->basis[1][2];
	tmp[2] += mbas[2][1] * this->basis[1][2];

	tmp[0] += mbas[0][2] * this->basis[2][2];
	tmp[1] += mbas[1][2] * this->basis[2][2];
	tmp[2] += mbas[2][2] * this->basis[2][2];

	rbas[0][2] = tmp[0];
	rbas[1][2] = tmp[1];
	rbas[2][2] = tmp[2];

	//MulMatrix4(this->basis,mbas,rbas);

	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			bas[i][j] = rbas[i][j];

	invertMatrix3(bas,fbas);

	for( i = 0; i < 3; i++ )
		for( j = 0; j < 3; j++ )
			invertBasis[i][j] = fbas[i][j];

	invertBasis[3][0] = this->basis[3][0];
	invertBasis[3][1] = this->basis[3][1];
	invertBasis[3][2] = this->basis[3][2];	
}

void a3dMathObject::correct() {

	Matrix3	bas, fbas;	
	int  i, j;

	float inv_scx, inv_scy, inv_scz;

	for( i = 0; i < 3; i++ )
		for( j = 0; j <3 ; j++ )
			bas[i][j] = this->basis[i][j];

	inv_scx = 1.0f / ( this->scaleX * this->scaleX );
	inv_scy = 1.0f / ( this->scaleY * this->scaleY );	
	inv_scz = 1.0f / ( this->scaleZ * this->scaleZ );

	bas[0][0] *= inv_scx;
	bas[1][0] *= inv_scx;
	bas[2][0] *= inv_scx;	

	bas[0][1] *= inv_scy;
	bas[1][1] *= inv_scy;
	bas[2][1] *= inv_scy;	

	bas[0][2] *= inv_scz;
	bas[1][2] *= inv_scz;
	bas[2][2] *= inv_scz;	

	invertMatrix3( bas, fbas );

	for( i = 0; i < 3; i++ )
		for( j = 0; j < 3; j++ )
			this->invertBasis[i][j] = fbas[i][j];

	this->invertBasis[3][0] = this->basis[3][0];
	this->invertBasis[3][1] = this->basis[3][1];
	this->invertBasis[3][2] = this->basis[3][2];		

}

void a3dMathObject::calcBVolume() { }

void a3dMathObject::setBasis3( Matrix4 newbas ) {

	for(int i = 0; i <= 3; i++ ) {
		for( int j = 0; j <= 3; j++) {
			this->basis[i][j] = newbas[i][j];
		}
	}
}

void a3dMathObject::initializeDefault() { 

	originMatrix4( this->basis );
	originMatrix4( this->invertBasis );

	this->scaleX = this->scaleY = this->scaleZ = 1.0f;
}

void a3dMathObject::clear() { 

	this->initializeDefault();
}