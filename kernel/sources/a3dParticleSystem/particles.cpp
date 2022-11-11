#include "particles.h"
#include "..\\utils.h"
#include "..\\rand.h"
#include "..\\a3dDesc\\a3dEffectDesc.h"

void a3dSmokeParticleSystem::initializeDefault() { 
	
	this->dirDelta = 0.0f;
	this->smokeSpeed1 = 0.0f;
	this->smokeSpeed1Delta = 0.0f;
	this->smokeTime1 = 0.0f;
	this->smokeTime1Delta = 0.0f;
	this->smokeSize1 = 0.0f;
	this->smokeSize1Delta = 0.0f;
	this->smokeSpeed2 = 0.0f;
	this->smokeSpeed2Delta = 0.0f;
	this->smokeTime2 = 0.0f;
	this->smokeTime2Delta = 0.0f;
	this->smokeSize2 = 0.0f;
	this->smokeSize2Delta = 0.0f;
	this->smokeSize3 = 0.0f;
	this->smokeSize3Delta = 0.0f;
	this->alpha1 = 0.0f;
	this->alpha1Delta = 0.0f;
	this->alpha2 = 0.0f;
	this->alpha2Delta = 0.0f;
	this->alpha3 = 0.0f;
	this->alpha3Delta = 0.0f;
	this->particleIndex = 0;		
}

void a3dSmokeParticleSystem::registerDependencyProperties() { 

	particleSystemBase::registerDependencyProperties();

	this->dirDeltaProperty				= this->registerDependencyProperty(TEXT("DirDelta"), TEXT("dirDelta"), DPROP_FLOAT, (BYTE*)&(this->dirDelta));
	this->smokeSpeed1Property			= this->registerDependencyProperty(TEXT("SmokeSpeed1"), TEXT("smokeSpeed1"), DPROP_FLOAT, (BYTE*)&(this->smokeSpeed1));
	this->smokeSpeed1DeltaProperty		= this->registerDependencyProperty(TEXT("SmokeSpeed1Delta"), TEXT("smokeSpeed1Delta"), DPROP_FLOAT, (BYTE*)&(this->smokeSpeed1Delta));
	this->smokeTime1Property			= this->registerDependencyProperty(TEXT("SmokeTime1"), TEXT("smokeTime1"), DPROP_FLOAT, (BYTE*)&(this->smokeTime1));
	this->smokeTime1DeltaProperty		= this->registerDependencyProperty(TEXT("SmokeTime1Delta"), TEXT("smokeTime1Delta"), DPROP_FLOAT, (BYTE*)&(this->smokeTime1Delta));
	this->smokeSize1Property			= this->registerDependencyProperty(TEXT("SmokeSize1"), TEXT("smokeSize1"), DPROP_FLOAT, (BYTE*)&(this->smokeSize1));
	this->smokeSize1DeltaProperty		= this->registerDependencyProperty(TEXT("SmokeSize1Delta"), TEXT("smokeSize1Delta"), DPROP_FLOAT, (BYTE*)&(this->smokeSize1Delta));
	this->smokeSpeed2Property			= this->registerDependencyProperty(TEXT("SmokeSpeed2"), TEXT("smokeSpeed2"), DPROP_FLOAT, (BYTE*)&(this->smokeSpeed2));
	this->smokeSpeed2DeltaProperty		= this->registerDependencyProperty(TEXT("SmokeSpeed2Delta"), TEXT("smokeSpeed2Delta"), DPROP_FLOAT, (BYTE*)&(this->smokeSpeed2Delta));
	this->smokeTime2Property			= this->registerDependencyProperty(TEXT("SmokeTime2"), TEXT("smokeTime2"), DPROP_FLOAT, (BYTE*)&(this->smokeTime2));
	this->smokeTime2DeltaProperty		= this->registerDependencyProperty(TEXT("SmokeTime2Delta"), TEXT("smokeTime2Delta"), DPROP_FLOAT, (BYTE*)&(this->smokeTime2Delta));
	this->smokeSize2Property			= this->registerDependencyProperty(TEXT("SmokeSize2"), TEXT("smokeSize2"), DPROP_FLOAT, (BYTE*)&(this->smokeSize2));
	this->smokeSize2DeltaProperty		= this->registerDependencyProperty(TEXT("SmokeSize2Delta"), TEXT("smokeSize2Delta"), DPROP_FLOAT, (BYTE*)&(this->smokeSize2Delta));
	this->smokeSize3Property			= this->registerDependencyProperty(TEXT("SmokeSize3"), TEXT("smokeSize3"), DPROP_FLOAT, (BYTE*)&(this->smokeSize3));
	this->smokeSize3DeltaProperty		= this->registerDependencyProperty(TEXT("SmokeSize3Delta"), TEXT("smokeSize3Delta"), DPROP_FLOAT, (BYTE*)&(this->smokeSize3Delta));
	this->alpha1Property				= this->registerDependencyProperty(TEXT("Alpha1"), TEXT("alpha1"), DPROP_FLOAT, (BYTE*)&(this->alpha1));
	this->alpha1DeltaProperty			= this->registerDependencyProperty(TEXT("Alpha1Delta"), TEXT("alpha1Delta"), DPROP_FLOAT, (BYTE*)&(this->alpha1Delta));
	this->alpha2Property				= this->registerDependencyProperty(TEXT("Alpha2"), TEXT("alpha2"), DPROP_FLOAT, (BYTE*)&(this->alpha2));
	this->alpha2DeltaProperty			= this->registerDependencyProperty(TEXT("Alpha2Delta"), TEXT("alpha2Delta"), DPROP_FLOAT, (BYTE*)&(this->alpha2Delta));
	this->alpha3Property				= this->registerDependencyProperty(TEXT("Alpha3"), TEXT("alpha3"), DPROP_FLOAT, (BYTE*)&(this->alpha3));
	this->alpha3DeltaProperty			= this->registerDependencyProperty(TEXT("Alpha3Delta"), TEXT("alpha3Delta"), DPROP_FLOAT, (BYTE*)&(this->alpha3Delta));
}

