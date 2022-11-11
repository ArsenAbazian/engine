#include "ncamera.h"
#include "d3dx9math.h"

void camera::clear()
{ 
	ZeroMemory( this, sizeof( camera ) ); 
	originMatrix4( this->state );
	setVector3( this->pos, 0.0f, 0.0f, 0.0f );
}

camera::camera()
{
    this->clear();
}

camera::~camera(){}

void camera::initialize( float fovy, float aspect, float nearZ, float farZ )
{
	
	fovy				= degToRad( fovy );
	this->h				= /*nearZ * */cos( fovy / 2.0f ) / sin( fovy / 2.0f );
    this->w				= this->h / aspect;
	this->nearZ			= nearZ;
	this->farZ			= farZ;
	this->fovy			= fovy;
	this->tanFovY05		= tan( this->fovy * 0.5f );
	this->invTanFovY05	= 1.0f / this->tanFovY05;
	this->aspect		= aspect;

	// set up camera position
	this->frustumApex[0] = this->pos[0] - this->state[0][2] * this->nearZ;
    this->frustumApex[1] = this->pos[1] - this->state[1][2] * this->nearZ;
    this->frustumApex[2] = this->pos[2] - this->state[2][2] * this->nearZ;

    this->state[3][0] = - this->frustumApex[0] * this->state[0][0] - this->frustumApex[1] * this->state[1][0] - this->frustumApex[2] * this->state[2][0];
    this->state[3][1] = - this->frustumApex[0] * this->state[0][1] - this->frustumApex[1] * this->state[1][1] - this->frustumApex[2] * this->state[2][1];
    this->state[3][2] = - this->frustumApex[0] * this->state[0][2] - this->frustumApex[1] * this->state[1][2] - this->frustumApex[2] * this->state[2][2];

    // set up camera projection
    ZeroMemory( this->proj, sizeof( Matrix4 ) );

	D3DXMatrixPerspectiveFovLH( (D3DXMATRIX*)this->proj, this->fovy, this->aspect, this->nearZ, this->farZ );
    
	/*
	this->proj[0][0] = this->w; //2.0f * this->nearZ / this->w;
    this->proj[1][1] = this->h; //2.0f * this->nearZ / this->h;
	this->proj[2][2] = this->farZ / ( this->farZ - this->nearZ );
    this->proj[2][3] = 1.0f;
	this->proj[3][2] = this->nearZ * this->farZ / ( this->nearZ - this->farZ );
	*/

	return ;
}

void camera::position( Vector3 newPos )
{
    
	Vector3		np;

    subVector3( newPos, this->pos, np );

    this->frustumApex[0]	+= np[0];
    this->frustumApex[1]	+= np[1];
    this->frustumApex[2]	+= np[2];

    this->state[3][0] = - this->frustumApex[0] * this->state[0][0] - this->frustumApex[1] * this->state[1][0] - this->frustumApex[2] * this->state[2][0];
    this->state[3][1] = - this->frustumApex[0] * this->state[0][1] - this->frustumApex[1] * this->state[1][1] - this->frustumApex[2] * this->state[2][1];
    this->state[3][2] = - this->frustumApex[0] * this->state[0][2] - this->frustumApex[1] * this->state[1][2] - this->frustumApex[2] * this->state[2][2];

    copyVector3( newPos, this->pos );

	return ;
}

void camera::position( float newX, float newY, float newZ )
{

    this->frustumApex[0]	+= newX - this->pos[0];
    this->frustumApex[1]	+= newY - this->pos[1];
    this->frustumApex[2]	+= newZ - this->pos[2];

    this->state[3][0] = - this->frustumApex[0] * this->state[0][0] - this->frustumApex[1] * this->state[1][0] - this->frustumApex[2] * this->state[2][0];
    this->state[3][1] = - this->frustumApex[0] * this->state[0][1] - this->frustumApex[1] * this->state[1][1] - this->frustumApex[2] * this->state[2][1];
    this->state[3][2] = - this->frustumApex[0] * this->state[0][2] - this->frustumApex[1] * this->state[1][2] - this->frustumApex[2] * this->state[2][2];

    setVector3( this->pos, newX, newY, newZ );

	return ;
}

