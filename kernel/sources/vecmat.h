#include <windows.h>
#include <math.h>
#include <float.h>
#include "commath.h"

#ifndef MATHLIB_INCLUDED
#define MATHLIB_INCLUDED

#ifndef M_PI
#define M_PI   3.14159265358979323846
#endif

// ------------------ макросы для управления кодом -----------------------
//#define QUICK_SQRT	// вычисляет корень чуть чуть приближенно но очень быстро

// ------------------ ---------------------------- -----------------------
#ifdef QUICK_SQRT

#define normalizeVector1( vec ) quicknormalizeVector( vec )
#define normalizeVector2( src, dst ) quicknormalizeVector( src, dst )

#else

#define normalizeVector1( vec ) slownormalizeVector( vec )
#define normalizeVector2( src, dst ) slownormalizeVector( src, dst )

#endif

#define ONE_AS_INTEGER ((DWORD)(0x3F800000))
#define degToRad( a ) a * 0.0174533f


typedef float Vector2[2];
typedef float Vector3[3];
typedef float Vector4[4];

typedef float Matrix3[3][3];
typedef float Matrix4[4][4];

inline void			setVector2( Vector3 v, float x, float y ){ v[0] = x; v[1] = y; }
inline void			setVector3( Vector3 v, float x, float y, float z ){ v[0] = x; v[1] = y; v[2] = z; }
void				setVector3( float* v, DWORD val );
void				setVector4( float* v, DWORD val );
DWORD				vector3ToDWORD( float* v );
DWORD				vector4ToDWORD( float* v ); 
inline void			setVector4( Vector4 v, float x, float y, float z, float t ){ v[0] = x; v[1] = y; v[2] = z; v[3] = t; }
inline void			clearVector2( Vector2 v ) { v[0] = v[1] = 0.0f; }
inline void			clearVector3( Vector3 v ) { v[0] = v[1] = v[2] = 0.0f; }
inline void			clearVector4( Vector4 v ) { v[0] = v[1] = v[2] = v[3] = 0.0f; }
void				swapVectors2( Vector2 v1, Vector2 v2 );

inline void			copyVector2( Vector2 src, Vector2 dst ){ dst[0] = src[0]; dst[1] = src[1]; }
inline void			copyVector3( Vector3 src, Vector3 dst ){ dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2]; }
inline void			copyVector4( Vector4 src, Vector4 dst ){ dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2]; dst[3] = src[3]; }

inline void			addVector3( Vector3 a, Vector3 b, Vector3 c ){ c[0] = a[0] + b[0]; c[1] = a[1] + b[1]; c[2] = a[2] + b[2]; }
inline void			addVector4( Vector4 a, Vector4 b, Vector4 c ){ c[0] = a[0] + b[0]; c[1] = a[1] + b[1]; c[2] = a[2] + b[2]; c[3] = a[3]+b[3]; }

inline void			subVector2( Vector2 a, Vector2 b, Vector2 c ) { c[0] = a[0] - b[0]; c[1] = a[1] - b[1]; }
inline void			subVector3( Vector3 a, Vector3 b, Vector3 c ){ c[0] = a[0] - b[0]; c[1] = a[1] - b[1]; c[2] = a[2] - b[2]; }
inline void			subVector4( Vector4 a, Vector4 b, Vector4 c ){ c[0] = a[0] - b[0]; c[1] = a[1] - b[1]; c[2] = a[2] - b[2]; c[3] = a[3]-b[3]; }

inline float		scalarMulOfVector2( Vector2 a, Vector2 b ){ return  a[0] * b[0] + a[1] * b[1]; }
inline float		scalarMulOfVector( Vector3 a, Vector3 b ){ return  a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }
inline void			vectorMulOfVector( Vector3 a, Vector3 b, Vector3 c ){ c[0] = a[1] * b[2] - b[1] * a[2]; c[1] = b[0] * a[2] - a[0] * b[2]; c[2] = a[0] * b[1] - b[0] * a[1]; }

