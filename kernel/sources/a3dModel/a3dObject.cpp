#include "a3dObject.h"
#include "a3dDetail.h"
#include "a3dModel.h"
#include "..\\a3dModelRef\\a3dObjectRef.h"
#include "..\\a3dTextModelFile.h"
#include "..\\a3dMaterial\\a3dMultiMaterial.h"
#include "..\\a3dMaterial\\a3dShaderMaterial.h"
#include <dxerr9.h>

#define DEBUG_MODEL
#define intsize		sizeof ( int )

void a3dObject::initializeDefault() { 

	this->kernel = null;
	this->model = null;

	this->detail = null;
	this->detailCount = 0;

	this->vertices = null;
	this->normals = null;
	this->vertexCount = 0;
	this->vertexData = null;
	this->vertexBuffer = null;
}

void a3dObject::clear() { 

	if( this->detail != null) delete[] this->detail;
	this->detail = null;
	this->detailCount = 0;

	if( this->vertices != null) delete[] this->vertices;
	if( this->normals != null) delete[] this->normals;
	this->vertices = null;
	this->normals = null;
	this->vertexCount = 0;

	if( this->vertexBuffer != null ) this->vertexBuffer->Release();
	this->vertexBuffer = null;

	this->clearVertexData();
}

void a3dObject::clearVertexData() {

	if( this->vertexData != null) delete[] this->vertexData;
	this->vertexData = null;
}

void a3dObject::setKernel( Kernel *krn ) { 

	this->kernel = krn;
	for( int i = 0; i < this->detailCount; i ++ ) { 
		this->detail[ i ].setKernel( this->kernel );
	}

	return ;
}

int a3dObject::getDetailCount( a3dTextModelFile &scFile, a3dMesh &mesh ) { 

	a3dMaterialBase *base = scFile.getMaterial( mesh.materialName );
	if( base->type != MT_MULTI ) return 1;

	a3dMultiMaterial *mat = ( a3dMultiMaterial* ) base;
	return mat->subMaterialCount;
}

bool a3dObject::importDetails( a3dTextModelFile &scFile, a3dMesh &mesh ) { 

	this->detailCount = this->getDetailCount( scFile, mesh );
	this->detail = new a3dDetail[ this->detailCount ];

	int di;
	for( di = 0; di < this->detailCount; di++ ) {
		this->detail[ di ].setObject( this );
		this->detail[ di ].setKernel( this->kernel );
		if( !this->detail[ di ].import( scFile, mesh, di ) ) return false;
	}

	return true;
}

bool a3dObject::initializeVertexInfo( a3dTextModelFile &scFile, a3dMesh &mesh ) {

	if( mesh.vertexFVF == shString( TEXT( "undefined" ) ) ) return false;

	this->vertexInfo.initialize( mesh.vertexFVF );
	return true;
}

bool a3dObject::import( a3dTextModelFile &scFile, a3dMesh &mesh ) { 

	this->clear();

	this->name = mesh.name;

	this->initializeBasis( mesh );

	if( !this->initializeVertexInfo( scFile, mesh ) ) return false;

	if( !this->initializeVertices( mesh ) ) return false;

	if( !this->processVertices( mesh ) ) return false;

	if( !this->initializeVertexData( mesh ) ) return false;

	if( !this->importDetails( scFile, mesh ) ) return false;

	this->calcBVolume();

	return true;
}

