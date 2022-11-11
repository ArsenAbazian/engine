#ifndef _LANDSCAPE_INC
#define _LANDSCAPE_INC

#include "hstring.h"
#include "kernel.h"
#include "vecmat.h"
#include "ncamera.h"
#include "a3dModel\\a3dModel.h"
#include "lightMapCreator.h"
#include <d3dx9effect.h>

class a3dLandscape;
class landscapePatch;
class landMapPassInfo;
class landMapPassFlags;

// флаг вершины
static DWORD FVF_LAND = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_XYZB1;
static DWORD resetVertexEn_Unknown			= 0xffffffff;	// неизвестно включать вершину или нет
static DWORD resetVertexEn_NotInclude		= 0xfffffffe;	// проверено, не включать вершину
static DWORD resetVertexEn_Include			= 0xfffffffd;	// включить вершину

#define QUADRO_NEXT_LEVEL(level)			( ( level << 2 ) + 1 )
#define QUADRO_PREV_LEVEL(level)			( ( level - 1 ) >> 2 ) 

#define TOPVERTEXROW		0
#define LEFTVERTEXROW		1
#define BOTTOMVERTEXROW		2
#define RIGHTVERTEXROW		3

// описание вершины
typedef struct
{
	Vector3		position;
	float		deltaY;
	Vector2		tex0;
}landVertex;

// описание ссылки на вершину
typedef struct {
	int row;
	int col;
} vertexRef;

// этот класс показывает какие карты надо рендерить а какие нет
class landMapPassFlags { 
public:
	landscapePatch	*patch;
	bool			enabled[16];
	bool			maskCalculated[16];
	bool			maskEnabled[16];	

	landMapPassFlags() { ZeroMemory(this, sizeof(landMapPassFlags)); }
	bool			isEnabled(int passIndex) { return enabled[passIndex]; }
	HRESULT			calcPassEnabled(int passIndex, landMapPassInfo *info);
	HRESULT			calcPassEnabled();
	HRESULT			calcMaskEnabled(int passIndex, landMapPassInfo *info);
	HRESULT			initialize(landscapePatch *patch);
	void			setPatch(landscapePatch	*patch) { this->patch = patch; }
};

class a3dLightMapPlaneCollection;
class a3dEffectDesc;
class a3dTextureDesc;
class a3dScene;

class a3dLandscape : public a3dLightMapCreator
{
public:
	a3dScene						*scene;
	
	DWORD							maxIndexBufferSize;		// размер буффера индекса
	DWORD							maxVertexBufferSize;	// размер буффера вершин
	
	shString						heightMapFileName;
	shString						effectFileName;			// имя файла эффекта
	a3dTextureDesc					*lightMapTextureDesc;	// карта освещенности
	int								lightMapWidth;						
	int								lightMapHeight;

	Kernel							*lkrn;					// ядро
	a3dEffectDesc					*effectDesc;
	LPD3DXEFFECT					lpEffect;				// эффект

	int								passCount;				// число материалов
	landMapPassInfo					*passInfo[16];			// максимальное число прохождений 16!

	TCHAR							tempBuffer[512];		// буффер для вывода
		
	float							pixelError;				// ошибка в пикселях

	int								mapSize;				// размер карты высот
	float							sideScale;				// масштаб по линии
	float							heightScale;			// масштаб по высоте
	float							landSize;				// длина стороны квадрата ландшафта
	landVertex						*heightLine;			// массив высот
	landVertex						**heightMap;			// эмуляция двумерного массива высот
	
	int								patchCount;				// число кусков ландшафта
	int								quadCountPerPatchSide;	// число квдратов на кусок
	int								varianceCount;			// глубина просчета отклонений
	float							patchSize;				// длина квадрата куска ландшафта
	landscapePatch					**patches;				// двумерный массив кусков ландшафта
		
	Vector3							distFromCamera;
	Vector3							cameraPos;				// позиция камеры на ландшафте
	float							cameraH;

	Matrix4							basis;					// базис ландшафта
	Matrix4							invertBasis;			// инвертирующий базис

	camera							*currCamera;			// текущая камера
	int								cameraPatchRow;			//
	int								cameraPatchCol;			// строка и столбец куска в котором расположена камера
	int								cameraRow;				// строка и столбец вершины
	int								cameraCol;				

