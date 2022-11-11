#include "vecmat.h"

unsigned short checkWord14;
unsigned short checkWord14_1;

float __a3d_fastcall slownormalizeVector( Vector3 v )
{
    
	float mod,	imod;
    
	mod		=  v[0] * v[0];
    mod		+= v[1] * v[1];
    mod		+= v[2] * v[2];
    
	mod		=sqrt(mod);
    
	if( mod )
	{
        imod	= 1 / mod;
        v[0]	*= imod;
        v[1]	*= imod;
        v[2]	*= imod;
    }
    
	return mod;
}

float __a3d_fastcall slownormalizeVector( Vector3 src, Vector3 dst )
{
    copyVector3( src, dst );
	
	return slownormalizeVector( dst );
}

float __a3d_fastcall quicknormalizeVector( Vector3 v )
{
	float L_squared, one_over_L;
	
	L_squared = ( v[0] * v[0] ) + ( v[1] * v[1] ) + ( v[2] * v[2] );
	one_over_L = invSqrt( L_squared );
  
	v[0] *= one_over_L;
	v[1] *= one_over_L;
	v[2] *= one_over_L;

	return 1.0f / one_over_L;
}

float __a3d_fastcall quicknormalizeVectorAndRetSquareDist( Vector3 v )
{
	float L_squared, one_over_L;

	L_squared = ( v[0] * v[0] ) + ( v[1] * v[1] ) + ( v[2] * v[2] );
	one_over_L = invSqrt( L_squared );

	v[0] *= one_over_L;
	v[1] *= one_over_L;
	v[2] *= one_over_L;

	return L_squared;
}

float __a3d_fastcall quicknormalizeVector( Vector3 src, Vector3 dst )
{
	copyVector3( src, dst );
	
	return quicknormalizeVector( dst );
}

void originMatrix3( Matrix3 v )
{
	v[0][0] = v[1][1] = v[2][2] = 1.0f;
	v[0][1] = v[0][2] = v[1][0] = v[1][2] = v[2][0] = v[2][1] = 0.0f;

	return ;
}

void copyMatrix3( Matrix3 src, Matrix3 dst )
{

	dst[0][0]	= src[0][0];
    dst[0][1]	= src[0][1];
    dst[0][2]	= src[0][2];
    dst[1][0]	= src[1][0];
    dst[1][1]	= src[1][1];
    dst[1][2]	= src[1][2];
    dst[2][0]	= src[2][0];
    dst[2][1]	= src[2][1];
    dst[2][2]	= src[2][2];

	return ;
}

void addMatrix3( Matrix3 a, Matrix3 b, Matrix3 c )
{

	c[0][0]		= a[0][0] + b[0][0];
    c[0][1]		= a[0][1] + b[0][1];
    c[0][2]		= a[0][2] + b[0][2];
    c[1][0]		= a[1][0] + b[1][0];
    c[1][1]		= a[1][1] + b[1][1];
    c[1][2]		= a[1][2] + b[1][2];
    c[2][0]		= a[2][0] + b[2][0];
    c[2][1]		= a[2][1] + b[2][1];
    c[2][2]		= a[2][2] + b[2][2];

	return ;
}

void subMatrix3( Matrix3 a, Matrix3 b, Matrix3 c )
{

	c[0][0]		= a[0][0] - b[0][0];
    c[0][1]		= a[0][1] - b[0][1];
    c[0][2]		= a[0][2] - b[0][2];
    c[1][0]		= a[1][0] - b[1][0];
    c[1][1]		= a[1][1] - b[1][1];
    c[1][2]		= a[1][2] - b[1][2];
    c[2][0]		= a[2][0] - b[2][0];
    c[2][1]		= a[2][1] - b[2][1];
    c[2][2]		= a[2][2] - b[2][2];

	return ;
}

void mulMatrix3( Matrix3 a, Matrix3 b, Matrix3 c )
{
    c[0][0]		= a[0][0] * b[0][0] + a[0][1] * b[1][0] + a[0][2] * b[2][0];
    c[0][1]		= a[0][0] * b[0][1] + a[0][1] * b[1][1] + a[0][2] * b[2][1];
    c[0][2]		= a[0][0] * b[0][2] + a[0][1] * b[1][2] + a[0][2] * b[2][2];
    c[1][0]		= a[1][0] * b[0][0] + a[1][1] * b[1][0] + a[1][2] * b[2][0];
    c[1][1]		= a[1][0] * b[0][1] + a[1][1] * b[1][1] + a[1][2] * b[2][1];
    c[1][2]		= a[1][0] * b[0][2] + a[1][1] * b[1][2] + a[1][2] * b[2][2];
    c[2][0]		= a[2][0] * b[0][0] + a[2][1] * b[1][0] + a[2][2] * b[2][0];
    c[2][1]		= a[2][0] * b[0][1] + a[2][1] * b[1][1] + a[2][2] * b[2][1];
    c[2][2]		= a[2][0] * b[0][2] + a[2][1] * b[1][2] + a[2][2] * b[2][2];

	return ;
}

void scaleMatrix3( Matrix3 mat, float sc )
{
	mat[0][0]	*= sc;
	mat[0][1]	*= sc;
	mat[0][2]	*= sc;
	mat[1][0]	*= sc;
	mat[1][1]	*= sc;
	mat[1][2]	*= sc;
	mat[2][0]	*= sc;
	mat[2][1]	*= sc;
	mat[2][2]	*= sc;

	return ;
}

void transMatrix3( Matrix3 mat, Matrix3 tmat )
{

	tmat[0][0]	= mat[0][0];
	tmat[1][1]	= mat[1][1];
	tmat[2][2]	= mat[2][2];
	tmat[1][0]	= mat[0][1];
	tmat[0][1]	= mat[1][0];
	tmat[0][2]	= mat[2][0];
	tmat[2][0]	= mat[0][2];
	tmat[1][2]	= mat[2][1];
	tmat[2][1]	= mat[1][2];
	
	return ;
}

