#include "grid.h"
#include <dxerr9.h>

void a3dGrid::initializeDefault() { 
	
	this->kernel = null;
	this->vertexBuffer = null;
	
	this->sideSize = 10.0f;
	this->smallStep = 0.1f;
	this->boldLineSteps = 10;

	setVector3( this->axisColor, 0.0f, 0.0f, 0.0f );
	setVector3( this->lineColor, 0.8f, 0.8f, 0.8f );
	setVector3( this->boldLineColor, 0.2f, 0.2f, 0.2f );
}

HRESULT a3dGrid::initialize( float sideSize, float smallStep, int boldLineStep ) { 

	this->clearBuffers();
	if( this->kernel == null ) return D3DERR_INVALIDCALL;

	HRESULT rv = this->kernel->lpDevice->CreateVertexBuffer( sizeof(Vector3) * this->getLinesCount() * 2, D3DUSAGE_WRITEONLY, D3DFVF_XYZ, D3DPOOL_MANAGED, &this->vertexBuffer, null );
	if( FAILED( rv ) ) { 
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT("Error: CreateVertexBuffer() -> %s"), DXGetErrorString9( rv ));	
		this->kernel->MessageBox( TEXT( "a3dBoundBoxRender" ), MB_ICONERROR );
		return rv;
	}

	Vector3 *data;
	rv = this->vertexBuffer->Lock(0, 0, (void**) &data, D3DLOCK_DISCARD);
	if( FAILED( rv ) ) { 
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT("Error: vertexBuffer->Lock() -> %s"), DXGetErrorString9( rv ));	
		this->kernel->MessageBox( TEXT( "a3dBoundBoxRender" ), MB_ICONERROR );
		return rv;
	}

	float halfSide = this->sideSize * 0.5f;
	setVector3( data[0], - halfSide, 0.0f, 0.0f );
	setVector3( data[1], halfSide, 0.0f, 0.0f );

	setVector3( data[2], 0.0f, 0.0f, - halfSide );
	setVector3( data[3], 0.0f, 0.0f, halfSide );

	this->fillBoldLines( &data[4] );
	this->fillLines( &data[ this->getBoldLineCount() * 2 + 4 ] );

	rv = this->vertexBuffer->Unlock();
	if( FAILED( rv ) ) { 
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT("Error: vertexBuffer->UnLock() -> %s"), DXGetErrorString9( rv ));	
		this->kernel->MessageBox( TEXT( "a3dBoundBoxRender" ), MB_ICONERROR );
		return rv;
	}
	
	rv = this->effectManager.createEffect( this->kernel->lpDevice );
	if( FAILED( rv ) ) { 
		this->kernel->MessageBox( this->effectManager.result.getCString(), MB_ICONERROR );
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT("Error: effectManager.createEffect -> %s"), DXGetErrorString9( rv ));	
		this->kernel->MessageBox( TEXT( "a3dGrid" ), MB_ICONERROR );
		return rv;
	}

	return rv;
}

int a3dGrid::getSimpleLineCount() { 
	
	return this->getLinesCount() - this->getBoldLineCount() - 2;
}

void a3dGrid::fillLines( Vector3 *data ) { 

	int li = 0, vIndex = 0;
	float val, halfSize = this->sideSize * 0.5f;
	
	for( val = 0.0f; val < halfSize - this->smallStep; val += this->smallStep, li ++) {  
		if( li % this->boldLineSteps == 0 ) continue;
		this->setLine( &data[vIndex], -val, -halfSize, -val, halfSize );
		vIndex += 2;
	}
	for( val = 0.0f; val < halfSize - this->smallStep; val += this->smallStep, li++ ) {
		if( li % this->boldLineSteps == 0 ) continue;
		this->setLine( &data[vIndex], val, -halfSize, val, halfSize );
		vIndex += 2;
	}
	for( val = 0.0f; val < halfSize - this->smallStep; val += this->smallStep, li++ ) {
		if( li % this->boldLineSteps == 0 ) continue;
		this->setLine( &data[vIndex], -halfSize, -val, halfSize, -val );
		vIndex += 2;
	}
	for( val = 0.0f; val < halfSize - this->smallStep; val += this->smallStep, li++ ) {
		if( li % this->boldLineSteps == 0 ) continue;
		this->setLine( &data[vIndex], -halfSize, val, halfSize, val );
		vIndex += 2;
	}
}

