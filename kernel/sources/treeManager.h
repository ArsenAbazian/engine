#ifndef TREE_MANAGER_INC
#define TREE_MANAGER_INC

#include "kernel.h"
#include "hstring.h"
#include "vecmat.h"
#include "n3dsfile.h"
#include "mathUtils.h"
#include "ncamera.h"
#include <d3dx9effect.h>

typedef enum { TREE_SIMPLE, TREE_SPRUCE } a3dTreeType;

// вершина дерева
typedef struct {
	Vector3		pos;
	Vector3		normal;
	Vector2		tex;
} treeVertex;

// информация о части дерева
typedef struct {
public:
	shString			textureName;		// имя текстуры
	LPDIRECT3DTEXTURE9	lpTexture;			// текстура
	D3DMATERIAL9		material;			// материал
	int					vertexBeginPos;		// говорящие названия
	int					indexBeginPos;
	int					vertexCount;
	int					indexCount;
} treePartInfo;

class a3dTreeRef;

// а вот из таких вот частей и будет состоять дерево
class treePartInfoRef {
public:	
	a3dTreeRef	*treeRef;
	Matrix4		basis;
	int			treePartIndex;

	treePartInfoRef() { 
		originMatrix4( basis ); 
		this->treePartIndex = 0;
		this->treeRef = null;
	}

	~treePartInfoRef() { }
};

class a3dTreeManager;

class a3dTree {
public:	
	a3dTreeManager	*manager;		
	
	shString		fileName;		// имя файла - ну так на всякий случай
	shString		name;			// имя дерева - для поиска
	a3dTreeType		type;			// тип дерева ель или обычное дерево

	int				vertexBeginPos;	// начало 0 вершине в общем буффере вершин
	int				indexBeginPos;	// начало 0 индекса в общем буффере индексов
	int				vertexCount;	// число вершин
	int				indexCount;		// число индексов
	int				partsCount;		// число частей дерева
	treeVertex		*vertex;		// буффер вершин
	WORD			*index;			// буффер индексов
	treePartInfo	*parts;			// части дерева из них 0 - основание а дальше 1 + 2 * n - ствол 2 + 2 * n листья на стволе
	
	a3dTree( a3dTreeManager *manager ) { 
		this->manager = manager;
		this->vertexBeginPos = 0;
		this->indexBeginPos = 0;
		this->vertexCount = 0;
		this->indexCount = 0;
		this->vertex = null;
		this->index = null;
		this->parts = null;
	} 

	~a3dTree() { this->clear(); }

	bool	initialize( shString &fileName, shString &name, a3dTreeType type );
	void	clear();
	void	restore( treeVertex *vertexBuffer, WORD *indexBuffer, int vertexIndex, int indexIndex );
	void	save( treeVertex *vertexBuffer, WORD *indexBuffer, int vertexIndex, int indexIndex  );
	void	calcVertexIndexCount( File3DS &f3ds );
	void	fillBuffers( File3DS &f3ds );
	void	copyVertices( treePartInfo *part, File3DS &f3ds, int objectIndex );
};

class a3dTreeRef { 
	int					partRefCount;
	int					partsCount;
	int					randPart();
	void				initSeed();
public:	
	static const int	maxPartCount = 64;
	a3dTree				*tree;							// указатель на дерево
	Matrix4				basis;							// базис дерева
	treePartInfoRef		*partRefs[ maxPartCount ];		// массив частей
	
	a3dTreeRef() {  
		this->partsCount = 0;
		this->partRefCount = 0;
		this->tree = null;
		originMatrix4( basis );
		ZeroMemory( this->partRefs, sizeof( treePartInfoRef* ) * maxPartCount );
	}

	~a3dTreeRef() { this->clear(); }

	void			clear();
	void			initialize( a3dTree *tree, int partsCount );
	int				getPartRefCount() { return this->partRefCount; }
};

class a3dTreeManager {
	LPD3DXEFFECT				lpEffect;			// эффект
	LPDIRECT3DVERTEXBUFFER9		vertexBuffer;		// буффер вершин
	LPDIRECT3DINDEXBUFFER9		indexBuffer;		// буффер индексов
	int							treesCount;			// число оригинальных деревьев
	int							treeRefsCount;		// число ссылок на деревья
	
	int							vertexCount;		// общее число вершин
	int							indexCount;			// общее число индексов
	
	void						initSeed();
	int							randPartCount(int maxPartCount);
	std::list< std::list< treePartInfoRef* >* >	renderGraph;
	
	HRESULT						renderGraphNodes();		// рендерим деревья 

public:
	Kernel						*tkrn;							// ядро
	shString					effectFileName;

	static const int			maxTreesCount = 64;
	static const int			maxTreeRefsCount = 1024;
	static const DWORD			FVF_TREE = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	
	a3dTree*					trees[maxTreesCount];			// всего возможно 64 различных моделей деревьев	
	a3dTreeRef*					treeRefs[maxTreeRefsCount];		// всего возможно 1024 различных ссылок на деревья в игре
	camera						*currCamera;
	
	a3dTreeManager( Kernel *tkrn ) { 
		this->initSeed();
		this->lpEffect = null;
		this->tkrn = tkrn;
		this->vertexBuffer = null;
		this->indexBuffer = null;
		this->treesCount = 0;
		this->treeRefsCount = 0;
		this->vertexCount = 0;
		this->indexCount = 0;
		ZeroMemory(trees, sizeof(a3dTree*) * maxTreesCount);
		ZeroMemory(treeRefs, sizeof(a3dTreeRef*) * maxTreeRefsCount);
	}
	~a3dTreeManager() { this->clear(); }

	void			clear();		// очищаем буфферы и уничтожаем массивы
	void			clearRenderGraph();	// очищаем граф рендеринга
	a3dTree*		addTree(shString &fileName, shString &name, a3dTreeType type);	// добавляем дерево из файла указанного типа
	a3dTreeRef*		addTreeRef( int treeIndex, int maxPartsCount );
	void			setCamera( camera *cam ) { this->currCamera = cam; }

	void			calcVertexIndexCount();	// считают число вершин и индексов
	HRESULT			initialize();		// из указанных деревьев инициализирует буфферы вершин
	HRESULT			save();				// из буфферов инициализирует заново дереьвя
	HRESULT			restore(){ return this->initialize(); }	// восстанавливает буфферы
	void			buildRenderGraph(); // строит граф по которому будет рендерить для оптимизации
	HRESULT			render();
	HRESULT			createEffect( shString &fileName );		// создается эффект
	HRESULT			createEffect();
	void			clearEffect();		// очистка эффекта
};

#endif