void additionMatrix4( Matrix4 mat, Matrix4 amat ) 
{
	// 00 01 02 03
	// 10 11 12 13
	// 20 21 22 23
	// 30 31 32 33

	amat[0][0] = mat[1][1] * ( mat[2][2] * mat[3][3] - mat[3][2] * mat[2][3] ) -  mat[1][2] * ( mat[2][1] * mat[3][3] - mat[3][1] * mat[2][3] ) +  mat[1][3] * ( mat[2][1] * mat[3][2] - mat[3][1] * mat[2][2] );
	amat[0][1] = - ( mat[1][0] * ( mat[2][2] * mat[3][3] - mat[3][2] * mat[2][3] ) -  mat[1][2] * ( mat[2][0] * mat[3][3] - mat[3][0] * mat[2][3] ) +  mat[1][3] * ( mat[2][0] * mat[3][2] - mat[3][0] * mat[2][2] ) );
	amat[0][2] = mat[1][0] * ( mat[2][1] * mat[3][3] - mat[3][1] * mat[2][3] ) -  mat[1][1] * ( mat[2][0] * mat[3][3] - mat[3][0] * mat[2][3] ) +  mat[1][3] * ( mat[2][0] * mat[3][1] - mat[3][0] * mat[2][1] );
	amat[0][3] = - ( mat[1][0] * ( mat[2][1] * mat[3][2] - mat[3][1] * mat[2][2] ) -  mat[1][1] * ( mat[2][0] * mat[3][2] - mat[3][0] * mat[2][2] ) +  mat[1][2] * ( mat[2][0] * mat[3][1] - mat[3][0] * mat[2][1] ) );
	
	amat[1][0] = - ( mat[0][1] * ( mat[2][2] * mat[3][3] - mat[3][2] * mat[2][3] ) -  mat[0][2] * ( mat[2][1] * mat[3][3] - mat[3][1] * mat[2][3] ) +  mat[0][3] * ( mat[2][1] * mat[3][2] - mat[3][1] * mat[2][2] ) );
	amat[1][1] = mat[0][0] * ( mat[2][2] * mat[3][3] - mat[3][2] * mat[2][3] ) -  mat[0][2] * ( mat[2][0] * mat[3][3] - mat[3][0] * mat[2][3] ) +  mat[0][3] * ( mat[2][0] * mat[3][2] - mat[3][0] * mat[2][2] );
	amat[1][2] = - ( mat[0][0] * ( mat[2][1] * mat[3][3] - mat[3][1] * mat[2][3] ) -  mat[0][1] * ( mat[2][0] * mat[3][3] - mat[3][0] * mat[2][3] ) +  mat[0][3] * ( mat[2][0] * mat[3][1] - mat[3][0] * mat[2][1] ) );
	amat[1][3] = mat[0][0] * ( mat[2][1] * mat[3][2] - mat[3][1] * mat[2][2] ) -  mat[0][1] * ( mat[2][0] * mat[3][2] - mat[3][0] * mat[2][2] ) +  mat[0][2] * ( mat[2][0] * mat[3][1] - mat[3][0] * mat[2][1] );
	
	amat[2][0] = mat[0][1] * ( mat[1][2] * mat[3][3] - mat[3][2] * mat[1][3] ) -  mat[0][2] * ( mat[1][1] * mat[3][3] - mat[3][1] * mat[1][3] ) +  mat[0][3] * ( mat[1][1] * mat[3][2] - mat[3][1] * mat[1][2] );
	amat[2][1] = - ( mat[0][0] * ( mat[1][2] * mat[3][3] - mat[3][2] * mat[1][3] ) -  mat[0][2] * ( mat[1][0] * mat[3][3] - mat[3][0] * mat[1][3] ) +  mat[0][3] * ( mat[1][0] * mat[3][2] - mat[3][0] * mat[1][2] ) );
	amat[2][2] = mat[0][0] * ( mat[1][1] * mat[3][3] - mat[3][1] * mat[1][3] ) -  mat[0][1] * ( mat[1][0] * mat[3][3] - mat[3][0] * mat[1][3] ) +  mat[0][3] * ( mat[1][0] * mat[3][1] - mat[3][0] * mat[1][1] );
	amat[2][3] = - ( mat[0][0] * ( mat[1][1] * mat[3][2] - mat[3][1] * mat[1][2] ) -  mat[0][1] * ( mat[1][0] * mat[3][2] - mat[3][0] * mat[1][2] ) +  mat[0][2] * ( mat[1][0] * mat[3][1] - mat[3][0] * mat[1][1] ) );
	
	amat[3][0] = - ( mat[0][1] * ( mat[1][2] * mat[2][3] - mat[2][2] * mat[1][3] ) -  mat[0][2] * ( mat[1][1] * mat[2][3] - mat[2][1] * mat[1][3] ) +  mat[0][3] * ( mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2] ) );
	amat[3][1] = mat[0][0] * ( mat[1][2] * mat[2][3] - mat[2][2] * mat[1][3] ) -  mat[0][2] * ( mat[1][0] * mat[2][3] - mat[2][0] * mat[1][3] ) +  mat[0][3] * ( mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2] );
	amat[3][2] = - ( mat[0][0] * ( mat[1][1] * mat[2][3] - mat[2][1] * mat[1][3] ) -  mat[0][1] * ( mat[1][0] * mat[2][3] - mat[2][0] * mat[1][3] ) +  mat[0][3] * ( mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1] ) );
	amat[3][3] = mat[0][0] * ( mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2] ) -  mat[0][1] * ( mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2] ) +  mat[0][2] * ( mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1] );

}

void additionMatrix( Matrix3 mat, Matrix3 amat )
{

	// 00 01 02
	// 10 11 12
	// 20 21 22
	
	amat[0][0]	=   mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2];
	amat[0][1]	= - mat[1][0] * mat[2][2] + mat[2][0] * mat[1][2];
	amat[0][2]	=   mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1];

	amat[1][0]	= - mat[0][1] * mat[2][2] + mat[2][1] * mat[0][2];
	amat[1][1]	=   mat[0][0] * mat[2][2] - mat[2][0] * mat[0][2];
	amat[1][2]	= - mat[0][0] * mat[2][1] + mat[2][0] * mat[0][1];

	amat[2][0]	=   mat[0][1] * mat[1][2] - mat[1][1] * mat[0][2];
	amat[2][1]	= - mat[0][0] * mat[1][2] + mat[1][0] * mat[0][2];
	amat[2][2]	=   mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];

	return ;
}

void mulMatrix3ToVector3( Matrix3 mat, Vector3 src, Vector3 dst )
{

	dst[0]		= mat[0][0] * src[0] + mat[0][1] * src[1] + mat[0][2] * src[2];
	dst[1]		= mat[1][0] * src[0] + mat[1][1] * src[1] + mat[1][2] * src[2];
	dst[2]		= mat[2][0] * src[0] + mat[2][1] * src[1] + mat[2][2] * src[2];

	return ;
}