inline void			invertVector3( Vector3 a, Vector3 b ){ b[0] = - a[0]; b[1] = - a[1]; b[2] = - a[2]; }
inline void			invertVector4( Vector4 a, Vector4 b ){ b[0] = - a[0]; b[1] = - a[1]; b[2] = - a[2]; b[3] = - a[3]; }

inline void			scaleVector2( Vector2 v, float a, float b ) { v[0] *= a; v[1] *= b; }
inline void			scaleVector3( Vector3 v, float s ){ v[0] *= s; v[1] *= s; v[2] *= s; }
inline void			scaleVector4( Vector4 v, float s ){ v[0] *= s; v[1] *= s; v[2] *=s; v[3] *= s; }

inline void			scaleVector3( Vector3 a, float f, Vector3 b ){ b[0] = a[0] * f; b[1] = a[1] * f; b[2] = a[2] * f; }
inline void			scaleVector4( Vector4 a, float f, Vector4 b ){ b[0] = a[0] * f; b[1] = a[1] * f; b[2] = a[2] * f; b[3] = a[3] * f; }

inline bool			eqVector3( Vector3 v1, Vector3 v2 ){ return v1[0] == v2[0] && v1[1] == v2[1] && v1[2] == v2[2]; } 

float __a3d_fastcall	slownormalizeVector( Vector3 vec );
float __a3d_fastcall	slownormalizeVector( Vector3 src, Vector3 dst );
float __a3d_fastcall	quicknormalizeVector( Vector3 vec );
float __a3d_fastcall	quicknormalizeVector( Vector3 src, Vector3 dst );
float __a3d_fastcall	quicknormalizeVectorAndRetSquareDist( Vector3 v );

inline float		normalizeVector( Vector3 vec ){ return normalizeVector1( vec ); }
inline float		normalizeVector( Vector3 src, Vector3 dst ){ return normalizeVector2( src, dst ); }

void				originMatrix3( Matrix3 mat );
void				copyMatrix3( Matrix3 src, Matrix3 dst );
void				addMatrix3( Matrix3 a, Matrix3 b, Matrix3 c );
void				subMatrix3( Matrix3 a, Matrix3 b, Matrix3 c );
void				mulMatrix3( Matrix3 a, Matrix3 b, Matrix3 c );
void				scaleMatrix3( Matrix3 mat, float sc );
void				transMatrix3( Matrix3 mat, Matrix3 tmat );
void				additionMatrix( Matrix3 mat, Matrix3 amat );
void				additionMatrix4( Matrix4 mat, Matrix4 amat );
void				mulMatrix3ToVector3( Matrix3 mat, Vector3 src, Vector3 dst );
void				mulVector3ToMatrix3( Vector3 src, Matrix3 mat, Vector3 dst );
float __a3d_fastcall	invertMatrix3( Matrix3 mat,Matrix3 imat );
float __a3d_fastcall	invertMatrix4( Matrix4 mat, Matrix4 imat );

void				originMatrix4( Matrix4 mat );
void				originMatrix4WithoutPosition( Matrix4 mat );
void				copyMatrix4( Matrix4 src, Matrix4 dst );
void				addMatrix4( Matrix4 a, Matrix4 b, Matrix4 c );
void				subMatrix4( Matrix4 a, Matrix4 b, Matrix4 c );
void				mulMatrix4( Matrix4 a, Matrix4 b, Matrix4 c );
void				scaleMatrix4( Matrix4 mat, float sc );
void				transMatrix4( Matrix4 mat, Matrix4 tmat );
void				transMatrix4( Matrix4 mat );
void				mulMatrix4ToVector4( Matrix4 mat, Vector4 src, Vector4 dst );
void				mulVector3ToMatrix4( Vector3 src, Matrix4 mat, Vector4 dst );
void				translateCoords( Vector3 src, Matrix4 mat, Vector3 dst );
void				translateVector( Vector3 src, Matrix4 mat, Vector3 dst );
void				normalizeBasis( Matrix4 basis);