DWORD __a3d_fastcall a3dSmokeParticleSystem::getParticleFVF() { return D3DFVF_XYZ | D3DFVF_XYZB1 | D3DFVF_TEX1 | D3DFVF_TEX2; }

int __a3d_fastcall a3dSmokeParticleSystem::getVertexSize() { return sizeof( smokeParticleVertex ); }

void a3dSmokeParticleSystem::updateParticles(DWORD dwTime) { 

	float dt, pt;
	this->fTime = dwTime * 0.001f;

	a3dSmokeParticleInfo *p = (a3dSmokeParticleInfo*) this->particles;
	
	for( this->particleIndex = 0; this->particleIndex < this->particleCount; this->particleIndex ++, p ++ ) { 
		
		if( p->firstTime ) { 
			this->generateCurrentSmokeParticle( p );
			this->particleIndex ++;
			continue;
		}
		
		pt = this->fTime - p->creationTime;
		if( pt < 0.0f ) continue;

		dt = this->fTime - p->currTime;

		// частица вылетает из трубы паровоза
		if( p->currentState == sp_state1 ) { 
			
			// если должны перейти в другое состояние
			if( pt + dt > p->time1 ) dt = p->time1 - pt;
			if( dt < 0.0f ) dt = 0.0f;			

			// текущее положение
			p->currPos[0] += p->currSpeed[0] * dt;
			p->currPos[1] += p->currSpeed[1] * dt;
			p->currPos[2] += p->currSpeed[2] * dt;

			// текущий размер
			p->size += p->dsize1 * dt;

			// считаем расстояние до камеры и видимость
			this->calcParticleVisibilityParams( p );
			
			// текущая прозрачность
			p->alpha += p->dalpha1 * dt;

			// текущее время
			p->currTime += dt;

			if( pt + dt >= p->time1 ) { 
				quicknormalizeVector( p->currSpeed );
				scaleVector3( p->currSpeed, getValueWithDelta( this->smokeSpeed2, this->smokeSpeed2Delta ) );
				p->currentState = sp_state2;
			}
		}
		else if(p->currentState == sp_state2) { 
			if( dt + pt > p->time1 + p->time2 ) dt = p->time1 + p->time2 - pt;
			if( dt < 0.0f ) dt = 0.0f;
		
			// текущее положение
			p->currPos[0] += p->currSpeed[0] * dt;
			p->currPos[1] += p->currSpeed[1] * dt;
			p->currPos[2] += p->currSpeed[2] * dt;

			// текущий размер
			p->size += p->dsize2 * dt;

			// считаем расстояние до камеры и видимость
			this->calcParticleVisibilityParams( p );

			// текущая прозрачность
			p->alpha += p->dalpha2 * dt;

			// текущее время
			p->currTime += dt;

			if( dt + pt >= p->time1 + p->time2 ) { 
				p->busy = false;
				if( !this->doNotGenerate )
					this->generateCurrentSmokeParticle( p );

			}
		}
	}

	this->sortParticlesByDistance();

	return;
}