void mulVector3ToMatrix3( Vector3 src, Matrix3 mat, Vector3 dst )
{

	dst[0]		= src[0] * mat[0][0] + src[1] * mat[1][0] + src[2] * mat[2][0];
	dst[1]		= src[0] * mat[0][1] + src[1] * mat[1][1] + src[2] * mat[2][1];
	dst[2]		= src[0] * mat[0][2] + src[1] * mat[1][2] + src[2] * mat[2][2];

	return ;
}

float __a3d_fastcall invertMatrix4( Matrix4 mat, Matrix4 imat ) { 
	
	Matrix4 add, dst1;
	
	additionMatrix4( mat, add );
	
	float det = mat[0][0] * add[0][0] + mat[0][1] * add[0][1] + mat[0][2] * add[0][2] + mat[0][3] * add[0][3];
	float det2 = 1.0f / det;
	copyMatrix4( add, dst1 );
	transMatrix4( dst1, imat );

	scaleMatrix4( imat, det2 );

	return det;
}

float __a3d_fastcall invertMatrix3( Matrix3 mat,Matrix3 imat )
{

	Matrix3	add, dst1; 	
	float det, det2;
	
	// 00 01 02
	// 10 11 12
	// 20 21 22
	
	additionMatrix( mat, add );
	
	det			= mat[0][0] * add[0][0] + mat[0][1] * add[0][1] + mat[0][2] * add[0][2];
	
	copyMatrix3( add, dst1 );
	transMatrix3( dst1, imat );
	
	det2 = 1.0f / det;

	scaleMatrix3( imat, det2 );
	
	return det;
}


void originMatrix4WithoutPosition( Matrix4 v ) { 
	v[0][0] = v[1][1] = v[2][2] = v[3][3] = 1.0f;
	v[0][1] = v[0][2] = v[0][3] = v[1][0] = v[1][2] = v[1][3] = v[2][0] = v[2][1] = v[2][3] = 0.0f;
}

void originMatrix4( Matrix4 v )
{
	v[0][0] = v[1][1] = v[2][2] = v[3][3] = 1.0f;
	v[0][1] = v[0][2] = v[0][3] = v[1][0] = v[1][2] = v[1][3] = v[2][0] = v[2][1] = v[2][3] = v[3][0] = v[3][1] = v[3][2] = 0.0f;

	return ;
}

void copyMatrix4( Matrix4 src, Matrix4 dst )
{

	dst[0][0]	= src[0][0]; 
	dst[0][1]	= src[0][1]; 
	dst[0][2]	= src[0][2]; 
	dst[0][3]	= src[0][3]; 
	dst[1][0]	= src[1][0]; 
	dst[1][1]	= src[1][1]; 
	dst[1][2]	= src[1][2]; 
	dst[1][3]	= src[1][3]; 
	dst[2][0]	= src[2][0]; 
	dst[2][1]	= src[2][1]; 
	dst[2][2]	= src[2][2]; 
	dst[2][3]	= src[2][3]; 
	dst[3][0]	= src[3][0]; 
	dst[3][1]	= src[3][1]; 
	dst[3][2]	= src[3][2]; 
	dst[3][3]	= src[3][3]; 

	return ;
}

void addMatrix4( Matrix4 a, Matrix4 b, Matrix4 c )
{

	c[0][0]		= a[0][0] + b[0][0];
    c[0][1]		= a[0][1] + b[0][1];
    c[0][2]		= a[0][2] + b[0][2];
    c[0][3] 	= a[0][3] + b[0][3];
    c[1][0] 	= a[1][0] + b[1][0];
    c[1][1] 	= a[1][1] + b[1][1];
    c[1][2] 	= a[1][2] + b[1][2];
    c[1][3] 	= a[1][3] + b[1][3];
    c[2][0] 	= a[2][0] + b[2][0];
    c[2][1] 	= a[2][1] + b[2][1];
    c[2][2] 	= a[2][2] + b[2][2];
    c[2][3] 	= a[2][3] + b[2][3];
    c[3][0] 	= a[3][0] + b[3][0];
    c[3][1] 	= a[3][1] + b[3][1];
    c[3][2] 	= a[3][2] + b[3][2];
    c[3][3] 	= a[3][3] + b[3][3];
	
	return ;
}

void subMatrix4( Matrix4 a, Matrix4 b, Matrix4 c )
{

	c[0][0]		= a[0][0] - b[0][0];
    c[0][1]		= a[0][1] - b[0][1];
    c[0][2]		= a[0][2] - b[0][2];
    c[0][3] 	= a[0][3] - b[0][3];
    c[1][0] 	= a[1][0] - b[1][0];
    c[1][1] 	= a[1][1] - b[1][1];
    c[1][2] 	= a[1][2] - b[1][2];
    c[1][3] 	= a[1][3] - b[1][3];
    c[2][0] 	= a[2][0] - b[2][0];
    c[2][1] 	= a[2][1] - b[2][1];
    c[2][2] 	= a[2][2] - b[2][2];
    c[2][3] 	= a[2][3] - b[2][3];
    c[3][0] 	= a[3][0] - b[3][0];
    c[3][1] 	= a[3][1] - b[3][1];
    c[3][2] 	= a[3][2] - b[3][2];
    c[3][3] 	= a[3][3] - b[3][3];
	
	return ;
}

