#include "radiosity.h"
#include "a3dScene.h"
#include "m_png.h"

void a3dLightMapPlaneInfo::initializeDefault() {
	
	clearVector3( this->a );
	clearVector3( this->b );
	clearVector3( this->c );
	clearVector3( this->na );
	clearVector3( this->nb );
	clearVector3( this->nc );
	clearVector3( this->normal );
	clearVector2( this->ta );
	clearVector2( this->tb );
	clearVector2( this->tc );

	this->dominantAxisIndex = 0;
	this->processed = false;
}

void a3dLightMapPlaneInfo::calcDominantAxisIndex() { 
	
	if( fabs( this->normal[0] ) > fabs( this->normal[1] ) ) {
		if( fabs( this->normal[0] ) > fabs( this->normal[2] ) )
			this->dominantAxisIndex = 0;
		else
			this->dominantAxisIndex = 2;
	}
	else { 
		if( fabs( this->normal[1] ) > fabs( this->normal[2] ) )
			this->dominantAxisIndex = 1;
		else
			this->dominantAxisIndex = 2;
	}
	
	return ;
}

void a3dLightMapPlaneInfo::initializeNormal() { 
	
	Vector3 v1, v2;

	subVector3( this->b, this->a, v1 );
	subVector3( this->c, this->a, v2 );
	vectorMulOfVector( v2, v1, this->normal );
	normalizeVector( this->normal );
	
	return ;
}

void a3dLightMapPlaneInfo::translate( Matrix4 basis, bool recalcNormal ) { 

	Vector4 v;
	Matrix3 m;

	extractMatrix3( basis, m );

	mulVector3ToMatrix4( this->a, basis, v ); copyVector3( v, this->a );
	mulVector3ToMatrix4( this->b, basis, v ); copyVector3( v, this->b );
	mulVector3ToMatrix4( this->c, basis, v ); copyVector3( v, this->c );
	
	mulVector3ToMatrix3( this->na, m, v ); copyVector3( v, this->na );
	mulVector3ToMatrix3( this->nb, m, v ); copyVector3( v, this->nb );
	mulVector3ToMatrix3( this->nc, m, v ); copyVector3( v, this->nc );

	if( recalcNormal )
		this->initializeNormal();

	return ;
}

void a3dLightMapPlaneInfo::bindToTree( a3dOcTreeManager *manager ) { 
	
	manager->addObjectToBindingListIfContains( this, this->a, this->b, this->c );
	return ;
}

/*
void a3dLightMapPlaneInfo::initializeBarycentricKoeffs() { 
	
	getBarycentricUVKoeffs( this->a, this->b, this->c, this->uKoeff, this->vKoeff );
	return ;
}
*/
void a3dLightMapPlaneInfo::clear() { 
	
	this->initializeDefault();
}

void a3dLightMapPixelInfo::initializeDefault() {

	clearVector3( this->point );
	clearVector3( this->normal );
	clearVector3( this->resultColor );
	clearVector3( this->emmitColor );
	this->ownerPlane = null;
	this->intMapIndex = -1;
}

void a3dLightMapPixelInfo::clear() { 
	
	this->initializeDefault();
}

void a3dLightMapPixelInfo::initialize( a3dLightMapPlaneInfo *ownerPlane, float u, float v ) { 

	Vector2 uv = { u, v };
	
	if( u > 1.0f ) u = 1.0f;
	if( v > 1.0f ) v = 1.0f;

	this->ownerPlane = ownerPlane;
	uv2XYZAndInterpolateNormal( this->ownerPlane->a, this->ownerPlane->b, this->ownerPlane->c, this->ownerPlane->ta, this->ownerPlane->tb, this->ownerPlane->tc, this->ownerPlane->na, this->ownerPlane->nb, this->ownerPlane->nc, uv, this->point, this->normal ); 
	
	return ;
}

void a3dLightMap::initializeDefault() { 

	this->width = 0;
	this->height = 0;

	this->pixels = null;
	this->map = null;
	this->planeColl = null;

	return ;
}

void a3dLightMap::initialize( int width, int height, a3dLightMapPlaneCollection *planeColl, shString &fileName ) { 

	this->clear();

	this->width = width;
	this->height = height;
	this->planeColl = planeColl;
	this->fileName = fileName;

	this->pixels = new a3dLightMapPixelInfo[ this->width * this->height ];
	
	this->map = new a3dLightMapPixelInfo*[ this->height ];
	for( int i = 0; i < this->height; i ++ )
		this->map[ i ] = &this->pixels[ i * this->width ];
	
	this->initializePixels();
	
	return ;
}