void a3dSmokeParticleSystem::generateCurrentSmokeParticle( a3dSmokeParticleInfo *p ) { 

	if( this->useLocalBasis ) {
		setVector3( p->beginPos, 0.0f, 0.0f, 0.0f );
		setVector3( p->currPos, 0.0f, 0.0f, 0.0f );
	}
	else { 
		setVector3( p->beginPos, this->localBasis[3][0], this->localBasis[3][1], this->localBasis[3][2] );
		copyVector3(p->beginPos, p->currPos );
	}

	// геренируем скорость текстуры согласно направлению
	Vector3 delta, res;
	setVector3( delta, getValueWithDelta( 0.0f, this->dirDelta ), 0.0f, getValueWithDelta( 0.0f, this->dirDelta ) );
	
	res[0] = delta[0];
	res[1] = 1.0f;
	res[2] = delta[2];

	quicknormalizeVector( res );
	
	if( !this->useLocalBasis ) { 
		translateVector( res, this->localBasis, res );
	}

	scaleVector3( res, getValueWithDelta( this->smokeSpeed1, this->smokeSpeed1Delta ), p->currSpeed );
	copyVector3( p->currSpeed, p->beginSpeed );

	p->time1 = getValueWithDelta( this->smokeTime1, this->smokeTime1Delta );
	p->time2 = getValueWithDelta( this->smokeTime2, this->smokeTime2Delta );
	p->size1 = getValueWithDelta( this->smokeSize1, this->smokeSize1Delta );
	p->size2 = getValueWithDelta( this->smokeSize2, this->smokeSize2Delta );
	p->size3 = getValueWithDelta( this->smokeSize3, this->smokeSize3Delta );
	p->alpha1 = getValueWithDelta( this->alpha1, this->alpha1Delta );
	p->alpha2 = getValueWithDelta( this->alpha2, this->alpha2Delta );
	p->alpha3 = getValueWithDelta( this->alpha3, this->alpha3Delta );
	p->size = p->size1;

	p->dsize1 = ( p->size2 - p->size1 ) / p->time1;
	p->dsize2 = ( p->size3 - p->size2 ) / p->time2;

	p->alpha = p->alpha1;
	p->dalpha1 = ( p->alpha2 - p->alpha1 ) / p->time1;
	p->dalpha2 = ( p->alpha3 - p->alpha2 ) / p->time2;
	
	p->currentState = sp_state1;
	p->busy = true;
	float tm = this->smokeTime1 + this->smokeTime1Delta + this->smokeTime2 + this->smokeTime2Delta;
	if( p->firstTime )
		p->creationTime = fTime + tm / this->particleCount * this->particleIndex;
	else
		p->creationTime = fTime;
	
	p->currTime = p->creationTime;
	
	p->firstTime = false;
	p->visible = false; // по умолчанию частица не видима

	return ;
}

void a3dSmokeParticleSystem::sortParticlesByDistance() { 
	
	int			pi;
	this->renderParticleCount = 0;
	a3dSmokeParticleInfo *p = (a3dSmokeParticleInfo*) this->particles;
	
	// фильтруем частицы
	for( pi = 0; pi < this->particleCount; pi ++, p ++ ) { 
		
		// если частицу не надо обрабатывать
		if( !p->busy || p->creationTime > this->fTime || !p->visible ) continue;
		this->partIndices[ this->renderParticleCount ] = pi;

		this->renderParticleCount++;
	}

	// не будем сортировать если нечего
	if( this->renderParticleCount == 0 ) return;
	
	// сортируем оставшиеся частицы
	this->quickSortR( this->partIndices, this->renderParticleCount - 1 );
}

