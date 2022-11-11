#include "treeManager.h"
#include <dxerr9.h>
#include <stdlib.h>
#include "hlslman.h"

void a3dTree::clear() { 
	this->manager = null;
	this->fileName.clear();
	this->name.clear();
	this->vertexBeginPos = 0;
	this->indexBeginPos = 0;
	this->vertexCount = 0;
	this->indexCount = 0;
	this->partsCount = 0;
	delete[] this->vertex;
	delete[] this->index;
	delete[] this->parts;
}

void a3dTree::restore( treeVertex *vertexBuffer, WORD *indexBuffer, int vertexIndex, int indexIndex ) { 
	
	this->vertexBeginPos = vertexIndex;
	this->indexBeginPos = indexIndex;

	int i, vi = this->vertexBeginPos, ii = this->indexBeginPos;
	memcpy( &vertexBuffer[ this->vertexBeginPos ], this->vertex, this->vertexCount * sizeof( treeVertex ) );
	for( i = 0; i < this->indexCount; i ++ ) this->index[ i ] += this->indexBeginPos;
	memcpy( &indexBuffer[ this->indexBeginPos ], this->index, this->indexCount * sizeof( indexIndex ) );	
	for( i = 0; i < this->partsCount; i ++ ) { 
		this->parts[i].vertexBeginPos += this->vertexBeginPos;
		this->parts[i].indexBeginPos += this->indexBeginPos;
	}
	
	delete[] this->vertex;
	delete[] this->index;
	this->vertex = null;
	this->index = null;
	
	return;
} 

void a3dTree::save( treeVertex *vertexBuffer, WORD *indexBuffer, int vertexIndex, int indexIndex ) { 
	
	this->vertexBeginPos = vertexIndex;
	this->indexBeginPos = indexIndex;

	this->vertex = new treeVertex[ this->vertexCount ];
	this->index = new WORD[ this->indexCount ];
	
	int i, vi = this->vertexBeginPos, ii = this->indexBeginPos;
	memcpy( this->vertex, &vertexBuffer[ this->vertexBeginPos ], this->vertexCount * sizeof( treeVertex ) );
	memcpy( this->index, &indexBuffer[ this->indexBeginPos ], this->indexCount * sizeof( indexIndex ) );	
	for( i = 0; i < this->indexCount; i ++ ) this->index[ i ] -= this->indexBeginPos;
	for( i = 0; i < this->partsCount; i ++ ) { 
		this->parts[i].vertexBeginPos -= this->vertexBeginPos;
		this->parts[i].indexBeginPos -= this->indexBeginPos;
	}
	
	this->vertexBeginPos = 0;
	this->indexBeginPos = 0;

	return;
}

void a3dTree::calcVertexIndexCount( File3DS &f3ds ) { 
	
	int i;
	
	this->vertexCount = this->indexCount = 0;
	for( i = 0; i < f3ds.get_real_obj_count(); i++ )
	{
		// на каждый объект возможен только один материал
		if(f3ds.Object[i].MLCount == 0) continue;
		this->vertexCount += f3ds.Object[i].VCount;
		this->indexCount += f3ds.Object[i].MList[0].FCount * 3; // число треугольников * 3
	}

	return;
}

void a3dTree::copyVertices( treePartInfo *part, File3DS &f3ds, int objectIndex ) { 
	
	int i;
	Vertex3DS *v;

	// копируем вершины
	for( i = 0; i < part->vertexCount; i ++ ) { 
		v = &f3ds.Object[ objectIndex ].Vert[ i ];
		setVector3(this->vertex[ part->vertexBeginPos + i ].pos, v->SCoor.x, v->SCoor.y, v->SCoor.z );
		this->vertex[ part->vertexBeginPos + i ].tex[0] = v->TCoor.u;
		this->vertex[ part->vertexBeginPos + i ].tex[1] = v->TCoor.v;
	} 

	// копируем индексы
	int faceCount = part->indexCount / 3;
	for( i = 0; i < faceCount; i ++ ) {
		this->index[ part->indexBeginPos + i * 3 ] = f3ds.Object[ objectIndex ].Face[ f3ds.Object[ objectIndex ].MList[ 0 ].F_index[ i ] ].Va_indx;
		this->index[ part->indexBeginPos + i * 3 + 1 ] = f3ds.Object[ objectIndex ].Face[ f3ds.Object[ objectIndex ].MList[ 0 ].F_index[ i ] ].Vb_indx;
		this->index[ part->indexBeginPos + i * 3 + 2 ] = f3ds.Object[ objectIndex ].Face[ f3ds.Object[ objectIndex ].MList[ 0 ].F_index[ i ] ].Vc_indx;
	}

	// отлично
	return ;
}