bool a3dLightMap::logPixelPosToImage( shString &fileName ) {

	Vector3 off = { 100.0f, 100.0f, 100.0f };
	
	int row, col;
	for( row = 0; row < this->height; row++ ) { 
		for( col = 0; col < this->width; col++ ) { 
			if( this->map[row][col].ownerPlane == null )
				setVector3( this->map[row][col].resultColor, 1.0f, 1.0f, 1.0f );
			else { 
				copyVector3( this->map[row][col].point, this->map[row][col].resultColor );
				addVector3( this->map[row][col].resultColor, off, this->map[row][col].resultColor );
				scaleVector3( this->map[row][col].resultColor, 0.005f );
			}
		}
	}

	return this->saveLightMap( fileName );
}

bool a3dLightMap::logUsedPixelsToImage( shString &fileName ) { 

	int row, col;
	for( row = 0; row < this->height; row++ ) { 
		for( col = 0; col < this->width; col++ ) { 
			if( this->map[row][col].ownerPlane == null )
				setVector3( this->map[row][col].resultColor, 1.0f, 1.0f, 1.0f );
			else { 
				setVector3( this->map[row][col].resultColor, (DWORD)this->map[row][col].ownerPlane );
			}
		}
	} 
	
	return this->saveLightMap( fileName );
}

bool a3dLightMap::saveLightMap( shString &fileName ) { 

	PNG png;
	char buf[1024];

	wcsToStr( fileName.getCString(), buf );
	
	png.data->bpp = 3;
	png.data->height = this->height;
	png.data->width = this->width;
	png.data->type = PNG_T_R8G8B8;

	png.allocateMem();
		
	Color888 *color;
	int row, col;
	for( row = 0; row < this->height; row++ ) { 
		for( col = 0; col < this->width; col++ ) { 
			color = (Color888*)( ((BYTE*)png.data->buf) + ( row * this->width + col ) * png.data->bpp );
			color->r = (BYTE) ( min( this->map[row][col].resultColor[0] * 255.0f, 255.0f ) );
			color->g = (BYTE) ( min( this->map[row][col].resultColor[1] * 255.0f, 255.0f ) );
			color->b = (BYTE) ( min( this->map[row][col].resultColor[2] * 255.0f, 255.0f ) );
		}
	}

	png.saveToFile( buf );

	return true;
}

void a3dLightMap::translate( Matrix4 basis, bool recalcNormal ) { 
	
	for( int pi = 0; pi < this->planeColl->planeCount; pi++ ) { 
		this->planeColl->plane[pi].translate( basis, recalcNormal );
	}

	return ;
}

void a3dLightMap::clear() { 

	if( this->pixels != null ) 
		delete[] this->pixels;
	this->pixels = null;		

	if( this->map != null ) 
		delete[] this->map;
	this->map = null;

	this->width = this->height = 0;
	this->planeColl = null;

	return ;
}

void a3dLightMap::mapPlaneToLightMap( a3dLightMapPlaneInfo &plane ) { 
	
	Vector2 a, b, c;
	
	// copy vectors
	copyVector2( plane.ta, a );
	copyVector2( plane.tb, b );
	copyVector2( plane.tc, c );

	// scale to texture size
	scaleVector2( a, (float)this->width - 1, (float)this->height - 1 );	
	scaleVector2( b, (float)this->width - 1, (float)this->height - 1 );
	scaleVector2( c, (float)this->width - 1, (float)this->height - 1 );

	// arrange a, b, c  by Y
    if (b[1] < a[1])
		swapVectors2( a, b );
    if (c[1] < a[1])
		swapVectors2( a, c );
    // now a is top
    // check b and c
    if (b[1] > c[1])
		swapVectors2( b, c );

	float du13 = 0.0f, du12 = 0.0f, du23 = 0.0f;
	
	if( c[1] != a[1] ) 
		du13 = (c[0] - a[0]) / (c[1] - a[1]);
	if( b[1] != a[1] )
		du12 = (b[0] - a[0]) / (b[1] - a[1]);
	if( c[1] != b[1] )
		du23 = (c[0] - b[0]) / (c[1] - b[1]);
	
	float u1 = a[0], u2 = a[0];
	
	float ddu13 = du13;

	// arrange x1, x2 from left to right
	if( du13 > du12 )
		swapFloat( du13, du12 );

	float u, v;
	for( v = a[1]; v < b[1]; v += 1.0f ) { 
		for( u = u1; u < u2; u += 1.0f ) { 
			// initialize lightMapPixel (u, v)
			this->map[ float2Int( v ) ][ float2Int( u ) ].initialize( &plane, u / ( this->width - 1 ), v / ( this->height - 1 ) );
		}
		u1 += du13;
		u2 += du12;
	}

	// if there is no upper half triangle
	if( a[1] == b[1] ) { 
		u1 = a[0];
		u2 = b[0];
		if( u1 > u2 )
			swapFloat( u1, u2 );
	} 

	if( ddu13 < du23 )
		swapFloat( ddu13, du23 );

	for( v = b[1]; v < c[1]; v += 1.0f ) { 
		for( u = u1; u < u2; u += 1.0f ) { 
			// initialize lightMapPixel (u, v)
			this->map[ float2Int( v ) ][ float2Int( u ) ].initialize( &plane, u / ( this->width - 1 ), v / ( this->height - 1 ) );
		}
		u1 += ddu13;
		u2 += du23;
	}

	return ;
}