bool a3dObject::calcNormals() { 

	Vector3 v1, v2;
	int i, di, ni;

	if( this->normals != null )
		delete[] this->normals;
	this->normals = null;

	if( this->vertices == null )
		this->extractVertices();

	this->normals = new Vector3[ this->vertexCount ];
	clearVector3Array( this->normals, this->vertexCount );

	int faceCount = this->calcFaceCount();
	Vector3 *norm = new Vector3[ faceCount ];

	ni = 0;
	for( di = 0; di < this->detailCount; di++ ) {
		for( i = 0; i < this->detail[di].faceCount; i++, ni++ ) { 
			subVector3( this->vertices[ this->detail[di].faces[ i ].bIndex ], this->vertices[ this->detail[di].faces[ i ].aIndex ], v1 );
			subVector3( this->vertices[ this->detail[di].faces[ i ].cIndex ], this->vertices[ this->detail[di].faces[ i ].aIndex ], v2 );
			vectorMulOfVector( v2, v1, norm[ ni ] );
			normalizeVector( norm[ ni ] );
		}
	}

	ni = 0;
	for( di = 0; di < this->detailCount; di++ ) {
		for( i = 0; i < this->detail[di].faceCount; i++, ni++ ) { 
			addVector3( this->normals[ this->detail[di].faces[i].aIndex ], norm[ ni ], this->normals[ this->detail[di].faces[i].aIndex ] );
			addVector3( this->normals[ this->detail[di].faces[i].bIndex ], norm[ ni ], this->normals[ this->detail[di].faces[i].bIndex ] );
			addVector3( this->normals[ this->detail[di].faces[i].cIndex ], norm[ ni ], this->normals[ this->detail[di].faces[i].cIndex ] );
		}
	}

	normalizeVector3Array( this->normals, this->vertexCount );
	delete[] norm;

	return true;
}

bool a3dObject::calcNormals( a3dMesh &mesh ) { 

	if( !this->vertexInfo.hasNormal() || this->normals != null ) return true;

	Vector3 v1, v2;
	int i;

	this->normals = new Vector3[ this->vertexCount ];
	clearVector3Array( this->normals, this->vertexCount );

	Vector3 *norm = new Vector3[ mesh.faceCount ];
	for( i = 0; i < mesh.faceCount; i++ ) { 
		subVector3( this->vertices[ mesh.faceList[ i ].bIndex ], this->vertices[ mesh.faceList[ i ].aIndex ], v1 );
		subVector3( this->vertices[ mesh.faceList[ i ].cIndex ], this->vertices[ mesh.faceList[ i ].aIndex ], v2 );
		vectorMulOfVector( v2, v1, norm[ i ] );
		normalizeVector( norm[ i ] );
	}

	for( i = 0; i < mesh.faceCount; i ++ ) {
		addVector3( this->normals[ mesh.faceList[i].aIndex ], norm[ i ], this->normals[ mesh.faceList[i].aIndex ] );
		addVector3( this->normals[ mesh.faceList[i].bIndex ], norm[ i ], this->normals[ mesh.faceList[i].bIndex ] );
		addVector3( this->normals[ mesh.faceList[i].cIndex ], norm[ i ], this->normals[ mesh.faceList[i].cIndex ] );
	}

	normalizeVector3Array( this->normals, this->vertexCount );
	delete[] norm;

	return true;
}

bool a3dObject::initializeVertexData( a3dMesh &mesh ) {

	this->clearVertexData();

	this->vertexData = new BYTE[ this->vertexCount * this->vertexInfo.getVertexSize() ];
	if( !this->fillVertexData( mesh ) ) return false;

	return true;
}