void mulMatrix4( Matrix4 a, Matrix4 b, Matrix4 c )
{

	c[0][0]		= a[0][0] * b[0][0] + a[0][1] * b[1][0] + a[0][2] * b[2][0] + a[0][3] * b[3][0];
    c[0][1]		= a[0][0] * b[0][1] + a[0][1] * b[1][1] + a[0][2] * b[2][1] + a[0][3] * b[3][1];
    c[0][2]		= a[0][0] * b[0][2] + a[0][1] * b[1][2] + a[0][2] * b[2][2] + a[0][3] * b[3][2];
    c[0][3]		= a[0][0] * b[0][3] + a[0][1] * b[1][3] + a[0][2] * b[2][3] + a[0][3] * b[3][3];
    c[1][0]		= a[1][0] * b[0][0] + a[1][1] * b[1][0] + a[1][2] * b[2][0] + a[1][3] * b[3][0];
    c[1][1]		= a[1][0] * b[0][1] + a[1][1] * b[1][1] + a[1][2] * b[2][1] + a[1][3] * b[3][1];
    c[1][2]		= a[1][0] * b[0][2] + a[1][1] * b[1][2] + a[1][2] * b[2][2] + a[1][3] * b[3][2];
    c[1][3]		= a[1][0] * b[0][3] + a[1][1] * b[1][3] + a[1][2] * b[2][3] + a[1][3] * b[3][3];
    c[2][0]		= a[2][0] * b[0][0] + a[2][1] * b[1][0] + a[2][2] * b[2][0] + a[2][3] * b[3][0];
    c[2][1]		= a[2][0] * b[0][1] + a[2][1] * b[1][1] + a[2][2] * b[2][1] + a[2][3] * b[3][1];
    c[2][2]		= a[2][0] * b[0][2] + a[2][1] * b[1][2] + a[2][2] * b[2][2] + a[2][3] * b[3][2];
    c[2][3]		= a[2][0] * b[0][3] + a[2][1] * b[1][3] + a[2][2] * b[2][3] + a[2][3] * b[3][3];
    c[3][0]		= a[3][0] * b[0][0] + a[3][1] * b[1][0] + a[3][2] * b[2][0] + a[3][3] * b[3][0];
    c[3][1]		= a[3][0] * b[0][1] + a[3][1] * b[1][1] + a[3][2] * b[2][1] + a[3][3] * b[3][1];
    c[3][2]		= a[3][0] * b[0][2] + a[3][1] * b[1][2] + a[3][2] * b[2][2] + a[3][3] * b[3][2];
    c[3][3]		= a[3][0] * b[0][3] + a[3][1] * b[1][3] + a[3][2] * b[2][3] + a[3][3] * b[3][3];
	
	return ;
}

void scaleMatrix4( Matrix4 mat, float sc )
{

	mat[0][0] 	*= sc;
    mat[0][1] 	*= sc;
    mat[0][2] 	*= sc;
    mat[0][3] 	*= sc;
    mat[1][0] 	*= sc;
    mat[1][1] 	*= sc;
    mat[1][2] 	*= sc;
    mat[1][3] 	*= sc;
    mat[2][0] 	*= sc;
    mat[2][1] 	*= sc;
    mat[2][2] 	*= sc;
    mat[2][3] 	*= sc;
    mat[3][0] 	*= sc;
    mat[3][1] 	*= sc;
    mat[3][2] 	*= sc;
    mat[3][3] 	*= sc;
	
	return ;
}

void transMatrix4( Matrix4 mat ) { 
	Matrix4 tmat;
	transMatrix4(mat, tmat);
	copyMatrix4(tmat, mat);
}

void transMatrix4( Matrix4 mat, Matrix4 tmat )
{

	tmat[0][0]	= mat[0][0];
    tmat[0][1]	= mat[1][0];
    tmat[0][2]	= mat[2][0];
    tmat[0][3]	= mat[3][0];
    tmat[1][0]	= mat[0][1];
    tmat[1][1]	= mat[1][1];
    tmat[1][2]	= mat[2][1];
    tmat[1][3]	= mat[3][1];
    tmat[2][0]	= mat[0][2];
    tmat[2][1]	= mat[1][2];
    tmat[2][2]	= mat[2][2];
    tmat[2][3]	= mat[3][2];
    tmat[3][0]	= mat[0][3];
    tmat[3][1]	= mat[1][3];
    tmat[3][2]	= mat[2][3];
    tmat[3][3]	= mat[3][3];

	return ;
}

void mulVector3ToMatrix4( Vector3 src, Matrix4 mat, Vector4 dst ) 
{
	
	dst[0] = src[0] * mat[0][0] + src[1] * mat[1][0] + src[2] * mat[2][0] + mat[3][0];
    dst[1] = src[0] * mat[0][1] + src[1] * mat[1][1] + src[2] * mat[2][1] + mat[3][1];
    dst[2] = src[0] * mat[0][2] + src[1] * mat[1][2] + src[2] * mat[2][2] + mat[3][2];
    dst[3] = src[0] * mat[0][3] + src[1] * mat[1][3] + src[2] * mat[2][3] + mat[3][3];

	return ;
}

void mulMatrix4ToVector4( Matrix4 mat, Vector4 src, Vector4 dst )
{

	dst[0] = mat[0][0] * src[0] + mat[0][1] * src[1] + mat[0][2] * src[2] + mat[0][3] * src[3];
    dst[1] = mat[1][0] * src[0] + mat[1][1] * src[1] + mat[1][2] * src[2] + mat[1][3] * src[3];
    dst[2] = mat[2][0] * src[0] + mat[2][1] * src[1] + mat[2][2] * src[2] + mat[2][3] * src[3];
    dst[3] = mat[3][0] * src[0] + mat[3][1] * src[1] + mat[3][2] * src[2] + mat[3][3] * src[3];

	return ;
}

float getVectorProjectionKoeff( Vector3 src, Vector3 vec ) { 
	
	float moda, modb;
    
	moda = src[0] * vec[0] + src[1] * vec[1] + src[2] * vec[2];
    modb = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
	
	if( modb ) return moda / modb;

	return 0.0f;
}

float getVectorProjection( Vector3 src, Vector3 vec, Vector3 dst )
{
	
	float moda,modb,k;
    
	moda = src[0] * vec[0] + src[1] * vec[1] + src[2] * vec[2];
    modb = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
    
	if( modb )
	{
        k		= moda / modb;
        
		dst[0]	= vec[0] * k;
        dst[1]	= vec[1] * k;
        dst[2]	= vec[2] * k;
    }

	return k;
}

void getPerpendicularVector( Vector3 src, Vector3 vec, Vector3 dst )
{

	float moda,modb,k;
    
	moda = src[0] * vec[0] + src[1] * vec[1] + src[2] * vec[2];
    modb = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
    
	if( modb )
	{
        k		= moda / modb;
        
		dst[0]	= src[0] - vec[0] * k;
        dst[1]	= src[1] - vec[1] * k;
        dst[2]	= src[2] - vec[2] * k;
    }

	return ;
}

void rotateVectorAroundVector( Vector3 src, Vector3 vec, Vector3 dst, float angle )
{

	static	Vector3 tmp;
	float	cosa, sina;

	cosa	= cos( angle );
	sina	= sin( angle );
	
	normalizeVector2( vec, tmp );
	
	rotateVectorAroundNormVector( src, tmp, dst, cosa, sina );
	
	return ;
}

void rotateVectorAroundVector( Vector3 src, Vector3 vec, Vector3 dst, float cosa, float sina )
{
	
	static	Vector3 tmp;
   
	normalizeVector2( vec, tmp );   
	
	rotateVectorAroundNormVector( src, tmp, dst, cosa, sina );
	
	return ;
}