void a3dLightMap::initializePixels() { 

	if( this->planeColl == null ) return ;
	
	for( int pi = 0; pi < this->planeColl->planeCount; pi++ ) { 
		this->mapPlaneToLightMap( this->planeColl->plane[pi] );
		this->logUsedPixelsToImage( TEXT( "c:\\temp\\debug.png" ) );
	}

	return ;
}

void a3dLightMap::preCalcLightMap() { 
	
	if( this->planeColl == null )
		return ;
	
	this->planeColl->preCalc();
}

void a3dLightMap::bindPlanesToTree( a3dOcTreeManager *manager ) { 
	
	if( this->planeColl == null ) return;
	this->planeColl->bindToTree( manager );
}

void a3dLightMapPlaneCollection::bindToTree( a3dOcTreeManager *manager ) { 
	
	if( this->plane == null ) return;   
	
	int pi;
	for( pi = 0; pi < this->planeCount; pi++ ) { 
		this->plane[pi].bindToTree( manager );
	}

	return ;
}

void a3dLightMapPlaneCollection::preCalc() {
	
	if( this->plane == null )
		return;
	
	int pi;
	for( pi = 0; pi < this->planeCount; pi++ ) { 
		this->plane[pi].initializeNormal();
		//this->plane[pi].initializeBarycentricKoeffs();
		this->plane[pi].calcDominantAxisIndex();
	}
}

void a3dLightMapPlaneCollection::initializeDefault() { 

	this->plane = null;
	this->planeCount = 0;

	return;
}

void a3dLightMapPlaneCollection::initialize( int planeCount ) { 
	
	this->planeCount = planeCount;
	this->plane = new a3dLightMapPlaneInfo[ this->planeCount ];
	
	return ;
}

void a3dLightMapPlaneCollection::clear() { 
	
	if( this->plane != null ) 
		delete[] this->plane;
	this->plane = null;

	this->planeCount = 0;
}

void a3dRadiosityCalculator::initializeAcosTable() { 
	
	float t = -1.0f;
	this->acosTable = new float[20000];
	
	for( int i = 0; i < 20000; i++, t+= 0.0001f ) {
		this->acosTable[i] = acos( (float)t );
	}
}

int a3dRadiosityCalculator::calcIntTableSize() { 
	
	std::list<a3dLightMap*>::iterator lli;

	int count = 0;
	for( lli = this->lightMapList.begin(); lli != this->lightMapList.end(); lli++ ) { 
		count += (*lli)->width * (*lli)->height;
	}

	return count;
}

void a3dRadiosityCalculator::initializeIntTable() {
	 
	this->intTableSize = this->calcIntTableSize();
	
	this->intTable = new BYTE*[ this->intTableSize ];
	for( int i = 0; i < this->intTableSize; i++ ) { 
		this->intTable[i] = new BYTE[ this->intTableSize ];
		memset( this->intTable[i], 0xff, this->intTableSize );	// by default uninitialized value = 0xff
	}

	// initialize light map pixel indexes in int table
	std::list<a3dLightMap*>::iterator lli;
	a3dLightMap *lm;

	int index = 0, r, c;
	for( lli = this->lightMapList.begin(); lli != this->lightMapList.end(); lli++ ) { 
		lm = (*lli);
		for( r = 0; r < lm->height; r++ ) { 
			for( c = 0; c < lm->width; c++ ) { 
				lm->map[r][c].intMapIndex = index;
				index++;
			}
		} 
	}
	
	// calculating intersection
	for( lli = this->lightMapList.begin(); lli != this->lightMapList.end(); lli++ ) { 
		lm = (*lli);
		for( r = 0; r < lm->height; r++ ) { 
			for( c = 0; c < lm->width; c++ ) { 
				this->calcPixelIntersection( &lm->map[r][c] );
			}
		}
	}
	
	return ;
}

