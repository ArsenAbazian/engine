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
	Vector3					pos;			// положение вершины
	Vector2					tex0;			// координаты текстуры
	Vector2					shiftVector;	// позиция + этот вектор дает вершину в шейдере
	static const	DWORD	FVF;			// fvf вершины
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
	int							*partIndices;		// упорядоченные индексы частиц
public:
	shString					name;				
	Kernel						*partKernel;		// ядро
	camera						*currCamera;		// текущая камера
	
	LPDIRECT3DVERTEXBUFFER9		lpVertexBuffer;		// буффер частиц
	LPDIRECT3DINDEXBUFFER9		lpIndexBuffer;		// буффер индексов
	shString					effectFileName;		// файл эффекта
	shString					textureFileName;	// файл текстуры
	a3dEffectDesc				*effectDesc;		// эффект частиц
	a3dTextureDesc				*textureDesc;		// текстура
		
	
	bool						doNotGenerate;		// если не хотим больше генерить	
	bool						useLocalBasis;		// использовать локальный базис или глобальный
	Matrix4						localBasis;			// базис
	Matrix4						invertBasis;		// инвертированный базис
	
	int							particleCount;			// число частиц
	int							renderParticleCount;	// число частиц которые необходимо отрендерить
	int							maxRenderParticleCount;	// максимальное число партиклов которое будет отрендерено за 1 раз
	a3dParticleInfoBase			*particles;	

	float						fTime;					// текущее время
	
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

	HRESULT	__a3d_fastcall  createDynamicBuffers( int pCount );		// создаем динамические буфферы
	HRESULT __a3d_fastcall  createBuffers( int pCount );			// буффер вершин динамический а буффер индексов нет
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
// определение частиц
typedef struct __particle
{
	
	Vector3			currPos;		// текущее положение частицы
	Vector3			currSpeed;		// текущая скорость частицы
	Vector3			currVel;		// ускорение частицы
	Vector3			beginPos;		// начальная позиция частицы
	Vector3			beginSpeed;		// начальная скорость частицы
	
	Vector4			color;			// результирующий цвет
	Vector4			color1;			// цвет частицы
	Vector4			color2;			// цвет затухания частицы
	Vector4			color3;			// цвет вспышки частицы
	Vector4			dcolor;			// приращение цвета

	DWORD			dwColor;		// результирующий цвет
	float			creationTime;	// время создания частицы
	float			currentTime;	// текущее время
	float			prevTime;		// предыдущее время
	float			size;			// размер частички
	int				currentState;	// состояние частицы
	
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

	bool			busy;			// если частица еще занята, то 1 если свободна то 0
	bool			blink;			// мерцание
	float			val1;			

}particle;

typedef struct { 
		
	float		dirDelta;				// отклонение направления
	float		smokeSpeed1;			// скорость 1
	float		smokeSpeed1Delta;		// отклонение скорости
	float		smokeTime1;				// время вылета дыма из трубы	
	float		smokeTime1Delta;		// отклонение
	float		smokeSize1;				// размер дыма в начале
	float		smokeSize1Delta;		// отклонение размера дыма в начале
	float		smokeSpeed2;			// скорость 2
	float		smokeSpeed2Delta;		// отлокнение скорости 2
	float		smokeTime2;				// время за которое дым рассеивается
	float		smokeTime2Delta;		// отклонение времени
	float		smokeSize2;				// конечный размер дыма
	float		smokeSize2Delta;		// отклонение конечного размера дыма
	float		smokeSize3;				// конечный размер дыма
	float		smokeSize3Delta;		// отклонение конечного размера дыма
	int			particleIndex;			// для внутренних расчетов

} a3dTrainSmokeParams;

// для травы, деревьев
// вообще для статических частиц все относительно ландшафта
typedef struct { 
	int				texturesCount;			// число текстур
	DWORD			textureColor;			// каждый цвет на карте соответсвтует текстуре
	Texture			*textures;				// массив тектур
	shString		fileName;				// или файл с позициями частиц
	a3dLandscape	*land;					// ландшафт
} a3dLandscapeEffectParams;