void rotateVectorAroundAxis( Vector3 src, Vector3 pos, Vector3 axis, Vector3 dst, float angle )
{
	static Vector3	tmp;
	float	cosa, sina;

	cosa	= cos( angle );
	sina	= sin( angle );

	normalizeVector2( axis, tmp ); 

	rotateVectorAroundNormAxis( src, pos, tmp, dst, cosa, sina );
	
	return ;
}

void rotateVectorAroundAxis( Vector3 src, Vector3 pos, Vector3 axis, Vector3 dst, float cosa, float sina )
{
	static Vector3	tmp;
	
	normalizeVector2( axis, tmp );

	rotateVectorAroundNormAxis( src, pos, tmp, dst, cosa, sina );
	
	return ;
}

void rotateVectorAroundNormVector( Vector3 src, Vector3 vec, Vector3 dst, float angle )
{

	float	cosa, sina;

	cosa = cos( angle );
	sina = sin( angle );

	rotateVectorAroundNormVector( src, vec, dst, cosa, sina );

	return ;
}


void rotateVectorAroundNormVector( Vector3 src, Vector3 vec, Vector3 dst, float cosa, float sina )
{

	static	Vector3 i, j, tmp, prj;

    getVectorProjection( src, vec, prj );
    
	i[0] = src[0] - prj[0];
    i[1] = src[1] - prj[1];
    i[2] = src[2] - prj[2];

    vectorMulOfVector( vec, i, j );

    i[0] *= cosa;
    i[1] *= cosa;
    i[2] *= cosa;
    j[0] *= sina;
    j[1] *= sina;
    j[2] *= sina;

    dst[0] = prj[0] + i[0] + j[0];
    dst[1] = prj[1] + i[1] + j[1];
    dst[2] = prj[2] + i[2] + j[2];
	
	return ;
}

void rotateVectorAroundNormAxis( Vector3 src, Vector3 pos, Vector3 axis, Vector3 dst, float angle )
{

	float	cosa, sina;

	cosa = cos( angle );
	sina = sin( angle );

	rotateVectorAroundNormAxis( src, pos, axis, dst, cosa, sina );

	return ;
}

void rotateVectorAroundNormAxis( Vector3 src, Vector3 pos, Vector3 axis, Vector3 dst, float cosa, float sina )
{

	static Vector3	tmp;
	
	subVector3( src, pos, tmp );
    rotateVectorAroundNormVector( tmp, axis, dst, cosa, sina );
    addVector3( dst, pos, dst );
	
	return ;
}

void rotateVectorsArrayAroundVector( Vector3 *src, int count, Vector3 vec, Vector3 *dst, float angle )
{

	static Vector3	tmp;
	float	cosa, sina;

	cosa	= cos( angle );
	sina	= sin( angle );

	normalizeVector2( vec, tmp );

	rotateVectorsArrayAroundNormVector( src, count, tmp, dst, cosa, sina );

	return ;
}

void rotateVectorsArrayAroundVector( Vector3 *src, int count, Vector3 vec, Vector3 *dst, float cosa, float sina )
{

	static		Vector3 tmp;
    
	normalizeVector2( vec, tmp );   
    
	rotateVectorsArrayAroundNormVector( src, count, tmp, dst, cosa, sina );
	
	return ;
}

void rotateVectorsArrayAroundAxis( Vector3 *src, int count, Vector3 pos, Vector3 axis, Vector3 *dst, float angle )
{

	static Vector3	tmp;
	float	cosa, sina;

	cosa	= cos( angle );
	sina	= sin( angle );

	normalizeVector2( axis, tmp );	
	
	rotateVectorsArrayAroundNormAxis( src, count, pos, tmp, dst, cosa, sina );
	
	return ;
}
	
void rotateVectorsArrayAroundAxis( Vector3 *src, int count, Vector3 pos, Vector3 axis, Vector3 *dst, float cosa, float sina )
{

	static		Vector3 tmp;
    
	normalizeVector2( axis, tmp );   
    
	rotateVectorsArrayAroundNormAxis( src, count, pos, tmp, dst, cosa, sina );

	return ;
}

void rotateVectorsArrayAroundNormVector( Vector3 *src, int count, Vector3 vec, Vector3 *dst, float angle )
{
	
	float	cosa, sina;

	cosa	= cos( angle );
	sina	= sin( angle );

	rotateVectorsArrayAroundNormVector( src, count, vec, dst, cosa, sina );
		
	return ;
}

void rotateVectorsArrayAroundNormVector( Vector3 *src, int count, Vector3 vec, Vector3 *dst, float cosa, float sina )
{

	static		Vector3 i, j, tmp, prj;
    int			index;

    for( index = 0; index < count; index ++ )
	{
        getVectorProjection( src[index], vec, prj );

        i[0]	= src[index][0] - prj[0];
        i[1]	= src[index][1] - prj[1];
        i[2]	= src[index][2] - prj[2];

        vectorMulOfVector( vec, i, j );

        i[0]	*= cosa;
        i[1]	*= cosa;
        i[2]	*= cosa;
        j[0]	*= sina;
        j[1]	*= sina;
        j[2]	*= sina;

        dst[index][0] = prj[0] + i[0] + j[0];
        dst[index][1] = prj[1] + i[1] + j[1];
        dst[index][2] = prj[2] + i[2] + j[2];
    }
	
	return ;
}

void rotateVectorsArrayAroundNormAxis( Vector3 *src, int count, Vector3 pos, Vector3 axis, Vector3 *dst, float angle )
{

	float	cosa, sina;

	cosa	= cos( angle );
	sina	= sin( angle );

	rotateVectorsArrayAroundNormAxis( src, count, pos, axis, dst, cosa, sina );	
	
	return ;
}

void rotateVectorsArrayAroundNormAxis( Vector3 *src, int count, Vector3 pos, Vector3 axis, Vector3 *dst, float cosa, float sina )
{

	static Vector3	i, j, prj;
    int				index;


    for( index = 0; index < count; index ++ )
	{
        subVector3( src[index], pos, src[index] );
        getVectorProjection( src[index], axis, prj );

        i[0]	= src[index][0] - prj[0];
        i[1]	= src[index][1] - prj[1];
        i[2]	= src[index][2] - prj[2];

        vectorMulOfVector( axis, i, j );

        i[0]	*= cosa;
        i[1]	*= cosa;
        i[2]	*= cosa;
        j[0]	*= sina;
        j[1]	*= sina;
        j[2]	*= sina;

        dst[index][0] = prj[0] + i[0] + j[0];
        dst[index][1] = prj[1] + i[1] + j[1];
        dst[index][2] = prj[2] + i[2] + j[2];

        addVector3( dst[index], pos, dst[index] );
    }

	return ;
}