void a3dRadiosityCalculator::calcPixelIntersection( a3dLightMapPixelInfo *pixel ) { 
	
	std::list<a3dLightMap*>::iterator lli;
	a3dLightMap *lm;
	int r, c;

	// calculating intersection
	for( lli = this->lightMapList.begin(); lli != this->lightMapList.end(); lli++ ) { 
		lm = (*lli);
		for( r = 0; r < lm->height; r++ ) { 
			for( c = 0; c < lm->width; c++ ) { 
				if( this->isIntersects( pixel, &lm->map[r][c] ) ) 
					this->intTable[pixel->intMapIndex][lm->map[r][c].intMapIndex] = 0x01;
				else 
					this->intTable[pixel->intMapIndex][lm->map[r][c].intMapIndex] = 0x00;
			}
		}
	}
	
	return ;
}

void a3dRadiosityCalculator::initializeDefault() { 
	
	this->statusTextSize = sizeof( this->statusText ) / sizeof( TCHAR );
	clearVector3( this->ambientColor );
	this->scaleCos = 0.7f;
	this->reflectionKoeff = 0.33333f;
	this->radiosityKoeff = 0.66666f;
	this->radiosityPassCount = 1;
	this->scene = null;
	this->intTable = null;
	this->intTableSize = 0;

	this->initializeAcosTable();

	return ;
}

void a3dRadiosityCalculator::initialize( a3dScene *scene, int levelCount ) { 

	this->initialize( scene->getLightMaps(), scene->getLightMapCount() );
	this->initializeOcTreeManager( levelCount );
	this->initializeLights( scene );
	this->scene = scene;

	this->preCalcLightMaps();
	this->bindLightMapsToTree();

	return ;
}

void a3dRadiosityCalculator::preCalcLightMaps() { 

	std::list<a3dLightMap*>::iterator li;
	int lIndex;
	for( lIndex = 0, li = this->lightMapList.begin(); li != this->lightMapList.end(); li++, lIndex ++ ) { 
		(*li)->preCalcLightMap();
	}
}

void a3dRadiosityCalculator::initializeLights( a3dScene *scene ) { 

	int li;
	for( li = 0; li < scene->lightsCount; li++ ) { 
		this->lightList.push_back( scene->lights[li] );
	}   
	
	return ;
}

void a3dRadiosityCalculator::initialize( a3dLightMap** lmList, int lightMapCount ) { 

	this->initializeDefault();
	for( int i = 0; i < lightMapCount; i++ ) { 
		this->lightMapList.push_back( lmList[i] );
	}
	
	return ;
}

void a3dRadiosityCalculator::clear() { 
	
	this->lightMapList.clear();
	this->lightList.clear();
	this->ocTreeManager.clear(); 
	
	clearVector3( this->ambientColor );
	this->scaleCos = 0.7f;

	if( this->acosTable != null )
		delete[] this->acosTable;
	this->acosTable = null;

	if( this->intTable != null ) { 
		for( int i = 0; i < this->intTableSize; i++ )
			delete[] this->intTable[i];
		delete[] this->intTable;
	}
	
	this->intTable = null;
	this->intTableSize = 0;
}

void a3dRadiosityCalculator::initializeOcTreeManager( int levelCount ) { 

	Vector3 a, b;

	this->calcOcTreeVolume( a, b );
	this->ocTreeManager.initialize( a, b, levelCount );
	
	return ;
}

void a3dRadiosityCalculator::bindLightMapsToTree() { 

	std::list< a3dLightMap *>::iterator lli;
	
	for( lli = this->lightMapList.begin(); lli != this->lightMapList.end(); lli++ ) {
		(*lli)->bindPlanesToTree( &this->ocTreeManager );
	} 
	
	return ;
}

