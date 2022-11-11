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

// ������� ������
typedef struct {
	Vector3		pos;
	Vector3		normal;
	Vector2		tex;
} treeVertex;

// ���������� � ����� ������
typedef struct {
public:
	shString			textureName;		// ��� ��������
	LPDIRECT3DTEXTURE9	lpTexture;			// ��������
	D3DMATERIAL9		material;			// ��������
	int					vertexBeginPos;		// ��������� ��������
	int					indexBeginPos;
	int					vertexCount;
	int					indexCount;
} treePartInfo;

class a3dTreeRef;

// � ��� �� ����� ��� ������ � ����� �������� ������
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
	
	shString		fileName;		// ��� ����� - �� ��� �� ������ ������
	shString		name;			// ��� ������ - ��� ������
	a3dTreeType		type;			// ��� ������ ��� ��� ������� ������

	int				vertexBeginPos;	// ������ 0 ������� � ����� ������� ������
	int				indexBeginPos;	// ������ 0 ������� � ����� ������� ��������
	int				vertexCount;	// ����� ������
	int				indexCount;		// ����� ��������
	int				partsCount;		// ����� ������ ������
	treeVertex		*vertex;		// ������ ������
	WORD			*index;			// ������ ��������
	treePartInfo	*parts;			// ����� ������ �� ��� 0 - ��������� � ������ 1 + 2 * n - ����� 2 + 2 * n ������ �� ������
	
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
	a3dTree				*tree;							// ��������� �� ������
	Matrix4				basis;							// ����� ������
	treePartInfoRef		*partRefs[ maxPartCount ];		// ������ ������
	
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
	LPD3DXEFFECT				lpEffect;			// ������
	LPDIRECT3DVERTEXBUFFER9		vertexBuffer;		// ������ ������
	LPDIRECT3DINDEXBUFFER9		indexBuffer;		// ������ ��������
	int							treesCount;			// ����� ������������ ��������
	int							treeRefsCount;		// ����� ������ �� �������
	
	int							vertexCount;		// ����� ����� ������
	int							indexCount;			// ����� ����� ��������
	
	void						initSeed();
	int							randPartCount(int maxPartCount);
	std::list< std::list< treePartInfoRef* >* >	renderGraph;
	
	HRESULT						renderGraphNodes();		// �������� ������� 

public:
	Kernel						*tkrn;							// ����
	shString					effectFileName;

	static const int			maxTreesCount = 64;
	static const int			maxTreeRefsCount = 1024;
	static const DWORD			FVF_TREE = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	
	a3dTree*					trees[maxTreesCount];			// ����� �������� 64 ��������� ������� ��������	
	a3dTreeRef*					treeRefs[maxTreeRefsCount];		// ����� �������� 1024 ��������� ������ �� ������� � ����
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

	void			clear();		// ������� ������� � ���������� �������
	void			clearRenderGraph();	// ������� ���� ����������
	a3dTree*		addTree(shString &fileName, shString &name, a3dTreeType type);	// ��������� ������ �� ����� ���������� ����
	a3dTreeRef*		addTreeRef( int treeIndex, int maxPartsCount );
	void			setCamera( camera *cam ) { this->currCamera = cam; }

	void			calcVertexIndexCount();	// ������� ����� ������ � ��������
	HRESULT			initialize();		// �� ��������� �������� �������������� ������� ������
	HRESULT			save();				// �� �������� �������������� ������ �������
	HRESULT			restore(){ return this->initialize(); }	// ��������������� �������
	void			buildRenderGraph(); // ������ ���� �� �������� ����� ��������� ��� �����������
	HRESULT			render();
	HRESULT			createEffect( shString &fileName );		// ��������� ������
	HRESULT			createEffect();
	void			clearEffect();		// ������� �������
};

#endif