void camera::direction( Vector3 newZDir, Vector3 newYDir )
{

	Vector3	tmp;
	
	normalizeVector( newZDir );
	normalizeVector( newYDir );
	
	this->state[0][2] = newZDir[0];
	this->state[1][2] = newZDir[1];	
	this->state[2][2] = newZDir[2];

	vectorMulOfVector( newYDir, newZDir, tmp );
	normalizeVector(tmp);

	vectorMulOfVector( newZDir, tmp, newYDir );
	
	this->state[0][0] = tmp[0];
	this->state[1][0] = tmp[1];
	this->state[2][0] = tmp[2];

	this->state[0][1] = newYDir[0];
	this->state[1][1] = newYDir[1];
	this->state[2][1] = newYDir[2];

	// set up camera position
    this->frustumApex[0]	= this->pos[0] - this->state[0][2] * this->nearZ;
    this->frustumApex[1]	= this->pos[1] - this->state[1][2] * this->nearZ;
    this->frustumApex[2]	= this->pos[2] - this->state[2][2] * this->nearZ;

	this->state[3][0]		= - this->frustumApex[0] * this->state[0][0] - this->frustumApex[1] * this->state[1][0] - this->frustumApex[2] * this->state[2][0];
    this->state[3][1]		= - this->frustumApex[0] * this->state[0][1] - this->frustumApex[1] * this->state[1][1] - this->frustumApex[2] * this->state[2][1];
    this->state[3][2]		= - this->frustumApex[0] * this->state[0][2] - this->frustumApex[1] * this->state[1][2] - this->frustumApex[2] * this->state[2][2];
		
	return ;
}

void camera::direction( float newZDirX, float newZDirY, float newZDirZ, float newYDirX, float newYDirY, float newYDirZ )
{
	
	Vector3	z, y;

	setVector3( z, newZDirX, newZDirY, newZDirZ );
	setVector3( y, newYDirX, newYDirY, newYDirZ );

	return this->direction( z, y );

}

HRESULT camera::setProjection()
{
    return this->lpDevice->SetTransform( D3DTS_PROJECTION, (D3DMATRIX*)this->proj );
}

HRESULT camera::setState()
{
    return this->lpDevice->SetTransform( D3DTS_VIEW, (D3DMATRIX*)this->state );
}

HRESULT camera::setScreen()
{
	this->screenWidth	= this->viewport.Width;
	this->screenHeight	= this->viewport.Height;
	
	return this->lpDevice->SetViewport( &this->viewport );
}

void camera::rotateAroundCameraY( float angle )
{
    
	Vector3		nx, dir;
    float		ca, sa;

    ca						= cos( angle );
    sa						= sin( angle );

    nx[0]					= this->state[0][0] * ca + this->state[0][2] * sa;
    nx[1]					= this->state[1][0] * ca + this->state[1][2] * sa;
    nx[2]					= this->state[2][0] * ca + this->state[2][2] * sa;

    this->state[0][2]		= this->state[0][2]*ca - this->state[0][0]*sa;
    this->state[1][2]		= this->state[1][2]*ca - this->state[1][0]*sa;
    this->state[2][2]		= this->state[2][2]*ca - this->state[2][0]*sa;

    this->state[0][0]		= nx[0];
    this->state[1][0]		= nx[1];
    this->state[2][0]		= nx[2];

    this->frustumApex[0]	= this->pos[0] + this->state[0][2] * this->proj[3][2];
    this->frustumApex[1]	= this->pos[1] + this->state[1][2] * this->proj[3][2];
    this->frustumApex[2]	= this->pos[2] + this->state[2][2] * this->proj[3][2];

    this->state[3][0]		= - this->frustumApex[0] * this->state[0][0] - this->frustumApex[1] * this->state[1][0] - this->frustumApex[2] * this->state[2][0];
    this->state[3][1]		= - this->frustumApex[0] * this->state[0][1] - this->frustumApex[1] * this->state[1][1] - this->frustumApex[2] * this->state[2][1];
    this->state[3][2]		= - this->frustumApex[0] * this->state[0][2] - this->frustumApex[1] * this->state[1][2] - this->frustumApex[2] * this->state[2][2];

    dir[0]					= this->state[0][1];
    dir[1]					= this->state[1][1];
    dir[2]					= this->state[2][1];

	return ;
}

void camera::rotateAroundCameraX( float angle )
{
    
	Vector3		nz, dir;
    float		ca, sa;

    ca						= cos( angle );
    sa						= sin( angle );

    nz[0]					= this->state[0][2] * ca + this->state[0][1] * sa;
    nz[1]					= this->state[1][2] * ca + this->state[1][1] * sa;
    nz[2]					= this->state[2][2] * ca + this->state[2][1] * sa;

    this->state[0][1]		= this->state[0][1] * ca - this->state[0][2] * sa;
    this->state[1][1]		= this->state[1][1] * ca - this->state[1][2] * sa;
    this->state[2][1]		= this->state[2][1] * ca - this->state[2][2] * sa;

    this->state[0][2]		= nz[0];
    this->state[1][2]		= nz[1];
    this->state[2][2]		= nz[2];

    this->frustumApex[0]	= this->pos[0] + this->state[0][2] * this->proj[3][2];
    this->frustumApex[1]	= this->pos[1] + this->state[1][2] * this->proj[3][2];
    this->frustumApex[2]	= this->pos[2] + this->state[2][2] * this->proj[3][2];

    this->state[3][0]		= -this->frustumApex[0] * this->state[0][0] - this->frustumApex[1] * this->state[1][0] - this->frustumApex[2] * this->state[2][0];
    this->state[3][1]		= -this->frustumApex[0] * this->state[0][1] - this->frustumApex[1] * this->state[1][1] - this->frustumApex[2] * this->state[2][1];
    this->state[3][2]		= -this->frustumApex[0] * this->state[0][2] - this->frustumApex[1] * this->state[1][2] - this->frustumApex[2] * this->state[2][2];

    dir[0]					= this->state[0][0];
    dir[1]					= this->state[1][0];
    dir[2]					= this->state[2][0];

    return ;
}