void a3dRadiosityCalculator::calcOcTreeVolume( Vector3 a, Vector3 b ) { 

	a3dLightMapPlaneCollection *planeColl;
	std::list< a3dLightMap *>::iterator lli;
	int pIndex;

	// initial values
	lli = this->lightMapList.begin();
	if( lli == this->lightMapList.end() ) return;
	
	copyVector3( (*lli)->planeColl->plane[0].a, a );
	copyVector3( a, b );
	
	for( lli = this->lightMapList.begin(); lli != this->lightMapList.end(); lli++ ) { 
		planeColl = (*lli)->planeColl;
		for( pIndex = 0; pIndex < planeColl->planeCount; pIndex++ ) { 
			if( a[0] > planeColl->plane[pIndex].a[0] ) a[0] = planeColl->plane[pIndex].a[0];
			if( a[1] < planeColl->plane[pIndex].a[1] ) a[1] = planeColl->plane[pIndex].a[1];
			if( a[2] < planeColl->plane[pIndex].a[2] ) a[2] = planeColl->plane[pIndex].a[2];
			
			if( a[0] > planeColl->plane[pIndex].b[0] ) a[0] = planeColl->plane[pIndex].b[0];
			if( a[1] < planeColl->plane[pIndex].b[1] ) a[1] = planeColl->plane[pIndex].b[1];
			if( a[2] < planeColl->plane[pIndex].b[2] ) a[2] = planeColl->plane[pIndex].b[2];

			if( a[0] > planeColl->plane[pIndex].c[0] ) a[0] = planeColl->plane[pIndex].c[0];
			if( a[1] < planeColl->plane[pIndex].c[1] ) a[1] = planeColl->plane[pIndex].c[1];
			if( a[2] < planeColl->plane[pIndex].c[2] ) a[2] = planeColl->plane[pIndex].c[2];

			if( b[0] < planeColl->plane[pIndex].a[0] ) b[0] = planeColl->plane[pIndex].a[0];
			if( b[1] > planeColl->plane[pIndex].a[1] ) b[1] = planeColl->plane[pIndex].a[1];
			if( b[2] > planeColl->plane[pIndex].a[2] ) b[2] = planeColl->plane[pIndex].a[2];
			
			if( b[0] < planeColl->plane[pIndex].b[0] ) b[0] = planeColl->plane[pIndex].b[0];
			if( b[1] > planeColl->plane[pIndex].b[1] ) b[1] = planeColl->plane[pIndex].b[1];
			if( b[2] > planeColl->plane[pIndex].b[2] ) b[2] = planeColl->plane[pIndex].b[2];

			if( b[0] < planeColl->plane[pIndex].c[0] ) b[0] = planeColl->plane[pIndex].c[0];
			if( b[1] > planeColl->plane[pIndex].c[1] ) b[1] = planeColl->plane[pIndex].c[1];
			if( b[2] > planeColl->plane[pIndex].c[2] ) b[2] = planeColl->plane[pIndex].c[2];
		}
	}

	// increase a and b
	a[0] -= 1.0f;
	a[1] += 1.0f;
	a[2] += 1.0f;

	b[0] += 1.0f;
	b[1] -= 1.0f;
	b[2] -= 1.0f;
	
	return ;
}

void a3dRadiosityCalculator::calculate( bool calcRadiosityAfterLightMap, radiosityDelegate fp ) { 

	this->calcLightMaps( fp );
	if( calcRadiosityAfterLightMap ) 
		this->calcRadiosity( fp );
	
	return ;
}

void a3dRadiosityCalculator::calcOmniLightRay( a3dLightMapPixelInfo *pixel, a3dOmniLight *light, Vector3 ray ) { 

	subVector3( pixel->point, light->pos, ray );
	//normalizeVector( ray );

	return ;
}

void a3dRadiosityCalculator::calcDirectionalLightRay( a3dLightMapPixelInfo *pixel, a3dDirectionalLight *light, Vector3 ray ) { 

	copyVector3( light->dir, ray );
	//invertVector3( ray, ray );
	normalizeVector( ray );

	return;
}

void a3dRadiosityCalculator::clearProcessedFlag() { 
	
	std::list<void*>::iterator pli;
	a3dLightMapPlaneInfo	*pl;
	a3dOcTree	*node;

	for( int i = 0; i < this->ocTreeManager.passedNodeCount; i ++ ) { 
		node = this->ocTreeManager.endNodeList[ i ];
		if( node->bindingList == null ) continue;
		for( pli = node->bindingList->begin(); pli != node->bindingList->end(); pli ++ ) { 
			pl = (a3dLightMapPlaneInfo*)(*pli);
			pl->processed = false;
		}
	}

}