void a3dSmokeParticleSystem::quickSortR( int *a, int N ) { 
	// На входе - массив a[], a[N] - его последний элемент.

	int		i = 0, j = N; 		// поставить указатели на исходные места
	int		temp, p;

	a3dSmokeParticleInfo *pInfo = (a3dSmokeParticleInfo*) this->particles;
	
	p = a[ N >> 1 ];		// центральный элемент

	// процедура разделения
	do {
		while ( pInfo[a[i]].distFromCamera > pInfo[p].distFromCamera ) i++;
		while ( pInfo[a[j]].distFromCamera < pInfo[p].distFromCamera ) j--;

		if ( i <= j ) {
			temp = a[i]; a[i] = a[j]; a[j] = temp;
			i++; j--;
		}
	} while ( i <= j );


	// рекурсивные вызовы, если есть, что сортировать 
	if ( j > 0 ) quickSortR( a, j );
	if ( N > i ) quickSortR( a + i, N - i );
}

void a3dSmokeParticleSystem::fillBuffer(BYTE *ptr) { 

	int			pi;
	float		size;

	a3dSmokeParticleInfo *part = (a3dSmokeParticleInfo*) this->particles, *p;
	smokeParticleVertex *pVert = (smokeParticleVertex*) ptr;

	// Начинаем с конца так как там большая отдаленность от камеры
	for( pi = 0; pi < this->renderParticleCount; pi ++ ) { 
		
		// если частицу не надо обрабатывать
		// updated это уже было решено в функции updateParticles
		p = part + this->partIndices[ pi ];

		size = p->size * 0.5f;
		
		copyVector3( p->currPos, pVert->pos );
		pVert->shiftVector[0] = -size; 
		pVert->shiftVector[1] = size;
		pVert->alpha = p->alpha;
		pVert->tex0[0] = 0.0f;
		pVert->tex0[1] = 0.0f;

		pVert ++;

		copyVector3( p->currPos, pVert->pos );
		pVert->shiftVector[0] = size; 
		pVert->shiftVector[1] = size;
		pVert->alpha = p->alpha;
		pVert->tex0[0] = 1.0f;
		pVert->tex0[1] = 0.0f;				

		pVert ++;

		copyVector3( p->currPos, pVert->pos );
		pVert->shiftVector[0] = -size; 
		pVert->shiftVector[1] = -size;
		pVert->alpha = p->alpha;
		pVert->tex0[0] = 0.0f;
		pVert->tex0[1] = 1.0f;				

		pVert ++;

		copyVector3( p->currPos, pVert->pos );
		pVert->shiftVector[0] = size; 
		pVert->shiftVector[1] = -size;
		pVert->alpha = p->alpha;
		pVert->tex0[0] = 1.0f;
		pVert->tex0[1] = 1.0f;				

		pVert ++;
	}
}

void a3dSmokeParticleSystem::calcParticleVisibilityParams( a3dSmokeParticleInfo *p ) { 
	
	// для случая когда они все находятся в базисе мы пока не пишем
	if( this->useLocalBasis ) { 
		this->partKernel->MessageBox( TEXT("TODO for this->useLocalBasis"), TEXT("a3dSmokeParticleSystem::calcParticleDistance"), MB_ICONERROR );
		return ;
	}
	
	Vector3 dist, camZ = { this->currCamera->getCameraZAxis(0), this->currCamera->getCameraZAxis(1), this->currCamera->getCameraZAxis(2) };
	Vector4 sphere = { p->currPos[0], p->currPos[1], p->currPos[2], p->size * 1.4142136f };
	
	// смотрим видима или невидима частица
	p->visible = this->currCamera->isSphereInFrustum( sphere ) != OUT_FRUSTUM;
	if( p->visible ) {
		subVector3( p->currPos, this->currCamera->pos, dist );	// радиус вектор в базисе камеры
		p->distFromCamera = scalarMulOfVector( dist, camZ );	// скалярное умножение - на единичный вектор и есть проекция
	}

	return ;
}