void camera::rotateAroundCameraZ( float angle )
{
    
	Vector3		nx, dir;
    float		ca, sa;

    ca						= cos( angle );
    sa						= sin( angle );

    nx[0]					= this->state[0][0] * ca + this->state[0][1] * sa;
    nx[1]					= this->state[1][0] * ca + this->state[1][1] * sa;
    nx[2]					= this->state[2][0] * ca + this->state[2][1] * sa;

    this->state[0][1]		= this->state[0][1] * ca - this->state[0][0] * sa;
    this->state[1][1]		= this->state[1][1] * ca - this->state[1][0] * sa;
    this->state[2][1]		= this->state[2][1] * ca - this->state[2][0] * sa;

    this->state[0][0]		= nx[0];
    this->state[1][0]		= nx[1];
    this->state[2][0]		= nx[2];

    this->frustumApex[0]	= this->pos[0] + this->state[0][2] * this->proj[3][2];
    this->frustumApex[1]	= this->pos[1] + this->state[1][2] * this->proj[3][2];
    this->frustumApex[2]	= this->pos[2] + this->state[2][2] * this->proj[3][2];

    this->state[3][0]		= - this->frustumApex[0] * this->state[0][0] - this->frustumApex[1] * this->state[1][0] - this->frustumApex[2] * this->state[2][0];
    this->state[3][1]		= - this->frustumApex[0] * this->state[0][1] - this->frustumApex[1] * this->state[1][1] - this->frustumApex[2] * this->state[2][1];
    this->state[3][2]		= - this->frustumApex[0] * this->state[0][2] - this->frustumApex[1] * this->state[1][2] - this->frustumApex[2] * this->state[2][2];

    dir[0]					= this->state[0][2];
    dir[1]					= this->state[1][2];
    dir[2]					= this->state[2][2];

	return ;    
}

void camera::rotateAroundVector( Vector3 vec, float angle )
{
    
	Vector3		basis[3], dir;
    float		ca, sa;

    ca						= cos( angle );
    sa						= sin( angle );

    normalizeVector( vec, dir );

    basis[0][0]				= this->state[0][0];
    basis[0][1]				= this->state[1][0];
    basis[0][2]				= this->state[2][0];

    basis[1][0]				= this->state[0][1];
    basis[1][1]				= this->state[1][1];
    basis[1][2]				= this->state[2][1];

    basis[2][0]				= this->state[0][2];
    basis[2][1]				= this->state[1][2];
    basis[2][2]				= this->state[2][2];

    rotateVectorsArrayAroundNormVector( basis, 3, dir, basis, ca, sa );
    
    this->state[0][0]		= basis[0][0];
    this->state[1][0]		= basis[0][1];
    this->state[2][0]		= basis[0][2];

    this->state[0][1]		= basis[1][0];
    this->state[1][1]		= basis[1][1];
    this->state[2][1]		= basis[1][2];

    this->state[0][2]		= basis[2][0];
    this->state[1][2]		= basis[2][1];
    this->state[2][2]		= basis[2][2];

	this->frustumApex[0]	= this->pos[0] - this->state[0][2] * this->nearZ;
    this->frustumApex[1]	= this->pos[1] - this->state[1][2] * this->nearZ;
    this->frustumApex[2]	= this->pos[2] - this->state[2][2] * this->nearZ;

    this->state[3][0]		= - this->frustumApex[0] * this->state[0][0] - this->frustumApex[1] * this->state[1][0] - this->frustumApex[2] * this->state[2][0];
    this->state[3][1]		= - this->frustumApex[0] * this->state[0][1] - this->frustumApex[1] * this->state[1][1] - this->frustumApex[2] * this->state[2][1];
    this->state[3][2]		= - this->frustumApex[0] * this->state[0][2] - this->frustumApex[1] * this->state[1][2] - this->frustumApex[2] * this->state[2][2];

	return ;
}

