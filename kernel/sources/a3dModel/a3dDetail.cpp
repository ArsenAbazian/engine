#include "a3dDetail.h"
#include "a3dObject.h"
#include "a3dModel.h"
#include "..\\vertexInfo.h"
#include "..\\radiosity.h"
#include "..\\a3dTextModelFile.h"
#include "..\\a3dModelRef\\a3dDetailRef.h"
#include "..\\a3dMaterial\\a3dMultiMaterial.h"
#include "..\\a3dMaterial\\a3dShaderMaterial.h"
#include "..\\a3dDesc\\a3dEffectDesc.h"
#include <d3dx9effect.h>
#include <dxerr9.h>
#include "..\\NCamera.h"
#include "..\\a3dUtils\\a3dTextUtils.h"

#define DEBUG_MODEL
#define intsize		sizeof ( int )

void a3dDetail::initializeDefault() { 

	this->kernel = null;
	this->object = null;
	this->faces = null;
	this->faceCount = 0;
	this->indexBuffer = null;
}

void a3dDetail::clear() {

	this->kernel = null;
	this->object = null;

	if( this->faces != null ) delete[] this->faces;
	this->faceCount = 0;

	if( this->indexBuffer != null ) this->indexBuffer->Release();
	this->indexBuffer = null;
}

void a3dDetail::setObject(a3dObject *obj ) {

	 this->object = obj;
}

bool a3dDetail::import( a3dTextModelFile &scFile, a3dMesh &mesh, int index ) { 

	if( !this->importFaces( scFile, mesh, index ) ) return false;
	if( !this->importMaterial( scFile, mesh, index ) ) return false;

	return true;
}

bool a3dDetail::importMaterial( a3dTextModelFile &scFile, a3dMesh &mesh, int index ) {

	a3dMaterialBase *base = scFile.getMaterial( mesh.materialName );
	a3dMultiMaterial *mat;

	if( base->type != MT_MULTI ) this->materialName = mesh.materialName;
	else { 
		mat = (a3dMultiMaterial *)base;
		this->materialName = mat->subMaterialNameList[index];
	}

	return true;
}

bool a3dDetail::importFaces( a3dTextModelFile &scFile, a3dMesh &mesh, int index ) { 

	a3dMaterialBase *base = scFile.getMaterial( mesh.materialName );
	a3dMultiMaterial *mat;

	if( base->type != MT_MULTI ) { 
		this->faceCount = mesh.faceCount;
	}
	else {
		mat = (a3dMultiMaterial*)base;
		this->faceCount = mesh.getSubMaterialFaceCount( mat->subMaterialIdList[index] );
	}

	this->faces = new a3dFace[ this->faceCount ];

	if( base->type != MT_MULTI ) {
		memcpy( this->faces, mesh.faceList, sizeof( a3dFace ) * this->faceCount );
	}
	else { 
		mesh.fillSubMaterialFaceList( this->faces, mat->subMaterialIdList[index] );
	}

	return true;
}

bool a3dDetail::supportSave() { return this->faceCount > 0; }

int a3dDetail::calcSizeCore() { 

	int totalSize = 0;

	totalSize +=	this->materialName.calcSize() // size
		+ intsize		// faceCount
		+ sizeof( a3dFace ) * this->faceCount 
		+ this->lightMapInfo.calcSize();

	return totalSize;
} 

bool a3dDetail::loadFromMemoryCore( a3dMemoryFile &file ) { 

	if( !this->materialName.loadFromMemory( file ) ) return false;
	if( !this->lightMapInfo.loadFromMemory( file ) ) return false;
	if( !file.readInt( &this->faceCount ) ) return false;

	if( this->faceCount > 0 )
		this->faces = new a3dFace[ this->faceCount ];

	if( !file.readMemBlock( (BYTE*) this->faces, sizeof( a3dFace ) * this->faceCount ) ) return false;

	return true;
}

bool a3dDetail::saveToMemoryCore(a3dMemoryFile &file) { 

	if( !this->materialName.saveToMemory( file ) ) return false;
	if( !this->lightMapInfo.saveToMemory( file ) ) return false;
 	if( !file.writeInt( this->faceCount ) ) return false;

	if( this->faces == null && !this->readDataFromBuffer() ) return false;
	if( !file.writeMemBlock( (BYTE*)this->faces, sizeof( a3dFace ) * this->faceCount ) ) return false;

	return true;
}