class particleSystem
{
	Matrix4		originBasis;
public:
	bool		smoothParticles;
	bool		useLocalBasis;		// использовать ли локальный базис
	bool		started;			// если произошел запуск
	float		particleSize;		// размер частицы
	float		particleSizeMin;	// минимальный размер частицы
	
	// это общие параметры частиц
	DWORD		dwColor1;			// цвет частиц
	DWORD		dwColor2;		
	DWORD		dwColor3;			

	// эти параметры используются для различных эффектов салюта
	// для нулевого эффекта
	Vector3		e0BeginSpeed;		// начальная скорость и направление салюта
	Vector3		e0Vel;				// ускорение для каждой частицы
	Vector3		e1Vel;

	float		e0ExpTime;			// время взрыва салюта
	float		e0ExpSpeed;			// скорость с которой после взрыва разлетятся частицы
	float		e0ExpSpeedDelta;	// отклонение скорости
	float		e0BlinkTime;		// время вспышки
	float		e0BlinkTimeDelta;	// отклонение от средней величины времени вспышки для каждой частицы
	float		e0FadeTime;			// время гашения
	float		e0FadeTimeDelta;	
	float		e0EndTime;			// время смерти частицы
	float		e0EndTimeDelta;		// отклонение

	float		e0BlinkPeriod;		// время мигания
	float		e0BlinkPeriodDelta;	// отклонение

	float		e1Speed;			// скорость с которой после взрыва разлетятся частицы
	float		e1SpeedDelta;		// отклонение скорости
	float		e1BlinkTime;		// время вспышки
	float		e1BlinkTimeDelta;	// отклонение от средней величины времени вспышки для каждой частицы
	float		e1FadeTime;			// время гашения
	float		e1FadeTimeDelta;	
	float		e1EndTime;			// время смерти частицы
	float		e1EndTimeDelta;		// отклонение

	float		e1BlinkPeriod;		// время мигания
	float		e1BlinkPeriodDelta;	// отклонение

	a3dTrainSmokeParams				trainSmokePars;	// параметры эффекта паровозного дыма
	a3dLandscapeEffectParams		landscapePars;	// параметры для ландшафта

	// информация для эффекта
	Vector3		e2dot;				// точка приложения оси
	Vector3		e2dir;				// направление оси - вектор нормирован
	Vector3		e2speed;			// направление скорости в момент времени
	Vector3		e2vel;				// ускорение
	float		e2from;				// начало линии генерации частиц
	float		e2to;				// конец линии генеации частиц
	float		e2blinkTime;		// время мерцания частицы
	float		e2blinkTimeDelta;	// отклонение мерцания частицы
	float		e2fadeTime;			// время угасания частицы
	float		e2fadeTimeDelta;	// отклоненеи времени угасания частицы
	float		e2dieTime;			// время смерти частицы
	float		e2dieTimeDelta;		// отклонение смерти частицы
	float		e2blinkPeriod;		// время мерцания частиц
	float		e2blinkPeriodDelta;	// отклонения времения мерцания частиц
	bool		doNotGenerate;

	// информация о частицах
	a3dParticleType					effectType;			// тип эффекта
	
	DWORD		partCount;						// частицы
	
	particle	*particles;						// частицы
	particle	*freeParticles;					// свободные частицы
	
	Kernel		*partKernel;					// ядро
	LPDIRECT3DVERTEXBUFFER9		lpVertexBuffer;	// буффер частиц
	LPDIRECT3DINDEXBUFFER9		lpIndexBuffer;	// буффер индексов
	LPD3DXEFFECT				lpEffect;		// эффект частиц
	Texture						*texture;		// текстура1
		
	Matrix4		localBasis;						// базис
	Matrix4		invertBasis;					// инвертированный базис
	
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

	// что то типа золотого дождя
	HRESULT getDataFromObject( Object *obj );	// получить данные 
    
	// что то типа развевающегося шлейфа
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