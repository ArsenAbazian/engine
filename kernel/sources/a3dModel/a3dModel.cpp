#include "a3dModel.h"
#include "a3dObject.h"
#include "memoryFile.h"
#include "..\\a3dModelRef\\a3dModelRef.h"
#include "..\\a3dTextModelFile.h"
#include "..\\radiosity.h"
#include "..\\a3dMaterial\\a3dShaderMaterial.h"
#include <dxerr9.h>

#define DEBUG_MODEL
#define intsize		sizeof ( int )

void a3dModel::initializeDefault() { 

	this->version = createVersion(1, 0);
	this->object = null;	
	this->objCount = 0;

	originMatrix4( this->basis );
	originMatrix4( this->invertBasis );

	this->material = null;
	this->materialCount = 0;

	this->light = null;
	this->lightCount = 0;

	this->kernel = null;
}

void a3dModel::clear() { 

	if( this->object != null ) delete[] this->object;
	this->object = null;
	this->objCount = 0;

	if( this->material != null ) { 
		for( int mi = 0; mi < this->materialCount; mi ++ ) { 
			if( this->material[mi] == null ) continue;
			delete this->material[mi];
		}
		delete[] this->material;
	}
	this->material = null;
	this->materialCount = 0;
}

bool a3dModel::import( a3dTextModelFile &scFile ) { 

	this->clear();

	this->objCount = scFile.meshCount;
	this->object = new a3dObject[ this->objCount ];

	this->materialCount = scFile.materialCount;
	this->material = new a3dMaterialBase*[ this->materialCount ];
	ZeroMemory( this->material, sizeof( a3dMaterialBase* ) * this->materialCount );

	int oi;
	for( oi = 0; oi < this->objCount; oi++ ) { 
		this->object[ oi ].setModel( this );
		this->object[ oi ].setKernel( this->kernel );
		if( !this->object[ oi ].import( scFile, scFile.meshList[ oi ] ) ) return false;
	}

	this->calcBVolume();

	int mi;
	for( mi = 0; mi < this->materialCount; mi++ ) { 
		this->material[ mi ] = scFile.materialList[ mi ]->clone();
	}
	return true;
}

void a3dModel::setKernel( Kernel *krn ) { 

	this->kernel = krn;
	for( int i = 0; i < this->objCount; i++ ) {
		this->object[ i ].setKernel( this->kernel );
	}

	return ;
}

bool a3dModel::loadFromMemoryCore( a3dMemoryFile &file ) { 

#ifdef DEBUG_MODEL 	
	int prevUsed = 0;
#endif

	int vs;

	if( !file.readInt( &vs ) ) return false;
	if( this->version < vs ) return false;

	if( !a3dMathObject::loadFromMemoryCore( file ) ) return false;
	if( !file.readInt( &this->objCount ) ) return false;

	if( this->objCount > 0 ) 
		this->object = new a3dObject[ this->objCount ];

	for( int oi = 0; oi < this->objCount; oi ++ ) { 
#ifdef DEBUG_MODEL
		prevUsed = file.usedSpace();
#endif		
		this->object[oi].setModel( this );
		this->object[oi].setKernel( this->kernel );
		if( !this->object[oi].loadFromMemory( file ) ) return false;
#ifdef DEBUG_MODEL		
		if( file.usedSpace() - prevUsed != this->object[oi].calcSize() ) return false;
#endif		
	} 

	int mi;
	if( !file.readInt( &this->materialCount ) ) return false;
	if( this->materialCount > 0 ) {
		this->material = new a3dMaterialBase*[ this->materialCount ];
		for( mi = 0; mi < this->materialCount; mi ++ ) 
			this->material[ mi ] = null;
	}

	BYTE *pos = NULL;
	int tp;

	for( mi = 0; mi < this->materialCount; mi ++ ) { 
#ifdef DEBUG_MODEL		
		prevUsed = file.usedSpace();
#endif		

		// reading param type
		file.savePos();
		if( !file.readInt( &tp ) ) return false;
		file.restorePos();

		this->material[mi] = this->createMaterial( tp );
		if( this->material[mi] == null ) return false;

		if( !this->material[mi]->loadFromMemory( file ) ) return false;

#ifdef DEBUG_MODEL
		if( file.usedSpace() - prevUsed != this->material[mi]->calcSize() ) return false;
#endif
	}

	int li;
	if( !file.readInt( &this->lightCount ) ) return false;
	for( li = 0; li < this->lightCount; li ++ ) { 
#ifdef DEBUG_MODEL		
		prevUsed = file.usedSpace();
#endif		

		// reading param type
		file.savePos();
		if( !file.readInt( &tp ) ) return false; // read version
		if( !file.readInt( &tp ) ) return false; // read type
		file.restorePos();

		this->light[li] = this->createLight( (a3dLightType) tp );
		if( this->light[li] == null ) return false;

		if( !this->light[li]->loadFromMemory( file ) ) return false;

#ifdef DEBUG_MODEL
		if( file.usedSpace() - prevUsed != this->light[li]->calcSize() ) return false;
#endif
	}

	return true;
}