void camera::rotateAroundAxis( Vector3 axisPos, Vector3 axis, float angle )
{
    Vector3		basis[3], dir;
    float		ca, sa;

    ca						= cos( angle );
    sa						= sin( angle );

    normalizeVector( axis, dir );

    basis[0][0]				= this->state[0][0];
    basis[0][1]				= this->state[1][0];
    basis[0][2]				= this->state[2][0];

    basis[1][0]				= this->state[0][1];
    basis[1][1]				= this->state[1][1];
    basis[1][2]				= this->state[2][1];

    basis[2][0]				= this->state[0][2];
    basis[2][1]				= this->state[1][2];
    basis[2][2]				= this->state[2][2];

    addVector3( basis[0], this->pos,basis[0] );
    addVector3( basis[1], this->pos,basis[1] );
    addVector3( basis[2], this->pos,basis[2] );

    rotateVectorsArrayAroundNormAxis( basis, 3, axisPos, dir, basis, ca, sa );
    rotateVectorAroundNormAxis( this->pos, axisPos, dir, this->pos, ca, sa );

    subVector3( basis[0], this->pos,basis[0] );
    subVector3( basis[1], this->pos,basis[1] );
    subVector3( basis[2], this->pos,basis[2] );

    this->state[0][0]		= basis[0][0];
    this->state[1][0]		= basis[0][1];
    this->state[2][0]		= basis[0][2];

    this->state[0][1]		= basis[1][0];
    this->state[1][1]		= basis[1][1];
    this->state[2][1]		= basis[1][2];

    this->state[0][2]		= basis[2][0];
    this->state[1][2]		= basis[2][1];
    this->state[2][2]		= basis[2][2];

    this->frustumApex[0]	= this->pos[0] - this->state[0][2] * this->nearZ;
    this->frustumApex[1]	= this->pos[1] - this->state[1][2] * this->nearZ;
    this->frustumApex[2]	= this->pos[2] - this->state[2][2] * this->nearZ;

    this->state[3][0]		= - this->frustumApex[0] * this->state[0][0] - this->frustumApex[1] * this->state[1][0] - this->frustumApex[2] * this->state[2][0];
    this->state[3][1]		= - this->frustumApex[0] * this->state[0][1] - this->frustumApex[1] * this->state[1][1] - this->frustumApex[2] * this->state[2][1];
    this->state[3][2]		= - this->frustumApex[0] * this->state[0][2] - this->frustumApex[1] * this->state[1][2] - this->frustumApex[2] * this->state[2][2];

	return ;
}

void camera::moveByCameraX( float distance )
{
    
	Vector3		v;

    v[0]					= this->state[0][0] * distance;
    v[1]					= this->state[1][0] * distance;
    v[2]					= this->state[2][0] * distance;

    this->pos[0]			+= v[0];
    this->pos[1]			+= v[1];
    this->pos[2]			+= v[2];

    this->frustumApex[0]	+= v[0];
    this->frustumApex[1]	+= v[1];
    this->frustumApex[2]	+= v[2];

    this->state[3][0]		= - this->frustumApex[0] * this->state[0][0] - this->frustumApex[1] * this->state[1][0] - this->frustumApex[2] * this->state[2][0];
    this->state[3][1]		= - this->frustumApex[0] * this->state[0][1] - this->frustumApex[1] * this->state[1][1] - this->frustumApex[2] * this->state[2][1];
    this->state[3][2]		= - this->frustumApex[0] * this->state[0][2] - this->frustumApex[1] * this->state[1][2] - this->frustumApex[2] * this->state[2][2];

	return ;
}

void camera::moveByCameraY( float distance )
{
    Vector3 v;

    v[0]					= this->state[0][1] * distance;
    v[1]					= this->state[1][1] * distance;
    v[2]					= this->state[2][1] * distance;

    this->pos[0]			+= v[0];
    this->pos[1]			+= v[1];
    this->pos[2]			+= v[2];

    this->frustumApex[0]	+= v[0];
    this->frustumApex[1]	+= v[1];
    this->frustumApex[2]	+= v[2];

    this->state[3][0]		= -this->frustumApex[0] * this->state[0][0] - this->frustumApex[1] * this->state[1][0] - this->frustumApex[2] * this->state[2][0];
    this->state[3][1]		= -this->frustumApex[0] * this->state[0][1] - this->frustumApex[1] * this->state[1][1] - this->frustumApex[2] * this->state[2][1];
    this->state[3][2]		= -this->frustumApex[0] * this->state[0][2] - this->frustumApex[1] * this->state[1][2] - this->frustumApex[2] * this->state[2][2];

	return ;
}