bool a3dSmokeParticleSystem::checkParams() { 
	
	if( this->dirDelta == FLT_MAX ) return false;
	if( this->smokeSize1 == FLT_MAX ) return false;
	if( this->smokeSize1Delta == FLT_MAX ) return false;
	if( this->smokeSize2 == FLT_MAX ) return false;
	if( this->smokeSize2Delta == FLT_MAX ) return false;
	if( this->smokeSize3 == FLT_MAX ) return false;
	if( this->smokeSize3Delta == FLT_MAX ) return false;
	if( this->smokeSpeed1 == FLT_MAX ) return false;
	if( this->smokeSpeed1Delta == FLT_MAX ) return false;
	if( this->smokeSpeed2 == FLT_MAX ) return false;
	if( this->smokeSpeed2Delta == FLT_MAX ) return false;
	if( this->smokeTime1 == FLT_MAX ) return false;
	if( this->smokeTime1Delta == FLT_MAX ) return false;
	if( this->smokeTime2 == FLT_MAX ) return false;
	if( this->smokeTime2Delta == FLT_MAX ) return false;

	return true;
}

int a3dSmokeParticleSystem::calcParticleSizeCore() { 

	int size = particleSystemBase::calcParticleSizeCore();
	
	size += sizeof( this->dirDelta );
	size += sizeof( this->smokeSize1 );
	size += sizeof( this->smokeSize1Delta );
	size += sizeof( this->smokeSize2 );
	size += sizeof( this->smokeSize2Delta );
	size += sizeof( this->smokeSize3 );
	size += sizeof( this->smokeSize3Delta );
	size += sizeof( this->smokeSpeed1 );
	size += sizeof( this->smokeSpeed1Delta );
	size += sizeof( this->smokeSpeed2 );
	size += sizeof( this->smokeSpeed2Delta );
	size += sizeof( this->smokeTime1 );
	size += sizeof( this->smokeTime1Delta );
	size += sizeof( this->smokeTime2 );
	size += sizeof( this->smokeTime2Delta );
	size += sizeof( this->alpha1 );
	size += sizeof( this->alpha1Delta );
	size += sizeof( this->alpha2 );
	size += sizeof( this->alpha2Delta );

	return size;
}

bool a3dSmokeParticleSystem::saveParticleDataToMemoryInternal(a3dMemoryFile &file) {  

	if( !particleSystemBase::saveParticleDataToMemoryInternal( file ) ) return false;
	if( !file.writeFloat(this->dirDelta) ) return false;
	if( !file.writeFloat(this->smokeSize1) ) return false;
	if( !file.writeFloat(this->smokeSize1Delta) ) return false;
	if( !file.writeFloat(this->smokeSize2) ) return false;
	if( !file.writeFloat(this->smokeSize2Delta) ) return false;
	if( !file.writeFloat(this->smokeSize3) ) return false;
	if( !file.writeFloat(this->smokeSize3Delta) ) return false;
	if( !file.writeFloat(this->smokeSpeed1) ) return false;
	if( !file.writeFloat(this->smokeSpeed1Delta) ) return false;
	if( !file.writeFloat(this->smokeSpeed2) ) return false;
	if( !file.writeFloat(this->smokeSpeed2Delta) ) return false;
	if( !file.writeFloat(this->smokeTime1) ) return false;
	if( !file.writeFloat(this->smokeTime1Delta) ) return false;
	if( !file.writeFloat(this->smokeTime2) ) return false;
	if( !file.writeFloat(this->smokeTime2Delta) ) return false;
	if( !file.writeFloat(this->alpha1) ) return false;
	if( !file.writeFloat(this->alpha1Delta) ) return false;
	if( !file.writeFloat(this->alpha2) ) return false;
	if( !file.writeFloat(this->alpha2Delta) ) return false;

	return true;
}