void a3dTree::fillBuffers( File3DS &f3ds ) { 

	Material3DS *mat;
	int i, objIndex;
	
	int bvp = 0, bip = 0;
	for( i = 0; i < this->partsCount; i ++ ) {
		objIndex = f3ds.getRealObjectByIndex( i );
		this->parts[ i ].vertexBeginPos = bvp;
		this->parts[ i ].indexBeginPos = bip;
		this->parts[ i ].vertexCount = f3ds.Object[ objIndex ].VCount;
		this->parts[ i ].indexCount = f3ds.Object[ objIndex ].MList[ 0 ].FCount * 3;
		bvp += this->parts[ i ].vertexCount;
		bip += this->parts[ i ].indexCount;
		this->copyVertices( &this->parts[ i ], f3ds, objIndex );
		mat = f3ds.getMaterialByName( f3ds.Object[ objIndex ].MList[ 0 ].Name );
		if(mat == NULL) continue;
		this->parts[ i ].textureName = mat->Texture1.Name;
	}

	// данные структуры должны быть идентичны	
	if( sizeof(T1Vertex) != sizeof( treeVertex ) && this->manager != null) {
		MessageBox(this->manager->tkrn->hWnd, TEXT("sizeof(T1Vertex) != sizeof( treeVertex )"), TEXT( "a3dTree::fillBuffers" ), MB_ICONERROR );
	}
	
	// высчитываем нормали
	calcPolygonNormals( (T1Vertex*)this->vertex, (Face*)f3ds.Object[objIndex].Face, f3ds.Object[ objIndex ].MList[ 0 ].FCount, f3ds.Object[objIndex].VCount );
	
	return;
}

bool a3dTree::initialize( shString &fileName, shString &name, a3dTreeType type ) { 
	
	File3DS			f3ds;
	
	// если было загружено хоть какое дерево то очистить
	if( this->fileName.getLength() != 0 )
		this->clear();
	
	this->fileName	= fileName;
	this->name		= name;
	this->type		= type;

	// не смогли загрузить
	if( !f3ds.Load( this->fileName.getCString() ))
		return false;

	f3ds.correct_for_dx();
	
	// число реальных объектов дерева
	this->partsCount = f3ds.get_real_obj_count();
	this->parts = new treePartInfo[ this->partsCount ];
	ZeroMemory( this->parts, sizeof( treePartInfo ) * this->partsCount );

	// подсчитываем общее число вершин и индексов
	this->calcVertexIndexCount(f3ds);

	// части дерева начинаются с treePart а дальше индекс от 1 до ...
	this->vertex = new treeVertex[ this->vertexCount ];
	this->index = new WORD[ this->indexCount ];
	
	// заполняем
	this->fillBuffers( f3ds );

	return true;
}

void a3dTreeManager::calcVertexIndexCount() { 
	
	int i;
	this->vertexCount = 0;
	this->indexCount = 0;
	for( i = 0; i < this->treesCount; i ++ ) { 
		this->vertexCount += this->trees[ i ]->vertexCount;
		this->indexCount += this->trees[ i ]->indexCount;
	}
	
	return;
}