void camera::moveByCameraZ( float distance )
{
    
	Vector3		v;

    v[0]					= this->state[0][2] * distance;
    v[1]					= this->state[1][2] * distance;
    v[2]					= this->state[2][2] * distance;

    this->pos[0]			+= v[0];
    this->pos[1]			+= v[1];
    this->pos[2]			+= v[2];

    this->frustumApex[0]	+= v[0];
    this->frustumApex[1]	+= v[1];
    this->frustumApex[2]	+= v[2];

    this->state[3][0]		= - this->frustumApex[0] * this->state[0][0] - this->frustumApex[1] * this->state[1][0] - this->frustumApex[2] * this->state[2][0];
    this->state[3][1]		= - this->frustumApex[0] * this->state[0][1] - this->frustumApex[1] * this->state[1][1] - this->frustumApex[2] * this->state[2][1];
    this->state[3][2]		= - this->frustumApex[0] * this->state[0][2] - this->frustumApex[1] * this->state[1][2] - this->frustumApex[2] * this->state[2][2];

	return ;
}

void camera::moveByVector( Vector3 vec )
{

    this->pos[0]			+= vec[0];
    this->pos[1]			+= vec[1];
    this->pos[2]			+= vec[2];

    this->frustumApex[0]	+= vec[0];
    this->frustumApex[1]	+= vec[1];
    this->frustumApex[2]	+= vec[2];

    this->state[3][0]		= - this->frustumApex[0] * this->state[0][0] - this->frustumApex[1] * this->state[1][0] - this->frustumApex[2] * this->state[2][0];
    this->state[3][1]		= - this->frustumApex[0] * this->state[0][1] - this->frustumApex[1] * this->state[1][1] - this->frustumApex[2] * this->state[2][1];
    this->state[3][2]		= - this->frustumApex[0] * this->state[0][2] - this->frustumApex[1] * this->state[1][2] - this->frustumApex[2] * this->state[2][2];

	return ;
}

void camera::walkFB( float distance )
{
    
	Vector3		z_prj, y, z;

    z[0]					= this->state[0][2];
    z[1]					= this->state[1][2];
    z[2]					= this->state[2][2];

    y[0]					= 0.0f;
    y[1]					= 1.0f;
    y[2]					= 0.0f;

    getPerpendicularVector( z, y, z_prj );
    normalizeVector( z_prj );

    z_prj[0]				*= distance;
    z_prj[1]				*= distance;
    z_prj[2]				*= distance;

    this->frustumApex[0]	+= z_prj[0];
    this->frustumApex[1]	+= z_prj[1];
    this->frustumApex[2]	+= z_prj[2];

    this->pos[0]			+= z_prj[0];
    this->pos[1]			+= z_prj[1];
    this->pos[2]			+= z_prj[2];

    this->state[3][0]		= - this->frustumApex[0] * this->state[0][0] - this->frustumApex[1] * this->state[1][0] - this->frustumApex[2] * this->state[2][0];
    this->state[3][1]		= - this->frustumApex[0] * this->state[0][1] - this->frustumApex[1] * this->state[1][1] - this->frustumApex[2] * this->state[2][1];
    this->state[3][2]		= - this->frustumApex[0] * this->state[0][2] - this->frustumApex[1] * this->state[1][2] - this->frustumApex[2] * this->state[2][2];

	return ;
}

void camera::liftUD( float distance )
{
   
	this->frustumApex[1]	+= distance;
    this->pos[1]			+= distance;
    this->state[3][1]		= - this->frustumApex[0] * this->state[0][1] - this->frustumApex[1] * this->state[1][1] - this->frustumApex[2] * this->state[2][1];
	
	return ;
}

void camera::strafeLR( float distance )
{
    
	Vector3		x_prj, y, x;

    x[0]					= this->state[0][0];
    x[1]					= this->state[1][0];
    x[2]					= this->state[2][0];

    y[0]					= 0.0f;
    y[1]					= 1.0f;
    y[2]					= 0.0f;

    getPerpendicularVector( x, y, x_prj );
    normalizeVector( x_prj );

    x_prj[0]				*= distance;
    x_prj[1]				*= distance;
    x_prj[2]				*= distance;

    this->frustumApex[0]	+= x_prj[0];
    this->frustumApex[1]	+= x_prj[1];
    this->frustumApex[2]	+= x_prj[2];

    this->pos[0]			+= x_prj[0];
    this->pos[1]			+= x_prj[1];
    this->pos[2]			+= x_prj[2];

    this->state[3][0]		= - this->frustumApex[0] * this->state[0][0] - this->frustumApex[1] * this->state[1][0] - this->frustumApex[2] * this->state[2][0];
    this->state[3][1]		= - this->frustumApex[0] * this->state[0][1] - this->frustumApex[1] * this->state[1][1] - this->frustumApex[2] * this->state[2][1];
    this->state[3][2]		= - this->frustumApex[0] * this->state[0][2] - this->frustumApex[1] * this->state[1][2] - this->frustumApex[2] * this->state[2][2];

	return ;
}

