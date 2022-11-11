#include "railManager.h"
#include "radiosity.h"
#include "a3dDesc\\a3dDesc.h"
#include "a3dModel\\a3dModel.h"
#include "a3dModel\\a3dObject.h"
#include "a3dModel\\a3dDetail.h"
#include "a3dMaterial\\a3dShaderMaterial.h"

void a3dRailInfo::clear() { 
	
	this->bBox.clear();
	this->angle = 0.0f;
	this->length = 0.0f;
	this->width = 0.0f;
	this->radius = 0.0f;
	this->isRound = false;
	this->isLeft = false;
	this->isVisible = true;
}

bool a3dRailInfo::readFromFile( FILE *fp ) { 
	
	fscanf_s( fp, "%f", &this->bBox.basis[3][0] );
	fscanf_s( fp, "%f", &this->bBox.basis[3][2] );
	fscanf_s( fp, "%f", &this->bBox.basis[3][1] );
	fscanf_s( fp, "%f", &this->angle );
	fscanf_s( fp, "%f", &this->length );
	fscanf_s( fp, "%f", &this->width );
	fscanf_s( fp, "%f", &this->radius );
	
	fgetc(fp); // пропускаем перевод строки
	
	if( fgetc( fp ) == 'T' )
		this->isRound = true;
	else 
		this->isRound = false;
	fgetc( fp );
	fgetc( fp );
	fgetc( fp );
	if( !this->isRound ) fgetc( fp );
	
	fgetc(fp); // пропускаем перевод строки
	if( fgetc( fp ) == 'T' )
		this->isLeft = true;
	else 
		this->isLeft = false;
	fgetc( fp );
	fgetc( fp );
	fgetc( fp );
	if( !this->isLeft ) fgetc( fp );
	
	this->calcBasis();
	
	return true;
}

bool a3dRailManager::loadFromFile(shString &fileName) { 
	
	FILE *fp;

	char buf[1024];
	wcsToStr( fileName.getCString(), buf );

	fopen_s( &fp, buf, "rt" );
	
	if( fp == null ) return false;

	// очищаем
	this->clear();
	
	fscanf_s( fp, "%f", &this->landSideSize );
	fscanf_s( fp, "%f", &this->landHeight );
	fscanf_s( fp, "%f", &this->railLength );
	fscanf_s( fp, "%f", &this->railWidth );
	fscanf_s( fp, "%d", &this->woodCount );
	fscanf_s( fp, "%f", &this->scaleMeters );
	fscanf_s( fp, "%d", &this->railCount );

	this->rails = new a3dRailInfo[ this->railCount ];
	int i;
	for( i = 0; i < this->railCount; i ++ ) {
		this->rails[i].readFromFile( fp );
		this->rails[i].bBox.basis[3][0] += this->origin[0];
		this->rails[i].bBox.basis[3][1] += this->origin[1];
		this->rails[i].bBox.basis[3][2] += this->origin[2];
	}
	
	fclose( fp );
	
	// подсчитать длину
	this->calcLength();

	return true;
}

void a3dRailManager::calcLength() { 
	
	int railIndex;
	this->length = 0.0f;
	for( railIndex = 0; railIndex < this->railCount; railIndex ++ ) { 
		this->length += this->rails[ railIndex ].length;
	}

	return ;
} 

int	a3dRailManager::getPositionPoint( float posT, Vector3 pos ) { 
	
	int railIndex;
	float l = 0.0f;
	for( railIndex = 0; railIndex < this->railCount; railIndex ++ ) { 
		l += this->rails[ railIndex ].length;
		if( l > posT ) break;
	}
	
	// узнали дельту
	posT -= l - this->rails[ railIndex ].length;
	
	this->rails[ railIndex ].getPoint( posT, pos );

	return railIndex;
}

