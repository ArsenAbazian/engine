#include "particleSystemBase.h"
#include "..\\utils.h"
#include "..\\rand.h"
#include "..\\texture.h"
#include "..\\a3dDesc\\a3dEffectDesc.h"
#include "..\\a3dDesc\\a3dTextureDesc.h"

BYTE	checkByte13;

WORD	checkWord16;
#define CHECK_WORD16

WORD	checkWord16_1;
#define CHECK_WORD16_1

BYTE	checkByte1_16;

DWORD __a3d_fastcall particleSystemBase::getParticleFVF() { return D3DFVF_XYZ | D3DFVF_XYZB1 | D3DFVF_TEX1 | D3DFVF_TEX2; }
int __a3d_fastcall particleSystemBase::getVertexSize() { return sizeof( particleVertexBase ); }

HRESULT __a3d_fastcall particleSystemBase::openVertexBuffer( void **ptr ) { 
	return lpVertexBuffer->Lock( 0, 0, ptr, D3DLOCK_DISCARD ); 
}

HRESULT __a3d_fastcall particleSystemBase::closeVertexBuffer() { 
	return this->lpVertexBuffer->Unlock();
}

HRESULT	__a3d_fastcall particleSystemBase::openIndexBuffer( WORD **ptr ) { 
	return this->lpIndexBuffer->Lock( 0, 0, (void**)ptr, D3DLOCK_DISCARD );
}

HRESULT	__a3d_fastcall particleSystemBase::closeIndexBuffer() { 
	return this->lpIndexBuffer->Unlock();
}

void particleSystemBase::initializeDefault() { 
	
	this->partKernel = null;
	this->lpVertexBuffer = null;
	this->lpIndexBuffer = null;
	this->effectDesc = null;
	this->textureDesc = null;
	this->particleCount = 0;
	this->renderParticleCount = 0;
	this->particles = null;
	this->useLocalBasis = true;
	this->doNotGenerate = false;
	this->fTime = 0.0f;
	this->maxRenderParticleCount = 0x7fffffff; // по умолчанию очень большое число
	originMatrix4( this->localBasis );
	originMatrix4( this->invertBasis );
	originMatrix4( this->originBasis );
}

void particleSystemBase::registerDependencyProperties() { 
	
	this->doNotGenerateProperty = this->registerDependencyProperty( TEXT("DoNotGenerate"), TEXT("doNotGenerate"), DPROP_BOOL, (BYTE*)&(this->doNotGenerate) );
 	this->useLocalBasisProperty = this->registerDependencyProperty( TEXT("UseLocalBasis"), TEXT("useLocalBasis"), DPROP_BOOL, (BYTE*)&(this->useLocalBasis) );
	this->particleCountProperty = this->registerDependencyProperty( TEXT("ParticleCount"), TEXT("particleCount"), DPROP_INT, (BYTE*)&(this->particleCount) );
	this->maxRenderParticleCountProperty = this->registerDependencyProperty( TEXT("MaxRenderParticleCount"), TEXT("maxRenderParticleCount"), DPROP_INT, (BYTE*)&(this->maxRenderParticleCount) );
	this->timeProperty = this->registerDependencyProperty( TEXT("Time"), TEXT("fTime"), DPROP_FLOAT, (BYTE*)&(this->fTime) );
	this->positionXProperty = this->registerDependencyProperty( TEXT("PositionX"), TEXT("localBasis[3][0]"), DPROP_FLOAT, (BYTE*)&(this->localBasis[3][0]) );
	this->positionYProperty = this->registerDependencyProperty( TEXT("PositionY"), TEXT("localBasis[3][1]"), DPROP_FLOAT, (BYTE*)&(this->localBasis[3][1]) );
	this->positionZProperty = this->registerDependencyProperty( TEXT("PositionZ"), TEXT("localBasis[3][2]"), DPROP_FLOAT, (BYTE*)&(this->localBasis[3][2]) );

	return;
}

HRESULT	__a3d_fastcall particleSystemBase::createDynamicBuffers( int pCount ) { 
	
	HRESULT rv;
	
	rv = partKernel->lpDevice->CreateVertexBuffer(	this->getVertexSize() * pCount * 8 * 4, 
													D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 
													this->getParticleFVF(),
													D3DPOOL_DEFAULT, 
													&lpVertexBuffer, 
													NULL );

	if( FAILED( rv ) ) return rv;
    
	rv = partKernel->lpDevice->CreateIndexBuffer( sizeof( WORD ) * pCount * 8 * 6, 
													D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 
													D3DFMT_INDEX16, 
													D3DPOOL_DEFAULT, 
													&lpIndexBuffer, 
													NULL );
	
	if( FAILED( rv ) ) return rv;

	return D3D_OK;
}

HRESULT	__a3d_fastcall particleSystemBase::createBuffers( int pCount ) { 
	
	HRESULT rv;
	
	rv = partKernel->lpDevice->CreateVertexBuffer(	this->getVertexSize() * this->particleCount * 8 * 4, 
													D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 
													this->getParticleFVF(),
													D3DPOOL_DEFAULT, 
													&lpVertexBuffer, 
													NULL );

	if( FAILED( rv ) ) return rv;
    
	rv = partKernel->lpDevice->CreateIndexBuffer( sizeof( WORD ) * this->particleCount * 8 * 6, 
													0, 
													D3DFMT_INDEX16, 
													D3DPOOL_DEFAULT, 
													&lpIndexBuffer, 
													NULL );
	
	if( FAILED( rv ) ) { 
		this->partKernel->DxErrorBox( TEXT( "particleSystemBase::createBuffers" ), TEXT("partKernel->lpDevice->CreateIndexBuffer"), rv );
		return rv;
	}

	WORD *ind;
	rv = this->openIndexBuffer( &ind );
	if( FAILED ( rv ) ) return rv;

	int i = 0, j = 0, l = 0;
	for( ; i < this->particleCount * 8; i ++, j += 6, l += 4 ) { 
		ind[ j ] = l;
		ind[ j + 1 ] = l + 1;
		ind[ j + 2 ] = l + 2;
		ind[ j + 3 ] = l + 1;
		ind[ j + 4 ] = l + 3;
		ind[ j + 5 ] = l + 2;
	}

	return this->closeIndexBuffer();
}

HRESULT __a3d_fastcall particleSystemBase::initialize() { 
	
	// считается что на данный момент число партиклов уже сгенерировано
	// выделяем память под буффер в 8 раз больше чтобы создать эффект размытости
	
	// создаем частицы
	if( this->particles == null )
		this->particles = this->createParticles( this->particleCount );
	
	this->partIndices = new int[ this->particleCount ];
	
	return this->createBuffers( this->particleCount );
}

HRESULT __a3d_fastcall particleSystemBase::clear()
{

	// если есть буффер вершин
	if( this->lpVertexBuffer != null )
	{
		this->lpVertexBuffer->Release();
		this->lpVertexBuffer = null;
	}
	
	// если есть буффер индексов
	if( this->lpIndexBuffer != null ) 
	{
		this->lpIndexBuffer->Release();
		this->lpIndexBuffer = null;
	}
	
	// если есть буффер частиц
	if( this->particles != null )
	{
		delete[] particles;
		this->particles = null;
	}

	if( this->partIndices != null )
		delete[] this->partIndices;
	this->partIndices = null;

	return D3D_OK;
}

void particleSystemBase::correct() {

	Matrix3	bas,fbas;	
	DWORD i,j;
	
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			bas[i][j] = localBasis[i][j];

	invertMatrix3(bas,fbas);

	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			invertBasis[i][j] = fbas[i][j];

	invertBasis[3][0] = localBasis[3][0];
	invertBasis[3][1] = localBasis[3][1];
	invertBasis[3][2] = localBasis[3][2];

	return ;
}

void particleSystemBase::updateParticles( DWORD dwTime ) { }

HRESULT __a3d_fastcall particleSystemBase::updateBuffer() { 
	
	HRESULT	rv;
	BYTE *ptr;
	
	rv = this->openVertexBuffer( (void**)&ptr );
	if( FAILED( rv ) )
		return rv;

	this->fillBuffer( ptr );
	
	rv = this->closeVertexBuffer();
	if( FAILED( rv ) )
		return rv;

	return D3D_OK;
} 

void particleSystemBase::fillBuffer( BYTE *ptr ) { 
	
	this->renderParticleCount = 0;
}

HRESULT __a3d_fastcall particleSystemBase::prepareEffect( ) { 
	
	static Matrix4 mat, mat2;
	Vector3 pos;
	HRESULT rv;

	if( this->effectDesc->lpEffect == null || this->currCamera == null ) return D3D_OK;

	if( this->useLocalBasis )
		mulMatrix4( this->invertBasis, this->currCamera->viewProj, mat );
	else 
		mulMatrix4( this->originBasis, this->currCamera->viewProj, mat );
	
	rv = this->effectDesc->lpEffect->SetMatrix( "WvpXf", (D3DXMATRIX*)mat );
	if(FAILED(rv)) return rv;
		
	setVector3( pos, this->currCamera->getCameraXAxis(0), this->currCamera->getCameraXAxis(1), this->currCamera->getCameraXAxis(2) );
	rv = this->effectDesc->lpEffect->SetValue( "CameraX", pos, 12 );
	if(FAILED(rv)) { 
		this->partKernel->DxErrorBox( TEXT( "particleSystem" ), TEXT( "lpEffect->SetValue( \"CameraX\", pos, 12 )" ), rv );
		return rv;
	}

	setVector3( pos, this->currCamera->getCameraYAxis(0), this->currCamera->getCameraYAxis(1), this->currCamera->getCameraYAxis(2) );
	rv = this->effectDesc->lpEffect->SetValue( "CameraY", pos, 12 );
	if(FAILED(rv)) { 
		this->partKernel->DxErrorBox( TEXT( "particleSystem" ), TEXT( "lpEffect->SetValue( \"CameraY\", pos, 12 )" ), rv );
		return rv;
	}

	rv = this->effectDesc->lpEffect->SetTexture( "ColorTexture", this->textureDesc->texture->lpTexture );
	if(FAILED(rv)) return rv;

	return D3D_OK;
}