void camera::refreshPlanes()
{

	float	invLen;
	
	// сначала перемножаем матрицу состояния на матрицу проекции
	mulMatrix4( this->state, this->proj, this->viewProj );
	
	// теперь создаем нормали плоскостей
	this->rightPlane[0]	= this->viewProj[ 0 ][ 3 ] - this->viewProj[ 0 ][ 0 ];
	this->rightPlane[1]	= this->viewProj[ 1 ][ 3 ] - this->viewProj[ 1 ][ 0 ];
	this->rightPlane[2]	= this->viewProj[ 2 ][ 3 ] - this->viewProj[ 2 ][ 0 ];
	this->rightPlane[3]	= this->viewProj[ 3 ][ 3 ] - this->viewProj[ 3 ][ 0 ];
	
	invLen = invSqrt( this->rightPlane[0] * this->rightPlane[0] + this->rightPlane[1] * this->rightPlane[1] + this->rightPlane[2] * this->rightPlane[2] + this->rightPlane[3] * this->rightPlane[3] );
	this->rightPlane[0]	*= invLen;	
	this->rightPlane[1]	*= invLen;	
	this->rightPlane[2]	*= invLen;	
	this->rightPlane[3]	*= invLen;	
	
	this->leftPlane[0]	= this->viewProj[ 0 ][ 3 ] + this->viewProj[ 0 ][ 0 ];
	this->leftPlane[1]	= this->viewProj[ 1 ][ 3 ] + this->viewProj[ 1 ][ 0 ];
	this->leftPlane[2]	= this->viewProj[ 2 ][ 3 ] + this->viewProj[ 2 ][ 0 ];
	this->leftPlane[3]	= this->viewProj[ 3 ][ 3 ] + this->viewProj[ 3 ][ 0 ];

	invLen = invSqrt( this->leftPlane[0] * this->leftPlane[0] + this->leftPlane[1] * this->leftPlane[1] + this->leftPlane[2] * this->leftPlane[2] + this->leftPlane[3] * this->leftPlane[3] );
	this->leftPlane[0]	*= invLen;	
	this->leftPlane[1]	*= invLen;	
	this->leftPlane[2]	*= invLen;	
	this->leftPlane[3]	*= invLen;	

	this->downPlane[0]	= this->viewProj[ 0 ][ 3 ] + this->viewProj[ 0 ][ 1 ];
	this->downPlane[1]	= this->viewProj[ 1 ][ 3 ] + this->viewProj[ 1 ][ 1 ];
	this->downPlane[2]	= this->viewProj[ 2 ][ 3 ] + this->viewProj[ 2 ][ 1 ];
	this->downPlane[3]	= this->viewProj[ 3 ][ 3 ] + this->viewProj[ 3 ][ 1 ];

	invLen = invSqrt( this->downPlane[0] * this->downPlane[0] + this->downPlane[1] * this->downPlane[1] + this->downPlane[2] * this->downPlane[2] + this->downPlane[3] * this->downPlane[3] );
	this->downPlane[0]	*= invLen;	
	this->downPlane[1]	*= invLen;	
	this->downPlane[2]	*= invLen;	
	this->downPlane[3]	*= invLen;	

	this->upPlane[0]	= this->viewProj[ 0 ][ 3 ] - this->viewProj[ 0 ][ 1 ];
	this->upPlane[1]	= this->viewProj[ 1 ][ 3 ] - this->viewProj[ 1 ][ 1 ];
	this->upPlane[2]	= this->viewProj[ 2 ][ 3 ] - this->viewProj[ 2 ][ 1 ];
	this->upPlane[3]	= this->viewProj[ 3 ][ 3 ] - this->viewProj[ 3 ][ 1 ];

	invLen = invSqrt( this->upPlane[0] * this->upPlane[0] + this->upPlane[1] * this->upPlane[1] + this->upPlane[2] * this->upPlane[2] + this->upPlane[3] * this->upPlane[3] );
	this->upPlane[0]	*= invLen;	
	this->upPlane[1]	*= invLen;	
	this->upPlane[2]	*= invLen;	
	this->upPlane[3]	*= invLen;	

	this->farPlane[0]	= this->viewProj[ 0 ][ 3 ] - this->viewProj[ 0 ][ 2 ];
	this->farPlane[1]	= this->viewProj[ 1 ][ 3 ] - this->viewProj[ 1 ][ 2 ];
	this->farPlane[2]	= this->viewProj[ 2 ][ 3 ] - this->viewProj[ 2 ][ 2 ];
	this->farPlane[3]	= this->viewProj[ 3 ][ 3 ] - this->viewProj[ 3 ][ 2 ];

	invLen = invSqrt( this->farPlane[0] * this->farPlane[0] + this->farPlane[1] * this->farPlane[1] + this->farPlane[2] * this->farPlane[2] + this->farPlane[3] * this->farPlane[3] );
	this->farPlane[0]	*= invLen;	
	this->farPlane[1]	*= invLen;	
	this->farPlane[2]	*= invLen;	
	this->farPlane[3]	*= invLen;

	this->nearPlane[0]	= this->viewProj[ 0 ][ 3 ] + this->viewProj[ 0 ][ 2 ];
	this->nearPlane[1]	= this->viewProj[ 1 ][ 3 ] + this->viewProj[ 1 ][ 2 ];
	this->nearPlane[2]	= this->viewProj[ 2 ][ 3 ] + this->viewProj[ 2 ][ 2 ];
	this->nearPlane[3]	= this->viewProj[ 3 ][ 3 ] + this->viewProj[ 3 ][ 2 ];

	invLen = invSqrt( this->nearPlane[0] * this->nearPlane[0] + this->nearPlane[1] * this->nearPlane[1] + this->nearPlane[2] * this->nearPlane[2] + this->nearPlane[3] * this->nearPlane[3] );
	this->nearPlane[0]	*= invLen;	
	this->nearPlane[1]	*= invLen;	
	this->nearPlane[2]	*= invLen;	
	this->nearPlane[3]	*= invLen;

	return ;
}