HRESULT a3dDetail::createRenderBuffer() { 

	HRESULT rv;

	if( this->kernel == null ) return D3DERR_INVALIDCALL;

	rv = this->kernel->lpDevice->CreateIndexBuffer( sizeof( WORD ) * 3 * this->faceCount, 
		0,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&this->indexBuffer,
		null
		);

	if( FAILED( rv ) ) { 
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error : can't create indexBuffer (%s)" ), DXGetErrorString9( rv ) );
		this->kernel->ErrorBox( TEXT("a3dDetail::createRenderBuffer") );
		a3dOutput::Default->printError(TEXT("a3dEffectDesc->\n%s\n"), Kernel::tempBuffer);
		return rv;
	}

	return D3D_OK;
}

HRESULT a3dDetail::fillRenderBuffer() { 

	HRESULT rv;
	WORD *tmp;

	if( this->kernel == null ) return D3DERR_INVALIDCALL;

	rv = this->indexBuffer->Lock( 0, 0, (void**)&tmp, 0 );
	if( FAILED( rv ) ) { 
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error : can't lock indexBuffer (%s)" ), DXGetErrorString9( rv ) );
		this->kernel->ErrorBox( TEXT("a3dDetail::createRenderBuffer") );
		a3dOutput::Default->printError(TEXT("a3dEffectDesc->\n%s\n"), Kernel::tempBuffer);
		return rv;
	}

	int fi;
	for( fi = 0; fi < this->faceCount; fi ++, tmp += 3 ) { 
		tmp[0] = (WORD)this->faces[fi].aIndex;
		tmp[1] = (WORD)this->faces[fi].bIndex;
		tmp[2] = (WORD)this->faces[fi].cIndex;
	}

	rv = this->indexBuffer->Unlock();
	if( FAILED( rv ) ) { 
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error : can't unlock indexBuffer (%s)" ), DXGetErrorString9( rv ) );
		this->kernel->ErrorBox( TEXT("a3dDetail::createRenderBuffer") );
		a3dOutput::Default->printError(TEXT("a3dEffectDesc->\n%s\n"), Kernel::tempBuffer);
		return rv;
	}

	return D3D_OK;
}

HRESULT a3dDetail::clearRenderBuffer() { 

	if( this->indexBuffer == null ) return D3D_OK;
	this->indexBuffer->Release(); 
	this->indexBuffer = null;

	return D3D_OK;
}

bool a3dDetail::readDataFromBuffer() { 

	HRESULT rv;
	WORD *tmp;

	if( this->kernel == null ) return false;
	if( this->faces != null ) return true;

	rv = this->indexBuffer->Lock( 0, 0, (void**)&tmp, 0 );
	if( FAILED( rv ) ) { 
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error : can't lock indexBuffer (%s)" ), DXGetErrorString9( rv ) );
		this->kernel->ErrorBox( TEXT("a3dDetail::createRenderBuffer") );
		a3dOutput::Default->printError(TEXT("a3dEffectDesc->\n%s\n"), Kernel::tempBuffer);
		return false;
	}

	this->faces = new a3dFace[ this->faceCount ];

	int fi;
	for( fi = 0; fi < this->faceCount; fi ++, tmp += 3 ) { 
		this->faces[fi].aIndex = tmp[0];
		this->faces[fi].bIndex = tmp[1];
		this->faces[fi].cIndex = tmp[2];
	}

	rv = this->indexBuffer->Unlock();
	if( FAILED( rv ) ) { 
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error : can't unlock indexBuffer (%s)" ), DXGetErrorString9( rv ) );
		this->kernel->ErrorBox( TEXT("a3dDetail::createRenderBuffer") );
		a3dOutput::Default->printError(TEXT("a3dEffectDesc->\n%s\n"), Kernel::tempBuffer);
		return false;
	}

	return true;
}

HRESULT a3dDetail::save() { 

	HRESULT rv; 

	if( !this->readDataFromBuffer() ) { 
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error: can't read data from render buffer" ) );
		this->kernel->ErrorBox( TEXT( "a3dDetail" ) );
		a3dOutput::Default->printError(TEXT("a3dEffectDesc->\n%s\n"), Kernel::tempBuffer);
		return D3DERR_INVALIDCALL;	
	}

	rv = this->clearRenderBuffer();
	if( FAILED( rv  ) ) { 
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error: can't clear render buffer (%s)" ), DXGetErrorString9( rv ) );
		this->kernel->ErrorBox( TEXT( "a3dDetail" ) );
		a3dOutput::Default->printError(TEXT("a3dEffectDesc->\n%s\n"), Kernel::tempBuffer);
		return D3DERR_INVALIDCALL;	
	}

	return D3D_OK;
}