HRESULT __a3d_fastcall particleSystemBase::renderCore( ) { 

	HRESULT rv;	
	
	partKernel->lpDevice->SetStreamSource( 0, lpVertexBuffer, 0, this->getVertexSize() );
	partKernel->lpDevice->SetFVF( this->getParticleFVF() );

	rv = partKernel->lpDevice->SetIndices( this->lpIndexBuffer );
	if(FAILED(rv))
		return rv;
	
	UINT uPasses, iPass;
	this->effectDesc->lpEffect->Begin( &uPasses, 0 );
	for( iPass = 0; iPass < uPasses; iPass++ )
	{
#if D3D_SDK_VERSION >= 32
		rv = this->effectDesc->lpEffect->BeginPass( iPass );
#else	
		rv = this->effectDesc->lpEffect->Pass( iPass );
#endif	
		if(FAILED(rv))
			return rv;
				
		rv = partKernel->lpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, this->renderParticleCount << 2, 0, this->renderParticleCount << 1 );
		if(FAILED(rv))
			return rv;	
		
#if D3D_SDK_VERSION >= 32
		rv = this->effectDesc->lpEffect->EndPass();
		if(FAILED(rv))
			return rv;
#endif
	}
	return this->effectDesc->lpEffect->End();
} 

HRESULT __a3d_fastcall particleSystemBase::render( ) { 

	HRESULT rv;	
	
	if( this->renderParticleCount == 0 ) return D3D_OK;
	
	rv = this->prepareEffect( );
	if( FAILED ( rv ) )
		return rv;

	return this->renderCore();
}

int particleSystemBase::calcParticleSizeCore() { 

	int size	 = this->name.calcSize();
	size		+= this->effectFileName.calcSize();
	size		+= this->textureFileName.calcSizeCore();
	size		+= sizeof( bool );
	size		+= sizeof( bool );
	size		+= sizeof( Matrix4 );
	size		+= sizeof( int );
	size		+= sizeof( int );

	return size;
}

int particleSystemBase::calcSizeCore() { 

	int size = this->calcParticleSizeCore();
	
	// dependency property
	size		+= a3dDependencyPropertyOwner::calcSizeCore();

	return size;
}

bool particleSystemBase::saveParticleDataToMemoryInternal( a3dMemoryFile &file ) {  
	
	if( !this->name.saveToMemory(file) ) return false;
	if( !this->effectFileName.saveToMemory(file) ) return false;
	if( !this->textureFileName.saveToMemory(file) ) return false;
	if( !file.writeBool( this->doNotGenerate ) ) return false;
	if( !file.writeBool( this->useLocalBasis ) ) return false;
	if( !file.writeMatrix4( this->localBasis) ) return false;
	if( !file.writeInt( this->particleCount ) ) return false;
	if( !file.writeInt( this->maxRenderParticleCount ) ) return false;
	
	return true;
}

bool particleSystemBase::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !this->saveParticleDataToMemoryInternal( file ) ) return false;
	if( !a3dDependencyPropertyOwner::saveToMemoryCore( file ) ) return false;

	return true;
}

bool particleSystemBase::loadParticleDataFromMemoryInternal(a3dMemoryFile &file) { 

	if( !this->name.loadFromMemory(file) ) return false;
	if( !this->effectFileName.loadFromMemory(file) ) return false;
	if( !this->textureFileName.loadFromMemory(file) ) return false;
	if( !file.readBool( &this->doNotGenerate ) ) return false;
	if( !file.readBool( &this->useLocalBasis ) ) return false;
	if( !file.readMatrix4( this->localBasis) ) return false;
	if( !file.readInt( &this->particleCount ) ) return false;
	if( !file.readInt( &this->maxRenderParticleCount ) ) return false;
	
	return true;
}

bool particleSystemBase::loadFromMemoryCore(a3dMemoryFile &file) { 
	
	if( !this->loadParticleDataFromMemoryInternal( file ) ) return false;
	if( !a3dDependencyPropertyOwner::loadFromMemoryCore( file ) ) return false;	

	return true;
}