	int								renderPatchCount;		// вспомогательная переменная показывает число отрендеренных патчей
	bool							enableTesselation;		// Разрешить запретить обновление тесселяции
	bool							forceMaxDetailCurrentPatch;	// для патча в котором находится камера установить максимальный уровень тесселяции

	a3dLandscape();											// конструктор
	~a3dLandscape();										// деструктор

	void					setPixelError( float pe ){ this->pixelError = pe; }
	bool					isCameraInLand() { return this->cameraPatchCol >= 0 && this->cameraPatchRow >= 0 && this->cameraPatchCol < this->patchCount && this->cameraPatchRow < this->patchCount; }

	void					setTextureCoors();
	bool					initializeHeightMap( int	offsetRow, int offsetCol,			// строка и столбец левого верхнего угла на карте
												 int	size,								// размер ландшафта
												 float  upperLeftX, float upperLeftY, float upperLeftZ,	// координаты левого верхнего угла  
												 float	fScale,								// коэффициент масштабирования
												 float	fHeightScale,
												 float	u0, float v0, float u1, float v1,	// координаты текстуры
												 float	**map,								// сама карта высот
												 int	patchPerSize );						// инициализация ландшафта 
			
	HRESULT					loadHeightMap( shString &heightFile, float sideScale, float heightScale, int patchPerSize );
	HRESULT					loadHeightMap( TCHAR *heightFile, float sideScale, float heightScale, int patchPerSize );
	
	void					clear();

	// функции directx
	void					setScene( a3dScene *sc ) { this->scene = sc; }
	void					setKernel( Kernel *krn ){ this->lkrn = krn; }
	void					setCamera( camera *cam ){ this->currCamera = cam; }
	void					setLightMapTexture( a3dTextureDesc *texDesc ) { this->lightMapTextureDesc = texDesc; }

	// сбасывает флаги вершин
	// функция
	// в будущем надо оптимизировать
	// используя строковые комманды ассемблера
	void					linkPatches();
	void					tesselate();					// оптимизируем ландшафт
	void					prepareConvValues();			// подготовка матриц для оптимизации
	int						calcDetalizationLevelCount();	// считаем число уровней детализации

	// рендерим
	HRESULT					render();						

	// рисуем ограничивающие объемы
	HRESULT					drawBoundBox( a3dModel &cube );
	HRESULT					drawBoundSphere( a3dModel &sphere );

	HRESULT					createEffect();	// создание эффекта
	void					clearEffect();	// очистка эффекта
	void					addPass( landMapPassInfo * pass);  
	void					insertPass( int index, landMapPassInfo *pass );
	void					removePass( int index );
	void					calcPassEnabled(landMapPassInfo * pass);
	void					calcPassEnabled(int index);
	void					calcMaskEnabled(landMapPassInfo * pass);
	void					calcMaskEnabled(int index);
	int						getPassIndex(landMapPassInfo * pass);
		
	void					offsetCameraByHeight( float deltaHeight );
	void					updateObjectYByLand( Vector3 v );
	void					getIntersectYFromPlane( Vector3 v, Vector3 p0, Vector3 p1, Vector3 p2 );
	landscapePatch*			getPatch( float x, float z );

	// for lightMap creation
	int						getLightMapCount();
	a3dLightMap**			getLightMaps();
	void					fillLightMapPlaneColl( a3dLightMapPlaneCollection *coll );
	a3dLightMapPlaneCollection* getLightMapPlaneColl();
	int						getPlaneCount();
	void					calcNormal(int row, int col, Vector3 v);
};

//#define TEST_PATCH 
// описание класса - куска поверхности состоящиего из двух треугольников
class landscapePatch
{
protected:	
	LPDIRECT3DVERTEXBUFFER9			vertexBuffer;			// массив вершин на рендеринг
	landVertex						*vertexPtr;				// указатель на буффер вершин
	
	LPDIRECT3DINDEXBUFFER9			indexBuffer;			// массив индексов вершин на рендеринг
	D3DFORMAT						indexFormat;			// формат индекса 32 или 16
	BYTE							*indexPtr;				// указатель на буффер индекса
	WORD							*wordIndexPtr;
	DWORD							*dwordIndexPtr;

