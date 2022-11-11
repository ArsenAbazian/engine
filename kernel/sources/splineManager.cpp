#include "splineManager.h"
#include "3dtypes.h"
#include <stdio.h>

a3dSplineManager::a3dSplineManager() { 
	
	this->isClose = false;
	this->scale = 1.0f;
	setVector3( this->origin, 0.0f, 0.0f, 0.0f );
	this->point = null;
	this->pointCount = 0;
}

void a3dSplineManager::clear() {
	
	if( this->point != null )
		delete[] this->point;
	this->point = null;
	this->pointCount = 0;
	this->isClose = false;
	this->scale = 1.0f;
	setVector3( this->origin, 0.0f, 0.0f, 0.0f );
}

bool a3dSplineManager::initialize( shString fileName, float scale, Vector3 origin, bool isClose ) { 
	
	this->clear();
	this->scale = scale;
	this->splineFileName = fileName;
	this->isClose = isClose;
	copyVector3( origin, this->origin );

	FILE *fp;

	_tfopen_s( &fp, fileName.getCString(), TEXT( "rt" ) );
	if( fp == null ) return false;

	_ftscanf_s( fp, TEXT("%d"), &this->pointCount );
	this->point = new Vector3[ this->pointCount ];

	int pointIndex, val;
	for( pointIndex = 0; pointIndex < this->pointCount; pointIndex ++ ) { 
		_ftscanf_s( fp, TEXT("%d"), &val );
		this->point[ pointIndex ][0] = val * scale + origin[0];
		this->point[ pointIndex ][1] = 0.0f;
		this->point[ pointIndex ][2] = val * scale + origin[2];
	}

	fclose( fp ); 
	return true;
}