bool a3dRadiosityCalculator::isIntersects( a3dLightMapPixelInfo *pixel, a3dLightMapPixelInfo *pixel2 ) { 
	
	if( pixel->ownerPlane == null || pixel2->ownerPlane == null || pixel->ownerPlane == pixel2->ownerPlane ) return false;
	// уже считали но наоборот
	if( this->intTable[ pixel2->intMapIndex ][ pixel->intMapIndex ] != 0xff ) return this->intTable[ pixel2->intMapIndex ][ pixel->intMapIndex ] == 0x01; // intersects

	Vector3 dir;
	subVector3( pixel2->point, pixel->point, dir );
	normalizeVector( dir );
		 
	a3dLightMapPlaneInfo *plane;
	bool res = this->isIntersects( pixel, pixel2->point, dir, &plane );
	
	return res;
}

bool a3dRadiosityCalculator::isIntersects( a3dLightMapPixelInfo *pixel, Vector3 point, Vector3 dir, a3dLightMapPlaneInfo **plane ) { 
	
	this->ocTreeManager.updateByRay( point, dir );
	this->ocTreeManager.updatePassedNodeList();
	
	std::list<void*>::iterator pli;
	a3dLightMapPlaneInfo	*pl;
	a3dOcTree	*node;
	Vector3 intPoint;

	this->clearProcessedFlag();

	for( int i = 0; i < this->ocTreeManager.passedNodeCount; i ++ ) { 
		node = this->ocTreeManager.endNodeList[ i ];
		if( node->bindingList == null ) continue;
		for( pli = node->bindingList->begin(); pli != node->bindingList->end(); pli ++ ) { 
			pl = (a3dLightMapPlaneInfo*)(*pli);
			
			// do not process itself ;)
			// no need to process plane second time
			if( pixel->ownerPlane == pl || pl->processed ) 
				continue;  
			
			pl->processed = true;
			if( isSimpleRayIntersectsTriangle( point, dir, pl->a, pl->b, pl->c, pl->normal, intPoint, pl->dominantAxisIndex ) && 
				checkNearPoint( intPoint, pixel->point, point ) ) { 
				*plane = pl;
				return true; 
			}
		}  
	}
	
	return false;
}

void a3dLightMapPixelInfo::calcLightColor( Vector3 lightColor, float intensity ) { 
	
	this->resultColor[0] = min( this->resultColor[0] + lightColor[0] * intensity, 1.0f );
	this->resultColor[1] = min( this->resultColor[1] + lightColor[1] * intensity, 1.0f );
	this->resultColor[2] = min( this->resultColor[2] + lightColor[2] * intensity, 1.0f );
	
	return ;
}

void a3dLightMapPixelInfo::calcLightColor( Vector3 lightColor, float intensity, float reflectionKoeff ) { 

	float k1 = ( 1.0f - reflectionKoeff ) * intensity, 
			k2 = reflectionKoeff * intensity;
	
	// result color clamp with 1.0f
	this->resultColor[0] = min( this->resultColor[0] + lightColor[0] * k1, 1.0f );
	this->resultColor[1] = min( this->resultColor[1] + lightColor[1] * k1, 1.0f );
	this->resultColor[2] = min( this->resultColor[2] + lightColor[2] * k1, 1.0f );
		
	// reflection part
	this->emmitColor[0] = min( this->emmitColor[0] + lightColor[0] * k2, 1.0f );
	this->emmitColor[1] = min( this->emmitColor[1] + lightColor[1] * k2, 1.0f );
	this->emmitColor[2] = min( this->emmitColor[2] + lightColor[2] * k2, 1.0f );

	return ;
}

void a3dRadiosityCalculator::calcLightPixel( a3dLightMapPixelInfo *pixel, a3dOmniLight *light ) {

	a3dLightMapPlaneInfo *plane;
	Vector3 ray;
	float dist, cosa, intensity;
	
	// calc ray vector
	this->calcOmniLightRay( pixel, light, ray );
	
	// calc distance
	dist = normalizeVector( ray );
 
	// better for omni is check distance greater than max (little optimization)
	// use farAttendEnd in 3d studio
	// nearAttend is ignored
	if( dist > light->farAttenEnd ) 
		return; // in shadow
	
	if( scalarMulOfVector( ray, pixel->ownerPlane->normal ) > 0.0f )
		return; // back side

	if( this->isIntersects(pixel, light->pos, ray, &plane ) )
		return; // in shadow
	
	// angle 
	cosa = ray[0] * pixel->normal[0] + ray[1] * pixel->normal[1] + ray[2] * pixel->normal[2];
	cosa = -cosa;
	// part of the light not reflecting so...
	cosa = ( 1.0f - this->scaleCos ) + cosa * this->scaleCos;
	
	// calculate fading
	intensity = ( light->farAttenEnd - dist ) / light->farAttenEnd;
	intensity *= cosa;

	// calc result color
	pixel->calcLightColor( light->color, intensity );
	//pixel->calcLightColor( light->color, intensity, this->reflectionKoeff );

	return ;
}