HRESULT a3dTreeManager::initialize() { 
	
	HRESULT rv;
	
	this->calcVertexIndexCount();
	
	rv = this->tkrn->lpDevice->CreateVertexBuffer( sizeof( treeVertex ) * this->vertexCount,
													0, 
													FVF_TREE,
													D3DPOOL_DEFAULT,
													&this->vertexBuffer,
													NULL
													);
	
	if( FAILED( rv ) ) { 
		_stprintf_s( this->tkrn->tempBuffer, this->tkrn->bufferSize, TEXT( "Error: CreateVertexBuffer -> %s" ), DXGetErrorString9( rv ) );
		this->tkrn->MessageBox( TEXT( "a3dTreeManager::initialize()"), MB_ICONERROR );
		return rv;
	}

	rv = this->tkrn->lpDevice->CreateIndexBuffer( sizeof( WORD ) * this->indexCount, 
													0,
													D3DFMT_INDEX16,
													D3DPOOL_DEFAULT,
													&this->indexBuffer,
													NULL );

	if( FAILED( rv ) ) { 
		_stprintf_s( this->tkrn->tempBuffer, this->tkrn->bufferSize, TEXT( "Error: CreateIndexBuffer -> %s" ), DXGetErrorString9( rv ) );
		this->tkrn->MessageBox( TEXT( "a3dTreeManager::initialize()"), MB_ICONERROR );
		return rv;
	}
	
	treeVertex *vertex;
	WORD		*index;
	
	rv = this->vertexBuffer->Lock( 0, 0, (void**)&vertex, D3DLOCK_DISCARD );
	if( FAILED( rv ) ) { 
		_stprintf_s( this->tkrn->tempBuffer, this->tkrn->bufferSize, TEXT( "Error: vertexBuffer->Lock -> %s" ), DXGetErrorString9( rv ) );
		this->tkrn->MessageBox( TEXT( "a3dTreeManager::initialize()"), MB_ICONERROR );
		return rv;
	}

	rv = this->indexBuffer->Lock( 0, 0, (void**)&index, D3DLOCK_DISCARD );
	if( FAILED( rv ) ) { 
		this->vertexBuffer->Unlock();
		_stprintf_s( this->tkrn->tempBuffer, this->tkrn->bufferSize, TEXT( "Error: indexBuffer->Lock -> %s" ), DXGetErrorString9( rv ) );
		this->tkrn->MessageBox( TEXT( "a3dTreeManager::initialize()"), MB_ICONERROR );
		return rv;
	}

	// копируем деревья в буффер
	int i, vi = 0, ii = 0;
	for( i = 0; i < this->treesCount; i ++ ) { 
		this->trees[ i ]->restore( vertex, index, vi, ii );
		vi += this->trees[ i ]->vertexCount;
		ii += this->trees[ i ]->indexCount;
	}
	
	this->vertexBuffer->Unlock();
	this->indexBuffer->Unlock();

	this->buildRenderGraph();
	
	return D3D_OK;
}

HRESULT a3dTreeManager::save() { 
	
	HRESULT rv;
	
	treeVertex *vertex;
	WORD		*index;
	
	rv = this->vertexBuffer->Lock( 0, 0, (void**)&vertex, D3DLOCK_DISCARD );
	if( FAILED( rv ) ) { 
		_stprintf_s( this->tkrn->tempBuffer, this->tkrn->bufferSize, TEXT( "Error: vertexBuffer->Lock -> %s" ), DXGetErrorString9( rv ) );
		this->tkrn->MessageBox( TEXT( "a3dTreeManager::save()"), MB_ICONERROR );
		return rv;
	}

	rv = this->indexBuffer->Lock( 0, 0, (void**)&index, D3DLOCK_DISCARD );
	if( FAILED( rv ) ) { 
		this->vertexBuffer->Unlock();
		_stprintf_s( this->tkrn->tempBuffer, this->tkrn->bufferSize, TEXT( "Error: indexBuffer->Lock -> %s" ), DXGetErrorString9( rv ) );
		this->tkrn->MessageBox( TEXT( "a3dTreeManager::save()"), MB_ICONERROR );
		return rv;
	}

	// копируем деревья в буффер
	int i, vi = 0, ii = 0;
	for( i = 0; i < this->treesCount; i ++ ) { 
		this->trees[ i ]->save( vertex, index, vi, ii );
		vi += this->trees[ i ]->vertexCount;
		ii += this->trees[ i ]->indexCount;
	}
	
	this->vertexBuffer->Unlock();
	this->indexBuffer->Unlock();

	this->vertexBuffer->Release();
	this->indexBuffer->Release();
	
	this->vertexBuffer = null;
	this->indexBuffer = null;

	return D3D_OK;
}