bool a3dSmokeParticleSystem::loadParticleDataFromMemoryInternal(a3dMemoryFile &file) { 
	
	if( !particleSystemBase::loadParticleDataFromMemoryInternal( file ) ) return false;
	if( !file.readFloat(&this->dirDelta) ) return false;
	if( !file.readFloat(&this->smokeSize1) ) return false;
	if( !file.readFloat(&this->smokeSize1Delta) ) return false;
	if( !file.readFloat(&this->smokeSize2) ) return false;
	if( !file.readFloat(&this->smokeSize2Delta) ) return false;
	if( !file.readFloat(&this->smokeSize3) ) return false;
	if( !file.readFloat(&this->smokeSize3Delta) ) return false;
	if( !file.readFloat(&this->smokeSpeed1) ) return false;
	if( !file.readFloat(&this->smokeSpeed1Delta) ) return false;
	if( !file.readFloat(&this->smokeSpeed2) ) return false;
	if( !file.readFloat(&this->smokeSpeed2Delta) ) return false;
	if( !file.readFloat(&this->smokeTime1) ) return false;
	if( !file.readFloat(&this->smokeTime1Delta) ) return false;
	if( !file.readFloat(&this->smokeTime2) ) return false;
	if( !file.readFloat(&this->smokeTime2Delta) ) return false;
	if( !file.readFloat(&this->alpha1) ) return false;
	if( !file.readFloat(&this->alpha1Delta) ) return false;
	if( !file.readFloat(&this->alpha2) ) return false;
	if( !file.readFloat(&this->alpha2Delta) ) return false;

	return true;
}

HRESULT __a3d_fastcall a3dTreeParticleSystem::initialize( shString &fileName ) { 

	FILE *fp;

	_tfopen_s( &fp, fileName.getCString(), TEXT("rt") );
	if( fp == null ) return D3DERR_INVALIDCALL;

	char	buf[256];
	TCHAR	tbuf[256];

	// читаем TreeColor
	fscanf_s( fp, "%s", buf );
	// читаем знак =
	fscanf_s( fp, "%s", buf );
	// читаем значение TreeColor
	fscanf_s( fp, "%s", buf );
	long l = atol(buf);
	this->treeColor.r = (BYTE)((l >> 16) & 0xff);
	this->treeColor.g = (BYTE)((l >> 8) & 0xff);
	this->treeColor.b = (BYTE)(l & 0xff);

	// читаем TreeName
	fscanf_s( fp, "%s", buf );
	// читаем знак =
	fscanf_s( fp, "%s", buf );
	// читаем значение TreeName
	fscanf_s( fp, "%s", buf );
	strToWCS( buf, tbuf );
	this->treeName.initialize( tbuf );
	
	// читаем MapWidth
	fscanf_s( fp, "%s", buf );
	// читаем знак =
	fscanf_s( fp, "%s", buf );
	// читаем значение MapWidth
	fscanf_s( fp, "%d", &this->mapWidth );

	// читаем MapHeight
	fscanf_s( fp, "%s", buf );
	// читаем знак =
	fscanf_s( fp, "%s", buf );
	// читаем значение MapHeight
	fscanf_s( fp, "%d", &this->mapHeight );
	
	// читаем ParticleCount
	fscanf_s( fp, "%s", buf );
	// читаем знак =
	fscanf_s( fp, "%s", buf );
	// читаем значение ParticleCount
	fscanf_s( fp, "%d", &this->particleCount );
	 
	this->particles = this->createParticles( this->particleCount );
	a3dTreeParticleInfo *p = (a3dTreeParticleInfo*) this->particles;

	int pi, x, y;
	for( pi = 0; pi < this->particleCount; pi++, p++ ) { 
		// читаем pi
		fscanf_s( fp, "%s", buf );
		// читаем знак =
		fscanf_s( fp, "%s", buf );
		// читаем х координату
		fscanf_s( fp, "%d", &x );
		
		if( this->land != null)
			p->pos[0] = ((float)x) * this->land->sideScale / this->land->mapSize + this->land->basis[3][0];
		else
			p->pos[0] = ((float)x) * this->mapWidth / this->mapWidth + this->offsetX;


		// читаем знак ,
		fscanf_s( fp, "%s", buf );
		// читаем y координату
		fscanf_s( fp, "%d", &y );
		
		if( this->land != null)
			p->pos[2] = (this->land->mapSize - ((float)y) ) * this->land->sideScale / this->land->mapSize - this->land->basis[3][2];
		else 
			p->pos[2] = (this->mapHeight - ((float)y) ) * this->mapHeight / this->mapHeight + this->offsetZ;

		// по умолчанию все деревья находятся на уровне моря
		// если что сдвигаем по высоте ландшафта
		if( this->land != null )
			this->land->updateObjectYByLand( p->pos );		
		else
			p->pos[1] = 0.0f;
	
		// размер
		p->size = generateValueFromRange( this->minSize, this->maxSize );
		p->pos[1] += p->size * 0.5f;
	
		// определяем какому патчу принадлежит частица
		p->ownerPatch = this->land->getPatch( p->pos[0], p->pos[2] );
	}
	
	// обрезаем максимальное число рендеренных частиц
	if( this->maxRenderParticleCount > this->particleCount )
		this->maxRenderParticleCount = this->particleCount;

	this->partIndices = new int[ this->particleCount ];

	// по умолчанию все видны
	this->renderParticleCount = this->particleCount;
	for( pi = 0; pi < this->particleCount; pi ++ ) { 
		this->partIndices[ pi ] = pi;
	}

	// создаем буфферы
	// для этого патчим particleCount чтобы 
	HRESULT rv = this->createBuffers( this->maxRenderParticleCount );
	if( FAILED( rv ) ) return rv;

	return D3D_OK;
}