void a3dRadiosityCalculator::calcLightPixel( a3dLightMapPixelInfo *pixel, a3dDirectionalLight *light ) {

	a3dLightMapPlaneInfo *plane;
	Vector3 ray, dist, startPoint;
	float cosa, intensity;

	this->calcDirectionalLightRay( pixel, light, ray );
	
	// calc direct light start point (far)
	copyVector3( ray, dist );
	scaleVector3( dist, -100000.0f );
	addVector3( pixel->point, dist, startPoint );

	if( this->isIntersects( pixel, startPoint, ray, &plane ) )
		return; // in shadow
	
	if( scalarMulOfVector( ray, pixel->ownerPlane->normal ) > 0.0f )
		return; // back side

	normalizeVector( ray );
	
	// angle 
	cosa = ray[0] * pixel->normal[0] + ray[1] * pixel->normal[1] + ray[2] * pixel->normal[2];
	cosa = -cosa;
	// part of the light not reflecting so...
	cosa = ( 1.0f - this->scaleCos ) + cosa * this->scaleCos;
	
	// no fading
	intensity = cosa;

	// calc result color
	pixel->calcLightColor( light->color, intensity );
	//pixel->calcLightColor( light->color, intensity, this->reflectionKoeff );

	return ;
}

void a3dRadiosityCalculator::calcLightPixel( a3dLightMapPixelInfo *pixel ) { 
	
	std::list< a3dLightBase* >::iterator lli;

	for( lli = this->lightList.begin(); lli != this->lightList.end(); lli++ ) { 
		if( (*lli)->type == LIGHT_OMNI ) 
			this->calcLightPixel( pixel, (a3dOmniLight*) (*lli) );
		else if( (*lli)->type == LIGHT_DIRECTIONAL ) 
			this->calcLightPixel( pixel, (a3dDirectionalLight*) (*lli) );
	}

	return ;
}

void a3dRadiosityCalculator::calcLightMap( a3dLightMap *lightMap, radiosityDelegate fp ) { 

	int u, v;
	for( v = 0; v < lightMap->height; v++ ) {
		for( u = 0; u < lightMap->width; u ++ ) { 
			// this map pixel not belong to any plane
			if( lightMap->map[v][u].ownerPlane == null ) continue;
			
			// calc pixel
			this->calcLightPixel( &lightMap->map[v][u] );
		}
	}
	
	return ;
}

void a3dRadiosityCalculator::calculate( a3dScene *scene, int levelCount, bool calcRadiosityAfterLightMap, radiosityDelegate fp  ) {

	this->initialize( scene, levelCount );
	this->calcLightMaps( fp );
	
	if( calcRadiosityAfterLightMap )
		this->calcRadiosity( fp );

	return ;
}

void a3dRadiosityCalculator::calcLightMaps( radiosityDelegate fp ) { 

	std::list<a3dLightMap*>::iterator li;
	int lIndex;
	for( lIndex = 0, li = this->lightMapList.begin(); li != this->lightMapList.end(); li++, lIndex ++ ) { 
		if( fp != null ) { 
			_stprintf_s( this->statusText, this->statusTextSize, TEXT( "processing light map %d of %d -> '%s' ...\n" ), lIndex, this->lightMapList.size(), (*li)->fileName.getCString() ); 
			fp( this->statusText );
		}
		// calc light map
		this->calcLightMap( (*li), fp );
		if( fp != null ) { 
			_stprintf_s( this->statusText, this->statusTextSize, TEXT( "done.\n" ) ); 
			fp( this->statusText );
		}
	}

	return ;
}