bool a3dObject::fillVertexData( a3dMesh &mesh ) {

	BYTE *data = this->vertexData;
	Vector3 tcoor;

	int i;
	if( this->vertexInfo.hasXYZ() ) { 
		for( i = 0; i < this->vertexCount; i ++, data += this->vertexInfo.getVertexSize() ) { 
			this->vertexInfo.setXYZ( data, this->vertices[ i ] );
		}
	}

	if( this->vertexInfo.hasNormal() ) { 
		data = this->vertexData;
		// calc normals if needed
		this->calcNormals( mesh );
		for( i = 0; i < this->vertexCount; i ++, data += this->vertexInfo.getVertexSize() ) { 
			this->vertexInfo.setNormal( data, this->normals[ i ] );
		}
	}

	if( this->vertexInfo.hasTexCoord1() && mesh.mapChannelCoors[0] != null ) { 
		data = this->vertexData;
		for( i = 0; i < this->vertexCount; i ++, data += this->vertexInfo.getVertexSize() ) { 
			mesh.getVertexTextureCoor( i, 0, tcoor );
			this->vertexInfo.setTex1( data, tcoor );
		}
	}

	if( this->vertexInfo.hasTexCoord2() && (mesh.mapChannelCoors[1] != null || mesh.mapChannelCoors[0]) ) { 
		data = this->vertexData;
		for( i = 0; i < this->vertexCount; i ++, data += this->vertexInfo.getVertexSize() ) { 
			mesh.getVertexTextureCoor( i, 1, tcoor );
			this->vertexInfo.setTex2( data, tcoor );
		}
	}

	if( this->vertexInfo.hasTexCoord3() && (mesh.mapChannelCoors[2] != null || mesh.mapChannelCoors[0]) ) { 
		data = this->vertexData;
		for( i = 0; i < this->vertexCount; i ++, data += this->vertexInfo.getVertexSize() ) { 
			mesh.getVertexTextureCoor( i, 2, tcoor );
			this->vertexInfo.setTex3( data, tcoor );
		}
	}

	if( this->vertexInfo.hasTexCoord4() && (mesh.mapChannelCoors[3] != null || mesh.mapChannelCoors[0]) ) { 
		data = this->vertexData;
		for( i = 0; i < this->vertexCount; i ++, data += this->vertexInfo.getVertexSize() ) { 
			mesh.getVertexTextureCoor( i, 3, tcoor );
			this->vertexInfo.setTex4( data, tcoor );
		}
	}

	if( this->vertexInfo.hasTexCoord5() && (mesh.mapChannelCoors[4] != null || mesh.mapChannelCoors[0]) ) { 
		data = this->vertexData;
		for( i = 0; i < this->vertexCount; i ++, data += this->vertexInfo.getVertexSize() ) { 
			mesh.getVertexTextureCoor( i, 4, tcoor );
			this->vertexInfo.setTex5( data, tcoor );
		}
	}

	if( this->vertexInfo.hasTexCoord6() && (mesh.mapChannelCoors[5] != null || mesh.mapChannelCoors[0]) ) { 
		data = this->vertexData;
		for( i = 0; i < this->vertexCount; i ++, data += this->vertexInfo.getVertexSize() ) { 
			mesh.getVertexTextureCoor( i, 5, tcoor );
			this->vertexInfo.setTex6( data, tcoor );
		}
	}

	if( this->vertexInfo.hasTexCoord7() && (mesh.mapChannelCoors[6] != null || mesh.mapChannelCoors[0]) ) { 
		data = this->vertexData;
		for( i = 0; i < this->vertexCount; i ++, data += this->vertexInfo.getVertexSize() ) { 
			mesh.getVertexTextureCoor( i, 6, tcoor );
			this->vertexInfo.setTex7( data, tcoor );
		}
	}

	if( this->vertexInfo.hasTexCoord8() && (mesh.mapChannelCoors[7] != null || mesh.mapChannelCoors[0]) ) { 
		data = this->vertexData;
		for( i = 0; i < this->vertexCount; i ++, data += this->vertexInfo.getVertexSize() ) { 
			mesh.getVertexTextureCoor( i, 7, tcoor );
			this->vertexInfo.setTex8( data, tcoor );
		}
	}

	return true;
}

bool a3dObject::extractVertices() { 

	if( this->vertexData == null )
		this->save();

	if( this->vertexData == null ) 
		return false;

	float *v;
	this->vertices = new Vector3[ this->vertexCount ];
	for( int i = 0; i < this->vertexCount; i++ ) { 
		v = this->vertexInfo.getXYZ( this->getVertexData( i ) );
		if( v != null ) copyVector3( v, this->vertices[ i ] );
	}

	return true;
}

bool a3dObject::initializeVertices( a3dMesh &mesh ) { 

	this->vertexCount = mesh.vertexCount;
	if( this->vertexCount == 0 ) return false;

	this->vertices = new Vector3[ this->vertexCount ];
	memcpy( this->vertices, mesh.vertexList, sizeof( Vector3 ) * this->vertexCount );

	return true;
} 

bool a3dObject::processVertices( a3dMesh &mesh ) { 

	this->swapVerticesYZ();
	this->localizeVertices();

	return true;
}

void a3dObject::localizeVertices() { 

	

	return;
} 

void a3dObject::swapVerticesYZ() { 

	float tmp;
	for( int i = 0; i < this->vertexCount; i ++ ) {
		tmp = this->vertices[i][1];
		this->vertices[i][1] = this->vertices[i][2];
		this->vertices[i][2] = tmp;
	}

	return ;
}