	DWORD							vertexBufferSize;		// размер буффера индексов вершин
	DWORD							indexBufferSize;		// размер буффера индексов вершин
	DWORD							vertexIndex;			// индекс свободной вершины в массиве вершин
	DWORD							indexIndex;				// индекс свободного индекса

	int								deltaThis, deltaTop, deltaLeft, deltaRight, deltaBottom;
	int								prevTopCol;				// необходимо для generateRight
	bool __a3d_fastcall					shouldCalcDetalizationLevel();
	bool							buffersLocked;
public:
	landscapePatch					*topPatch;			// верхний сосед
	landscapePatch					*leftPatch;			// левый сосед
	landscapePatch					*bottomPatch;		// нижний сосед
	landscapePatch					*rightPatch;		// правый сосед
	
	int								*prevRowIndicies;	// индексы предыдущей строки

	int								rowIndex;			// индекс строки
	int								colIndex;			// индекс столбца
	
	int								beginRow;			// начальная строка
	int								beginCol;			// начальный столбец
	
	a3dLandscape					*land;				// указатель на класс ландшафта	
	landMapPassFlags				passInfo;			// информация

	float							*variance;			// массив левых значений отклонений
	float							**deltaMap;			//
	int								**vertexRefMap;		// массив ссылок на добавленные вершины
	float							updateValue;		// 

	
	int								detalizationLevel;		// уровень детализации конкретного патча, который должен быть
	int								realDetalizationLevel;	// текущий уровень детализации (пока происходит итерация)
	bool							update;					// изменился ли уровень детализации с предыдущего рендеринга

	Vector3							boundBox[8];		// ограничивающий куб
	Vector4							boundSphere;		// ограничивающая сфера
	bool							visible;			// а видим ли вообще данный кусок
	bool							levelUp;			// уровень наверх

	float							*error;

	
	landscapePatch();				// конструктор
	~landscapePatch();				// деструктор

	void							initialize( a3dLandscape *l, int row, int col, int rowInd, int colInd );
	HRESULT							initializeRenderBuffers();
	void							clearRenderBuffers();
	void							initializeDeltaMap();
	void							clearDeltaMap();
	void							initializeVertexRefMap();
	void							clearVertexRefMap();
	void							addBottomVertexRef(int col) {
#ifdef TEST_PATCH
										if( this->vertexRefMap[BOTTOMVERTEXROW][col - this->beginCol] > 0) {
											MessageBox(this->land->lkrn->hWnd, TEXT("addBottomVertexRef(int col) failed. has prev index"), TEXT(""), MB_OK); 								
										}
#endif
										this->vertexRefMap[BOTTOMVERTEXROW][col - this->beginCol] = vertexIndex - 1;
									}
	void							addBottomVertexRef(int col, int index) {
#ifdef TEST_PATCH
										if( this->vertexRefMap[BOTTOMVERTEXROW][col - this->beginCol] > 0) {
											MessageBox(this->land->lkrn->hWnd, TEXT("addBottomVertexRef(int col) failed. has prev index"), TEXT(""), MB_OK); 								
										}
#endif
										this->vertexRefMap[BOTTOMVERTEXROW][col - this->beginCol] = index;
									}
	void							addRightVertexRef(int row) {
#ifdef TEST_PATCH
										if( this->vertexRefMap[RIGHTVERTEXROW][row - this->beginRow] > 0) {
											MessageBox(this->land->lkrn->hWnd, TEXT("addRightVertexRef(int col) failed. has prev index"), TEXT(""), MB_OK); 								
										}
#endif
										this->vertexRefMap[RIGHTVERTEXROW][row - this->beginRow] = vertexIndex - 1;
									}
	void							addTopVertexRef(int col) {  
#ifdef TEST_PATCH
										if(this->vertexRefMap[TOPVERTEXROW][col - this->beginCol] > 0) { 
											MessageBox(this->land->lkrn->hWnd, TEXT("addTopVertexRef(int col) failed. has prev index"), TEXT(""), MB_OK); 
										}  
#endif
										this->vertexRefMap[TOPVERTEXROW][col - this->beginCol] = vertexIndex - 1;
									}
	void							addTopVertexRef(int col, int index) {  
#ifdef TEST_PATCH	
										if(this->vertexRefMap[TOPVERTEXROW][col - this->beginCol] > 0) { 
											MessageBox(this->land->lkrn->hWnd, TEXT("addTopVertexRef(int col, int index) failed. has prev index"), TEXT(""), MB_OK); 
										}
#endif
										this->vertexRefMap[TOPVERTEXROW][col - this->beginCol] = index;
									}
	void							addLeftVertexRef(int row) {  
#ifdef TEST_PATCH	
										if(this->vertexRefMap[LEFTVERTEXROW][row - this->beginRow] > 0) { 
											MessageBox(this->land->lkrn->hWnd, TEXT("addLeftVertexRef(int col) failed. has prev index"), TEXT(""), MB_OK); 
										}
#endif										
										this->vertexRefMap[LEFTVERTEXROW][row - this->beginRow] = vertexIndex - 1;
									
									}
	
