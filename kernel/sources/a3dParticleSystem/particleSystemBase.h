#ifndef PARTICLE_SYSTEM_INC
#define PARTICLE_SYSTEM_INC

#include "..\\kernel.h"
#include "..\\vecmat.h"
#include "..\\a3dModel\\a3dModel.h"
#include "..\\landscape.h"
#include "..\\a3dDependency\\a3dDependencyProperty.h"
#include "..\\a3dDependency\\a3dDependencyPropertyOwner.h"
#include "particleTypes.h"
#include "serialization.h"
#include "memoryFile.h"
#include "..\\headerCodes.h"

typedef struct
{
	Vector3					pos;			// ��������� �������
	Vector2					tex0;			// ���������� ��������
	Vector2					shiftVector;	// ������� + ���� ������ ���� ������� � �������
	static const	DWORD	FVF;			// fvf �������
} particleVertexBase;

class a3dParticleInfoBase { 
public:	
	Vector3 pos;
	
	a3dParticleInfoBase() { 
		setVector3( this->pos, 0.0f, 0.0f, 0.0f );
	}
};

class a3dTextureDesc;
class a3dEffectDesc;
class particleSystemBase : public a3dDependencyPropertyOwner { 
	Matrix4						originBasis;
protected:
	int							*partIndices;		// ������������� ������� ������
public:
	shString					name;				
	Kernel						*partKernel;		// ����
	camera						*currCamera;		// ������� ������
	
	LPDIRECT3DVERTEXBUFFER9		lpVertexBuffer;		// ������ ������
	LPDIRECT3DINDEXBUFFER9		lpIndexBuffer;		// ������ ��������
	shString					effectFileName;		// ���� �������
	shString					textureFileName;	// ���� ��������
	a3dEffectDesc				*effectDesc;		// ������ ������
	a3dTextureDesc				*textureDesc;		// ��������
		
	
	bool						doNotGenerate;		// ���� �� ����� ������ ��������	
	bool						useLocalBasis;		// ������������ ��������� ����� ��� ����������
	Matrix4						localBasis;			// �����
	Matrix4						invertBasis;		// ��������������� �����
	
	int							particleCount;			// ����� ������
	int							renderParticleCount;	// ����� ������ ������� ���������� �����������
	int							maxRenderParticleCount;	// ������������ ����� ��������� ������� ����� ����������� �� 1 ���
	a3dParticleInfoBase			*particles;	

	float						fTime;					// ������� �����
	
	// dependency properties
	a3dDependencyProperty		*doNotGenerateProperty;
	a3dDependencyProperty		*useLocalBasisProperty;
	a3dDependencyProperty		*particleCountProperty;
	a3dDependencyProperty		*timeProperty;
	a3dDependencyProperty		*maxRenderParticleCountProperty;
	a3dDependencyProperty		*positionXProperty;
	a3dDependencyProperty		*positionYProperty;
	a3dDependencyProperty		*positionZProperty;

	particleSystemBase() { 
		this->initializeDefault();
	}

	~particleSystemBase() { this->clear(); }

	virtual DWORD __a3d_fastcall	getParticleFVF();
	virtual int __a3d_fastcall		getVertexSize();

	void					setKernel( Kernel *krn ) { this->partKernel = krn; }
	void					setEffect( a3dEffectDesc* desc) { this->effectDesc = desc; }
	void					setTexture( a3dTextureDesc *texDesc ) { this->textureDesc = texDesc; }
	void					setCamera( camera *cam ) { this->currCamera = cam; }

	void					correct();

	HRESULT	__a3d_fastcall  createDynamicBuffers( int pCount );		// ������� ������������ �������
	HRESULT __a3d_fastcall  createBuffers( int pCount );			// ������ ������ ������������ � ������ �������� ���
	HRESULT	__a3d_fastcall	openVertexBuffer( void **ptr );
	HRESULT __a3d_fastcall	closeVertexBuffer();
	HRESULT	__a3d_fastcall  openIndexBuffer( WORD **ptr );
	HRESULT	__a3d_fastcall	closeIndexBuffer();

	virtual a3dParticleInfoBase* createParticles( int count ) { return new a3dParticleInfoBase[count]; }
	
	virtual void					initializeDefault();
	virtual HRESULT	__a3d_fastcall	initialize();
	virtual HRESULT	__a3d_fastcall	clear();
	virtual void					updateParticles(DWORD dwTime);
	virtual HRESULT	__a3d_fastcall	updateBuffer();
	virtual void					fillBuffer( BYTE *ptr );
	virtual HRESULT	__a3d_fastcall	prepareEffect( );
	virtual HRESULT	__a3d_fastcall	render( );
	virtual HRESULT	__a3d_fastcall	renderCore( );

	virtual void					registerDependencyProperties();
	shString&						getName() { return this->name; }


