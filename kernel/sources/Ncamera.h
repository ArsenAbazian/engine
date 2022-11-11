#ifndef  NCAMERA_INCLUDED
#define  NCAMERA_INCLUDED

#include <windows.h>
#include <d3d9.h>
#include "vecmat.h"

#define		OUT_FRUSTUM			0
#define		IN_FRUSTUM			1
#define		INTERSECT_FRUSTUM	2

class camera
{
public:
    LPDIRECT3DDEVICE9	lpDevice;
	D3DVIEWPORT9		viewport;
	DWORD				screenWidth;
	DWORD				screenHeight;

	Matrix4				proj;
    Matrix4				state;
	Matrix4				viewProj;
	Matrix4				iView;
		
    Vector3				pos;

    Vector4				nearPlane;
	Vector4				farPlane;
	Vector4				leftPlane;
	Vector4				rightPlane;
	Vector4				upPlane;
    Vector4				downPlane;
		
	Vector3				frustumApex;

    float				w;
    float				h;
	float				nearZ;
	float				farZ;
	float				fovy;
	float				tanFovY05;
	float				invTanFovY05;
	float				aspect;

	camera();
   ~camera();

    void                initialize( float fovy, float aspect, float nearZ, float farZ );
	void				setDevice( LPDIRECT3DDEVICE9 lpDev ){ this->lpDevice = lpDev; }

	void                position( Vector3 newPos );
    void                position( float newX, float newY, float newZ );
	void				direction( Vector3 newZdir, Vector3 newYDir );
    void				direction( float newZDirX, float newZDirY, float newZDirZ, float newYDirX, float newYDirY, float newYDirZ );
	
	void                target( Vector3 targetPos );
    void                target( float targetPosX, float targetPosY, float targetPosZ );

    HRESULT             setProjection();
    HRESULT             setState();
    HRESULT             setScreen();

    void                rotateAroundCameraX( float angle );
    void                rotateAroundCameraY( float angle );
    void                rotateAroundCameraZ( float angle );
    void                rotateAroundVector( Vector3 vec, float angle );
    void                rotateAroundAxis( Vector3 pos, Vector3 axis, float angle );

    void                moveByCameraX( float distance );
    void                moveByCameraY( float distance );
    void                moveByCameraZ( float distance );
    void                moveByVector( Vector3 vec );
    void                walkFB( float distance );
    void                strafeLR( float distance );
    void                liftUD( float distance );

	float               getCameraXAxis( int index ){ return this->state[index][0]; }
	float               getCameraYAxis( int index ){ return this->state[index][1]; }
	float               getCameraZAxis( int index ){ return this->state[index][2]; }
	float               getCameraPos( int index ){ return this->pos[index]; }
	float               getFrustumApexPos( int index ){ return this->frustumApex[index]; }

	void                clear();

	bool				isVertexInFrustum( Vector3 vert );
	bool				isVertexInFrustum( Vector3 vert, Vector3 origin );
	int					isBoxInFrustum( Vector3 bbox[8] );
	int					isBoxInFrustum( Vector3 a, Vector3 b );
	int					isBoxInFrustum( Vector3 bbox[8], Vector3 origin );
	int					isSphereInFrustum( Vector4 sphere );
	int					isSphereInFrustum( Vector4 sphere, Vector3 origin );
	void				refreshPlanes();
	void				getRay( float cursorX, float cursorY, Vector3 rayDot, Vector3 rayDir );
	void				getRay( float cursorX, float cursorY, DWORD screenWidth, DWORD screenHeight, Vector3 rayDot, Vector3 rayDir );

	void				preapreViewProj(){ 
		mulMatrix4( this->state, this->proj, this->viewProj ); 
		invertMatrix4( this->state, this->iView );
	}
	static float		getIntersectYFromPlane( Vector3 cameraPos, Vector3 p0, Vector3 p1, Vector3 p2 ); // возвращает необходимое изменение высоты
};
#endif