	// открыть буффер вершин
	HRESULT							openVertexBuffer(){ this->vertexIndex = 0; return this->vertexBuffer->Lock( 0, 0, (void**) &this->vertexPtr, D3DLOCK_DISCARD ); }
	HRESULT							openVertexBuffer(bool bDiscard){ this->vertexIndex = 0; return this->vertexBuffer->Lock( 0, 0, (void**) &this->vertexPtr, bDiscard? D3DLOCK_DISCARD: 0 ); }
	// закрыть буффер вершин
	HRESULT							closeVertexBuffer(){ return this->vertexBuffer->Unlock(); }
	// открыть буффер индекса
	HRESULT							openIndexBuffer(){ 
														HRESULT rv; 
														this->indexIndex = 0; 
														rv = this->indexBuffer->Lock( 0, 0, (void**) &this->indexPtr, D3DLOCK_DISCARD ); 
														this->wordIndexPtr = (WORD*)this->indexPtr;
														this->dwordIndexPtr = (DWORD*)this->indexPtr;
														return rv; 
													}
	// закрыть буффер индекса
	HRESULT							closeIndexBuffer(){ return this->indexBuffer->Unlock(); }
	HRESULT __a3d_fastcall				openRenderBuffers();
	HRESULT	__a3d_fastcall				closeRenderBuffers();

	void							clear();
	void							testPatch();
	void							testPatchSoftwareBlend();
	void							calcUpdateFlag();

	void							calcVariance();
	float							calcVariance( int row0, int col0, int row1, int col1, bool shouldProcessLastRow, bool shouldProcessLastCol );
	int								getCountByLevel(int level);
	float							calcVariance(int level);
	float							getError(int r0, int c0, int r1, int c1);
	void							calcDelta();
	void							recurseCalcDelta( int leftRow, int leftCol, int sideSize, int node );
	
	void							getNearVertex( int *vertexRow, int *vertexCol );
	void							calcDetalizationLevel();
	float __a3d_fastcall				calcUpdateValue(int level);
	void							calcBoundVolume();
	void							calcMinMaxHeight( float *minHeight, float *maxHeight );
	void							setNeighbors( landscapePatch *top, landscapePatch *left, landscapePatch *bottom, landscapePatch *right );

	void							clearRow(int row) { memset(this->vertexRefMap[row], 0xff, (this->land->quadCountPerPatchSide + 1) << 4); }
	
	void							checkVisibility();
	HRESULT __a3d_fastcall				tesselate();
	void							testVertexRefIndices();
	void							clearVertexRefIndicies();
	void							calcBlendParams();
	void							calcBlendParams(int delta);
	void							copyBlendParams(int delta);
	HRESULT __a3d_fastcall				updateBlendVertByNeighboors();
	bool __a3d_fastcall					needUpdate();
	void							calcNeighboorDelta();
	void							generate32();
	void							generate16();
		
	HRESULT __a3d_fastcall				render(int passIndex);
	HRESULT __a3d_fastcall				drawBoundSphere( a3dModel &sphere );
	HRESULT	__a3d_fastcall				drawBoundBox( a3dModel &cube );

	void							addVertex( int row, int col )
									{		
#ifdef TEST_PATCH
										if( this->vertexPtr[ this->vertexIndex ].deltaY < -1000 || this->vertexPtr[ this->vertexIndex ].deltaY > 1000) {
											MessageBox(this->land->lkrn->hWnd, TEXT("addVertex deltaY failed"), TEXT(""), MB_OK);
										}
#endif
										memcpy( &this->vertexPtr[ this->vertexIndex ], &this->land->heightMap[ row ][ col ], sizeof( landVertex ) );
										this->vertexIndex ++;
									}
	