inline void			extractX( Matrix4 basis, Vector3 x ) { setVector3(x, basis[0][0], basis[1][0], basis[2][0]); }
inline void			extractY( Matrix4 basis, Vector3 y ) { setVector3(y, basis[0][1], basis[1][1], basis[2][1]); }
inline void			extractZ( Matrix4 basis, Vector3 z ) { setVector3(z, basis[0][2], basis[1][2], basis[2][2]); }

inline void			includeX( Matrix4 basis, Vector3 x ) { basis[0][0] = x[0]; basis[1][0] = x[1]; basis[2][0] = x[2]; }
inline void			includeY( Matrix4 basis, Vector3 y ) { basis[0][1] = y[0]; basis[1][1] = y[1]; basis[2][1] = y[2]; }
inline void			includeZ( Matrix4 basis, Vector3 z ) { basis[0][2] = z[0]; basis[1][2] = z[1]; basis[2][2] = z[2]; }

float				getVectorProjectionKoeff( Vector3 src, Vector3 vec );
float				getVectorProjection( Vector3 src, Vector3 vec, Vector3 dst );
void				getPerpendicularVector( Vector3 src, Vector3 vec, Vector3 dst );

void				rotateVectorAroundVector( Vector3 src, Vector3 vec, Vector3 dst, float angle );
void				rotateVectorAroundAxis( Vector3 src, Vector3 pos, Vector3 axis, Vector3 dst, float angle );

void				rotateVectorAroundNormVector( Vector3 src, Vector3 vec, Vector3 dst, float angle );
void				rotateVectorAroundNormAxis( Vector3 src, Vector3 pos, Vector3 axis, Vector3 dst, float angle );

void				rotateVectorsArrayAroundVector( Vector3 *src, int count, Vector3 vec, Vector3 *dst, float angle );
void				rotateVectorsArrayAroundAxis( Vector3 *src, int count, Vector3 pos, Vector3 axis, Vector3 *dst, float angle );

void				rotateVectorsArrayAroundNormVector( Vector3 *src, int count, Vector3 vec, Vector3 *dst, float angle );
void				rotateVectorsArrayAroundNormAxis( Vector3 *src, int count, Vector3 pos, Vector3 axis, Vector3 *dst, float angle );

void				rotateVectorAroundVector( Vector3 src, Vector3 vec, Vector3 dst, float cosa, float sina );
void				rotateVectorAroundAxis( Vector3 src, Vector3 pos, Vector3 axis, Vector3 dst, float cosa, float sina );

void				rotateVectorAroundNormVector( Vector3 src, Vector3 vec, Vector3 dst, float cosa, float sina );
void				rotateVectorAroundNormAxis( Vector3 src, Vector3 pos, Vector3 axis, Vector3 dst, float cosa, float sina );

void				rotateVectorsArrayAroundVector( Vector3 *src, int count, Vector3 vec, Vector3 *dst, float cosa, float sina );
void				rotateVectorsArrayAroundAxis( Vector3 *src, int count, Vector3 pos, Vector3 axis, Vector3 *dst, float cosa, float sina );

void				rotateVectorsArrayAroundNormVector( Vector3 *src, int count, Vector3 vec, Vector3 *dst, float cosa, float sina );
void				rotateVectorsArrayAroundNormAxis( Vector3 *src, int count, Vector3 pos, Vector3 axis, Vector3 *dst, float cosa, float sina );

void				calcDirectionMatrix( Matrix4 mat, Vector3 newZDir, Vector3 newYDir );
void				extractMatrix3( Matrix4 mat4, Matrix3 mat3 );
void				insertMatrix3( Matrix4 mat4, Matrix3 mat3 );
void				makeDirectXTransformMatrix( Matrix4 src, Matrix4 dst );
void				clearVector3Array( Vector3 *arr, int count );
void				normalizeVector3Array( Vector3 *arr, int count );