void a3dTreeParticleSystem::updateParticles( DWORD dwTime ) { 
	
	this->renderParticleCount = 0;
	a3dTreeParticleInfo *p = (a3dTreeParticleInfo*)this->particles;

	// обновляем список частиц по видимости
	int pi;
	for( pi = 0; pi < this->particleCount; pi ++, p++ ) { 
		// считаем расстояние до камеры и видимость
		this->calcParticleVisibilityParams( p );
		
		if( !p->visible ) continue;
		
		this->partIndices[ this->renderParticleCount ] = pi;
		this->renderParticleCount ++;
	}

	// сортируем по удаленности
	this->sortParticlesByDistance();

	return;
}

int a3dTreeParticleSystem::fillBuffer(int beginIndex, BYTE *ptr) { 

	int			pi;
	float		sz;

	a3dTreeParticleInfo *part = (a3dTreeParticleInfo*) this->particles, *p;
	treeParticleVertex *pVert = (treeParticleVertex*) ptr;

	int endIndex = beginIndex + this->maxRenderParticleCount;
	if(endIndex > this->renderParticleCount) endIndex = this->renderParticleCount;
	
	for( pi = beginIndex; pi < endIndex; pi ++ ) { 
		
		// если частицу не надо обрабатывать
		// updated это уже было решено в функции updateParticles
		p = part + this->partIndices[ pi ];

		sz = p->size * 0.5f;

		copyVector3( p->pos, pVert->pos );
		pVert->shiftVector[0] = -sz; 
		pVert->shiftVector[1] = sz;
		pVert->tex0[0] = 0.0f;
		pVert->tex0[1] = 0.0f;

		pVert ++;

		copyVector3( p->pos, pVert->pos );
		pVert->shiftVector[0] = sz; 
		pVert->shiftVector[1] = sz;
		pVert->tex0[0] = 1.0f;
		pVert->tex0[1] = 0.0f;				

		pVert ++;

		copyVector3( p->pos, pVert->pos );
		pVert->shiftVector[0] = -sz; 
		pVert->shiftVector[1] = -sz;
		pVert->tex0[0] = 0.0f;
		pVert->tex0[1] = 1.0f;				

		pVert ++;

		copyVector3( p->pos, pVert->pos );
		pVert->shiftVector[0] = sz; 
		pVert->shiftVector[1] = -sz;
		pVert->tex0[0] = 1.0f;
		pVert->tex0[1] = 1.0f;				

		pVert ++;
	}

	return endIndex - beginIndex;
}

HRESULT __a3d_fastcall a3dTreeParticleSystem::updateBuffer() { return D3D_OK; }