void calcDirectionMatrix( Matrix4 mat, Vector3 newZDir, Vector3 newYDir ) { 
	
	Vector3	tmp;
	
	normalizeVector( newZDir );
	normalizeVector( newYDir );

	mat[0][2] = newZDir[0];
	mat[1][2] = newZDir[1];	
	mat[2][2] = newZDir[2];

	vectorMulOfVector( newYDir, newZDir, tmp );
	normalizeVector(tmp);

	vectorMulOfVector( newZDir, tmp, newYDir );

	mat[0][0] = tmp[0];
	mat[1][0] = tmp[1];
	mat[2][0] = tmp[2];

	mat[0][1] = newYDir[0];
	mat[1][1] = newYDir[1];
	mat[2][1] = newYDir[2];
}

void translateCoords( Vector3 src, Matrix4 mat, Vector3 dst ) { 
	
	Vector3 tmp;

	tmp[0] = src[0] * mat[0][0] + src[1] * mat[1][0] + src[2] * mat[2][0] + mat[3][0];
    tmp[1] = src[0] * mat[0][1] + src[1] * mat[1][1] + src[2] * mat[2][1] + mat[3][1];
    tmp[2] = src[0] * mat[0][2] + src[1] * mat[1][2] + src[2] * mat[2][2] + mat[3][2];

	copyVector3( tmp, dst );
}

void translateVector( Vector3 src, Matrix4 mat, Vector3 dst ) { 
	
	Vector3 tmp;

	tmp[0] = src[0] * mat[0][0] + src[1] * mat[1][0] + src[2] * mat[2][0];
    tmp[1] = src[0] * mat[0][1] + src[1] * mat[1][1] + src[2] * mat[2][1];
    tmp[2] = src[0] * mat[0][2] + src[1] * mat[1][2] + src[2] * mat[2][2];

	copyVector3( tmp, dst );
}

void extractMatrix3( Matrix4 mat4, Matrix3 mat3 ) {

	int i, j;
	for( i = 0; i < 3; i++ ) {
		for( j = 0; j < 3; j++ ) {
			mat3[i][j] = mat4[i][j];
		}
	}
}

void insertMatrix3( Matrix4 mat4, Matrix3 mat3 ) {

	int i, j;
	for( i = 0; i < 3; i++ ) {
		for( j = 0; j <3 ; j++ ) {
			mat4[i][j] = mat3[i][j];
		}
	}
}

void makeDirectXTransformMatrix( Matrix4 src, Matrix4 dst ) { 

	Matrix3	bas, fbas;	
	
	extractMatrix3( src, bas );
	invertMatrix3( bas, fbas );
	insertMatrix3( dst, fbas );

	dst[3][0] = src[3][0];
	dst[3][1] = src[3][1];
	dst[3][2] = src[3][2];
}

void clearVector3Array( Vector3 *arr, int count ) {
	
	for( int i = 0; i < count; i ++ ) { 
		clearVector3( arr[i] );
	}
	
	return ;
}

void normalizeVector3Array( Vector3 *arr, int count ) { 
	
	for( int i = 0; i < count; i ++ ) { 
		normalizeVector( arr[i] );
	}
	
	return ;
}

void swapFloat( float &a, float &b ) { 

	float temp = a; a = b; b = temp;
}

void swapVectors2( Vector2 v1, Vector2 v2 ) { 

	float temp;

	temp = v1[0]; v1[0] = v2[0]; v2[0] = temp;
	temp = v1[1]; v1[1] = v2[1]; v2[1] = temp;
}

void normalizeBasis(Matrix4 basis) { 
	
	Vector3 v;

	extractX(basis, v); normalizeVector( v ); includeX(basis, v);
	extractY(basis, v); normalizeVector( v ); includeY(basis, v);
	extractZ(basis, v); normalizeVector( v ); includeZ(basis, v);
}

void getVector2Projection( Vector2 ta, Vector2 tb, Vector2 tc, Vector2 uv, float *projT1, float *projT2 ) { 

	Vector2 ab, ac, ap;

	subVector2( tb, ta, ab );
	subVector2( tc, ta, ac );

	subVector2( uv, ta, ap );

	float k = 1.0f / ( ab[0] * ac[1] - ac[0] * ab[1] ); 
	
	*projT1 = ( ac[1] * ap[0] - ac[0] * ap[1] ) * k;
	*projT2 = ( ab[0] * ap[1] - ab[1] * ap[0] ) * k;

	return ;
}

void uv2XYZ( Vector3 a, Vector3 b, Vector3 c, float projT1, float projT2, Vector3 xyz ) { 

	Vector3 v1, v2;
	subVector3( b, a, v1 );
	subVector3( c, a, v2 );

	scaleVector3( v1, projT1 );
	scaleVector3( v2, projT2 );
	
	addVector3( a, v1, xyz );
	addVector3( xyz, v2, xyz );
}

void uv2XYZ( Vector3 a, Vector3 b, Vector3 c, Vector2 ta, Vector2 tb, Vector2 tc, Vector2 uv, Vector3 xyz ) { 

	float projT1, projT2;

	getVector2Projection( ta, tb, tc, uv, &projT1, &projT2 );
	uv2XYZ( a, b, c, projT1, projT2, xyz );

	return ;
}

void interpolateNormal( Vector3 a, Vector3 b, Vector3 c, Vector3 na, Vector3 nb, Vector3 nc, Vector3 point, float projT1, float projT2, Vector3 normal ) { 
	
	Vector3 v1, v2;
	Vector3 n1, n2;
	Vector3 p1, p2, p1p2, p1p;
	
	subVector3( b, a, v1 );
	subVector3( c, a, v2 );

	scaleVector3( v1, projT1 );
	scaleVector3( v2, projT2 );

	addVector3( a, v1, p1 );
	addVector3( a, v2, p2 );
	
	subVector3( p2, p1, p1p2 );
	subVector3( point, p1, p1p );

	float koeff = getVectorProjectionKoeff( p1p, p1p2 );
	
	Vector3 k;

	subVector3( nb, na, k ); 
	scaleVector3( k, projT1 );
	addVector3( na, k, n1 );

	subVector3( nc, na, k ); 
	scaleVector3( k, projT2 );
	addVector3( na, k, n2 );

	subVector3( n2, n1, k );
	scaleVector3( k, koeff );
	addVector3( n1, k, normal );
	
	normalizeVector( normal );
}