void a3dRadiosityCalculator::formFactor( a3dLightMapPixelInfo *pixel1, a3dLightMapPixelInfo *pixel2 ) { 

	Vector3 ray;
	
	// они не могут друг с другом...
	if( pixel1->ownerPlane == pixel2->ownerPlane ) return;
	
	if( this->intTable[pixel1->intMapIndex][pixel2->intMapIndex] == 1 ) //intersects
		return; // do not calculate	

	// calc ray
	subVector3( pixel1->point, pixel2->point, ray );
	float lenSqr = quicknormalizeVectorAndRetSquareDist( ray );

	float cos2 = scalarMulOfVector( pixel2->normal, ray );
	
	//invertVector3( ray, ray );
	// invert and scalar
	float cos1 = - pixel1->normal[0] * ray[0] - pixel1->normal[1] * ray[1] - pixel1->normal[2] * ray[2]; //scalarMulOfVector( pixel1->normal, ray );
		
	float angle2 = this->acosTable[(int)( cos2 * 10000.0f ) + 10000];
	float ffactor = (float)(cos1 * cos2 * angle2 / ( M_PI * lenSqr + angle2 ));
	
	float k1 = this->radiosityKoeff * ffactor;
	pixel2->resultColor[0] += k1 * pixel1->emmitColor[0];
	pixel2->resultColor[1] += k1 * pixel1->emmitColor[1];
	pixel2->resultColor[2] += k1 * pixel1->emmitColor[2];

	float k2 = ffactor - k1;
	pixel2->emmitColor[0] += k2 * pixel1->emmitColor[0];
	pixel2->emmitColor[1] += k2 * pixel1->emmitColor[1];
	pixel2->emmitColor[2] += k2 * pixel1->emmitColor[2];
	
	return ;
}

void a3dRadiosityCalculator::calcPixelRadiosity( a3dLightMapPixelInfo *pixel ) { 

	std::list<a3dLightMap*>::iterator lli;
	a3dLightMap *lm;
	int u, v;

	for( lli = this->lightMapList.begin(); lli != this->lightMapList.end(); lli++ ) { 
		
		lm = (*lli);
		for( v = 0; v < lm->height; v++ ) { 
			for( u = 0; u < lm->width; u++ ) { 
				formFactor( pixel, &lm->map[v][u] );
			}
 		} 
	}
	
	return ;
}

void a3dRadiosityCalculator::calcMapRadiosity( a3dLightMap *lightMap, int lIndex, radiosityDelegate fp ) { 

	if( fp != null ) { 
		_stprintf_s( this->statusText, this->statusTextSize, TEXT( "radiosity processing light map %d of %d -> '%s' ...\n" ), lIndex, this->lightMapList.size(), lightMap->fileName.getCString() ); 
		fp( this->statusText );
	}

	int u, v;
	for( v = 0; v < lightMap->height; v++ ) { 
		for( u = 0; u < lightMap->width; u++ ) { 
			// empty cell
			if( lightMap->map[v][u].ownerPlane == null || !lightMap->map[v][u].isEnabled() ) continue;
			
			this->calcPixelRadiosity( &lightMap->map[v][u] );
		}
	}

	if( fp != null ) { 
		_stprintf_s( this->statusText, this->statusTextSize, TEXT( "done.\n" ) ); 
		fp( this->statusText );
	}

	return ;
}

void a3dLightMap::prepareRadiosity() { 
	
	int row, col;
	for( row = 0; row < this->height; row++ ) {
		for( col = 0; col < this->width; col++ ) { 
			copyVector3( this->map[row][col].resultColor, this->map[row][col].emmitColor );
		}
	}
	
	return ;
}

void a3dRadiosityCalculator::prepareRadiosity() { 
	
	std::list<a3dLightMap*>::iterator lli;
	for( lli = this->lightMapList.begin(); lli != this->lightMapList.end(); lli ++ ) { 
		(*lli)->prepareRadiosity();		
	}
}

void a3dRadiosityCalculator::calcRadiosity(radiosityDelegate fp) { 

	this->prepareRadiosity();
	this->initializeIntTable();
	
	std::list<a3dLightMap*>::iterator lli;		
	
	int iPass, lIndex;
	for( iPass = 0; iPass < this->radiosityPassCount; iPass++ ) {
		if( fp != null ) { 
			_stprintf_s( this->statusText, this->statusTextSize, TEXT( "radiosity pass %d of %d...\n" ), iPass, this->radiosityPassCount ); 
			fp( this->statusText );
		}
		for( lIndex = 0, lli = this->lightMapList.begin(); lli != this->lightMapList.end(); lli ++, lIndex ++ ) { 
			this->calcMapRadiosity( (*lli), lIndex, fp );
		}
		if( fp != null ) { 
			_stprintf_s( this->statusText, this->statusTextSize, TEXT( "radiosity pass %d done.\n" ), iPass ); 
			fp( this->statusText );
		}
	}

	return ;
}

void a3dRadiosityCalculator::saveLightMaps() { 
	
	std::list<a3dLightMap*>::iterator li;
	int lIndex;
	for( lIndex = 0, li = this->lightMapList.begin(); li != this->lightMapList.end(); li++, lIndex ++ ) { 
		(*li)->saveLightMap( (*li)->fileName );		
	}
} 