HRESULT __a3d_fastcall a3dTreeParticleSystem::renderCore( ) { 

	HRESULT rv;	
	UINT uPasses, iPass;
	int beginIndex = 0, pCount;

	partKernel->lpDevice->SetStreamSource( 0, lpVertexBuffer, 0, this->getVertexSize() );
	partKernel->lpDevice->SetFVF( this->getParticleFVF() );

	rv = partKernel->lpDevice->SetIndices( this->lpIndexBuffer );
	if(FAILED(rv))
		return rv;
	
	treeParticleVertex *ptr;
		
	this->effectDesc->lpEffect->Begin( &uPasses, 0 );
	
	while( 1 ) { 
		
		rv = this->lpVertexBuffer->Lock( 0, 0, (void**)&ptr, D3DLOCK_DISCARD ); 
		if(FAILED(rv))
			return rv;

		pCount = this->fillBuffer( beginIndex, (BYTE*)ptr );	
		
		rv = this->lpVertexBuffer->Unlock();
		if( FAILED( rv ) )
			return rv;

		for( iPass = 0; iPass < uPasses; iPass++ )
		{
			
#if D3D_SDK_VERSION >= 32
		rv = this->effectDesc->lpEffect->BeginPass( iPass );
#else	
		rv = this->effectDesc->lpEffect->Pass( iPass );
#endif	
			if(FAILED(rv))
				return rv;
					
			rv = partKernel->lpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pCount << 2, 0, pCount << 1 );
			if(FAILED(rv))
				return rv;	
			
#if D3D_SDK_VERSION >= 32
		rv = this->effectDesc->lpEffect->EndPass();
			if(FAILED(rv))
				return rv;
#endif			
		}
		
		// если отрендерили последние частички
		if( pCount < this->maxRenderParticleCount ) break;
		beginIndex += pCount;
	}
	
	return this->effectDesc->lpEffect->End();
} 

void a3dTreeParticleSystem::quickSortR( int *a, int N ) { 
	// На входе - массив a[], a[N] - его последний элемент.

	int		i = 0, j = N; 		// поставить указатели на исходные места
	int		temp, p;

	a3dTreeParticleInfo *pInfo = (a3dTreeParticleInfo*) this->particles;
	
	p = a[ N >> 1 ];		// центральный элемент

	// процедура разделения
	do {
		while ( pInfo[a[i]].distFromCamera > pInfo[p].distFromCamera ) i++;
		while ( pInfo[a[j]].distFromCamera < pInfo[p].distFromCamera ) j--;

		if ( i <= j ) {
			temp = a[i]; a[i] = a[j]; a[j] = temp;
			i++; j--;
		}
	} while ( i <= j );


	// рекурсивные вызовы, если есть, что сортировать 
	if ( j > 0 ) quickSortR( a, j );
	if ( N > i ) quickSortR( a + i, N - i );
}

void a3dTreeParticleSystem::sortParticlesByDistance() { 
	
	// не будем сортировать если нечего
	if( this->renderParticleCount == 0 ) return;
	
	// сортируем оставшиеся частицы
	this->quickSortR( this->partIndices, this->renderParticleCount - 1 );
}

void a3dTreeParticleSystem::calcParticleVisibilityParams( a3dTreeParticleInfo *p ) { 
	
	// для случая когда они все находятся в базисе мы пока не пишем
	if( this->useLocalBasis ) { 
		this->partKernel->MessageBox( TEXT("TODO for this->useLocalBasis"), TEXT("a3dSmokeParticleSystem::calcParticleDistance"), MB_ICONERROR );
		return ;
	}
	
	if( !p->ownerPatch->visible ) {
		p->visible = false;
		return;
	}

	Vector3 dist, camZ = { this->currCamera->getCameraZAxis(0), this->currCamera->getCameraZAxis(1), this->currCamera->getCameraZAxis(2) };
	Vector4 sphere = { p->pos[0], p->pos[1], p->pos[2], p->size * 1.4142136f };
	
	// смотрим видима или невидима частица
	p->visible = this->currCamera->isSphereInFrustum( sphere ) != OUT_FRUSTUM;
	if( p->visible ) {
		subVector3( p->pos, this->currCamera->pos, dist );	// радиус вектор в базисе камеры
		p->distFromCamera = scalarMulOfVector( dist, camZ );	// скалярное умножение - на единичный вектор и есть проекция
	}

	return ;
}

DWORD __a3d_fastcall a3dTreeParticleSystem::getParticleFVF() { return D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEX2; }
int __a3d_fastcall a3dTreeParticleSystem::getVertexSize() { return sizeof( treeParticleVertex ); }