a3dMaterialBase* a3dModel::createMaterial( int header ) { 

	switch( header ) { 
		case a3dHeaders::a3dShaderMaterialHeader:
			return ( a3dMaterialBase* ) new a3dShaderMaterial();
	}

	return null;
}

a3dLightBase* a3dModel::createLight( int header ) { 

	switch( header ) { 
		case a3dHeaders::a3dOmniLightHeader:
			return ( a3dLightBase* ) new a3dOmniLight();
		case a3dHeaders::a3dDirectionalLightHeader:
			return ( a3dLightBase* ) new a3dDirectionalLight();
	}

	return null;
}

bool a3dModel::saveToMemoryCore( a3dMemoryFile &file ) { 

#ifdef DEBUG_MODEL 	
	int prevUsed = 0;
#endif

	if( !file.writeInt( this->createVersion( 1, 0 ) ) ) return false;
	if( !a3dMathObject::saveToMemoryCore( file ) ) return false;
	if( !file.writeInt( this->calcSaveObjCount() ) ) return false;
	for( int oi = 0; oi < this->objCount; oi ++ ) { 
		if( !this->object[oi].supportSave() ) continue;
#ifdef DEBUG_MODEL
		prevUsed = file.usedSpace();
#endif		
		if( !this->object[oi].saveToMemory( file ) ) return false;
#ifdef DEBUG_MODEL		
		if( file.usedSpace() - prevUsed != this->object[oi].calcSize() ) return false;
#endif
	}

	if( !file.writeInt( this->calcSaveMaterialCount() ) ) return false;
	for( int mi = 0; mi < this->materialCount; mi ++ ) { 
		if( !this->material[mi]->supportSave() ) continue;
#ifdef DEBUG_MODEL		
		prevUsed = file.usedSpace();
#endif		
		if( !this->material[mi]->saveToMemory( file ) ) return false;
#ifdef DEBUG_MODEL
		if( file.usedSpace() - prevUsed != this->material[mi]->calcSize() ) return false;
#endif
	}

	if( !file.writeInt( this->calcSaveLightCount() ) ) return false;
	for( int li = 0; li < this->lightCount; li ++ ) { 
		if( !this->light[li]->supportSave() ) continue;
#ifdef DEBUG_MODEL		
		prevUsed = file.usedSpace();
#endif		
		if( !this->light[li]->saveToMemory( file ) ) return false;
#ifdef DEBUG_MODEL
		if( file.usedSpace() - prevUsed != this->light[li]->calcSize() ) return false;
#endif
	}

	return true;
}

int a3dModel::calcSaveObjCount() { 

	int count = 0;
	for( int i = 0; i < this->objCount; i ++ ) { 
		if( this->object[i].supportSave() ) count ++;
	}

	return count;
}

bool a3dModel::saveToFile( shString &fileName ) { 

	a3dMemoryFile file;

	file.initialize( this->calcSize() );
	if( !this->saveToMemory( file ) ) return false;
	if( file.emptySpace() != 0 ) return false;

	if( !file.saveToFile( fileName ) ) return false;

	return true;
}

bool a3dModel::loadFromFile( shString &fileName ) { 

	a3dMemoryFile file;

	if( !file.loadFromFile( fileName ) ) return false;
	if( !this->loadFromMemory( file ) ) return false;
	if( file.emptySpace() != 0 ) return false;

	return true;
}

int a3dModel::calcSaveMaterialCount() { 

	int mc = 0;

	for( int mi = 0; mi < this->materialCount; mi++ ) { 
		if( this->material[ mi ]->supportSave() ) mc ++; 
	}

	return mc;
}

int a3dModel::calcSaveLightCount() { 

	int lc = 0;

	for( int li = 0; li < this->lightCount; li++ ) { 
		if( this->light[ li ]->supportSave() ) lc ++; 
	}

	return lc;
}

int a3dModel::calcSizeCore() { 

	int totalSize = intsize;;

	totalSize +=	 a3dMathObject::calcSizeCore();
	totalSize +=	intsize;				// object count

	int oi;
	for( oi = 0; oi < this->objCount; oi ++ ) {
		if( !this->object[oi].supportSave() ) continue;
		totalSize += this->object[oi].calcSize(); // object size + content
	}

	totalSize += intsize;						// material count

	int mi;
	for( mi = 0; mi < this->materialCount; mi ++ ) { 
		if( !this->material[mi]->supportSave() ) continue;
		totalSize += this->material[mi]->calcSize(); // material size + content
	}

	totalSize += intsize;						// light count

	int li;
	for( li = 0; li < this->lightCount; li ++ ) { 
		if( !this->light[li]->supportSave() ) continue;
		totalSize += this->light[li]->calcSize();
	}

	return totalSize;
}	