void a3dTreeManager::clear() { 
	
	if( this->vertexBuffer != null ) {
		this->vertexBuffer->Release();
		this->vertexBuffer = null;
	}
	if( this->indexBuffer != null ) {
		this->indexBuffer->Release();
		this->indexBuffer = null;
	}

	int i;
	for( i = 0; i < this->treesCount; i++ )
		delete this->trees[i];
	for( i = 0; i < this->treeRefsCount; i ++ )
		delete this->treeRefs[i];

	this->vertexCount = 0;
	this->indexCount = 0;
	this->treesCount = 0;
	this->treeRefsCount = 0;
	this->tkrn = null;
	
	return;
}

a3dTree* a3dTreeManager::addTree(shString &fileName, shString &name, a3dTreeType type) { 

	if( this->treesCount == maxTreesCount ) return null;

	a3dTree *tree = new a3dTree( this );
	if( !tree->initialize( fileName, name, type) ) { 
		delete tree;
		return null;
	}

	this->trees[ this->treesCount ] = tree;
	this->treesCount ++;

	return tree;
}

void a3dTreeRef::clear() { 
	
	int i; for( i = 0; i < this->partRefCount; i++ ) delete this->partRefs[ i ];
	this->partRefCount = 0;
	this->partsCount = 0;
	this->tree = null;

}

void a3dTreeManager::initSeed() { 
	srand( timeGetTime() );
}

int a3dTreeRef::randPart() { 
	
	float frnd = ((float)rand()) / RAND_MAX;
	return (int)(frnd * ( this->partsCount - 1 ) * 2);
}

void a3dTreeRef::initialize( a3dTree *tree, int partsCount ) { 
	
	this->clear();
	
	this->partsCount = partsCount;
	this->partRefCount = partsCount * 2 + 1; // основание всегда должно быть и не считается
	this->tree = tree;

	int i;
	for( i = 0; i < this->partRefCount; i++ ) { 
		this->partRefs[ i ] = new treePartInfoRef();
		this->partRefs[ i ]->treeRef = this;
	}

	// основание
	this->partRefs[ 0 ]->treePartIndex = 0;
	
	for( i = 0; i < this->partsCount; i ++ ) { 
		this->partRefs[ 1 + i * 2 ]->treePartIndex = this->randPart();
		// основание должно быть одно
		if( this->partRefs[ 1 + i * 2 ]->treePartIndex == 0 ) this->partRefs[ 1 + i * 2 ]->treePartIndex = 1; 
		this->partRefs[ 1 + i * 2 + 1]->treePartIndex = this->partRefs[ 1 + i * 2 ]->treePartIndex + 1;
	}
}

int a3dTreeManager::randPartCount( int maxPartsCount) {
	
	float frnd = ((float)rand()) / RAND_MAX;
	return (int)( frnd * maxPartsCount );
}

a3dTreeRef* a3dTreeManager::addTreeRef( int treeIndex, int maxPartsCount ) { 
	
	if( this->treesCount <= treeIndex ) return null;
	if( this->treeRefsCount >= maxTreeRefsCount ) return null;

	a3dTreeRef *ref = new a3dTreeRef();
	ref->initialize( this->trees[ treeIndex ], maxPartsCount );
	this->treeRefs[ this->treeRefsCount ] = ref;
	this->treeRefsCount ++;

	return ref;
}

void a3dTreeManager::buildRenderGraph() { 
	
	// очищаем предыдущий граф рендернига
	this->clearRenderGraph();
	std::list< std::list< treePartInfoRef* >* >::iterator lli;
	std::list< treePartInfoRef* >::iterator sli;

	if( this->treesCount == 0 || this->treeRefsCount == 0 ) return;
	int ti, pi, ri, rpi;
	
	std::list< treePartInfoRef* > *list = null;
	for( ti = 0; ti < this->treesCount; ti++ ) { 
		for( pi = 0; pi < this->trees[ ti ]->partsCount; pi ++ ) { 
			for( ri = 0; ri < this->treeRefsCount; ri ++ ) { 
				if( this->trees[ ti ] != this->treeRefs[ ri ]->tree ) continue;
				for( rpi = 0; rpi < this->treeRefs[ ri ]->getPartRefCount(); rpi++ ) { 
					if( pi != this->treeRefs[ ri ]->partRefs[ rpi ]->treePartIndex ) continue;
					if( list == null ) list = new std::list< treePartInfoRef* >();
					list->push_back( this->treeRefs[ ri ]->partRefs[ rpi ] );
				} 
			}
			if( list != null ) this->renderGraph.push_back( list );
			list = null;
		}
	}
	
	return;
}