void a3dGrid::fillBoldLines( Vector3 *data ) { 

	int li, lineCount = this->getBoldLineCount() / 4;
	float bigStep = this->smallStep * this->boldLineSteps;
	float halfSide = this->sideSize * 0.5f;

	int vIndex = 0;
	for( li = 0; li < lineCount; li++, vIndex += 2 ) {
		this->setLine( &data[ vIndex ], -( li + 1 ) * bigStep, - halfSide, -( li + 1 ) * bigStep, halfSide );
	}
	for( li = 0; li < lineCount; li++, vIndex += 2 ) { 
		this->setLine( &data[ vIndex ], ( li + 1 ) * bigStep, - halfSide, ( li + 1 ) * bigStep, halfSide );		
	}
	for( li = 0; li < lineCount; li++, vIndex += 2 ) {
		this->setLine( &data[ vIndex ],  - halfSide, -( li + 1 ) * bigStep, halfSide, -( li + 1 ) * bigStep );
	}
	for( li = 0; li < lineCount; li++, vIndex += 2 ) {
		this->setLine( &data[ vIndex ],  - halfSide, ( li + 1 ) * bigStep, halfSide, ( li + 1 ) * bigStep );	
	}
}

void a3dGrid::setLine( Vector3 *line, float x0, float z0, float x1, float z1 ) { 

	setVector3( line[0], x0, 0.0f, z0 );
	setVector3( line[1], x1, 0.0f, z1 );
	
	return ;
}

void a3dGrid::clearBuffers() { 

	if( this->vertexBuffer != null ) this->vertexBuffer->Release();
	
	return ;
} 

void a3dGrid::clear() { 
	
	this->clearBuffers();
	this->initializeDefault();
}

int a3dGrid::getBoldLineCount() { 

	float halfSize = this->sideSize * 0.5f, step = this->smallStep * this->boldLineSteps;
	return (int) (halfSize / step * 4);
}

int a3dGrid::getLinesCount() {
	
	float halfSize = this->sideSize * 0.5f;
	return (int) (halfSize / this->smallStep + 0.5f) * 4 + 2;
}

HRESULT a3dGrid::render() { 

	HRESULT			rv;

	rv = this->effectManager.lpEffect->SetMatrix( "WvpXf", (D3DXMATRIX*) this->currCamera->viewProj );
	if( FAILED( rv ) )
		return rv;

	UINT uPass;
	rv = this->effectManager.lpEffect->Begin( &uPass, 0 );
	if( FAILED ( rv ) )
		return rv;

#if D3D_SDK_VERSION >= 32
	rv = this->effectManager.lpEffect->BeginPass( 0 );
#else	
	rv = this->effectManager.lpEffect->Pass( 0 );
#endif	
	if( FAILED ( rv ) )
		return rv;

	rv = this->kernel->lpDevice->SetStreamSource( 0, this->vertexBuffer, 0, sizeof( Vector3 ) );
	if( FAILED( rv ) )
		return rv;

	rv = this->kernel->lpDevice->SetFVF( D3DFVF_XYZ );
	if( FAILED( rv ) )
		return rv;

	// draw axis
	this->drawLines( this->axisColor, 0, 2 );
	
	// draw bold
	this->drawLines( this->boldLineColor, 4, this->getBoldLineCount() );
	
	// draw simple
	this->drawLines( this->lineColor, 4 + this->getBoldLineCount() * 2, this->getSimpleLineCount() );
	
#if D3D_SDK_VERSION >= 32
	rv = this->effectManager.lpEffect->EndPass();
	if( FAILED( rv ) )
		return rv;
#endif	

	rv = this->effectManager.lpEffect->End();
	if( FAILED ( rv ) )
		return rv;
	
	return D3D_OK;
}

HRESULT	a3dGrid::drawLines( Vector3 color, int startIndex, int count ) {

	HRESULT rv;
	
	rv = this->effectManager.lpEffect->SetFloatArray( "AmbiColor", color, 3 );  
	if( FAILED( rv ) )
		return rv;

	rv = this->kernel->lpDevice->DrawPrimitive( D3DPT_LINELIST, startIndex, count);
	if( FAILED( rv ) )
		return rv;
	
	return D3D_OK;
}