void a3dRailManager::getLinePositionPoints( float posT, float len, Vector3 pos1, Vector3 pos2 ) { 
	
	int railIndex;
	float l = 0.0f;
	for( railIndex = 0; railIndex < this->railCount; railIndex ++ ) { 
		l += this->rails[ railIndex ].length;
		if( l > posT ) break;
	}
	l -= this->rails[ railIndex ].length;

	this->rails[ railIndex ].getPoint( posT - l, pos1 );

	posT += len;
	for( ; railIndex < this->railCount; railIndex ++ ) { 
		l += this->rails[ railIndex ].length;
		if( l > posT ) break;
	}
	
	l -= this->rails[ railIndex ].length;
	this->rails[ railIndex ].getPoint( posT - l, pos2 );

	pos1[1] = origin[1];
	pos2[1] = origin[1];
}

void a3dRailInfo::getPoint( float len, Vector3 point ) { 
	
	if( this->isRound ) { 
		if( this->isLeft )
			this->getLeftPoint( len, point );
		else 
			this->getRightPoint( len, point );
	}
	else { 
		this->getLinePoint( len, point );
	}
}

void a3dRailInfo::getLeftCenterPoint(Vector3 centerPoint) { 
	
	float a = (float)(this->angle - M_PI * 0.5f);
	setVector3( centerPoint, this->bBox.basis[3][0] - this->radius * cos( a ), 0.0f, this->bBox.basis[3][2] - this->radius * sin( a ) );
}

void a3dRailInfo::getRightCenterPoint(Vector3 centerPoint) {
	
	float a = (float)(this->angle + M_PI * 0.5f);
	setVector3( centerPoint, this->bBox.basis[3][0] - this->radius * cos( a ), 0.0f, this->bBox.basis[3][2] - this->radius * sin( a ) );
}

void a3dRailInfo::getLinePoint( float len, Vector3 point ) { 
	
	setVector3( point, this->bBox.basis[3][0], this->bBox.basis[3][1], this->bBox.basis[3][2] );
	point[0] += this->bBox.basis[0][2] * len;
	point[1] += this->bBox.basis[1][2] * len;
	point[2] += this->bBox.basis[2][2] * len;
}

void a3dRailInfo::getLeftPoint(float len, Vector3 leftPoint) { 
	
	float da = len / this->radius;
    float a = (float)(this->angle - M_PI * 0.5f);
	
	setVector3( leftPoint, centerPoint[0] + this->radius * cos ( a + da ), 0.0f, centerPoint[2] + this->radius * sin ( a + da ) );
}

void a3dRailInfo::getRightPoint(float len, Vector3 rightPoint) { 
	
	float da = len / this->radius;
    float a = (float)(this->angle + M_PI * 0.5f);
	
	setVector3( rightPoint, centerPoint[0] + this->radius * cos ( a - da ), 0.0f, centerPoint[2] + this->radius * sin ( a - da ) );
}

void a3dRailInfo::getLeftRoundLeftPoint(Vector3 point, float len, float wdt) { 

	Vector3 pos, dt;
	
	this->getLeftPoint( len, pos );
	subVector3( pos, centerPoint, dt );
	normalizeVector( dt );
	setVector3(point, pos[0] + dt[0] * wdt, pos[1], pos[2] + dt[2] * wdt);
}

void a3dRailInfo::getLeftRoundRightPoint(Vector3 point, float len, float wdt) {

	Vector3 pos, dt;
	
	this->getLeftPoint( len, pos );
	subVector3( pos, centerPoint, dt );
	normalizeVector( dt );
	setVector3(point, pos[0] - dt[0] * wdt, pos[1], pos[2] - dt[2] * wdt);
}

void a3dRailInfo::getRightRoundLeftPoint(Vector3 point, float len, float wdt) { 

	Vector3 pos, dt;
	
	this->getRightPoint( len, pos );
	subVector3( pos, centerPoint, dt );
	normalizeVector( dt );
	setVector3(point, pos[0] - dt[0] * wdt, pos[1], pos[2] - dt[2] * wdt);
}

void a3dRailInfo::getRightRoundRightPoint(Vector3 point, float len, float wdt) { 

	Vector3 pos, dt;
	
	this->getRightPoint( len, pos );
	subVector3( pos, centerPoint, dt );
	normalizeVector( dt );
	setVector3(point, pos[0] + dt[0] * wdt, pos[1], pos[2] + dt[2] * wdt);
}