void a3dObject::swapBasisYZ() { 

	Vector3 axis;
	
	// поменяем местами координаты векторов базиса
	swapFloat(this->basis[1][0], this->basis[2][0]);
	swapFloat(this->basis[1][1], this->basis[2][1]);
	swapFloat(this->basis[1][2], this->basis[2][2]);
	
	setVector3( axis, this->basis[0][1], this->basis[1][1], this->basis[2][1] );

	// меняем местами yz базиса
	this->basis[0][1] = this->basis[0][2];
	this->basis[1][1] = this->basis[1][2];
	this->basis[2][1] = this->basis[2][2];

	this->basis[0][2] = axis[0];
	this->basis[1][2] = axis[1];
	this->basis[2][2] = axis[2];

	float tmp = this->basis[3][1];
	this->basis[3][1] = this->basis[3][2];
	this->basis[3][2] = tmp;

	return ;
}

void a3dObject::initializeBasis( a3dMesh &mesh ) { 

	originMatrix4( this->basis);
	copyMatrix4( mesh.transform, this->basis );
	
	this->basis[0][0] = mesh.transform[0][0];
	this->basis[1][0] = mesh.transform[0][1];
	this->basis[2][0] = mesh.transform[0][2];

	this->basis[0][1] = mesh.transform[1][0];
	this->basis[1][1] = mesh.transform[1][1];
	this->basis[2][1] = mesh.transform[1][2];

	this->basis[0][2] = mesh.transform[2][0];
	this->basis[1][2] = mesh.transform[2][1];
	this->basis[2][2] = mesh.transform[2][2];

	this->basis[3][0] = mesh.transform[3][0];
	this->basis[3][1] = mesh.transform[3][1];
	this->basis[3][2] = mesh.transform[3][2];

	this->swapBasisYZ();
	originMatrix4( this->invertBasis );
	
	this->correctBasisMirrors();
	copyMatrix4(this->basis, this->bVolume.basis);
	this->updateBasis();
	
	return ;
}

void a3dObject::correctBasisMirrors() { 
	
	
}

void a3dObject::updateBasis() { 

	makeDirectXTransformMatrix( this->basis, this->invertBasis );
	return ;
}

HRESULT a3dObject::createRenderBuffer() {

	HRESULT rv;

	if( this->kernel == null ) return D3DERR_INVALIDCALL;

	// создаем вершины
	rv = this->kernel->lpDevice->CreateVertexBuffer( this->vertexCount * this->vertexInfo.getVertexSize(),
		0,
		this->vertexInfo.getFVF(),
		D3DPOOL_MANAGED,
		&this->vertexBuffer,
		NULL );

	if( FAILED( rv ) ) {
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error : can't create verteBuffer (%s)" ), DXGetErrorString9( rv ) );
		this->kernel->ErrorBox( TEXT("a3dObject::createRenderBuffer") );
		return rv;
	}

	return D3D_OK;
}

void a3dObject::clearRenderBuffer() { 

	if( this->vertexBuffer == null ) return;
	this->vertexBuffer->Release();
	this->vertexBuffer = null;

	return ;
}

HRESULT a3dObject::fillRenderBuffer() {

	HRESULT rv;
	BYTE	*tmp;

	if( this->kernel == null ) return D3DERR_INVALIDCALL;

	rv = this->vertexBuffer->Lock( 0, 0, (VOID**)&tmp, 0 );
	if( FAILED( rv ) ) {
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error : can't lock vertexBuffer (%s)" ), DXGetErrorString9( rv ) );
		this->kernel->ErrorBox( TEXT( "a3dObject::fillRenderBuffer" ) );
		return rv;
	}

	memcpy( tmp, this->vertexData, this->vertexCount * this->vertexInfo.getVertexSize() );

	rv = this->vertexBuffer->Unlock();
	if( FAILED( rv ) ) {
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error : can't unlock vertexBuffer (%s)" ), DXGetErrorString9( rv ) );
		this->kernel->ErrorBox( TEXT( "a3dObject::fillRenderBuffer" ) );
		return rv;
	}

	return D3D_OK;
}