void				uv2XYZ( Vector3 a, Vector3 b, Vector3 c, float projT1, float projT2, Vector3 xyz );
void				uv2XYZ( Vector3 a, Vector3 b, Vector3 c, Vector2 ta, Vector2 tb, Vector2 tc, Vector2 uv, Vector3 xyz );

void				getVector2Projection( Vector2 ta, Vector2 tb, Vector2 tc, Vector2 uv, float *projT1, float *projT2 );
void				uv2XYZAndInterpolateNormal( Vector3 a, Vector3 b, Vector3 c, Vector2 ta, Vector2 tb, Vector2 tc, Vector3 na, Vector3 nb, Vector3 nc, Vector2 uv, Vector3 xyz, Vector3 normal );

void				interpolateNormal( Vector3 a, Vector3 b, Vector3 c, Vector3 na, Vector3 nb, Vector3 nc, Vector3 point, Vector3 normal );
void				interpolateNormal( Vector3 a, Vector3 b, Vector3 c, Vector3 na, Vector3 nb, Vector3 nc, Vector3 point, float projT1, float projT2, Vector3 normal );

float __a3d_fastcall	calcRay2T( Vector2 point, Vector2 dir, Vector2 linePoint, Vector2 lineNormal );
float __a3d_fastcall	calcRay3T( Vector3 point, Vector3 dir, Vector3 planePoint, Vector3 planeNormal );
void				calcRay2IntersectionPoint( Vector2 point, Vector2 dir, Vector2 linePoint, Vector2 lineNormal, Vector2 intPoint );
bool  __a3d_fastcall	calcRay3IntersectionPoint( Vector3 point, Vector3 dir, Vector3 planePoint, Vector3 planeNormal, Vector3 intPoint );
bool  __a3d_fastcall	isBarycentricRayIntersectsTriangle( Vector3 point, Vector3 dir,  Vector3 a, Vector3 barycentricUKoeff, Vector3 barycentricVKoeff, Vector3 planeNormal, Vector3 intPoint );
bool  __a3d_fastcall	isRayIntersectsTriangle( Vector3 point, Vector3 dir,  Vector3 a, Vector3 b, Vector3 c, Vector3 planeNormal, Vector3 intPoint );
bool  __a3d_fastcall	isSimpleRayIntersectsTriangle( Vector3 point, Vector3 dir,  Vector3 a, Vector3 b, Vector3 c, Vector3 planeNormal, Vector3 intPoint, int dominantAxisIndex );
void				getBarycentricUVKoeffs( Vector3 a, Vector3 b, Vector3 c, Vector3 uKoeff, Vector3 vKoeff );

bool __a3d_fastcall		checkNearPoint( Vector3 nearPoint, Vector3 farPoint, Vector3 srcPoint );

/*
bool __a3d_fastcall		isLineIntersectVertLine( Vector2 a, Vector2 b, float vLineX, float vLineY1, float vLineY2 );
bool __a3d_fastcall		isLineIntersectHorzLine( Vector2 a, Vector2 b, float hLineY, float hLineX1, float hLineX2 );
*/
bool __a3d_fastcall		isLineIntersectRect( Vector2 a, Vector2 b, Vector2 rectA, Vector2 rectB );
bool __a3d_fastcall		isLineIntersectCube( Vector3 a, Vector3 b, Vector3 ca, Vector3 cb );
bool __a3d_fastcall		isTriangleIntersectsCube( Vector3 a, Vector3 b, Vector3 c, Vector3 ca, Vector3 cb );
 
inline bool is2dPointOutside( float a1, float a2, float b1, float b2, float i1, float i2 ) { 
	/*
	float x = b1 - a1;
	float y = b2 - a2;
	float nx = - y = a2 - b2;
	float ny = x = b1 - a1;
	*/
	
	return ( i1 - a1 ) * ( a2 - b2 ) < ( i2 - a2 ) * ( a1 - b1 );
}

void				swapFloat( float &a, float &b );
void				swapInt( int &a, int &b );
inline int			float2Int( float a ) { return (int)( a + 0.5f ); }

#endif