bool camera::isVertexInFrustum( Vector3 vertex, Vector3 origin )
{

	Vector4		*planes = &this->nearPlane;
	int			i;

	for( i = 0; i < 6; i ++ )
	{
		if( planes[i][0] * (vertex[0] + origin[0]) + planes[i][1] * (vertex[1] + origin[1]) + planes[i][2] * (vertex[2] + origin[2]) + planes[i][3] < 0 )
			return false;
	}

	return true;
}

bool camera::isVertexInFrustum( Vector3 vertex )
{

	Vector4		*planes = &this->nearPlane;
	int			i;

	for( i = 0; i < 6; i ++ )
	{
		if( planes[i][0] * vertex[0] + planes[i][1] * vertex[1] + planes[i][2] * vertex[2] + planes[i][3] < 0 )
			return false;
	}

	return true;
}

int camera::isBoxInFrustum( Vector3 a, Vector3 b ) { 
	
	int	totalIn = 0, inCount, ptIn, p;
	Vector4		*planes = &this->nearPlane;

	for( p = 0; p < 6; p ++ )
	{
		inCount = 8;
		ptIn	= 1;
	
		// если за плоскостью
		if( planes[p][0] * a[0] + planes[p][1] * b[1] + planes[p][2] * b[2] + planes[p][3] < 0 ) {
			ptIn	= 0;
			inCount	--;
		}

		// если за плоскостью
		if( planes[p][0] * a[0] + planes[p][1] * b[1] + planes[p][2] * a[2] + planes[p][3] < 0 ) {
			ptIn	= 0;
			inCount	--;
		}

		// если за плоскостью
		if( planes[p][0] * b[0] + planes[p][1] * b[1] + planes[p][2] * a[2] + planes[p][3] < 0 ) {
			ptIn	= 0;
			inCount	--;
		}

		// если за плоскостью
		if( planes[p][0] * b[0] + planes[p][1] * b[1] + planes[p][2] * b[2] + planes[p][3] < 0 ) {
			ptIn	= 0;
			inCount	--;
		}

		// если за плоскостью
		if( planes[p][0] * a[0] + planes[p][1] * a[1] + planes[p][2] * b[2] + planes[p][3] < 0 ) {
			ptIn	= 0;
			inCount	--;
		}

		// если за плоскостью
		if( planes[p][0] * a[0] + planes[p][1] * a[1] + planes[p][2] * a[2] + planes[p][3] < 0 ) {
			ptIn	= 0;
			inCount	--;
		}

		// если за плоскостью
		if( planes[p][0] * b[0] + planes[p][1] * a[1] + planes[p][2] * a[2] + planes[p][3] < 0 ) {
			ptIn	= 0;
			inCount	--;
		}

		// если за плоскостью
		if( planes[p][0] * b[0] + planes[p][1] * a[1] + planes[p][2] * b[2] + planes[p][3] < 0 ) {
			ptIn	= 0;
			inCount	--;
		}
		
		if( inCount == 0 )
			return OUT_FRUSTUM;
	
		totalIn += ptIn;
	}

	if( totalIn == 6 )
		return IN_FRUSTUM;
	
	return INTERSECT_FRUSTUM;
}