HRESULT a3dObject::save() { 

	HRESULT rv; 

	if( !this->readDataFromBuffer() ) { 
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error: can't read data from render buffer" ) );
		this->kernel->ErrorBox( TEXT( "a3dObject" ) );
		return D3DERR_INVALIDCALL;	
	}

	this->clearRenderBuffer();

	for( int i = 0; i < this->detailCount; i++ ) { 
		rv = this->detail[i].save();
		if( FAILED( rv ) )
			return rv;
	}

	return D3D_OK;
}

HRESULT a3dObject::render( camera *cam ) { 
	return this->render(cam, null);
} 

HRESULT a3dObject::render( camera *cam, a3dObjectRef *objRef ) { 

	static Matrix4 WorldXf, WvpXf, WorldITXf, invertWorld;
	HRESULT rv;

	mulMatrix4( this->invertBasis, this->model->invertBasis, WorldXf );
	mulMatrix4( WorldXf, cam->viewProj, WvpXf );

	mulMatrix4( this->basis, this->model->basis, invertWorld );
	transMatrix4( invertWorld, WorldITXf );

	rv = this->kernel->lpDevice->SetStreamSource( 0, this->vertexBuffer, 0, this->vertexInfo.getVertexSize() );
	if( FAILED( rv ) )
		return rv;

	rv = this->kernel->lpDevice->SetFVF( this->vertexInfo.getFVF() );
	if( FAILED( rv ) )
		return rv;

	int index;
	for( index = 0; index < this->detailCount; index++ ) {
		rv = this->detail[index].render( cam, WorldXf, WvpXf, WorldITXf, objRef == null? null: objRef->detailRef[index] );
		if( FAILED( rv ) )
			return rv;
	}

	return D3D_OK;
}

HRESULT a3dObject::restore() { 

	HRESULT rv; 

	rv = this->createRenderBuffer();
	if( FAILED( rv ) ) { 
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error: can't create render buffer (%s)" ), DXGetErrorString9( rv ) );
		this->kernel->ErrorBox( TEXT( "a3dObject" ) );
		return rv;
	}

	rv = this->fillRenderBuffer();
	if( FAILED( rv ) ) {
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error: can't fill render buffer (%s)" ), DXGetErrorString9( rv ) );
		this->kernel->ErrorBox( TEXT( "a3dObject" ) );
		return rv;
	}

	for( int i = 0; i < this->detailCount; i++ ) { 
		rv = this->detail[i].restore();
		if( FAILED( rv ) )
			return rv;
	}

	delete[] this->vertexData;
	this->vertexData = null;

	return D3D_OK;
}

int a3dObject::calcFaceCount() { 

	int count = 0, di;
	for( di = 0; di < this->detailCount; di++ ) 
		count += this->detail[di].faceCount;

	return count;
}

bool a3dObject::loadFromMemoryCore( a3dMemoryFile &file ) { 

#ifdef DEBUG_MODEL
	int prevUsed = file.usedSpace();
#endif

	if( !a3dMathObject::loadFromMemoryCore( file ) ) return false;
	if( !this->name.loadFromMemory( file ) ) return false;
	if( !this->vertexInfo.loadFromMemory( file ) ) return false;
	if( !file.readInt( &this->vertexCount ) ) return false;

	if( this->vertexCount > 0 ) { 
		this->vertexData = new BYTE[ this->vertexInfo.calcVertexSize() * this->vertexCount ];
	}
	if( !file.readMemBlock( (BYTE*) this->vertexData, this->vertexInfo.calcVertexSize() * this->vertexCount ) ) return false;
	if( !file.readInt( &this->detailCount ) ) return false;

	if( this->detailCount > 0 ) 
		this->detail = new a3dDetail[ this->detailCount ];

	for( int di = 0; di < this->detailCount; di ++ ) { 
#ifdef DEBUG_MODEL
		prevUsed = file.usedSpace();
#endif
		this->detail[di].setObject( this );
		this->detail[di].setKernel( this->kernel );
		if( !this->detail[ di ].loadFromMemory( file ) ) return false;
#ifdef DEBUG_MODEL
		if( file.usedSpace() - prevUsed != this->detail[ di ].calcSize() ) return false;
#endif
	}

	return true;
}