HRESULT a3dDetail::render( camera *cam, Matrix4 WorldXf, Matrix4 WvpXf, Matrix4 WorldITXf ) { 
	return this->render( cam, WorldXf, WvpXf, WorldITXf, null );
}

HRESULT a3dDetail::render( camera *cam, Matrix4 WorldXf, Matrix4 WvpXf, Matrix4 WorldITXf, a3dDetailRef *detailRef ) { 

	HRESULT rv;
	a3dShaderMaterial *mat = detailRef != null? &(detailRef->material): this->material;
	
	// эти без ошибок так как не все эффекты содержат данные матрицы
	rv = mat->effect->lpEffect->SetMatrix( (D3DXHANDLE)mat->worldXfParam, (D3DXMATRIX*)WorldXf );
	rv = mat->effect->lpEffect->SetMatrix( (D3DXHANDLE)mat->wvpXfParam, (D3DXMATRIX*)WvpXf );
	rv = mat->effect->lpEffect->SetMatrix( (D3DXHANDLE)mat->worldITXfParam, (D3DXMATRIX*)WorldITXf );
	rv = mat->effect->lpEffect->SetMatrix( (D3DXHANDLE)mat->viewIXfParam, (D3DXMATRIX*)cam->iView );

	rv = mat->applyParameters();
	if( FAILED( rv ) )
		return rv;

	UINT uPasses, iPass;
	rv = mat->effect->lpEffect->Begin( &uPasses, 0 );
	if( FAILED( rv ) )
		return rv;

	for( iPass = 0; iPass < uPasses; iPass ++ ) { 

#if D3D_SDK_VERSION >= 32
		rv = mat->effect->lpEffect->BeginPass( iPass );
#else	
		rv = mat->effect->lpEffect->Pass( iPass );
#endif	
		if( FAILED( rv ) )
			return rv;

		rv = this->kernel->lpDevice->SetIndices( this->indexBuffer );
		if(FAILED(rv))
			return rv;

		rv = this->kernel->lpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, this->object->vertexCount, 0, this->faceCount );
		if(FAILED(rv))
			return rv;

#if D3D_SDK_VERSION >= 32
		rv = mat->effect->lpEffect->EndPass();
		if( FAILED( rv ) )
			return rv;
#endif	
	}

	rv = mat->effect->lpEffect->End();
	if( FAILED( rv ) )
		return rv;

	return D3D_OK;
}

HRESULT a3dDetail::restore() { 

	HRESULT rv; 

	rv = this->createRenderBuffer();
	if( FAILED( rv ) ) { 
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error: can't create render buffer (%s)" ), DXGetErrorString9( rv ) );
		this->kernel->ErrorBox( TEXT( "a3dDetail" ) );
		return rv;
	}

	rv = this->fillRenderBuffer();
	if( FAILED( rv ) ) {
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error: can't fill render buffer (%s)" ), DXGetErrorString9( rv ) );
		this->kernel->ErrorBox( TEXT( "a3dDetail" ) );
		return rv;
	}

	delete[] this->faces;
	this->faces = null;

	this->material = dynamic_cast<a3dShaderMaterial*>( this->object->model->getMaterialByName( this->materialName ) );

	return D3D_OK;
}

void a3dDetail::fillLightMapPlaneCollection( a3dLightMapPlaneCollection *coll, int textureIndex, processLightMapPlaneDeletate fp, void *lpData ) { 
	
	this->fillLightMapPlaneCollection(coll, textureIndex, fp, lpData, this->object->basis);
}

void a3dDetail::fillLightMapPlaneCollection( a3dLightMapPlaneCollection *coll, int textureIndex, processLightMapPlaneDeletate fp, void *lpData, Matrix4 basis ) { 

	bool saved = false;
	if( this->object->vertexData == null ) {
		this->object->save();
		saved = true;
	}
	if( this->object->normals == null )
		this->object->calcNormals();

	if( this->object->vertexData == null ) return;

	int i;
	for( i = 0; i < this->faceCount; i++ ) { 
		this->initializeLightMapPlane( &coll->plane[i], i, textureIndex, fp, lpData, basis );
	}
	
	if(saved)
		this->object->restore();
}

void a3dDetail::initializeLightMapPlane( a3dLightMapPlaneInfo *info, int faceIndex, int textureIndex, processLightMapPlaneDeletate fp, void *lpData) {
	this->initializeLightMapPlane(info, faceIndex, textureIndex, fp, lpData, this->object->basis);
}