void interpolateNormal( Vector3 a, Vector3 b, Vector3 c, Vector3 na, Vector3 nb, Vector3 nc, Vector3 point, Vector3 normal ) { 

	float projT1, projT2;
	Vector3 v1, v2, p;
	
	subVector3( b, a, v1 );
	subVector3( c, a, v2 );
	subVector3( point, a, p );

	projT1 = getVectorProjectionKoeff( p, v1 );
	projT2 = getVectorProjectionKoeff( p, v2 );

	interpolateNormal( a, b, c, na, nb, nc, point, projT1, projT2, normal );

	return ;
}

void uv2XYZAndInterpolateNormal( Vector3 a, Vector3 b, Vector3 c, Vector2 ta, Vector2 tb, Vector2 tc, Vector3 na, Vector3 nb, Vector3 nc, Vector2 uv, Vector3 xyz, Vector3 normal ) { 

	float projT1, projT2;

	getVector2Projection( ta, tb, tc, uv, &projT1, &projT2 );
	uv2XYZ( a, b, c, projT1, projT2, xyz );

	interpolateNormal( a, b, c, na, nb, nc, xyz, projT1, projT2, normal );
	normalizeVector( normal );
}

void calcRay2IntersectionPoint( Vector2 point, Vector2 dir, Vector2 linePoint, Vector2 lineNormal, Vector2 intPoint ) { 

	float t = calcRay2T( point, dir, linePoint, lineNormal );	
	
	if( t == FLT_MAX || t < 0.0f ) { 
		// no intersection point
		setVector2( intPoint, FLT_MAX, FLT_MAX );
		return ;
	}

	intPoint[0] = point[0] + dir[0] * t;
	intPoint[1] = point[1] + dir[1] * t;

	return ;
}

bool __a3d_fastcall calcRay3IntersectionPoint( Vector3 point, Vector3 dir, Vector3 planePoint, Vector3 planeNormal, Vector3 intPoint ) { 

	float t = calcRay3T( point, dir, planePoint, planeNormal );	
	
	if( t == FLT_MAX || t < 0.0f ) { 
		// no intersection point
		//setVector3( intPoint, FLT_MAX, FLT_MAX, FLT_MAX );
		return false;
	}

	intPoint[0] = point[0] + dir[0] * t;
	intPoint[1] = point[1] + dir[1] * t;
	intPoint[2] = point[2] + dir[2] * t;

	return true;
}

float __a3d_fastcall calcRay2T( Vector2 point, Vector2 dir, Vector2 linePoint, Vector2 lineNormal ) { 

	float t1 = scalarMulOfVector2( linePoint, lineNormal ) - scalarMulOfVector2( point, lineNormal );
	float t2 = scalarMulOfVector2( dir, lineNormal );
	
	if( t2 < 0.000001f && t2 > -0.000001f ) return FLT_MAX;
	
	return t1 / t2;
}

float __a3d_fastcall calcRay3T( Vector3 point, Vector3 dir, Vector3 planePoint, Vector3 planeNormal ) { 
	
	Vector3 ap;
	
	float t2 = scalarMulOfVector( dir, planeNormal );
	if( /*t2 < 0.000001f && */t2 > -0.000001f ) return FLT_MAX;

	subVector3( planePoint, point, ap );
	float t1 = scalarMulOfVector( ap, planeNormal );
	return t1 / t2;
}

bool __a3d_fastcall isBarycentricRayIntersectsTriangle( Vector3 point, Vector3 dir,  Vector3 a, Vector3 barycentricUKoeff, Vector3 barycentricVKoeff, Vector3 planeNormal, Vector3 intPoint ) { 

	// get any intersection point
	calcRay3IntersectionPoint( point, dir, a, planeNormal, intPoint );
	// if plane is behind point
	if( intPoint[0] == FLT_MAX ) return false;
	// check if point in triangle
	//look Barycentric intersection algorithm!!!!!!!
	
	float u = barycentricUKoeff[0] * intPoint[1] + barycentricUKoeff[1] * intPoint[0] + barycentricUKoeff[2];
	float v = barycentricVKoeff[0] * intPoint[1] + barycentricVKoeff[1] * intPoint[0] + barycentricVKoeff[2];

	//look Barycentric intersection algorithm!!!!!!!
	return ( u > 0.0f && v > 0.0f && u + v <= 1.0f );
}

bool __a3d_fastcall checkNearPoint( Vector3 nearPoint, Vector3 farPoint, Vector3 srcPoint ) { 

	Vector3 v1, v2;

	subVector3( farPoint, srcPoint, v1 );
	subVector3( nearPoint, srcPoint, v2 );

#define FN_TOLERANCE 0.01f

	if( v1[0] * v1[0] + v1[1] * v1[1] + v1[2] * v1[2] > 
		v2[0] * v2[0] + v2[1] * v2[1] + v2[2] * v2[2] + FN_TOLERANCE ) return true;
	
	return false;
}

bool __a3d_fastcall isSimpleRayIntersectsTriangle( Vector3 point, Vector3 dir, Vector3 a, Vector3 b, Vector3 c, Vector3 planeNormal, Vector3 intPoint, int dominantAxisIndex ) { 

	if( !calcRay3IntersectionPoint( point, dir, a, planeNormal, intPoint ) ) return false;

	switch( dominantAxisIndex ) { 
		case 0:
			if( is2dPointOutside( a[2], a[1], b[2], b[1], intPoint[2], intPoint[1] ) ) 
				return false;
			if( is2dPointOutside( b[2], b[1], c[2], c[1], intPoint[2], intPoint[1] ) )
				return false;
			if( is2dPointOutside( c[2], c[1], a[2], a[1], intPoint[2], intPoint[1] ) )
				return false;
		
			return true;

		case 1:
			if( is2dPointOutside( a[0], a[2], b[0], b[2], intPoint[0], intPoint[2] ) )
				return false;
			if( is2dPointOutside( b[0], b[2], c[0], c[2], intPoint[0], intPoint[2] ) )
				return false;
			if( is2dPointOutside( c[0], c[2], a[0], a[2], intPoint[0], intPoint[2] ) )
				return false;

			return true;
	
		case 2:
			if( is2dPointOutside( a[0], a[1], b[0], b[1], intPoint[0], intPoint[1] ) )
				return false;
			if( is2dPointOutside( b[0], b[1], c[0], c[1], intPoint[0], intPoint[1] ) )
				return false;
			if( is2dPointOutside( c[0], c[1], a[0], a[1], intPoint[0], intPoint[1] ) )
				return false;

			return true;
	}
	
	return true;
}

