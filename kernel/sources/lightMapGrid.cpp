#include "lightMapGrid.h"
#include "radiosity.h"
#include "a3dModel\\a3dMathObject.h"

void a3dLightMapGrid::initializeDefault() { 
	
	this->kernel = null;
	this->creator = null;
	this->vertexBuffer = null;
	this->objectWithBasis = null;
	this->currCamera = null;
	this->vertexCount = 0;
	this->pointSize = 1.0f;

	setVector3(this->color, 0xffffff);
}

void a3dLightMapGrid::clear(bool clearHlslManager) { 

	this->kernel = null;
	if(this->vertexBuffer != null)
		this->vertexBuffer->Release();
	
	if(clearHlslManager)
			this->effectManager.lpEffect->Release();
	
	this->initializeDefault();
}

void a3dLightMapGrid::saveClear() { 
	
	Kernel *krn = this->kernel;
	float pt = this->pointSize;
	Vector3 c; copyVector3(this->color, c);
	
	this->clear(false);
	
	this->kernel = krn;
	copyVector3( c, this->color );
	this->pointSize = pt;
}

void a3dLightMapGrid::preCalcLightMaps(a3dLightMap **lm, int lmCount) {
	
	for(int i = 0; i < lmCount; i++) {
		lm[i]->preCalcLightMap();
	}
}

int a3dLightMapGrid::calcVertexCount(a3dLightMap **lm, int lmCount) {

	int vCount = 0, lmIndex, row, col;
	
	for( lmIndex = 0; lmIndex < lmCount; lmIndex++ ) {
		for( row = 0; row < lm[lmIndex]->height; row++ ) {
			for( col = 0; col < lm[lmIndex]->width; col++ ) { 
				if(lm[lmIndex]->map[row][col].ownerPlane == null) continue;
				vCount++;
			}
		}
	}

	return vCount;
}

void a3dLightMapGrid::deleteLightMaps(a3dLightMap **lm, int lmCount) {
	
	for(int i = 0; i < lmCount; i++) { 
		delete lm[i];
	}
	delete[] lm;
}

HRESULT a3dLightMapGrid::createEffect() { 
	
	if(this->kernel == null) 
		return D3DERR_INVALIDCALL;
	if( this->effectManager.lpEffect != null )
		return D3D_OK;
	return this->effectManager.createEffect(this->kernel->lpDevice);
}

HRESULT a3dLightMapGrid::createRenderBuffer() { 

	if( this->kernel == null )
		return D3DERR_INVALIDCALL;
	
	return this->kernel->lpDevice->CreateVertexBuffer( sizeof(Vector3) * this->vertexCount, D3DUSAGE_WRITEONLY, D3DFVF_XYZ, D3DPOOL_SYSTEMMEM, &this->vertexBuffer, null );
}

HRESULT a3dLightMapGrid::openRenderBuffer( VOID **b ) { 
	
	return this->vertexBuffer->Lock( 0, 0, b, D3DLOCK_DISCARD );
}

HRESULT a3dLightMapGrid::closeRenderBuffer() { 
	return this->vertexBuffer->Unlock();
}

void a3dLightMapGrid::clearRenderBuffer() { 
	
	this->vertexBuffer->Release();
	this->vertexBuffer = null;
}

void a3dLightMapGrid::fillRenderBuffer(a3dLightMap **lm, int lmCount, Vector3 *v) {

	int lmIndex, row, col, vIndex = 0;

	this->objectWithBasis = dynamic_cast<a3dMathObject*>(this->creator);
	Vector4 v4;

	for( lmIndex = 0; lmIndex < lmCount; lmIndex++ ) {
		for( row = 0; row < lm[lmIndex]->height; row++ ) {
			for( col = 0; col < lm[lmIndex]->width; col++ ) { 
				if(lm[lmIndex]->map[row][col].ownerPlane == null) continue;
				if( this->objectWithBasis!= null) {
					mulVector3ToMatrix4(lm[lmIndex]->map[row][col].point, this->objectWithBasis->invertBasis, v4 );
					copyVector3(v4, v[vIndex]);
				}
				else
					copyVector3(lm[lmIndex]->map[row][col].point, v[vIndex]);
				vIndex++;
			}
		}
	}

	return;
}

HRESULT a3dLightMapGrid::setLightMapCreator(a3dLightMapCreator *crt ) {  
	
	this->saveClear();
	this->creator = crt;
	if( this->creator == null )
		return D3D_OK;

	int lmCount = this->creator->getLightMapCount();
	if(lmCount == 0) 
		return D3D_OK;

	a3dLightMap **lm = this->creator->getLightMaps();
	this->preCalcLightMaps(lm, lmCount);
	
	this->vertexCount = this->calcVertexCount(lm, lmCount);
	
	HRESULT rv = this->createRenderBuffer();
	if( FAILED(rv) ) {
		this->deleteLightMaps(lm, lmCount);
		return rv;
	}
	
	Vector3 *v;
	rv = this->openRenderBuffer( (VOID**) &v );
	if( FAILED(rv) ) {
		this->clearRenderBuffer();
		this->deleteLightMaps(lm, lmCount);
		return rv;
	}

	this->fillRenderBuffer(lm, lmCount, v);

	this->deleteLightMaps(lm, lmCount);
	rv = this->closeRenderBuffer();
	if( FAILED(rv) ) {
		this->clearRenderBuffer();
		return rv;
	}

	rv = this->createEffect();
	if( FAILED(rv) ) {
		this->clearRenderBuffer();
		return rv;
	}

	return D3D_OK;
}

HRESULT a3dLightMapGrid::render() { 
	
	HRESULT			rv;

	if( this->currCamera == null || this->vertexBuffer == null )
		return D3D_OK;

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

	rv = this->effectManager.lpEffect->SetFloatArray( "AmbiColor", this->color, 3 );  
	if( FAILED( rv ) )
		return rv;

	rv = this->effectManager.lpEffect->SetFloat( "GridPointSize", this->pointSize );
	if( FAILED( rv ) )
		return rv;

	rv = this->kernel->lpDevice->DrawPrimitive( D3DPT_POINTLIST, 0, this->vertexCount );
	if( FAILED( rv ) )
		return rv;

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