void a3dDetail::initializeLightMapPlane( a3dLightMapPlaneInfo *info, int faceIndex, int textureIndex, processLightMapPlaneDeletate fp, void *lpData, Matrix4 basis ) { 

	float *vertex; 

	vertex = this->object->vertexInfo.getXYZ( this->object->getVertexData( this->faces[ faceIndex ].aIndex ) );
	if( vertex != null )
		copyVector3( vertex, info->a );

	vertex = this->object->vertexInfo.getXYZ( this->object->getVertexData( this->faces[ faceIndex ].bIndex ) );
	if( vertex != null ) 
		copyVector3( vertex, info->b );

	vertex = this->object->vertexInfo.getXYZ( this->object->getVertexData( this->faces[ faceIndex ].cIndex ) );
	if( vertex != null )
		copyVector3( vertex, info->c );


	vertex = this->object->vertexInfo.getNormal( this->object->getVertexData( this->faces[ faceIndex ].aIndex ) );
	if( vertex != null ) 
		copyVector3( vertex, info->na );
	else	
		copyVector3( this->object->normals[ this->faces[ faceIndex ].aIndex ], info->na ); // object normals must be calculated

	vertex = this->object->vertexInfo.getNormal( this->object->getVertexData( this->faces[ faceIndex ].bIndex ) );
	if( vertex != null ) 
		copyVector3( vertex, info->nb );
	else
		copyVector3( this->object->normals[ this->faces[ faceIndex ].bIndex ], info->nb ); // object normals must be calculated

	vertex = this->object->vertexInfo.getNormal( this->object->getVertexData( this->faces[ faceIndex ].cIndex ) );
	if( vertex != null ) 
		copyVector3( vertex, info->nc );
	else
		copyVector3( this->object->normals[ this->faces[ faceIndex ].cIndex ], info->nc ); // object normals must be calculated

	vertex = this->object->vertexInfo.getTex( this->object->getVertexData( this->faces[ faceIndex ].aIndex ), textureIndex );
	if( vertex != null ) 
		copyVector3( vertex, info->ta );

	vertex = this->object->vertexInfo.getTex( this->object->getVertexData( this->faces[ faceIndex ].bIndex ), textureIndex );
	if( vertex != null ) 
		copyVector3( vertex, info->tb );

	vertex = this->object->vertexInfo.getTex( this->object->getVertexData( this->faces[ faceIndex ].cIndex ), textureIndex );
	if( vertex != null ) 
		copyVector3( vertex, info->tc );

	if( fp != null )
		fp( info, lpData );

	info->translate( basis, false );

	return ;
}

a3dLightMapPlaneCollection* a3dDetail::getLightMapPlaneCollection( int textureIndex, processLightMapPlaneDeletate fp, void *lpData ) { 
	return this->getLightMapPlaneCollection(textureIndex, fp, lpData, this->object->basis);
}

a3dLightMapPlaneCollection* a3dDetail::getLightMapPlaneCollection( int textureIndex, processLightMapPlaneDeletate fp, void *lpData, Matrix4 basis ) { 

	if( this->faceCount == 0 ) return null;

	a3dLightMapPlaneCollection *coll = new a3dLightMapPlaneCollection( this->faceCount );
	this->fillLightMapPlaneCollection( coll, textureIndex, fp, lpData, basis );

	return coll;
}

bool a3dDetail::hasLightMap() { 

	return this->lightMapInfo.enabled;
}

a3dLightMap* a3dDetail::getLightMap( processLightMapPlaneDeletate fp, void *lpData, a3dLightMapInfo *lmInfo) {
	
	return this->getLightMap(fp, lpData, lmInfo, this->object->basis);
}

a3dLightMap* a3dDetail::getLightMap( processLightMapPlaneDeletate fp, void *lpData, a3dLightMapInfo *lmInfo, Matrix4 basis ) { 

	a3dLightMapPlaneCollection *coll = this->getLightMapPlaneCollection( lmInfo->textureCoorIndex, fp, lpData, basis );
	if( coll == null) return null;

	return new a3dLightMap( lmInfo->lightMapWidth, lmInfo->lightMapHeight, coll, lmInfo->lightMapFileName ); 
}

a3dLightMap* a3dDetail::getLightMap( processLightMapPlaneDeletate fp, void *lpData ) { 

	return this->getLightMap( fp, lpData, &this->lightMapInfo );
}

a3dDetailRef* a3dDetail::createReference( a3dObjectRef *objectRef ) { 
	return new a3dDetailRef( objectRef, this );
}


#undef DEBUG_MODEL
#undef intsize