void a3dRailInfo::calcBasis() { 
	
	// знаем угол - остальное легко!
	float a = (float)(this->angle - M_PI * 0.5f);
	this->bBox.basis[0][0] = cos( a );
	this->bBox.basis[2][0] = sin( a );
	this->bBox.basis[1][0] = 0.0f;

	// z под углом 90 к x
	this->bBox.basis[0][2] = - this->bBox.basis[2][0];
	this->bBox.basis[2][2] = this->bBox.basis[0][0];
	this->bBox.basis[1][2] = 0.0f;

	this->bBox.basis[0][1] = 0.0f;
	this->bBox.basis[1][1] = 1.0f;
	this->bBox.basis[2][1] = 0.0f;

	if( this->isRound ) { 
		if( this->isLeft ) this->getLeftCenterPoint( this->centerPoint );
		else this->getRightCenterPoint( this->centerPoint );
	}
}

void a3dRailManager::clear() { 
	
	if( this->rails != null ) delete[] this->rails;
	
	this->rails = null;
	this->railCount = 0;
	this->lineRailModel = null;
	this->roundRailModel = null;
}


void a3dRailInfo::calcBVolume(a3dBoundBox &box) {

	this->bBox.copyWithoutBasis( box );
	if( this->isRound ) { 
		if( this->isLeft ) this->getLeftRoundLeftPoint( this->bBox.a, this->length, this->width * 0.5f );
		else this->getRightRoundLeftPoint( this->bBox.a, this->length, this->width * 0.5f );
	}
	this->bBox.updateBoundBox();
}

void a3dRailManager::setRailModels(a3dModelDesc *lineModel, a3dModelDesc *roundModel) {
	
	this->lineRailModel = lineModel;
	this->roundRailModel = roundModel;

	// проходимся по всем рельсам и копируем боундбокс
	int i;
	for( i = 0; i < this->railCount; i ++ ) { 
		// копируем без базиса потому что базис у нас свой
		if( !this->rails[i].isRound ) this->rails[i].bBox.copyWithoutBasis( lineRailModel->model->bVolume );
		else this->rails[i].bBox.copyWithoutBasis( roundRailModel->model->bVolume );
		this->rails[i].bBox.updateBoundBox();
	}
}

HRESULT a3dRailManager::prepareLineModel( a3dModel *model, a3dRailInfo *info ) { 
	
	if( model == null ) return D3D_OK;

	a3dDetail *det;
	int objIndex, detIndex;
	float scale;

	model->origin();
	copyMatrix4( info->bBox.basis, model->basis );
	scale = info->length / model->bVolume.getDepth();
	model->scale( scale, scale, scale );
	model->correct();
	
	for( objIndex = 0; objIndex < model->objCount; objIndex ++ ) {
		for( detIndex = 0; detIndex < model->object[objIndex].detailCount; detIndex ++ ) { 
			det = &model->object[objIndex].detail[detIndex];
			det->material->skipErrors = true;
			det->material->effect->lpEffect = this->lineRailEffect->lpEffect;
		}
	}

	return D3D_OK;
}

HRESULT a3dRailManager::prepareRoundModel( a3dModel *model, a3dRailInfo *info ) { 
	
	if( model == null ) return D3D_OK;

	HRESULT rv;
	a3dDetail *det;
	int objIndex, detIndex;
	float scale;

	model->origin();
	copyMatrix4( info->bBox.basis, model->basis );
	scale = info->length / model->bVolume.getDepth();
	model->scale( scale, scale, scale );
	model->correct();
	
	for( objIndex = 0; objIndex < model->objCount; objIndex ++ ) {
		for( detIndex = 0; detIndex < model->object[objIndex].detailCount; detIndex ++ ) { 
			det = &model->object[objIndex].detail[detIndex];
			det->material->skipErrors = true;

			if( info->isLeft )
				det->material->effect->lpEffect = this->roundRailEffect->lpEffect;
			else
				det->material->effect->lpEffect = this->roundRailEffect2->lpEffect;
			
			rv = det->material->effect->lpEffect->SetFloat( "Length", info->length );
			if( FAILED( rv ) ) return rv;
				
			rv = det->material->effect->lpEffect->SetFloat( "Radius", info->radius / scale );
			if( FAILED( rv ) ) return rv;
		}
	}

	return D3D_OK;

}

