#include "a3dModelRef.h"
#include "a3dObjectRef.h"
#include "..\\a3dModel\\a3dModel.h"
#include "..\\a3dModel\\a3dObject.h"
#include "..\\a3dDesc\\a3dModelDesc.h"
#include "..\\a3dObjectInfo\\a3dModelInfo.h"
#include "..\\radiosity.h"

/*
int a3dModelRef::getLightMapCount() { 

	if( this->model == null ) return 0;
	return this->model->getLightMapCount();
}

a3dLightMap** a3dModelRef::getLightMaps() { 

	return this->getLightMaps(null, null);
}

a3dLightMap** a3dModelRef::getLightMaps( processLightMapPlaneDeletate fp, void *lpData ) { 

	if( this->model == null ) return null;

	// apply basises
	this->applyBasises();

	return this->model->getLightMaps( fp, lpData );
}
*/

void a3dModelRef::applyBasises() { 

	copyMatrix4( this->basis, this->model->basis );
	copyMatrix4( this->invertBasis, this->model->invertBasis );

	this->model->scaleX = this->scaleX;
	this->model->scaleY = this->scaleY;
	this->model->scaleZ = this->scaleZ;

	for( int i = 0; i < this->objCount; i++ ) 
		this->objectRef[i]->applyBasis();

	return ;
}

HRESULT a3dModelRef::render( camera *cam ) { 

	if( this->model == null )
		return D3D_OK;

	return this->model->render( cam, this );
}

void a3dModelRef::initializeDefault() { 

	a3dMathObject::initializeDefault();

	this->desc = null;
	this->model = null;
	this->objectRef = null;
	this->objCount = 0;

	originMatrix4( this->basis );
	originMatrix4( this->invertBasis );

	this->scaleX = 1.0f;
	this->scaleY = 1.0f;
	this->scaleZ = 1.0f;

	return ;
}

void a3dModelRef::initialize( a3dModelInfo *info ) { 

	this->info = info;
	this->initialize( info->model );
	copyMatrix4( info->basis, this->basis );
}

void a3dModelRef::initialize( a3dModelDesc *desc ) { 

	this->initialize( desc->model );
	this->desc = desc;
}

void a3dModelRef::initialize( a3dModel *model ) { 

	this->clear();

	this->model = model;

	copyMatrix4( model->basis, this->basis );
	copyMatrix4( model->invertBasis, this->basis );

	this->scaleX = this->model->scaleX;
	this->scaleY = this->model->scaleY;
	this->scaleZ = this->model->scaleZ;

	this->objCount = model->objCount;
	this->objectRef = new a3dObjectRef*[ this->objCount ];

	for( int oi = 0; oi < this->objCount; oi ++ ) { 
		this->objectRef[oi] = model->object[oi].createReference( this );
	}

	return ;
}

void a3dModelRef::clear() { 

	this->desc = null;
	this->model = null;
	if( this->objectRef != null ) { 
		for( int oi = 0; oi < this->objCount; oi ++ ) { 
			if( this->objectRef[oi] != null ) delete this->objectRef[oi];
		}
		delete[] this->objectRef;
	}
	this->objectRef = null;
	this->objCount = 0;

	return ;
}

int a3dModelRef::getLightMapCount() { 

	int lmCount = 0;
	for( int i = 0; i < this->objCount; i++ ) { 
		lmCount += this->objectRef[i]->getLightMapCount();
	}
	return lmCount;
}

a3dLightMap** a3dModelRef::getLightMaps() { 
	return this->getLightMaps(null, null);
}

a3dLightMap** a3dModelRef::getLightMaps( processLightMapPlaneDeletate fp, void *lpData ) {

	int lightMapCount = this->getLightMapCount();
	if(lightMapCount == 0) return null;

	this->applyBasises();
	a3dLightMap** lmList= new a3dLightMap*[ lightMapCount ];

	int oi;
	for( oi = 0; oi < this->objCount; oi++ ) {
		this->objectRef[ oi ]->getLightMaps( &lmList[ oi ], fp, lpData );
	}

	int li;
	for( li = 0; li < lightMapCount; li++ ) { 
		lmList[li]->translate( this->basis, true );
	}

	return lmList;
}
