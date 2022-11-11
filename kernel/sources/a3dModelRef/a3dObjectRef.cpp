#include "a3dObjectRef.h"
#include "..\\a3dModel\\a3dDetail.h"
#include "..\\a3dModel\\a3dObject.h"
#include "..\\a3dModelRef\\a3dDetailRef.h"

void a3dObjectRef::initialize( a3dModelRef *modelRef, a3dObject *object ) { 

	this->clear();

	this->modelRef = modelRef;
	this->object = object;

	object->updateBasis();
	copyMatrix4( object->basis, this->basis );
	copyMatrix4( object->invertBasis, this->invertBasis );

	this->scaleX = this->object->scaleX;
	this->scaleY = this->object->scaleY;
	this->scaleZ = this->object->scaleZ;

	this->detailCount = object->detailCount;
	this->detailRef = new a3dDetailRef*[ this->detailCount ];

	for( int di = 0; di < this->detailCount; di ++ ) { 
		this->detailRef[di] = object->detail[di].createReference( this );
	}
}

void a3dObjectRef::applyBasis() { 

	copyMatrix4( this->basis, this->object->basis );
	copyMatrix4( this->invertBasis, this->object->invertBasis );

	this->object->scaleX = this->scaleX;
	this->object->scaleY = this->scaleY;
	this->object->scaleZ = this->scaleZ;

	return ;
}

void a3dObjectRef::clear() { 

	this->modelRef = null;
	this->object = null;
	if( this->detailRef != null ) { 
		for( int di = 0; di < this->detailCount; di ++ ) { 
			if( this->detailRef[di] != null ) delete this->detailRef[di];
		}
		delete[] this->detailRef;
	}
	this->detailRef = null;
	this->detailCount = 0;

}

void a3dObjectRef::initializeDefault() { 

	this->modelRef = null;
	this->object = null;
	this->detailRef = null;
	this->detailCount = 0;

	originMatrix4( this->basis );
	originMatrix4( this->invertBasis );

	this->scaleX = 1.0f;
	this->scaleY = 1.0f;
	this->scaleZ = 1.0f;
}

int a3dObjectRef::getLightMapCount() { 
	
	int lmCount = 0;
	for(int i=0; i < this->detailCount; i++ ) { 
		if( !this->detailRef[i]->hasLightMap() ) continue;
		lmCount ++;
	}

	return lmCount;
}

void a3dObjectRef::getLightMaps( a3dLightMap** llList, processLightMapPlaneDeletate fp, void *lpData ) { 

	int di;
	for( di = 0; di < this->detailCount; di++ ) { 
		llList[di] = this->detailRef[di]->getLightMap( fp, lpData );
	} 

	return ;
}