HRESULT	a3dRailManager::render( camera *cam ) { 
	
	a3dRailInfo *info;
	
	int railIndex;
	HRESULT rv;

	if( this->lineRailModel == null || this->roundRailModel == null ) return D3D_OK;
		//_stprintf_s( this->roundRailModel->ModelKernel->tempBuffer, TEXT("in railManager->render(): this->roundRailmodel->object[0] = 0x%8.8x this->roundRailmodel->object[0].detail = 0x%8.8x"), (DWORD)this->roundRailmodel->object, (DWORD)this->roundRailmodel->object[0].detail );
	//this->roundRailModel->ModelKernel->MessageBox( TEXT(""), MB_ICONINFORMATION );

	//if( det != this->roundRailmodel->object[0].detail || (DWORD)det == 0x00000000) { 
	//	_stprintf_s( this->roundRailModel->ModelKernel->tempBuffer, TEXT("very strange bug: detail != this->roundRailmodel->object[0].detail.  detail = 0x%8.8x   this->roundRailmodel->object[0].detail = 0x%8.8x"), (DWORD)det, (DWORD)this->roundRailmodel->object[0].detail );
	//	this->roundRailModel->ModelKernel->MessageBox( TEXT(""), MB_ICONERROR );
	//}

	for( railIndex = 0; railIndex < this->railCount; railIndex ++ ) { 
		info = &this->rails[ railIndex ];
		if( !info->isVisible ) continue;
		
		if( info->isRound ) { 
			
			// устанавливаем параметры
			rv = this->prepareRoundModel( this->roundRailModel->model, info );
			if( FAILED( rv ) ) return rv;
			rv = this->roundRailModel->model->render( cam );
			if( FAILED( rv ) ) { 
				this->roundRailModel->kernel->DxErrorBox( TEXT( "railManager->render()" ), TEXT( "Error: this->roundRailModel->Render()." ), rv );
				return rv;
			}
		}
		else { 
			rv = this->prepareLineModel( this->lineRailModel->model, info );
			if( FAILED( rv ) ) return rv;
			rv = this->lineRailModel->model->render( cam );
			if( FAILED( rv ) ) return rv;
		}
	}
	
	return D3D_OK;
}

void a3dRailManager::setRailEffects( a3dEffectDesc *lineEffect, a3dEffectDesc *roundEffect, a3dEffectDesc *roundEffect2 ) { 
	
	this->lineRailEffect = lineEffect;
	this->roundRailEffect = roundEffect;
	this->roundRailEffect2 = roundEffect2;
}

void a3dRailManager::setOrigin( float x, float y, float z ) { 
	
	Vector3 v = { x, y, z };
	this->setOrigin( v );
}

void a3dRailManager::setOrigin( Vector3 origin ) { 
	
	int i;
	
	Vector3 delta;
	subVector3( origin, this->origin, delta );
	for( i = 0; i < this->railCount; i ++ ) { 
		this->rails[ i ].bBox.basis[3][0] += delta[0];
		this->rails[ i ].bBox.basis[3][1] += delta[1];
		this->rails[ i ].bBox.basis[3][2] += delta[2];
		if( this->rails[ i ].isRound ) { 
			addVector3( this->rails[i].centerPoint, origin, this->rails[i].centerPoint );
		}
		this->rails[ i ].bBox.updateBoundBox();
	}
	copyVector3( origin, this->origin );
}

int a3dRailManager::getLightMapCount() { 
	
	if( this->lineRailModel == null || this->roundRailModel == null ) return 0;
	
	int lightMapCount = 0;
	for( int ri = 0; ri < this->railCount; ri++ ) { 
		if( this->rails[ri].isRound ) 
			lightMapCount += this->roundRailModel->model->getLightMapCount();
		else 
			lightMapCount += this->lineRailModel->model->getLightMapCount();
	}
	
	return lightMapCount;
}