a3dModelRef* a3dModel::createReference() {
	return new a3dModelRef( this );
}

HRESULT a3dModel::initializeData( a3dScene *scene ) { 

	HRESULT rv;

	int matIndex;
	for( matIndex = 0; matIndex < this->materialCount; matIndex++ ) { 
		rv = this->material[matIndex]->initializeData( scene );
		if( FAILED (rv) ) { 
			_stprintf_s( Kernel::tempBuffer, this->kernel->bufferSize, TEXT( "Error : can't initialize effect data (%s)" ), DXGetErrorString9( rv ) );
			this->kernel->ErrorBox( TEXT("a3dModel::initializeData") );
			return rv;
		} 
	}

	return D3D_OK;
}

int a3dModel::getLightMapCount() { 

	int oi, lightMapCount = 0;
	for( oi = 0; oi < this->objCount; oi++ ) { 
		lightMapCount += this->object[oi].getLightMapCount();
	}

	return lightMapCount;

}

a3dLightMap** a3dModel::getLightMaps( processLightMapPlaneDeletate fp, void *lpData ) { 

	int lightMapCount = this->getLightMapCount();

	a3dLightMap** lmList= new a3dLightMap*[ lightMapCount ];

	int oi;
	for( oi = 0; oi < this->objCount; oi++ ) {
		this->object[ oi ].getLightMaps( &lmList[ oi ], fp, lpData );
	}

	int li;
	for( li = 0; li < lightMapCount; li++ ) { 
		lmList[li]->translate( this->basis, true );
	}

	return lmList;
}

HRESULT a3dModel::save() { 

	HRESULT rv;

	for( int i = 0; i < this->objCount; i++ ) { 
		rv = this->object[i].save();
		if( FAILED( rv ) ) 
			return rv;
	}

	return D3D_OK;
}

HRESULT a3dModel::restore() { 

	HRESULT rv;

	for( int i = 0; i < this->objCount; i++ ) { 
		rv = this->object[i].restore();
		if( FAILED( rv ) ) 
			return rv;
	}

	return D3D_OK;
}

HRESULT a3dModel::render( camera *cam ) { 
	
	HRESULT rv;
	int index;

	for( index = 0; index < this->objCount; index ++ ) { 
		rv = this->object[index].render( cam );
		if( FAILED( rv ) )
			return rv;
	}

	return D3D_OK;
}

HRESULT a3dModel::render( camera *cam, a3dModelRef *modelRef ) { 

	HRESULT rv;
	int index;

	for( index = 0; index < this->objCount; index ++ ) { 
		rv = this->object[index].render( cam, modelRef->objectRef[index] );
		if( FAILED( rv ) )
			return rv;
	}

	return D3D_OK;
}

int a3dModel::calcFaceCount() { 

	int oi, count = 0;
	for( oi = 0; oi < this->objCount; oi++ ) { 
		count = this->object[oi].calcFaceCount();
	}

	return count;
}

void a3dModel::correct() { 

	a3dMathObject::correct();

	float inv_scx = 1.0f / ( this->scaleX * this->scaleX );
	float inv_scy = 1.0f / ( this->scaleY * this->scaleY );	
	float inv_scz = 1.0f / ( this->scaleZ * this->scaleZ );

	for( int i = 0; i < objCount; i++ ) {
		this->object[i].correct( this->basis, inv_scx, inv_scy, inv_scz );	
	}
}

int a3dModel::getObjectIndexByName( shString &nm )
{
	int index;

	for( index = 0; index < this->objCount; index ++ )
	{
		if( nm == this->object[index].name )
			return index;
	}

	return -1;
}

void a3dModel::calcBVolume() { 

	if( this->objCount == 0 ) return ;

	a3dBoundBox **box = new a3dBoundBox*[ this->objCount ];

	for( int i = 0; i < this->objCount; i ++ ) { 
		box[ i ] = &this->object[ i ].bVolume;
	}

	this->bVolume.calcBoundBox( this->objCount, box );

	delete[] box;
	return ;
}

a3dMaterialBase* a3dModel::getMaterialByName(shString &matName) { 

	int mi;
	for( mi = 0; mi < this->materialCount; mi++ ) { 
		if( this->material[mi]->name == matName ) return this->material[mi];
	}

	return null;
}

void a3dModel::skipMaterialErrors(bool value) {
	
	a3dShaderMaterial *mat;
	for( int i = 0; i < this->materialCount; i++ ) {
		mat = dynamic_cast<a3dShaderMaterial*>(this->material[i]);
		if(mat == null) continue;
		mat->skipErrors = value;
	}		
}

#undef DEBUG_MODEL
#undef intsize