/*
const DWORD particleVertex::FVF = ( D3DFVF_XYZ | D3DFVF_XYZB1 | D3DFVF_TEX1 | D3DFVF_TEX2 );

HRESULT particleSystem::initialize( DWORD pCount )
{

	HRESULT rv;

	// сначала выделяем память под частицы
	partCount	= pCount;
	particles	= new particle[ partCount ];
	// очищаем 
	ZeroMemory( particles, sizeof( particle ) * partCount );
	
	// выделяем память под буффер в 8 раз больше чтобы создать эффект размытости
	rv = partKernel->lpDevice->CreateVertexBuffer(	sizeof( particleVertex ) * partCount * 8 * 4, 
													D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 
													particleVertex::FVF,
													D3DPOOL_DEFAULT, 
													&lpVertexBuffer, 
													NULL );

	if( FAILED( rv ) ) return rv;
    
	rv = partKernel->lpDevice->CreateIndexBuffer( sizeof( WORD ) * this->partCount * 8 * 6, 
													0, 
													D3DFMT_INDEX16, 
													D3DPOOL_DEFAULT, 
													&lpIndexBuffer, 
													NULL );

	if( FAILED( rv ) ) return rv;

	// один раз заполняем буффер
	WORD *ind;
	rv = this->lpIndexBuffer->Lock( 0, 0, (void**)&ind, D3DLOCK_DISCARD );
	if( FAILED( rv ) ) return rv;

	int i, j, l;
	for( i = 0; i < this->partCount * 8; i ++ ) { 
		j = i * 6;
		l = i * 4;
		ind[ j ] = l;
		ind[ j + 1 ] = l + 1;
		ind[ j + 2 ] = l + 2;
		ind[ j + 3 ] = l + 1;
		ind[ j + 4 ] = l + 3;
		ind[ j + 5 ] = l + 2;
	}
	
	rv = this->lpIndexBuffer->Unlock();
	if( FAILED( rv ) ) return rv;
	
	return D3D_OK;
}

HRESULT particleSystem::clear()
{

	// если есть буффер вершин
	if( lpVertexBuffer != NULL )
	{
		lpVertexBuffer->Release();
		lpVertexBuffer = NULL;
	}
	
	// если есть буффер индексов
	if( lpIndexBuffer != NULL ) 
	{
		lpIndexBuffer->Release();
		lpIndexBuffer = NULL;
	}
	
	// если есть буффер частиц
	if( particles != NULL )
	{
		delete[] particles;
		particles = NULL;
	}

	return D3D_OK;
}

HRESULT particleSystem::correct()
{

	Matrix3	bas,fbas;	
	DWORD i,j;
	
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			bas[i][j] = localBasis[i][j];

	invertMatrix3(bas,fbas);

	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			invertBasis[i][j] = fbas[i][j];

	invertBasis[3][0] = localBasis[3][0];
	invertBasis[3][1] = localBasis[3][1];
	invertBasis[3][2] = localBasis[3][2];

	return D3D_OK;
}

bool particleSystem::isBusy()
{

	DWORD partIndex;

	for( partIndex=0; partIndex<partCount; partIndex++ )
	{
	
		if( particles[partIndex].busy == true )
			return true;
	}

	return false;
}

HRESULT particleSystem::render( camera *cam )
{
	
	float	fTime = timeGetTime() * 0.001f;
	float	size;
	float	length;
	int		steps, iStep;
	Vector3 speed;
	Vector3 pos;
	HRESULT rv;
	UINT	uPasses, iPass;

	particleVertex	*pVert;
	
	if( this->lpEffect == null || cam == null ) return D3D_OK;

	static Matrix4 mat, mat2;
	if( this->useLocalBasis )
		mulMatrix4( this->invertBasis, cam->viewProj, mat );
	else 
		mulMatrix4( this->originBasis, cam->viewProj, mat );
	
	rv = lpEffect->SetMatrix( "WvpXf", (D3DXMATRIX*)mat );
	if(FAILED(rv)) return rv;
		
	setVector3( pos, cam->getCameraXAxis(0), cam->getCameraXAxis(1), cam->getCameraXAxis(2) );
	rv = lpEffect->SetValue( "CameraX", pos, 12 );
	if(FAILED(rv)) { 
		this->partKernel->ErrorBox( TEXT( "particleSystem" ), TEXT( "lpEffect->SetValue( \"CameraX\", pos, 12 )" ), rv );
		return rv;
	}

	setVector3( pos, cam->getCameraYAxis(0), cam->getCameraYAxis(1), cam->getCameraYAxis(2) );
	rv = lpEffect->SetValue( "CameraY", pos, 12 );
	if(FAILED(rv)) { 
		this->partKernel->ErrorBox( TEXT( "particleSystem" ), TEXT( "lpEffect->SetValue( \"CameraY\", pos, 12 )" ), rv );
		return rv;
	}

	rv = lpEffect->SetTexture( "ColorTexture", this->texture->lpTexture );
	if(FAILED(rv)) return rv;
	
	partKernel->lpDevice->SetStreamSource( 0, lpVertexBuffer, 0, sizeof( particleVertex ) );
	partKernel->lpDevice->SetFVF( particleVertex::FVF ); 

	DWORD partIndex, vertCount;

	rv = lpVertexBuffer->Lock( 0, 0, (void**)&pVert, D3DLOCK_DISCARD ); 
	if( FAILED( rv ) )
		return rv;
	
	for( partIndex=0, vertCount=0; partIndex<partCount; partIndex++ )
	{
		
		if( particles[partIndex].busy != true )
			continue;
		
		if( this->smoothParticles )
		{
			length =	particles[partIndex].currSpeed[0] * particles[partIndex].currSpeed[0] + 
						particles[partIndex].currSpeed[1] * particles[partIndex].currSpeed[1] + 
						particles[partIndex].currSpeed[2] * particles[partIndex].currSpeed[2];
		

			if( length < 1.0f )				steps = 1;
			else if( length < 9.0f )		steps = 3;
			else if( length < 16.0f )		steps = 4;
			else if( length < 25.25f )		steps = 5;
			else if( length < 36.00f )		steps = 6;
			else if( length < 49.00f )		steps = 7;
			else							steps = 8;

		}
		else
			steps = 1;

		copyVector3( particles[partIndex].currSpeed, speed );
		copyVector3( particles[partIndex].currPos, pos );

		scaleVector3( speed, -0.04f / ( float )steps );
		size = particles[partIndex].size * 0.5f;

		for( iStep=0; iStep<steps; iStep++, vertCount += 4 ){
				
			if( vertCount >= partCount * 8 )
				break;
			
			// если частица участвует в процессе рендерига
			copyVector3( pos, pVert->pos );
			pVert->shiftVector[0] = -size; 
			pVert->shiftVector[1] = size;
			pVert->alpha = particles[partIndex].color[0];
			pVert->tex0[0] = 0.0f;
			pVert->tex0[1] = 0.0f;

			pVert ++;

			copyVector3( pos, pVert->pos );
			pVert->shiftVector[0] = size; 
			pVert->shiftVector[1] = size;
			pVert->alpha = particles[partIndex].color[0];
			pVert->tex0[0] = 1.0f;
			pVert->tex0[1] = 0.0f;				

			pVert ++;

			copyVector3( pos, pVert->pos );
			pVert->shiftVector[0] = -size; 
			pVert->shiftVector[1] = -size;
			pVert->alpha = particles[partIndex].color[0];
			pVert->tex0[0] = 0.0f;
			pVert->tex0[1] = 1.0f;				

			pVert ++;

			copyVector3( pos, pVert->pos );
			pVert->shiftVector[0] = size; 
			pVert->shiftVector[1] = -size;
			pVert->alpha = particles[partIndex].color[0];
			pVert->tex0[0] = 1.0f;
			pVert->tex0[1] = 1.0f;				

			pVert ++;

			addVector3( pos, speed, pos );
		}
	}
	
	rv = lpVertexBuffer->Unlock();
	if( FAILED( rv ) )
		return rv;

	rv = partKernel->lpDevice->SetIndices( this->lpIndexBuffer );
	if(FAILED(rv))
		return rv;
	
	this->lpEffect->Begin( &uPasses, 0 );
	for( iPass = 0; iPass < uPasses; iPass++ )
	{
		rv = lpEffect->/*BeginPass( iPass );
		if(FAILED(rv))
			return rv;
				
		rv = partKernel->lpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, vertCount, 0, vertCount >> 1 );
		if(FAILED(rv))
			return rv;	
		
		/*
		rv = lpEffect->EndPass();
		if(FAILED(rv))
			return rv;
		
	}
	lpEffect->End();

	return D3D_OK;
}

HRESULT particleSystem::update( DWORD dwTime )
{
	if( effectType == ptSalut )
	{
		return updateSalut( dwTime );
	}
	else if( effectType == ptType1 )
	{
		return updateType1( dwTime );
	}
	else if( effectType == ptMagicWand )
	{
		return updateMagicWand( dwTime );
	}
	else if( effectType == ptSmoke ) 
	{ 
		return updateTrainSmoke( dwTime );
	}
	
	return D3D_OK;
}

HRESULT particleSystem::start(  )
{
	
	HRESULT rv;
	
	if( effectType == ptSalut )
	{
		rv = startSalut(  );
	}
	else if( effectType == ptType1 )
	{
		rv = startType1( );
	}
	else if( effectType == ptMagicWand )
	{
		rv = startMagicWand( );
	}
	else if( effectType == ptSmoke )
	{
		rv = startTrainSmoke();
	}
	this->started = true;

	return rv;
}

inline float getValueWithDelta( float a, float da )
{
	
	float val;

	val  = ( float ) rand();
	val *= ( 1.0f / RAND_MAX );
	val *= da;
	val += a;

	return val;
}

HRESULT particleSystem::startSalut()
{

	DWORD	partIndex;
	DWORD	dwTime;
	float	fTime;

	dwTime	= timeGetTime();
	fTime	= ((float) dwTime) * 0.001f;
	
	for( partIndex=0; partIndex < partCount; partIndex++ )
	{
		// текущее положение 
		setVector3( particles[partIndex].beginPos, 0.0f, 0.0f, 0.0f );
		
		// текущая скорость частицы
		copyVector3( e0BeginSpeed, particles[partIndex].currSpeed );
		
		// текущая скорость частицы
		copyVector3( e0BeginSpeed, particles[partIndex].beginSpeed );

		// начальное ускорение частицы
		copyVector3( e0Vel, particles[partIndex].currVel );
		
		// начальное положение 
		copyVector3( particles[partIndex].beginPos, particles[partIndex].currPos );
			
		// начальный цвет
		particles[partIndex].dwColor		= dwColor1;

		// теперь остальные цвета
		RGB2Float( dwColor1, particles[partIndex].color1 );
		RGB2Float( dwColor2, particles[partIndex].color2 );
		RGB2Float( dwColor3, particles[partIndex].color3 );

		// время создания частицы
		particles[partIndex].creationTime	= fTime;

		// теперь устанавливаем время когда частицы взорвутся
		// в пределах +/- e0ExpTimeDelta
		particles[partIndex].time1			= getValueWithDelta( e0ExpTime, 0.0f );
					
		// теперь устанавливаем время, когда частицы начнут светиться
		particles[partIndex].time2			= getValueWithDelta( e0BlinkTime, e0BlinkTimeDelta );

		// теперь устанавливаем время, когда частицы начнут гаснуть
		particles[partIndex].time3			= getValueWithDelta( e0FadeTime, e0FadeTimeDelta );

		// теперь устанавливаем время, когда частицы погибнут
		particles[partIndex].time4			= getValueWithDelta( e0EndTime, e0EndTimeDelta );
		
		// теперь устанавливаем время периодичности, через какое частицы будут мигать
		particles[partIndex].time5			= getValueWithDelta( e0BlinkPeriod, e0BlinkPeriodDelta );
		while( particles[partIndex].time5 == 0.0f)
			particles[partIndex].time5			= getValueWithDelta( e0BlinkPeriod, e0BlinkPeriodDelta ); // чтобы не равнялось 0

		// текущее время частицы
		particles[partIndex].currentTime    = particles[partIndex].creationTime;
		
		// состояние частицы
		particles[partIndex].currentState	= 0;
		
		// частица задействована
		particles[partIndex].busy			= true;
	
	}
		
	return D3D_OK;

}

HRESULT particleSystem::updateSalut( DWORD dwTime )
{

	DWORD	partIndex; 
	float	fTime, fTime1, tmp, tmp2, speed;
	
	float	*color;
	Vector4	color2;

	fTime  = dwTime * 0.001f;
	
	for( partIndex=0; partIndex<partCount; partIndex++ )
	{
	
		if( particles[partIndex].busy == false )
			continue;
		
		fTime1 = fTime - particles[partIndex].currentTime;
		
		// смотрим состояние каждой частицы
		switch( particles[partIndex].currentState )
		{
		
		case 0:
			// частица двигается вместе со всеми вверх, пока взрыва нет
			tmp = fTime1 * fTime1 * 0.5f;

			// движение тела брошенного под углом к горизонту
			particles[partIndex].currPos[0] += particles[partIndex].currSpeed[0] * fTime1 + particles[partIndex].currVel[0] * tmp; 
			particles[partIndex].currPos[1] += particles[partIndex].currSpeed[1] * fTime1 + particles[partIndex].currVel[1] * tmp; 
			particles[partIndex].currPos[2] += particles[partIndex].currSpeed[2] * fTime1 + particles[partIndex].currVel[2] * tmp; 

			particles[partIndex].currSpeed[0] += particles[partIndex].currVel[0] * fTime1;
			particles[partIndex].currSpeed[1] += particles[partIndex].currVel[1] * fTime1;
			particles[partIndex].currSpeed[2] += particles[partIndex].currVel[2] * fTime1;
			
			particles[partIndex].currentTime = fTime;

			// частица должна перейти во второе состояние
			// взрыв
			if( particles[partIndex].currentTime - particles[partIndex].creationTime >= particles[partIndex].time1 )
			{
			
				// переход в состояние взрыва
				particles[partIndex].currentState = 1;

				particles[partIndex].creationTime = fTime;
				
				// создаем скорость частицы после взрыва
				// сначала ее направление 
				particles[partIndex].beginSpeed[0] = - 0.5f + (float) rand() * ( 1.0f / RAND_MAX );
				particles[partIndex].beginSpeed[1] = - 0.5f + (float) rand() * ( 1.0f / RAND_MAX );
				particles[partIndex].beginSpeed[2] = - 0.5f + (float) rand() * ( 1.0f / RAND_MAX );
				
				// нормируем вектор
				normalizeVector( particles[partIndex].beginSpeed );
                
				// вычисляем скорость
				particles[partIndex].currSpeed[0] = particles[partIndex].beginSpeed[0];
				particles[partIndex].currSpeed[1] = particles[partIndex].beginSpeed[1];
				particles[partIndex].currSpeed[2] = particles[partIndex].beginSpeed[2];

				// теперь получаем скорость
				speed = getValueWithDelta( e0ExpSpeed, e0ExpSpeedDelta );

				// масштабируем вектор
				scaleVector3( particles[partIndex].currSpeed, speed );
			}

			break;
		
		case 1:
			// в этом состоянии частицы летят в разные стороны
			tmp = fTime1 * fTime1 * 0.5f;

			// движение тела брошенного под углом к горизонту
			particles[partIndex].currPos[0] += particles[partIndex].currSpeed[0] * fTime1 + particles[partIndex].currVel[0] * tmp; 
			particles[partIndex].currPos[1] += particles[partIndex].currSpeed[1] * fTime1 + particles[partIndex].currVel[1] * tmp; 
			particles[partIndex].currPos[2] += particles[partIndex].currSpeed[2] * fTime1 + particles[partIndex].currVel[2] * tmp; 

			particles[partIndex].currSpeed[0] += particles[partIndex].currVel[0] * fTime1;
			particles[partIndex].currSpeed[1] += particles[partIndex].currVel[1] * fTime1;
			particles[partIndex].currSpeed[2] += particles[partIndex].currVel[2] * fTime1;
			
			particles[partIndex].currentTime = fTime;

			// частица достигла своей цели
			if( particles[partIndex].currentTime - particles[partIndex].creationTime >= particles[partIndex].time2 )
			{
			
				// ее движение замедляется
				scaleVector3( particles[partIndex].currSpeed, 0.2f );
				
				// теперь она должна помигать
				// для этого цвет частицы меняется
				particles[partIndex].dwColor = Float2RGB( particles[partIndex].color2 );

				// меняем состояние
				particles[partIndex].currentState	= 2;

				// запоминаем время перехода в состояние
				particles[partIndex].creationTime	= fTime;

				// запоминаем предыдущее время
				particles[partIndex].prevTime		= fTime;

				// запоминаем свечение
				particles[partIndex].blink			= true;

			}
						
			break;

		case 2:
			
			tmp = fTime1 * fTime1 * 0.5f;
			
			// теперь тело просто падает
			particles[partIndex].currPos[0] += particles[partIndex].currSpeed[0] * fTime1 + particles[partIndex].currVel[0] * tmp; 
			particles[partIndex].currPos[1] += particles[partIndex].currSpeed[1] * fTime1 + particles[partIndex].currVel[1] * tmp; 
			particles[partIndex].currPos[2] += particles[partIndex].currSpeed[2] * fTime1 + particles[partIndex].currVel[2] * tmp; 
			
			particles[partIndex].currSpeed[0] += particles[partIndex].currVel[0] * fTime1;
			particles[partIndex].currSpeed[1] += particles[partIndex].currVel[1] * fTime1;
			particles[partIndex].currSpeed[2] += particles[partIndex].currVel[2] * fTime1;
			
			particles[partIndex].currentTime = fTime;
			
			// здесь частицы просто мерцают
			if( fTime - particles[partIndex].prevTime >= particles[partIndex].time5 )
			{
				if( particles[partIndex].blink == true )
				{
					particles[partIndex].blink = false;
					particles[partIndex].dwColor = Float2RGB( particles[ partIndex ].color1 );
				}
				else 
				{
					particles[partIndex].blink = true;
					particles[partIndex].dwColor = Float2RGB( particles[ partIndex ].color2 );
				}
			
				particles[partIndex].prevTime = fTime;
			}
			
			if( particles[partIndex].currentTime - particles[partIndex].creationTime > particles[partIndex].time3 )
			{
				
				// меняем состояние 
				particles[ partIndex ].currentState = 3;

				// запоминаем время
				particles[ partIndex ].creationTime = fTime;
			
				particles[ partIndex ].val1 = 1.0f / particles[partIndex].time4;
			}
			
			break;

		case 3:
			
			tmp = fTime1 * fTime1 * 0.5f;
			
			// теперь тело просто падает
			particles[partIndex].currPos[0] += particles[partIndex].currSpeed[0] * fTime1 + particles[partIndex].currVel[0] * tmp; 
			particles[partIndex].currPos[1] += particles[partIndex].currSpeed[1] * fTime1 + particles[partIndex].currVel[1] * tmp; 
			particles[partIndex].currPos[2] += particles[partIndex].currSpeed[2] * fTime1 + particles[partIndex].currVel[2] * tmp; 
			
			particles[partIndex].currSpeed[0] += particles[partIndex].currVel[0] * fTime1;
			particles[partIndex].currSpeed[1] += particles[partIndex].currVel[1] * fTime1;
			particles[partIndex].currSpeed[2] += particles[partIndex].currVel[2] * fTime1;
			
			particles[partIndex].currentTime = fTime;
			
			if( particles[partIndex].blink )
				color = particles[partIndex].color2;
			else 
				color = particles[partIndex].color1;
			
			// теперь вычисляем новый цвет частицы 
			tmp2 = ( fTime - particles[partIndex].creationTime ) * ( particles[partIndex].val1 );
			color2[0] = color[0] + ( color[0] - particles[partIndex].color3[0] ) * tmp2;  			
			color2[1] = color[1] + ( color[1] - particles[partIndex].color3[1] ) * tmp2;  			
			color2[2] = color[2] + ( color[2] - particles[partIndex].color3[2] ) * tmp2;  
			color2[3] = color[3] + ( color[3] - particles[partIndex].color3[3] ) * tmp2;  			

			// смотрим умерла ли частица
			if( particles[partIndex].currentTime - particles[partIndex].creationTime > particles[partIndex].time4 )
			{
			
				// всё ... частица умерла
				particles[partIndex].currentState = 4;
                particles[partIndex].busy = false;

			}

			break;

		case 4:
			break;

		}

	}
	
	return D3D_OK;
}

void particleSystem::getObjectBasis()
{
	
	this->e2dot[0] = this->lpObj->basis[3][0];
	this->e2dot[1] = this->lpObj->basis[3][1];
	this->e2dot[2] = this->lpObj->basis[3][2];

	this->e2dir[0] = this->lpObj->basis[0][2];
	this->e2dir[1] = this->lpObj->basis[1][2];
	this->e2dir[2] = this->lpObj->basis[2][2];

	this->e2speed[0] = -this->lpObj->basis[0][0];
	this->e2speed[1] = -this->lpObj->basis[1][0];
	this->e2speed[2] = -this->lpObj->basis[2][0];

	return ;
}

HRESULT particleSystem::getDataFromObject( Object *obj )
{

	T1Vertex	*vert;
	HRESULT		rv;
	
	DWORD       partIndex;
	DWORD		vertIndex, dv;
	
	effectType = ptType1;
	
	// получаем доступ к данным
	rv = obj->vertexBuffer->Lock( 0, 0, (void**)&vert, 0 );
	if( FAILED( rv ) )
		return rv;

	// инициализируем частицы
	if( obj->vertexCount < partCount )
		dv = 1;
	else
		dv = obj->vertexCount / partCount;

	for( vertIndex = 0, partIndex=0; partIndex < partCount && vertIndex < obj->vertexCount; partIndex++, vertIndex += dv )
	{
		
		// копируем начальное положение и начальную скорость
		copyVector3( vert[ vertIndex ].SCoor, particles[ partIndex ].beginPos );
		copyVector3( vert[ vertIndex ].Normal, particles[ partIndex ].beginSpeed );

		// копируем цвета
		particles[partIndex].color1[0] = 1.0f; 
		particles[partIndex].color1[1] = obj->detail[0].Material.Diffuse.r; 
		particles[partIndex].color1[2] = obj->detail[0].Material.Diffuse.g; 
		particles[partIndex].color1[3] = obj->detail[0].Material.Diffuse.b;

		particles[partIndex].color2[0] = 1.0f; 
		particles[partIndex].color2[1] = obj->detail[0].Material.Specular.r; 
		particles[partIndex].color2[2] = obj->detail[0].Material.Specular.g; 
		particles[partIndex].color2[3] = obj->detail[0].Material.Specular.b;

		particles[partIndex].color3[0] = 0.0f; 
		particles[partIndex].color3[1] = obj->detail[0].Material.Ambient.r; 
		particles[partIndex].color3[2] = obj->detail[0].Material.Ambient.g; 
		particles[partIndex].color3[3] = obj->detail[0].Material.Ambient.b;
	
	}

	// освобождаем доступ к данным
	rv = obj->vertexBuffer->Unlock( );
	if( FAILED( rv ) )
		return rv;

	return D3D_OK;
}

HRESULT particleSystem::startType1()
{

	DWORD	partIndex;
	DWORD	dwTime;
	float	fTime;

	dwTime	= timeGetTime();
	fTime	= ((float) dwTime) * 0.001f;
	
	for( partIndex=0; partIndex < partCount; partIndex++ )
	{
		// текущее положение 
		copyVector3( particles[partIndex].beginPos, particles[partIndex].currPos );
		
		// текущая скорость частицы
		particles[partIndex].currSpeed[0] = generateValueFromRange( particles[ partIndex ].beginSpeed[0] - 0.05f, particles[ partIndex ].beginSpeed[0] + 0.05f );
		particles[partIndex].currSpeed[1] = generateValueFromRange( particles[ partIndex ].beginSpeed[1] - 0.05f, particles[ partIndex ].beginSpeed[1] + 0.05f );
		particles[partIndex].currSpeed[2] = generateValueFromRange( particles[ partIndex ].beginSpeed[2] - 0.05f, particles[ partIndex ].beginSpeed[2] + 0.05f );

		// нормируем вектор скорости
		normalizeVector( particles[partIndex].currSpeed );

		// новая текущая скорость
		scaleVector3( particles[partIndex].currSpeed, generateValueFromRange( e1Speed - e1SpeedDelta, e1Speed - e1SpeedDelta ) ); 
		
		// начальное ускорение частицы
		copyVector3( e1Vel, particles[partIndex].currVel );
					
		// начальный цвет
		particles[partIndex].dwColor		= Float2RGB( particles[partIndex].color1 ) ;

		// время создания частицы
		particles[partIndex].creationTime	= generateValueFromRange( fTime - e1BlinkTime, fTime + e1BlinkTime );

		// теперь устанавливаем время, когда частицы начнут светиться
		particles[partIndex].time2			= getValueWithDelta( e1BlinkTime, e1BlinkTimeDelta );

		// теперь устанавливаем время, когда частицы начнут гаснуть
		particles[partIndex].time3			= getValueWithDelta( e1FadeTime, e1FadeTimeDelta );

		// теперь устанавливаем время, когда частицы погибнут
		particles[partIndex].time4			= getValueWithDelta( e1EndTime, e1EndTimeDelta );
		
		// теперь устанавливаем время периодичности, через какое частицы будут мигать
		particles[partIndex].time5			= getValueWithDelta( e1BlinkPeriod, e1BlinkPeriodDelta );
		while( particles[partIndex].time5 == 0.0f)
			particles[partIndex].time5			= getValueWithDelta( e1BlinkPeriod, e1BlinkPeriodDelta ); // чтобы не равнялось 0

		// текущее время частицы
		particles[partIndex].currentTime    = particles[partIndex].creationTime;
		
		// состояние частицы
		particles[partIndex].currentState	= 0;
		
		// частица задействована
		particles[partIndex].busy			= true;
	
	}
		
	
	return D3D_OK;

}

HRESULT particleSystem::updateType1( DWORD dwTime )
{
	
	DWORD	partIndex; 
	float	fTime, fTime1, tmp, tmp2;
	
	float	*color;
	Vector4	color2;

	fTime  = dwTime * 0.001f;
	
	for( partIndex=0; partIndex<partCount; partIndex++ )
	{
	
		fTime1 = fTime - particles[partIndex].currentTime;
		
		if( fTime1 < 0.0f )
			continue;
		
		else 
			particles[partIndex].busy = true;
		
		if( particles[partIndex].busy == false )
			continue;
				
		// смотрим состояние каждой частицы
		switch( particles[partIndex].currentState )
		{
		
		case 0:
			// частица двигается вместе со всеми вверх, пока взрыва нет
			tmp = fTime1 * fTime1 * 0.5f;

			// движение тела брошенного под углом к горизонту
			// движение тела брошенного под углом к горизонту
			particles[partIndex].currPos[0] += particles[partIndex].currSpeed[0] * fTime1 + particles[partIndex].currVel[0] * tmp; 
			particles[partIndex].currPos[1] += particles[partIndex].currSpeed[1] * fTime1 + particles[partIndex].currVel[1] * tmp; 
			particles[partIndex].currPos[2] += particles[partIndex].currSpeed[2] * fTime1 + particles[partIndex].currVel[2] * tmp; 

			particles[partIndex].currSpeed[0] += particles[partIndex].currVel[0] * fTime1;
			particles[partIndex].currSpeed[1] += particles[partIndex].currVel[1] * fTime1;
			particles[partIndex].currSpeed[2] += particles[partIndex].currVel[2] * fTime1;

			particles[partIndex].currentTime = fTime;

			// частица должна перейти во второе состояние
			// взрыв
			
			if( particles[partIndex].currentTime - particles[partIndex].creationTime >= particles[partIndex].time2 )
			{
			
				scaleVector3( particles[partIndex].currSpeed, 0.2f );
				
				// переход в состояние взрыва
				particles[partIndex].currentState = 1;

				// запоминание времения
				particles[partIndex].creationTime = fTime;
								
			}
			

			break;
		
		case 1:

			// частица двигается вместе со всеми вверх, пока взрыва нет
			tmp = fTime1 * fTime1 * 0.5f * 0.2f;

			// движение тела брошенного под углом к горизонту
			particles[partIndex].currPos[0] += particles[partIndex].currSpeed[0] * fTime1 + particles[partIndex].currVel[0] * tmp; 
			particles[partIndex].currPos[1] += particles[partIndex].currSpeed[1] * fTime1 + particles[partIndex].currVel[1] * tmp; 
			particles[partIndex].currPos[2] += particles[partIndex].currSpeed[2] * fTime1 + particles[partIndex].currVel[2] * tmp; 

			particles[partIndex].currSpeed[0] += particles[partIndex].currVel[0] * fTime1;
			particles[partIndex].currSpeed[1] += particles[partIndex].currVel[1] * fTime1;
			particles[partIndex].currSpeed[2] += particles[partIndex].currVel[2] * fTime1;

			particles[partIndex].currentTime = fTime;
			
			// здесь частицы просто мерцают
			if( fTime - particles[partIndex].prevTime >= particles[partIndex].time5 )
			{
				if( particles[partIndex].blink == true )
				{
					particles[partIndex].blink = false;
					particles[partIndex].dwColor = Float2RGB( particles[ partIndex ].color1 );
				}
				else 
				{
					particles[partIndex].blink = true;
					particles[partIndex].dwColor = Float2RGB( particles[ partIndex ].color2 );
				}
			
				particles[partIndex].prevTime = fTime;
			}
			
			if( particles[partIndex].currentTime - particles[partIndex].creationTime > particles[partIndex].time3 )
			{
				
				// меняем состояние 
				particles[ partIndex ].currentState = 2;

				// запоминаем время
				particles[ partIndex ].creationTime = fTime;
			
				particles[ partIndex ].val1 = 1.0f / particles[partIndex].time4;
			}
			
			break;

		case 2:
			
			// частица двигается вместе со всеми вверх, пока взрыва нет
			tmp = fTime1 * fTime1 * 0.5f * 0.2f;

			// движение тела брошенного под углом к горизонту
			particles[partIndex].currPos[0] += particles[partIndex].currSpeed[0] * fTime1 + particles[partIndex].currVel[0] * tmp; 
			particles[partIndex].currPos[1] += particles[partIndex].currSpeed[1] * fTime1 + particles[partIndex].currVel[1] * tmp; 
			particles[partIndex].currPos[2] += particles[partIndex].currSpeed[2] * fTime1 + particles[partIndex].currVel[2] * tmp; 

			particles[partIndex].currSpeed[0] += particles[partIndex].currVel[0] * fTime1;
			particles[partIndex].currSpeed[1] += particles[partIndex].currVel[1] * fTime1;
			particles[partIndex].currSpeed[2] += particles[partIndex].currVel[2] * fTime1;

			particles[partIndex].currentTime = fTime;
			
			if( particles[partIndex].blink )
				color = particles[partIndex].color2;
			else 
				color = particles[partIndex].color1;
			
			// теперь вычисляем новый цвет частицы 
			tmp2 = ( fTime - particles[partIndex].creationTime ) * ( particles[partIndex].val1 );
			color2[0] = color[0] + ( color[0] - particles[partIndex].color3[0] ) * tmp2;  			
			color2[1] = color[1] + ( color[1] - particles[partIndex].color3[1] ) * tmp2;  			
			color2[2] = color[2] + ( color[2] - particles[partIndex].color3[2] ) * tmp2;  
			color2[3] = color[3] + ( color[3] - particles[partIndex].color3[3] ) * tmp2;  			

			// смотрим умерла ли частица
			if( particles[partIndex].currentTime - particles[partIndex].creationTime > particles[partIndex].time4 )
			{
			
				// всё ... частица умерла
				particles[partIndex].currentState = 3;
                
			}

			break;

		case 3: // рождение новой частицы !!!

			// текущее положение 
			copyVector3( particles[partIndex].beginPos, particles[partIndex].currPos );
			
			// текущая скорость частицы
			particles[partIndex].currSpeed[0] = generateValueFromRange( particles[ partIndex ].beginSpeed[0] - 0.05f, particles[ partIndex ].beginSpeed[0] + 0.05f );
			particles[partIndex].currSpeed[1] = generateValueFromRange( particles[ partIndex ].beginSpeed[1] - 0.05f, particles[ partIndex ].beginSpeed[1] + 0.05f );
			particles[partIndex].currSpeed[2] = generateValueFromRange( particles[ partIndex ].beginSpeed[2] - 0.05f, particles[ partIndex ].beginSpeed[2] + 0.05f );

			// нормируем вектор скорости
			normalizeVector( particles[partIndex].currSpeed );

			// новая текущая скорость
			scaleVector3( particles[partIndex].currSpeed, generateValueFromRange( e1Speed - e1SpeedDelta, e1Speed + e1SpeedDelta ) ); 
			
			// начальное ускорение частицы
			copyVector3( e1Vel, particles[partIndex].currVel );
						
			// начальный цвет
			particles[partIndex].dwColor		= Float2RGB( particles[partIndex].color1 ) ;

			// время создания частицы
			particles[partIndex].creationTime	= fTime;

			// теперь устанавливаем время, когда частицы начнут светиться
			particles[partIndex].time2			= getValueWithDelta( e1BlinkTime, e1BlinkTimeDelta );

			// теперь устанавливаем время, когда частицы начнут гаснуть
			particles[partIndex].time3			= getValueWithDelta( e1FadeTime, e1FadeTimeDelta );

			// теперь устанавливаем время, когда частицы погибнут
			particles[partIndex].time4			= getValueWithDelta( e1EndTime, e1EndTimeDelta );
			
			// теперь устанавливаем время периодичности, через какое частицы будут мигать
			particles[partIndex].time5			= getValueWithDelta( e1BlinkPeriod, e1BlinkPeriodDelta );
			while( particles[partIndex].time5 == 0.0f)
				particles[partIndex].time5			= getValueWithDelta( e1BlinkPeriod, e1BlinkPeriodDelta ); // чтобы не равнялось 0

			// текущее время частицы
			particles[partIndex].currentTime    = particles[partIndex].creationTime;
			
			// состояние частицы
			particles[partIndex].currentState	= 0;
			
			// частица задействована
			particles[partIndex].busy			= true;
						
			break;

		}

	}
	
	return D3D_OK;
	
}

HRESULT particleSystem::startMagicWand()
{

	DWORD	partIndex;
	DWORD	dwTime;
	float	fTime, fPos;
	Vector3	off;

	dwTime	= timeGetTime();
	fTime	= ((float) dwTime) * 0.001f;
	
	this->getObjectBasis();
	
	for( partIndex=0; partIndex < partCount; partIndex++ )
	{
		// создаем текущее положение частицы в длине
		fPos = generateValueFromRange( this->e2from, this->e2to );
		// теперь получаем вектор смещеня
		scaleVector3( this->e2dir, fPos, off );
		// получаем точку в пространстве
		addVector3( this->e2dot, off, particles[ partIndex ].beginPos );
        
		// текущее положение 
		copyVector3( particles[partIndex].beginPos, particles[partIndex].currPos );
				
		// текущая скорость частицы
		copyVector3( this->e2speed, particles[partIndex].currSpeed );
		//scaleVector3( particles[partIndex].currSpeed, generateValueFromRange( 1.0f, 1.5f ) );						

		// начальное ускорение частицы
		copyVector3( this->e2vel, particles[partIndex].currVel );
		//scaleVector3( particles[partIndex].currVel, generateValueFromRange( 1.0f, 1.3f ) );
		
		// теперь остальные цвета
		RGB2Float( dwColor1, particles[partIndex].color1 );
		RGB2Float( dwColor2, particles[partIndex].color2 );
		RGB2Float( dwColor3, particles[partIndex].color3 );
		
		// начальный цвет
		particles[partIndex].dwColor		= dwColor1 ;

		// теперь устанавливаем время, когда частицы начнут светиться
		particles[partIndex].time2			= getValueWithDelta( this->e2blinkTime, this->e2blinkTimeDelta );

		// теперь устанавливаем время, когда частицы начнут гаснуть
		particles[partIndex].time3			= getValueWithDelta( this->e2fadeTime, this->e2fadeTimeDelta );

		// теперь устанавливаем время, когда частицы погибнут
		particles[partIndex].time4			= getValueWithDelta( this->e2dieTime, this->e2dieTimeDelta );
		
		// время создания частицы
		particles[partIndex].creationTime	= fTime + 3 * generateValueFromRange( 0.0f, particles[partIndex].time2 + particles[partIndex].time3 + particles[partIndex].time4 );
		if( partIndex < 10 )
			particles[partIndex].creationTime	= fTime;

		// теперь устанавливаем время периодичности, через какое частицы будут мигать
		particles[partIndex].time5			= 0.0f; // getValueWithDelta( this->e2blinkPeriod, this->e2blinkPeriodDelta );
		//while( particles[partIndex].time5 == 0.0f)
		//	particles[partIndex].time5			= 0.0f; // getValueWithDelta( this->e2blinkPeriod, this->e2blinkPeriodDelta ); // чтобы не равнялось 0

		// текущее время частицы
		particles[partIndex].currentTime    = particles[partIndex].creationTime;
		
		// состояние частицы
		particles[partIndex].currentState	= 3;
		
		// частица задействована
		particles[partIndex].busy			= partIndex < 10 ? true : false;
	
	}
	
	return D3D_OK;
}

void particleSystem::generateTrainSmokeParticle( particle* p, float fTime ) { 

	Vector3 delta, res;
	
	if( this->useLocalBasis ) {
		setVector3( p->beginPos, 0.0f, 0.0f, 0.0f );
		setVector3( p->currPos, 0.0f, 0.0f, 0.0f );
	}
	else { 
		setVector3( p->beginPos, this->localBasis[3][0], this->localBasis[3][1], this->localBasis[3][2] );
		copyVector3(p->beginPos, p->currPos );
	}

	// геренируем скорость текстуры согласно направлению
	setVector3( delta, getValueWithDelta( 0.0f, this->trainSmokePars.dirDelta ), 0.0f, getValueWithDelta( 0.0f, this->trainSmokePars.dirDelta ) );
	
	res[0] = delta[0];
	res[1] = 1.0f;
	res[2] = delta[2];

	quicknormalizeVector( res );
	
	if( !this->useLocalBasis ) { 
		translateVector( res, this->localBasis, res );
	}

	scaleVector3( res, getValueWithDelta( this->trainSmokePars.smokeSpeed1, this->trainSmokePars.smokeSpeed1Delta ), p->currSpeed );
	copyVector3( p->currSpeed, p->beginSpeed );

	p->time1 = getValueWithDelta( this->trainSmokePars.smokeTime1, this->trainSmokePars.smokeTime1Delta );
	p->time2 = getValueWithDelta( this->trainSmokePars.smokeTime2, this->trainSmokePars.smokeTime2Delta );
	p->size1 = getValueWithDelta( this->trainSmokePars.smokeSize1, this->trainSmokePars.smokeSize1Delta );
	p->size2 = getValueWithDelta( this->trainSmokePars.smokeSize2, this->trainSmokePars.smokeSize2Delta );
	p->size3 = getValueWithDelta( this->trainSmokePars.smokeSize3, this->trainSmokePars.smokeSize3Delta );
	p->size = p->size1;

	p->dsize1 = ( p->size2 - p->size1 ) / p->time1;
	p->dsize2 = ( p->size3 - p->size2 ) / p->time2;

	RGB2Float( this->dwColor1, p->color1 );
	RGB2Float( this->dwColor2, p->color2 );
	RGB2Float( this->dwColor3, p->color3 );

	copyVector4( p->color1, p->color );
	p->dcolor[0] = ( p->color2[0] - p->color1[0] ) / p->time1; 
	p->dcolor[1] = ( p->color2[1] - p->color1[1] ) / p->time1;
	p->dcolor[2] = ( p->color2[2] - p->color1[2] ) / p->time1;
	p->dcolor[3] = ( p->color2[3] - p->color1[3] ) / p->time1;
	
	p->currentState = 0;
	p->busy = true;
	float tm = this->trainSmokePars.smokeTime1 + this->trainSmokePars.smokeTime1Delta + this->trainSmokePars.smokeTime2 + this->trainSmokePars.smokeTime2Delta;
	if( !this->started )
		p->creationTime = fTime + tm / this->partCount * this->trainSmokePars.particleIndex;
	else
		p->creationTime = fTime;
	p->currentTime = fTime;

	return;
}

HRESULT particleSystem::startTrainSmoke()
{

	DWORD	partIndex;
	DWORD	dwTime;
	float	fTime;

	dwTime	= timeGetTime();
	fTime	= ((float) dwTime) * 0.001f;
	
	this->effectType = ptSmoke;
	//normalizeVector( this->trainSmokePars.direction );

	for( this->trainSmokePars.particleIndex = 0; this->trainSmokePars.particleIndex < partCount; this->trainSmokePars.particleIndex ++ ) { 
		this->generateTrainSmokeParticle( &this->particles[ this->trainSmokePars.particleIndex ], fTime );
	}
		
	return D3D_OK;
}

HRESULT particleSystem::updateTrainSmoke( DWORD dwTime )
{
	
	float		fTime = dwTime * 0.001f;
	float		deltaTime;
	float		fdt;
	particle	*p;

	int pi;
	for( pi = 0, p = this->particles; pi < this->partCount; pi ++, p ++ ) { 
		 
		fdt = fTime - p->currentTime;
		if( fdt <= 0.0f ) continue;

		p->busy = true;

		deltaTime = fTime - p->creationTime;
		if(deltaTime < 0.0f) continue;
		// проверяем состояние
		if( p->currentState == 0 ) { // дым резко поднимается по трубе под давлением
			
			if( deltaTime + fdt > p->time1 ) fdt = p->time1 - deltaTime;
			if( fdt < 0.0f ) fdt = 0.0f;

			// текущее положение
			p->currPos[0] += p->currSpeed[0] * fdt;
			p->currPos[1] += p->currSpeed[1] * fdt;
			p->currPos[2] += p->currSpeed[2] * fdt;
	
			// текущий размер
			p->size += p->dsize1 * fdt;
            
			interpolateColor( p->color1, p->color, p->dcolor, deltaTime + fdt );
			
			p->currentTime += fdt;
			if( deltaTime + fdt >= p->time1 ) { 
				quicknormalizeVector( p->currSpeed );
				scaleVector3( p->currSpeed, getValueWithDelta( this->trainSmokePars.smokeSpeed2, this->trainSmokePars.smokeSpeed2Delta ) );
				p->currentState = 1;
			
				p->dcolor[0] = ( p->color3[0] - p->color2[0] ) / p->time2; 
				p->dcolor[1] = ( p->color3[1] - p->color2[1] ) / p->time2;
				p->dcolor[2] = ( p->color3[2] - p->color2[2] ) / p->time2;
				p->dcolor[3] = ( p->color3[3] - p->color2[3] ) / p->time2;
			}
		}
		else if( p->currentState == 1 ) { 
			
			if( deltaTime + fdt > p->time1 + p->time2 ) fdt = p->time1 + p->time2 - deltaTime;
			if( fdt < 0.0f ) fdt = 0.0f;
			
			// текущее положение
			p->currPos[0] += p->currSpeed[0] * fdt;
			p->currPos[1] += p->currSpeed[1] * fdt;
			p->currPos[2] += p->currSpeed[2] * fdt;
	
			// текущий размер
			p->size += p->dsize2 * fdt;
		
			// текущий цвет
			interpolateColor( p->color2, p->color, p->dcolor, deltaTime + fdt - p->time1 );
			
			p->currentTime += fdt;

			if( deltaTime + fdt >= p->time1 + p->time2 ) { 
				p->busy = false;
				if( this->doNotGenerate ) continue;
				this->generateTrainSmokeParticle( p, fTime );
			}
		}
	}

	return D3D_OK;
}

HRESULT particleSystem::updateMagicWand( DWORD dwTime )
{
	
	DWORD	partIndex; 
	float	fTime, fTime1, tmp, tmp2, fPos;
	Vector3	off;

	float	*color;
	Vector4  color2;
	
	fTime  = dwTime * 0.001f;
	
	for( partIndex=0; partIndex<partCount; partIndex++ )
	{
	
		fTime1 = fTime - particles[partIndex].currentTime;
		
		if( fTime1 < 0.0f )
			continue;
		else 
			particles[partIndex].busy = true;
		
		if( particles[partIndex].busy == false )
			continue;
		
		// смотрим состояние каждой частицы
		switch( particles[partIndex].currentState )
		{
		
		case 0:
			tmp = fTime1 * fTime1 * 0.5f;

			// частыцы вылетают из линии стрелок
			particles[partIndex].currPos[0] += particles[partIndex].currSpeed[0] * fTime1 + particles[partIndex].currVel[0] * tmp; 
			particles[partIndex].currPos[1] += particles[partIndex].currSpeed[1] * fTime1 + particles[partIndex].currVel[1] * tmp; 
			particles[partIndex].currPos[2] += particles[partIndex].currSpeed[2] * fTime1 + particles[partIndex].currVel[2] * tmp; 

			particles[partIndex].currSpeed[0] += particles[partIndex].currVel[0] * fTime1;
			particles[partIndex].currSpeed[1] += particles[partIndex].currVel[1] * fTime1;
			particles[partIndex].currSpeed[2] += particles[partIndex].currVel[2] * fTime1;

			particles[partIndex].currentTime = fTime;

			// если подошло время мерцания
			if( particles[partIndex].currentTime - particles[partIndex].creationTime >= particles[partIndex].time2 )
			{
				//scaleVector3( particles[partIndex].currSpeed, 0.2f );
				
				// переход в состояние мерцания
				particles[partIndex].currentState = 1;

				// запоминание времения
				particles[partIndex].creationTime = fTime;
			}
			break;
		
		case 1:

			// частица двигается также
			tmp = fTime1 * fTime1 * 0.5f;

			// движение тела брошенного под углом к горизонту
			particles[partIndex].currPos[0] += particles[partIndex].currSpeed[0] * fTime1 + particles[partIndex].currVel[0] * tmp; 
			particles[partIndex].currPos[1] += particles[partIndex].currSpeed[1] * fTime1 + particles[partIndex].currVel[1] * tmp; 
			particles[partIndex].currPos[2] += particles[partIndex].currSpeed[2] * fTime1 + particles[partIndex].currVel[2] * tmp; 

			particles[partIndex].currSpeed[0] += particles[partIndex].currVel[0] * fTime1;
			particles[partIndex].currSpeed[1] += particles[partIndex].currVel[1] * fTime1;
			particles[partIndex].currSpeed[2] += particles[partIndex].currVel[2] * fTime1;

			particles[partIndex].currentTime = fTime;
			
			// здесь частицы просто мерцают
			if( fTime - particles[partIndex].prevTime >= particles[partIndex].time5 )
			{
				if( particles[partIndex].blink == true )
				{
					particles[partIndex].blink = false;
					particles[partIndex].dwColor = Float2RGB( particles[ partIndex ].color1 );
				}
				else 
				{
					particles[partIndex].blink = true;
					particles[partIndex].dwColor = Float2RGB( particles[ partIndex ].color2 );
				}
			
				particles[partIndex].prevTime = fTime;
			}
			
			// нужно ли менять состояние частиц
			if( particles[partIndex].currentTime - particles[partIndex].creationTime > particles[partIndex].time3 )
			{
				// меняем состояние 
				particles[ partIndex ].currentState = 2;

				// запоминаем время
				//particles[partIndex].dwColor = Float2RGB( particles[ partIndex ].color1 );
				//particles[partIndex].blink   = false;
				particles[ partIndex ].creationTime = fTime;
				particles[ partIndex ].val1 = 1.0f / particles[partIndex].time4;
			}
			break;

		case 2:
			
			// частица двигается вместе со всеми вверх, пока взрыва нет
			tmp = fTime1 * fTime1 * 0.5f;

			// движение тела брошенного под углом к горизонту
			particles[partIndex].currPos[0] += particles[partIndex].currSpeed[0] * fTime1 + particles[partIndex].currVel[0] * tmp; 
			particles[partIndex].currPos[1] += particles[partIndex].currSpeed[1] * fTime1 + particles[partIndex].currVel[1] * tmp; 
			particles[partIndex].currPos[2] += particles[partIndex].currSpeed[2] * fTime1 + particles[partIndex].currVel[2] * tmp; 

			particles[partIndex].currSpeed[0] += particles[partIndex].currVel[0] * fTime1;
			particles[partIndex].currSpeed[1] += particles[partIndex].currVel[1] * fTime1;
			particles[partIndex].currSpeed[2] += particles[partIndex].currVel[2] * fTime1;

			particles[partIndex].currentTime = fTime;
			
			if( particles[partIndex].blink )
				color = particles[partIndex].color2;
			else 
				color = particles[partIndex].color1;
			
			// теперь вычисляем новый цвет частицы 
			tmp2 = ( fTime - particles[partIndex].creationTime ) * ( particles[partIndex].val1 );
			color2[0] = color[0] + ( particles[partIndex].color3[0] - color[0] ) * tmp2;  			
			color2[1] = color[1] + ( particles[partIndex].color3[1] - color[1] ) * tmp2;  			
			color2[2] = color[2] + ( particles[partIndex].color3[2] - color[2] ) * tmp2;  
			color2[3] = color[3] + ( particles[partIndex].color3[3] - color[3] ) * tmp2;  			
			particles[partIndex].dwColor = Float2RGB( color2 );

			// смотрим умерла ли частица
			if( particles[partIndex].currentTime - particles[partIndex].creationTime > particles[partIndex].time4 )
			{
				// всё ... частица умерла
				particles[partIndex].currentState = 3;
				particles[partIndex].busy = false;
    		}
			break;

		case 3: // рождение новой частицы !!!
			particles[partIndex].busy = false;
			
			if( this->doNotGenerate == false ){	

				this->getObjectBasis();
			
				// создаем текущее положение частицы в длине
				fPos = generateValueFromRange( this->e2from, this->e2to );
				// теперь получаем вектор смещеня
				scaleVector3( this->e2dir, fPos, off );
				// получаем точку в пространстве
				addVector3( this->e2dot, off, particles[ partIndex ].beginPos );
		        
				// текущее положение 
				copyVector3( particles[partIndex].beginPos, particles[partIndex].currPos );
						
				// текущая скорость частицы
				copyVector3( this->e2speed, particles[partIndex].currSpeed );
				scaleVector3( particles[partIndex].currSpeed, generateValueFromRange( 1.5f, 2.5f ) );						

				// начальное ускорение частицы
				// начальное ускорение частицы
				copyVector3( this->e2vel, particles[partIndex].currVel );
				//scaleVector3( particles[partIndex].currVel, generateValueFromRange( 1.0f, 1.3f ) );
							
				// начальный цвет
				particles[partIndex].dwColor		= Float2RGB( particles[partIndex].color1 ) ;

				// время создания частицы
				particles[partIndex].creationTime	= fTime;

				// теперь устанавливаем время, когда частицы начнут светиться
				particles[partIndex].time2			= getValueWithDelta( this->e2blinkTime, this->e2blinkTimeDelta );

				// теперь устанавливаем время, когда частицы начнут гаснуть
				particles[partIndex].time3			= getValueWithDelta( this->e2fadeTime, this->e2fadeTimeDelta );

				// теперь устанавливаем время, когда частицы погибнут
				particles[partIndex].time4			= getValueWithDelta( this->e2dieTime, this->e2dieTimeDelta );
				
				// теперь устанавливаем время периодичности, через какое частицы будут мигать
				particles[partIndex].time5			= getValueWithDelta( this->e2blinkPeriod, this->e2blinkPeriodDelta );
				while( particles[partIndex].time5 == 0.0f)
					particles[partIndex].time5			= getValueWithDelta( this->e2blinkPeriod, this->e2blinkPeriodDelta ); // чтобы не равнялось 0

				// текущее время частицы
				particles[partIndex].currentTime    = particles[partIndex].creationTime;
				
				// состояние частицы
				particles[partIndex].currentState	= 0;
				
				// частица задействована
				particles[partIndex].busy			= true;												
			}
			break;
		}
	}
	
	return D3D_OK;
}

void interpolateColor( Vector4 color1, Vector4 color, Vector4 dcolor, float scale ) { 

	color[0] = color1[0] + dcolor[0] * scale;
	color[1] = color1[1] + dcolor[1] * scale;
	color[2] = color1[2] + dcolor[2] * scale;
	color[3] = color1[3] + dcolor[3] * scale;
}

void particleSystem::position(Vector3 v){
		
	localBasis[3][0] = v[0];
	localBasis[3][1] = v[1];
	localBasis[3][2] = v[2];

	invertBasis[3][0] = v[0];
	invertBasis[3][1] = v[1];
	invertBasis[3][2] = v[2];
	
}

void particleSystem::position(float x,float y,float z){
			
	localBasis[3][0]	= x;
	localBasis[3][1]	= y;
	localBasis[3][2]	= z;

	invertBasis[3][0]	= x;
	invertBasis[3][1]	= y;
	invertBasis[3][2]	= z;
	
}

void particleSystem::origin(){
		
	localBasis[0][0] = localBasis[1][1] = localBasis[2][2] = 1.0f;
	localBasis[0][1] = localBasis[0][2] = 0.0f;
	localBasis[1][0] = localBasis[1][2] = 0.0f;
	localBasis[2][0] = localBasis[2][1] = 0.0f;

}

void particleSystem::rotateAroundX(float angle){
	Vector3 nz;
    float ca,sa;

    ca = cos(angle);
    sa = sin(angle);

    nz[0] = localBasis[0][2]*ca + localBasis[0][1]*sa;
    nz[1] = localBasis[1][2]*ca + localBasis[1][1]*sa;
    nz[2] = localBasis[2][2]*ca + localBasis[2][1]*sa;

    localBasis[0][1] = localBasis[0][1]*ca - localBasis[0][2]*sa;
    localBasis[1][1] = localBasis[1][1]*ca - localBasis[1][2]*sa;
    localBasis[2][1] = localBasis[2][1]*ca - localBasis[2][2]*sa;

    localBasis[0][2] = nz[0];
    localBasis[1][2] = nz[1];
    localBasis[2][2] = nz[2];

}

void particleSystem::rotateAroundY(float angle){
	Vector3 nx;
    float ca,sa;

    ca = cos(angle);
    sa = sin(angle);

    nx[0] = localBasis[0][0]*ca + localBasis[0][2]*sa;
    nx[1] = localBasis[1][0]*ca + localBasis[1][2]*sa;
    nx[2] = localBasis[2][0]*ca + localBasis[2][2]*sa;

    localBasis[0][2] = localBasis[0][2]*ca - localBasis[0][0]*sa;
    localBasis[1][2] = localBasis[1][2]*ca - localBasis[1][0]*sa;
    localBasis[2][2] = localBasis[2][2]*ca - localBasis[2][0]*sa;

    localBasis[0][0] = nx[0];
    localBasis[1][0] = nx[1];
    localBasis[2][0] = nx[2];

}

void particleSystem::rotateAroundZ(float angle){
	Vector3 nx;
    float ca,sa;

    ca = cos(angle);
    sa = sin(angle);

    nx[0] = localBasis[0][0]*ca + localBasis[0][1]*sa;
    nx[1] = localBasis[1][0]*ca + localBasis[1][1]*sa;
    nx[2] = localBasis[2][0]*ca + localBasis[2][1]*sa;

    localBasis[0][1] = localBasis[0][1]*ca - localBasis[0][0]*sa;
    localBasis[1][1] = localBasis[1][1]*ca - localBasis[1][0]*sa;
    localBasis[2][1] = localBasis[2][1]*ca - localBasis[2][0]*sa;

    localBasis[0][0] = nx[0];
    localBasis[1][0] = nx[1];
    localBasis[2][0] = nx[2];

}

void particleSystem::rotateAroundV(Vector3 v,float angle){
	Vector3 tmp[3],dir;
    float ca,sa;

    ca = cos(angle);
    sa = sin(angle);

    normalizeVector(v,dir);

    tmp[0][0] = localBasis[0][0];
    tmp[0][1] = localBasis[1][0];
    tmp[0][2] = localBasis[2][0];

    tmp[1][0] = localBasis[0][1];
    tmp[1][1] = localBasis[1][1];
    tmp[1][2] = localBasis[2][1];

    tmp[2][0] = localBasis[0][2];
    tmp[2][1] = localBasis[1][2];
    tmp[2][2] = localBasis[2][2];

    rotateVectorsArrayAroundNormVector(tmp,3,dir,tmp,ca,sa);
    
    localBasis[0][0] = tmp[0][0];
    localBasis[1][0] = tmp[0][1];
    localBasis[2][0] = tmp[0][2];

    localBasis[0][1] = tmp[1][0];
    localBasis[1][1] = tmp[1][1];
    localBasis[2][1] = tmp[1][2];

    localBasis[0][2] = tmp[2][0];
    localBasis[1][2] = tmp[2][1];
    localBasis[2][2] = tmp[2][2];

}

void particleSystem::rotateAroundA(Vector3 p,Vector3 a,float angle){
	Vector3 tmp[4],dir;
    float ca,sa;

    ca = cos(angle);
    sa = sin(angle);

    normalizeVector(a,dir);

    tmp[0][0] = localBasis[0][0];
    tmp[0][1] = localBasis[1][0];
    tmp[0][2] = localBasis[2][0];

    tmp[1][0] = localBasis[0][1];
    tmp[1][1] = localBasis[1][1];
    tmp[1][2] = localBasis[2][1];

    tmp[2][0] = localBasis[0][2];
    tmp[2][1] = localBasis[1][2];
    tmp[2][2] = localBasis[2][2];

    tmp[3][0] = localBasis[3][0];
	tmp[3][1] = localBasis[3][1];
	tmp[3][2] = localBasis[3][2];
	
	addVector3(tmp[0],tmp[3],tmp[0]);
    addVector3(tmp[1],tmp[3],tmp[1]);
    addVector3(tmp[2],tmp[3],tmp[2]);
	
	rotateVectorsArrayAroundNormAxis(tmp,4,dir,p,tmp,ca,sa);
    	
	subVector3(tmp[0],tmp[3],tmp[0]);
    subVector3(tmp[1],tmp[3],tmp[1]);
    subVector3(tmp[2],tmp[3],tmp[2]);
	
	localBasis[0][0] = tmp[0][0];
    localBasis[1][0] = tmp[0][1];
    localBasis[2][0] = tmp[0][2];

    localBasis[0][1] = tmp[1][0];
    localBasis[1][1] = tmp[1][1];
    localBasis[2][1] = tmp[1][2];

    localBasis[0][2] = tmp[2][0];
    localBasis[1][2] = tmp[2][1];
    localBasis[2][2] = tmp[2][2];

	localBasis[3][0] = tmp[3][0];
	localBasis[3][1] = tmp[3][1];
	localBasis[3][2] = tmp[3][2];

}

void particleSystem::moveX(float f){
	localBasis[3][0] += localBasis[0][0]*f;
	localBasis[3][1] += localBasis[1][0]*f;
	localBasis[3][2] += localBasis[2][0]*f;

	invertBasis[3][0] = localBasis[3][0];
	invertBasis[3][1] = localBasis[3][1];
	invertBasis[3][2] = localBasis[3][2];
}

void particleSystem::moveY(float f){ 
	localBasis[3][0] += localBasis[0][1]*f;
	localBasis[3][1] += localBasis[1][1]*f;
	localBasis[3][2] += localBasis[2][1]*f;

	invertBasis[3][0] = localBasis[3][0];
	invertBasis[3][1] = localBasis[3][1];
	invertBasis[3][2] = localBasis[3][2];
}

void particleSystem::moveZ(float f){
	localBasis[3][0] += localBasis[0][2]*f;
	localBasis[3][1] += localBasis[1][2]*f;
	localBasis[3][2] += localBasis[2][2]*f;

	invertBasis[3][0] = localBasis[3][0];
	invertBasis[3][1] = localBasis[3][1];
	invertBasis[3][2] = localBasis[3][2];
}

void particleSystem::moveV(Vector3 v){
	localBasis[3][0] += v[0];
	localBasis[3][1] += v[1];
	localBasis[3][2] += v[2];

	invertBasis[3][0] = localBasis[3][0];
	invertBasis[3][1] = localBasis[3][1];
	invertBasis[3][2] = localBasis[3][2];
}

void particleSystem::moveV(float x,float y,float z){
	localBasis[3][0] += x;
	localBasis[3][1] += y;
	localBasis[3][2] += z;

	invertBasis[3][0] = localBasis[3][0];
	invertBasis[3][1] = localBasis[3][1];
	invertBasis[3][2] = localBasis[3][2];
}

bool particleSystem::checkTrainSmokeParams() { 
	
	if( this->trainSmokePars.dirDelta == FLT_MAX ) return false;
	if( this->trainSmokePars.smokeSize1 == FLT_MAX ) return false;
	if( this->trainSmokePars.smokeSize1Delta == FLT_MAX ) return false;
	if( this->trainSmokePars.smokeSize2 == FLT_MAX ) return false;
	if( this->trainSmokePars.smokeSize2Delta == FLT_MAX ) return false;
	if( this->trainSmokePars.smokeSize3 == FLT_MAX ) return false;
	if( this->trainSmokePars.smokeSize3Delta == FLT_MAX ) return false;
	if( this->trainSmokePars.smokeSpeed1 == FLT_MAX ) return false;
	if( this->trainSmokePars.smokeSpeed1Delta == FLT_MAX ) return false;
	if( this->trainSmokePars.smokeSpeed2 == FLT_MAX ) return false;
	if( this->trainSmokePars.smokeSpeed2Delta == FLT_MAX ) return false;
	if( this->trainSmokePars.smokeSize1 == FLT_MAX ) return false;
	if( this->trainSmokePars.smokeSize1Delta == FLT_MAX ) return false;
	if( this->trainSmokePars.smokeSize2 == FLT_MAX ) return false;
	if( this->trainSmokePars.smokeSize2Delta == FLT_MAX ) return false;
	if( this->trainSmokePars.smokeTime1 == FLT_MAX ) return false;
	if( this->trainSmokePars.smokeTime1Delta == FLT_MAX ) return false;
	if( this->trainSmokePars.smokeTime2 == FLT_MAX ) return false;
	if( this->trainSmokePars.smokeTime2Delta == FLT_MAX ) return false;

	return true;
}
*/