void a3dTreeManager::clearRenderGraph() { 
	
	std::list< std::list< treePartInfoRef* >* >::iterator lli;
	for( lli = this->renderGraph.begin(); lli != this->renderGraph.end(); lli++ ) { 
		delete (*lli);
	}
	this->renderGraph.clear();
}

HRESULT	a3dTreeManager::render() { 
	
	HRESULT rv;

	if( this->lpEffect == null ) return D3D_OK;

	// подготавливаем камеру
	this->currCamera->preapreViewProj();
	
	rv = this->tkrn->lpDevice->SetStreamSource( 0, this->vertexBuffer, 0, sizeof( treeVertex ) );
	if( FAILED( rv ) )
		return rv;

	rv = this->tkrn->lpDevice->SetFVF( FVF_TREE );
	if( FAILED( rv ) )
		return rv;

	rv = this->tkrn->lpDevice->SetIndices( this->indexBuffer );
	if( FAILED( rv ) )
		return rv;
	
	UINT uPasses = 0, iPass;
	rv = lpEffect->Begin(&uPasses, 0);
	if(FAILED(rv))
		return rv;
	
	for( iPass = 0; iPass < uPasses; iPass ++ ) {

#if D3D_SDK_VERSION >= 32
		rv = this->lpEffect->BeginPass( iPass );
#else	
		rv = this->lpEffect->Pass( iPass );
#endif
		if(FAILED(rv))
			return rv;
		
		rv = this->renderGraphNodes();
		if( FAILED( rv ) )
			return rv;

#if D3D_SDK_VERSION >= 32
		rv = this->lpEffect->EndPass();
		if( FAILED( rv ) )
			return rv;
#endif
	}

	rv = lpEffect->End();
	if( FAILED(rv) )
		return rv;

	return D3D_OK;
}

HRESULT a3dTreeManager::renderGraphNodes() { 
	
	HRESULT rv;
	Matrix4 mat, mvp;
	treePartInfo *info;

	std::list< std::list< treePartInfoRef* >* >::iterator lli;
	std::list< treePartInfoRef* >::iterator sli;

	for( lli = this->renderGraph.begin(); lli != this->renderGraph.end(); lli ++ ) { 
		
		sli = (*lli)->begin(); 
		info = &((*sli)->treeRef->tree->parts[ (*sli)->treePartIndex ]);
		rv = this->lpEffect->SetTexture( (D3DXHANDLE)"ColorTexture", info->lpTexture );
		if( FAILED( rv ) )
			return rv;
		
		for( ; sli != (*lli)->end(); sli ++ ) { 
			
			// преобразовываем координаты
			mulMatrix4( (*sli)->treeRef->basis, (*sli)->basis, mat );
			mulMatrix4( mat, this->currCamera->viewProj, mvp );
			
			this->lpEffect->SetMatrix( (D3DXHANDLE)"WvpXf", (D3DXMATRIX*)mvp );
			
			rv = this->tkrn->lpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, info->vertexCount, info->indexBeginPos, info->indexCount / 3 );
			if( FAILED( rv ) )
				return rv;
		}
	}

	return D3D_OK;
}

void a3dTreeManager::clearEffect() { 
	
	lpEffect->Release();
	lpEffect = null;
	
	return;
}

HRESULT a3dTreeManager::createEffect( ) { 
	
	HLSLManager	hlslman;
	HRESULT rv;

	rv = hlslman.createEffectFromFile( this->tkrn->lpDevice, this->effectFileName );
	lpEffect = hlslman.lpEffect;
	hlslman.result.saveToFile(shString(TEXT("treeManager.log")));
	if(FAILED(rv))
		return rv;
	
	rv = this->lpEffect->OnResetDevice();
	if(FAILED(rv))
		return rv;

	return D3D_OK;
}

HRESULT a3dTreeManager::createEffect( shString &fileName )
{
	this->effectFileName = fileName;
	return this->createEffect(); 
}