bool __a3d_fastcall isRayIntersectsTriangle( Vector3 point, Vector3 dir,  Vector3 a, Vector3 b, Vector3 c, Vector3 planeNormal, Vector3 intPoint ) { 
	
	//look Barycentric intersection algorithm!!!!!!!
	Vector3 uKoeff, vKoeff;
	getBarycentricUVKoeffs( a, b, c, uKoeff, vKoeff );
	
	return isBarycentricRayIntersectsTriangle( point, dir, a, uKoeff, vKoeff, planeNormal, intPoint );
}

void getBarycentricUVKoeffs( Vector3 a, Vector3 b, Vector3 c, Vector3 uKoeff, Vector3 vKoeff ) { 

	Vector3 sb, sc;
	
	// see barycentric ray intersection algorithm
	subVector3( b, a, sb );
	subVector3( c, a, sc );

	// for u	
	float k = 1.0f / ( sb[1] * sc[0] - sb[0] * sc[1] );
	
	uKoeff[0] = sc[0] * k ;
	uKoeff[1] = - sc[1] * k;
	uKoeff[2] = ( sc[1] * a[0] - sc[0] * a[1] ) * k;

	// for k
	k = 1.0f / ( sc[1] * sb[0] - sc[0] * sb[1] );
	
	vKoeff[0] = sb[0] * k;
	vKoeff[1] = - sb[1] * k;
	vKoeff[2] = ( sb[1] * a[0] - sb[0] * a[1] );

	return ;
}

void setVector3( float *v, DWORD val ) { 
	
	float r, g, b;

	r = ((float)( (val >> 16) & 0xff )) / 255.0f;
	g = ((float)( (val >> 8) & 0xff )) / 255.0f;
	b = ((float)( (val ) & 0xff )) / 255.0f;
	
	setVector3( v, r, g, b );
}

void setVector4( float *v, DWORD val ) { 

	float r, g, b, a;

	a = ((float)( (val >> 24) & 0xff )) / 255.0f;
	r = ((float)( (val >> 16) & 0xff )) / 255.0f;
	g = ((float)( (val >> 8) & 0xff )) / 255.0f;
	b = ((float)( (val ) & 0xff )) / 255.0f;

	setVector4( v, a, r, g, b );
}

DWORD vector3ToDWORD( float *v ) {
	DWORD r, g, b;	
	
	r = ((DWORD)( v[0] * 255.0f ) << 16) & 0xff0000;
	g = ((DWORD)( v[1] * 255.0f ) << 8) & 0x00ff00;
	b = (DWORD)( v[2] * 255.0f ) & 0xff;

	return r | g | b;
}

DWORD vector4ToDWORD( float *v ) {
	DWORD a, r, g, b;	

	a = ((DWORD)( v[0] * 255.0f ) << 24) & 0xff000000;
	r = ((DWORD)( v[1] * 255.0f ) << 16) & 0x00ff0000;
	g = ((DWORD)( v[2] * 255.0f ) << 8) & 0x0000ff00;
	b = (DWORD)( v[3] * 255.0f ) & 0xff;

	return a | r | g | b;
}

int __a3d_fastcall outCode( Vector2 p, Vector2 ra, Vector2 rb ) { 
	
	int code = 0;
	if( p[0] < ra[0] ) code |= 0x01;
	if( p[1] > ra[1] ) code |= 0x02;
	if( p[0] > rb[0] ) code |= 0x04;
	if( p[1] < rb[1] ) code |= 0x08;

	return code;
} 

void swapInt( int &a, int &b ) { 
	int c = a; a = b; b = c;
}

bool __a3d_fastcall isLineIntersectRect( Vector2 a, Vector2 b, Vector2 rectA, Vector2 rectB ) { 
	
	int code1 = outCode( a, rectA, rectB );
	int code2 = outCode( b, rectA, rectB );

	// one of the points inside
	if( code1 == 0 || code2 == 0 ) return true;
	
	// outside
	if( ( code1 & code2 ) != 0 ) return false;
				
	if( code1 & 0x01 ) { // check left 
		a[1] += ( b[1] - a[1] ) * ( rectA[0] - a[0] ) / ( b[0] - a[0] );
		return a[1] >= rectB[1] && a[1] <= rectA[1];
	}
	else if( code1 & 0x02 ) { // check above
		a[0] += ( b[0] - a[0] ) * ( rectA[1] - a[1] ) / ( b[1] - a[1] );
		return a[0] >= rectA[0] && a[0] <= rectB[0];
	}
	else if( code1 & 0x04 ) { // check right
		a[1] += ( b[1] - a[1] ) * ( rectB[0] - a[0] ) / ( b[0] - a[0] );
		return a[1] >= rectB[1] && a[1] <= rectA[1];
	}
	else if( code1 & 0x08 ) { // check below
		a[0] += ( b[0] - a[0] ) * ( rectB[1] - a[1] ) / ( b[1] - a[1] ); 
		return a[0] >= rectA[0] && a[0] <= rectB[0];
	}
	
	return false;
}

bool __a3d_fastcall isLineIntersectCube( Vector3 a, Vector3 b, Vector3 ca, Vector3 cb ) { 

	Vector2 la, lb, ra, rb;
	
	// on xz
	setVector2( la, a[0], a[2] );
	setVector2( lb, b[0], b[2] );
	setVector2( ra, ca[0], ca[2] );
	setVector2( rb, cb[0], cb[2] );
	
	bool intXZ = isLineIntersectRect( la, lb, ra, rb );

	// on xy
	setVector2( la, a[0], a[1] );
	setVector2( lb, b[0], b[1] );
	setVector2( ra, ca[0], ca[1] );
	setVector2( rb, cb[0], cb[1] );

	bool intXY = isLineIntersectRect( la, lb, ra, rb );

	setVector2( la, a[2], a[1] );
	setVector2( lb, b[2], b[1] );
	setVector2( ra, cb[2], ca[1] );
	setVector2( rb, ca[2], cb[1] );

	bool intZY = isLineIntersectRect( la, lb, ra, rb );

	return intXZ && intZY && intXY;
}

bool __a3d_fastcall isTriangleIntersectsCube( Vector3 a, Vector3 b, Vector3 c, Vector3 ca, Vector3 cb ) { 
	return isLineIntersectCube( a, b, ca, cb ) || isLineIntersectCube( a, c, ca, cb ) || isLineIntersectCube( b, c, ca, cb );
}