	void							addIndex32( int vIndex )
									{
										this->dwordIndexPtr[ this->indexIndex ] = vIndex;
										this->indexIndex ++;
									}

	void							addIndex16( int vIndex )
									{
										this->wordIndexPtr[ this->indexIndex ] = vIndex;
										this->indexIndex ++;
									}
	
	void							generateTopLeft32();
	void							generateTop32( int col1 );
	void							generateTopRight32();
	void							generateLeft32( int row1 );
	void							generateRight32( int row1 );
	void							generateBottomLeft32();
	void							generateBottom32( int col1 );
	void							generateBottomRight32();	

	void							generateSingle16();				// эта функция вызывается когда уровень тесселляции самый низкий
	void							generateTopLeft16();
	void							generateTop16( int col1 );
	void							generateTopRight16();
	void							generateLeft16( int row1 );
	void							generateRight16( int row1 );
	void							generateBottomLeft16();
	void							generateBottom16( int col1 );
	void							generateBottomRight16();	
	void							generateMiddle16( int row, int col )
									{
										this->addVertex( this->beginRow + row, this->beginCol + col );
										this->addIndex16( this->vertexIndex - 1 );
										this->addIndex16( this->prevRowIndicies[ col ] );
										this->prevTopCol = this->prevRowIndicies[ col ];
										this->prevRowIndicies[ col ] = this->vertexIndex - 1;
									}
	
	void							link16()
									{
										// необходимо добавить предыдущую вершину 
										// и индекс следующей вершины чтобы перейти к следующему стрипу
										this->addIndex16( this->wordIndexPtr[ this->indexIndex - 1 ] );
										this->addIndex16( this->vertexIndex );	
									}

};

class landMapPassInfo {  

	void setLandMapPassInfo( a3dLandscape *land, a3dTextureDesc *mapTexDesc, a3dTextureDesc *maskTexDesc, a3dTextureDesc *detTexDesc, float mapUSize, float mapVSize, float detUSize, float detVSize, float midDetDist, float maxDetDist, float maxDetAlpha );
	
	a3dTextureDesc			*maskTextureDesc;
public:	
	a3dLandscape			*land;
	
	a3dTextureDesc			*mapTextureDesc;
	a3dTextureDesc			*detailTextureDesc;
	
	float					mapUSize;
	float					mapVSize;
	float					detUSize;
	float					detVSize;
	
	float					minDetailDistance;
	float					maxDetailDistance;
	float					maxDetailAlphaValue;

	float					currentDetailAlphaValue;

	bool					enabled;
	bool					animated;

	a3dTextureDesc*			getMaskTexture() { return this->maskTextureDesc; }
	void					setMaskTexture( a3dTextureDesc *desc );

	landMapPassInfo( a3dLandscape *land, a3dTextureDesc *mapTexDesc, a3dTextureDesc *maskTexDesc, a3dTextureDesc *detTexDesc, float mapUSize, float mapVSize, float detUSize, float detVSize ) {
		this->setLandMapPassInfo( land, mapTexDesc, maskTexDesc, detTexDesc, mapUSize, mapVSize, detUSize, detVSize, 10.0f, 1.0f, 0.6f);
	}
	
	landMapPassInfo( a3dLandscape *land, a3dTextureDesc *mapTexDesc, a3dTextureDesc *maskTexDesc, a3dTextureDesc *detTexDesc, float mapUSize, float mapVSize, float detUSize, float detVSize, float minDetDist, float maxDetDist, float maxDetAlpha ) { 
		this->setLandMapPassInfo( land, mapTexDesc, maskTexDesc, detTexDesc, mapUSize, mapVSize, detUSize, detVSize, minDetDist, maxDetDist, maxDetAlpha);
	}
	
	void		calcCurrentDetailAlphaValue();
	bool		shouldRenderDetail() { return this->currentDetailAlphaValue > 0.0f; }
	int			__a3d_fastcall	getPassIndex();
	HRESULT		__a3d_fastcall beginPass();
	HRESULT		__a3d_fastcall endPass();
};

// загружаем карту высот из png файла, в котором градации серого цвета представлены тремя компонентами
// с одинаковым значеним r, g, b
bool	loadFloatMap( TCHAR *fileName, float ***map, int *size );

#endif