	// serialization
	virtual int					calcParticleSizeCore();
	int							calcSizeCore();
	virtual bool				loadParticleDataFromMemoryInternal( a3dMemoryFile &file );
	virtual bool				saveParticleDataToMemoryInternal( a3dMemoryFile &file );
	bool						loadFromMemoryCore( a3dMemoryFile &file );
	bool						saveToMemoryCore( a3dMemoryFile &file );
	int							getHaderCode() { return a3dHeaders::particleSystemBaseHeader; }

};

/*
// ����������� ������
typedef struct __particle
{
	
	Vector3			currPos;		// ������� ��������� �������
	Vector3			currSpeed;		// ������� �������� �������
	Vector3			currVel;		// ��������� �������
	Vector3			beginPos;		// ��������� ������� �������
	Vector3			beginSpeed;		// ��������� �������� �������
	
	Vector4			color;			// �������������� ����
	Vector4			color1;			// ���� �������
	Vector4			color2;			// ���� ��������� �������
	Vector4			color3;			// ���� ������� �������
	Vector4			dcolor;			// ���������� �����

	DWORD			dwColor;		// �������������� ����
	float			creationTime;	// ����� �������� �������
	float			currentTime;	// ������� �����
	float			prevTime;		// ���������� �����
	float			size;			// ������ ��������
	int				currentState;	// ��������� �������
	
	float			time1;		
	float			time2;
	float			time3;
	float			time4;
	float			time5;
		
	float			size1;
	float			size2;
	float			size3;
	float			size4;

	float			dsize1;
	float			dsize2;
	float			dsize3;
	float			dsize4;

	bool			busy;			// ���� ������� ��� ������, �� 1 ���� �������� �� 0
	bool			blink;			// ��������
	float			val1;			

}particle;

typedef struct { 
		
	float		dirDelta;				// ���������� �����������
	float		smokeSpeed1;			// �������� 1
	float		smokeSpeed1Delta;		// ���������� ��������
	float		smokeTime1;				// ����� ������ ���� �� �����	
	float		smokeTime1Delta;		// ����������
	float		smokeSize1;				// ������ ���� � ������
	float		smokeSize1Delta;		// ���������� ������� ���� � ������
	float		smokeSpeed2;			// �������� 2
	float		smokeSpeed2Delta;		// ���������� �������� 2
	float		smokeTime2;				// ����� �� ������� ��� ������������
	float		smokeTime2Delta;		// ���������� �������
	float		smokeSize2;				// �������� ������ ����
	float		smokeSize2Delta;		// ���������� ��������� ������� ����
	float		smokeSize3;				// �������� ������ ����
	float		smokeSize3Delta;		// ���������� ��������� ������� ����
	int			particleIndex;			// ��� ���������� ��������

} a3dTrainSmokeParams;

// ��� �����, ��������
// ������ ��� ����������� ������ ��� ������������ ���������
typedef struct { 
	int				texturesCount;			// ����� �������
	DWORD			textureColor;			// ������ ���� �� ����� ������������� ��������
	Texture			*textures;				// ������ ������
	shString		fileName;				// ��� ���� � ��������� ������
	a3dLandscape	*land;					// ��������
} a3dLandscapeEffectParams;

class particleSystem
{
	Matrix4		originBasis;
public:
	bool		smoothParticles;
	bool		useLocalBasis;		// ������������ �� ��������� �����
	bool		started;			// ���� ��������� ������
	float		particleSize;		// ������ �������
	float		particleSizeMin;	// ����������� ������ �������
	
	// ��� ����� ��������� ������
	DWORD		dwColor1;			// ���� ������
	DWORD		dwColor2;		
	DWORD		dwColor3;			

	// ��� ��������� ������������ ��� ��������� �������� ������
	// ��� �������� �������
	Vector3		e0BeginSpeed;		// ��������� �������� � ����������� ������
	Vector3		e0Vel;				// ��������� ��� ������ �������
	Vector3		e1Vel;

	float		e0ExpTime;			// ����� ������ ������
	float		e0ExpSpeed;			// �������� � ������� ����� ������ ���������� �������
	float		e0ExpSpeedDelta;	// ���������� ��������
	float		e0BlinkTime;		// ����� �������
	float		e0BlinkTimeDelta;	// ���������� �� ������� �������� ������� ������� ��� ������ �������
	float		e0FadeTime;			// ����� �������
	float		e0FadeTimeDelta;	
	float		e0EndTime;			// ����� ������ �������
	float		e0EndTimeDelta;		// ����������

	float		e0BlinkPeriod;		// ����� �������
	float		e0BlinkPeriodDelta;	// ����������

	float		e1Speed;			// �������� � ������� ����� ������ ���������� �������
	float		e1SpeedDelta;		// ���������� ��������
	float		e1BlinkTime;		// ����� �������
	float		e1BlinkTimeDelta;	// ���������� �� ������� �������� ������� ������� ��� ������ �������
	float		e1FadeTime;			// ����� �������
	float		e1FadeTimeDelta;	
	float		e1EndTime;			// ����� ������ �������
	float		e1EndTimeDelta;		// ����������

	float		e1BlinkPeriod;		// ����� �������
	float		e1BlinkPeriodDelta;	// ����������

	a3dTrainSmokeParams				trainSmokePars;	// ��������� ������� ����������� ����
	a3dLandscapeEffectParams		landscapePars;	// ��������� ��� ���������

	// ���������� ��� �������
	Vector3		e2dot;				// ����� ���������� ���
	Vector3		e2dir;				// ����������� ��� - ������ ����������
	Vector3		e2speed;			// ����������� �������� � ������ �������
	Vector3		e2vel;				// ���������
	float		e2from;				// ������ ����� ��������� ������
	float		e2to;				// ����� ����� �������� ������
	float		e2blinkTime;		// ����� �������� �������
	float		e2blinkTimeDelta;	// ���������� �������� �������
	float		e2fadeTime;			// ����� �������� �������
	float		e2fadeTimeDelta;	// ���������� ������� �������� �������
	float		e2dieTime;			// ����� ������ �������
	float		e2dieTimeDelta;		// ���������� ������ �������
	float		e2blinkPeriod;		// ����� �������� ������
	float		e2blinkPeriodDelta;	// ���������� �������� �������� ������
	bool		doNotGenerate;

	// ���������� � ��������
	a3dParticleType					effectType;			// ��� �������
	
	DWORD		partCount;						// �������
	
	particle	*particles;						// �������
	particle	*freeParticles;					// ��������� �������
	
	Kernel		*partKernel;					// ����
	LPDIRECT3DVERTEXBUFFER9		lpVertexBuffer;	// ������ ������
	LPDIRECT3DINDEXBUFFER9		lpIndexBuffer;	// ������ ��������
	LPD3DXEFFECT				lpEffect;		// ������ ������
	Texture						*texture;		// ��������1
		
	Matrix4		localBasis;						// �����
	Matrix4		invertBasis;					// ��������������� �����
	
	Object		*lpObj;

	particleSystem()
	{
		ZeroMemory( this, sizeof( particleSystem ) );	
		localBasis[0][0] = localBasis[1][1] = localBasis[2][2] = localBasis[3][3] = 
		invertBasis[0][0] = invertBasis[1][1] = invertBasis[2][2] = invertBasis[3][3] = 1.0f;
		originMatrix4( originBasis );
		this->useLocalBasis = true;
		this->smoothParticles = true;
		this->started = false;
	}

	~particleSystem()
	{
		clear();
	}
	
	void setKernel( Kernel *krn )
	{
		partKernel = krn;
	}

	void setEffect( LPD3DXEFFECT lpEffect ) { this->lpEffect = lpEffect; }
	void setTexture( Texture *tex ) { this->texture = tex; }

	HRESULT initialize( DWORD pCount );
	HRESULT clear();	
	HRESULT startSalut();
	HRESULT startType1();
	HRESULT startMagicWand();
	HRESULT startTrainSmoke();
	HRESULT	startLandscapeParticle();
	HRESULT start();
	HRESULT update( DWORD dwTime );
	HRESULT updateSalut( DWORD dwTime );
	HRESULT updateType1( DWORD dwTime );
	HRESULT	updateMagicWand( DWORD dwTime );
	HRESULT updateTrainSmoke( DWORD dwTime );
	HRESULT	updateLandscape( DWORD dwTime );
	HRESULT renderLandscapeParticle( camera *cam );
	void	generateTrainSmokeParticle( particle *p, float fTime );
	void	generateLandscapeParticle( particle *p );
	HRESULT initializeLandscapeParticle( );

	HRESULT correct();
	HRESULT render( camera *cam );

	// ��� �� ���� �������� �����
	HRESULT getDataFromObject( Object *obj );	// �������� ������ 
    
	// ��� �� ���� �������������� ������
	void	getObjectBasis();
	void	setObject( Object *obj ){ this->lpObj = obj; }

	bool	isBusy();

	void    position(Vector3 v);
	void	position(float x,float y,float z);

	void    moveX(float);
	void	moveY(float);
	void	moveZ(float);
	void	moveV(Vector3);
	void	moveV(float,float,float);
	
	void    rotateAroundX(float angle);
	void    rotateAroundY(float angle);
	void    rotateAroundZ(float angle);

	void    rotateAroundV(Vector3 v,float angle);
	void    rotateAroundA(Vector3 p,Vector3 a,float angle);

	void	rotateHead(float angle);
	
	void	scale(float fx,float fy,float fz);
	void    origin();
	
	bool	checkTrainSmokeParams();
};

void	interpolateColor( Vector4 color1, Vector4 color, Vector4 dcolor, float scale );
*/
#endif