bool a3dObject::saveToMemoryCore( a3dMemoryFile &file ) {

#ifdef DEBUG_MODEL
	int prevUsed = file.usedSpace();
#endif

	if( !a3dMathObject::saveToMemoryCore( file ) ) return false;
	if( !this->name.saveToMemory( file ) ) return false;
	if( !this->vertexInfo.saveToMemory( file ) ) return false;
	if( !file.writeInt( this->vertexCount ) ) return false;

	if( this->vertexData == null && !this->readDataFromBuffer() ) return false;
	if( !file.writeMemBlock( (BYTE*) this->vertexData, this->vertexInfo.getVertexSize() * this->vertexCount ) ) return false;
	if( !file.writeInt( this->calcSaveDetailCount() ) ) return false;

	for( int i = 0; i < this->detailCount; i ++ ) {
		if( !this->detail[ i ].supportSave() ) continue;	
#ifdef DEBUG_MODEL
		prevUsed = file.usedSpace();
#endif
		if( !this->detail[ i ].saveToMemory( file ) ) return false;
#ifdef DEBUG_MODEL
		if( file.usedSpace() - prevUsed != this->detail[ i ].calcSize() ) return false;
#endif
	}

	return true;
}

int a3dObject::calcSaveDetailCount() { 

	int dc = 0;

	for( int i = 0; i < this->detailCount; i ++ ) { 
		if( this->detail[ i ].supportSave() ) dc ++;
	}

	return dc;
}

bool a3dObject::readDataFromBuffer( ) { 

	HRESULT rv;
	BYTE *tmp;

	rv = this->vertexBuffer->Lock( 0, 0, (VOID**)&tmp, 0 );
	if( FAILED( rv ) ) {
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error : can't lock vertexBuffer (%s)" ), DXGetErrorString9( rv ) );
		this->kernel->ErrorBox( TEXT( "a3dObject::readDataFromBuffer" ) );
		return false;
	}

	this->vertexData = new BYTE[ this->vertexInfo.calcVertexSize() * this->vertexCount ];
	memcpy( this->vertexData, tmp, this->vertexInfo.calcVertexSize() * this->vertexCount );

	rv = this->vertexBuffer->Unlock();
	if( FAILED( rv ) ) {
		_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error : can't unlock vertexBuffer (%s)" ), DXGetErrorString9( rv ) );
		this->kernel->ErrorBox( TEXT( "a3dObject::readDataFromBuffer" ) );
		return false;
	}

	return true;
}

void a3dObject::getLightMaps( a3dLightMap** llList, processLightMapPlaneDeletate fp, void *lpData ) { 

	int di;
	for( di = 0; di < this->detailCount; di++ ) { 
		llList[di] = this->detail[di].getLightMap( fp, lpData );
	} 

	return ;
}

int a3dObject::getLightMapCount() { 

	int di, lightMapCount = 0;
	for( di = 0; di < this->detailCount; di++ ) { 
		if( this->detail[di].hasLightMap() )
			lightMapCount ++;
	}

	return lightMapCount;
}

bool a3dObject::supportSave() { return this->vertexCount > 0; }

int a3dObject::calcSizeCore() { 

	int totalSize = a3dMathObject::calcSizeCore();

	totalSize +=	this->name.calcSize();	// save in char 
	totalSize +=	vertexInfo.calcSize();
	totalSize +=	intsize;			// vertex count
	totalSize +=	vertexInfo.getVertexSize() * this->vertexCount; // vertex array
	totalSize +=	intsize;			// detalcount

	for( int di = 0; di < this->detailCount; di ++ ) { 
		if( !this->detail[ di ].supportSave() ) continue;
		totalSize += this->detail[ di ].calcSize(); // detail size + content
	}

	return totalSize;
}

a3dObjectRef* a3dObject::createReference( a3dModelRef *modelRef ) { 
	return new a3dObjectRef( modelRef, this );
}

void a3dObject::correct( Matrix4 mat, float invScX, float invScY, float invScZ ) {

}

void a3dObject::calcBVolume() { 

	this->bVolume.calcBoundBox( this->vertexCount, this->vertices );
	return ;
}

#undef DEBUG_MODEL
#undef intsize