void getLeftPointCore( float radius, Vector3 v, Vector3 dst ) { 
	
	float da = v[2] / radius;
	setVector3( dst,  - radius + radius * cos ( da ), v[1], radius * sin(da) );
}
void updateLeftPoint( a3dRailInfo *ri, Vector3 pos ) { 
	
	Vector3 v, cp;
	
	setVector3( cp, -ri->radius, 0.0f, 0.0f );
	getLeftPointCore( ri->radius, pos, v );
	subVector3(v, cp, cp);
	normalizeVector(cp);
	scaleVector3(cp, pos[0]);
	addVector3(v, cp, pos);
}

void getRightPointCore( float radius, Vector3 pos, Vector3 dst ) {
	
	float da = (float)(M_PI - pos[2] / radius);
	setVector3( dst, radius + radius * cos( da ), pos[1], radius * sin( da ) );
}

void updateRightPoint( a3dRailInfo *ri, Vector3 pos ) {
	
	Vector3 v, cp;
	
	getRightPointCore( ri->radius, pos, v );
	setVector3( cp, ri->radius, 0.0f, 0.0f );
	subVector3(v, cp, cp);
	normalizeVector(cp);
	scaleVector3( cp, pos[0]);
	addVector3(v, cp, pos);
}

void parseNormal( Vector3 axis, Vector3 normal, Vector3 nEnd ) { 
	addVector3( axis, normal, nEnd);
}

void processLeftNormal( a3dRailInfo *ri, Vector3 axis, Vector3 normal ) {
	
	Vector3 nEnd;
	parseNormal(axis, normal, nEnd);
	updateLeftPoint(ri, nEnd);
	copyVector3(nEnd, normal);
}

void processRightNormal( a3dRailInfo *ri, Vector3 axis, Vector3 normal ) {
	
	Vector3 nEnd;
	parseNormal(axis, normal, nEnd);
	updateRightPoint(ri, nEnd);
	copyVector3(nEnd, normal);
}

void processRailManagerLightPlane( a3dLightMapPlaneInfo *info, void *lpData ) { 
	
	a3dRailInfo *ri = (a3dRailInfo*)lpData;
	if( ri == null || !ri->isRound ) return;
		
	if( ri->isLeft ) { 
		processLeftNormal(ri, info->a, info->na);
		processLeftNormal(ri, info->b, info->nb);
		processLeftNormal(ri, info->c, info->nc);

		updateLeftPoint(ri, info->a);
		updateLeftPoint(ri, info->b);
		updateLeftPoint(ri, info->c);
	}	
	else { 
		processRightNormal(ri, info->a, info->na);
		processRightNormal(ri, info->b, info->nb);
		processRightNormal(ri, info->c, info->nc);

		updateRightPoint(ri, info->a);
		updateRightPoint(ri, info->b);
		updateRightPoint(ri, info->c);
	}

	subVector3(info->na, info->a, info->na);
	subVector3(info->nb, info->b, info->nb);
	subVector3(info->nc, info->c, info->nc);

	normalizeVector(info->na);
	normalizeVector(info->nb);
	normalizeVector(info->nc);
	
	return;
}

a3dLightMap** a3dRailManager::getLightMaps() { 
	
	if( this->lineRailModel == null || this->roundRailModel == null ) return null;
	
	int lightMapCount = this->getLightMapCount();
	if(lightMapCount == 0) return null;

	a3dLightMap **lmList = new a3dLightMap*[ lightMapCount ];
	a3dLightMap **lmTemp;
	a3dModel	*model;
	int			tempListCount, li, lmIndex = 0;

	for( int ri = 0; ri < this->railCount; ri++ ) {
		if( this->rails[ri].isRound )
			model = this->roundRailModel->model;
		else 
			model = this->lineRailModel->model;
		
		tempListCount = model->getLightMapCount();
		if(tempListCount == 0) continue;

		lmTemp = model->getLightMaps( processRailManagerLightPlane, &this->rails[ri] );
		
		for( li = 0; li < tempListCount; li++, lmIndex++ ) {
			lmList[lmIndex] = lmTemp[li];
		}
		
		delete[] lmTemp;
	}

	return lmList;
}