int	camera::isBoxInFrustum( Vector3 bbox[8] )
{
	int	totalIn = 0, inCount, ptIn, p, i;
	Vector4		*planes = &this->nearPlane;

	for( p = 0; p < 6; p ++ )
	{
		inCount = 8;
		ptIn	= 1;
	
		for( i = 0; i < 8; i ++ )
		{
			// если за плоскостью
			if( planes[p][0] * bbox[i][0] + planes[p][1] * bbox[i][1] + planes[p][2] * bbox[i][2] + planes[p][3] < 0 )
			{
				ptIn	= 0;
				inCount	--;
			}
		}
		
		if( inCount == 0 )
			return OUT_FRUSTUM;
	
		totalIn += ptIn;
	}

	if( totalIn == 6 )
		return IN_FRUSTUM;
	
	return INTERSECT_FRUSTUM;
}

int	camera::isBoxInFrustum( Vector3 bbox[8], Vector3 origin )
{
	int	totalIn = 0, inCount, ptIn, p, i;
	Vector4		*planes = &this->nearPlane;

	for( p = 0; p < 6; p ++ )
	{
		inCount = 8;
		ptIn	= 1;
	
		for( i = 0; i < 8; i ++ )
		{
			// если за плоскостью
			if( planes[p][0] * (bbox[i][0] + origin[0]) + planes[p][1] * (bbox[i][1] + origin[1]) + planes[p][2] * (bbox[i][2] + origin[2]) + planes[p][3] < 0 )
			{
				ptIn	= 0;
				inCount	--;
			}
		}
		
		if( inCount == 0 )
			return OUT_FRUSTUM;
	
		totalIn += ptIn;
	}

	if( totalIn == 6 )
		return IN_FRUSTUM;
	
	return INTERSECT_FRUSTUM;
}

int	camera::isSphereInFrustum( Vector4 sphere )
{

	Vector4		*planes = &this->nearPlane;
	float		dist;
	int			p;

	for( p = 0; p < 6; p ++ )
	{
		dist	= planes[p][0] * sphere[0] + planes[p][1] * sphere[1] + planes[p][2] * sphere[2] + planes[p][3];
		if( dist < - sphere[3] )
			return OUT_FRUSTUM;

		if( fabs( dist ) < sphere[3] )
			return INTERSECT_FRUSTUM;
	}

	return IN_FRUSTUM;
}

int	camera::isSphereInFrustum( Vector4 sphere, Vector3 origin )
{

	Vector4		*planes = &this->nearPlane;
	float		dist;
	int			p;

	for( p = 0; p < 6; p ++ )
	{
		dist	= planes[p][0] * (sphere[0] + origin[0]) + planes[p][1] * (sphere[1] + origin[1]) + planes[p][2] * (sphere[2] + origin[2]) + planes[p][3];
		if( dist < - sphere[3] )
			return OUT_FRUSTUM;

		if( fabs( dist ) < sphere[3] )
			return INTERSECT_FRUSTUM;
	}

	return IN_FRUSTUM;
}

void camera::getRay( float cursorX, float cursorY, Vector3 rayDot, Vector3 rayDir )
{
	return this->getRay( cursorX, cursorY, this->viewport.Width, this->viewport.Height, rayDot, rayDir );
}

void camera::getRay( float cursorX, float cursorY, DWORD screenWidth, DWORD screenHeight, Vector3 rayDot, Vector3 rayDir )
{
	
	float	cx, cy;

	cx				= ( ( ( 2.0f * cursorX ) / screenWidth ) - 1.0f ) / this->proj[0][0];
	cy				= - ( ( ( 2.0f * cursorY ) / screenHeight ) - 1.0f ) / this->proj[1][1];

	rayDir[0]		= this->state[0][0] * cx;
	rayDir[1]		= this->state[1][0] * cx;
	rayDir[2]		= this->state[2][0] * cx;

	rayDir[0]		+= this->state[0][1] * cy;
	rayDir[1]		+= this->state[1][1] * cy;
	rayDir[2]		+= this->state[2][1] * cy;

	rayDir[0]		+= this->state[0][2];
	rayDir[1]		+= this->state[1][2];
	rayDir[2]		+= this->state[2][2];
	
	normalizeVector( rayDir );
	copyVector3( this->frustumApex, rayDot );

	return;
}

float camera::getIntersectYFromPlane( Vector3 cameraPos, Vector3 p0, Vector3 p1, Vector3 p2 ) { 
	
	Vector3 p, p0p1, p0p2;
	
	subVector3( cameraPos, p0, p);
	subVector3( p1, p0, p0p1 );
	subVector3( p2, p0, p0p2 );

	// p[0]			  y - p0[1]     p[2]
	// p0p1[0]         p0p1[1]      p0p1[2]
	// p0p2[0]         p0p2[1]      p0p2[2]

	return (p[0] * ( p0p1[1] * p0p2[2] - p0p2[1] * p0p1[2] ) + p[2] * ( p0p1[0] * p0p2[1] - p0p2[0] * p0p1[1] )) / ( p0p1[0] * p0p2[2] - p0p2[0] * p0p1[2] ) + p0[1];
} 