#ifndef A3D_OCTREE_INC
#define A3D_OCTREE_INC

#include "vecmat.h"
#include "ncamera.h"
#include "3dtypes.h"
#include <list>

class a3dOcTreeManager;

class a3dOcTree { 
	int					calcId;			// идентификатор расчета для кеширования
	a3dOcTreeManager	*manager;		// менеджер
public:
	std::list< void* >	*bindingList;	// в этом списке будут храниться обеъкты которые принадлежат данному кубу
	Vector3				a;				// вершины однозначно задают параллелепипед
	Vector3				b;

	a3dOcTree			*parent;
	
	a3dOcTree			*bottomLeftNear;
	a3dOcTree			*bottomLeftFar;
	a3dOcTree			*bottomRightFar;
	a3dOcTree			*bottomRightNear;
	a3dOcTree			*topLeftNear;
	a3dOcTree			*topLeftFar;
	a3dOcTree			*topRightFar;
	a3dOcTree			*topRightNear;

	bool				visible;

	a3dOcTree( a3dOcTreeManager *manager ) { 
		ZeroMemory( this, sizeof( a3dOcTree ) ); 
		this->manager = manager;
	}
	~a3dOcTree() { ZeroMemory( this, sizeof( a3dOcTree ) ); }
	bool		isVisible();
	void		clear();
	void		deleteNode( a3dOcTree	*node );
	void		swapVertices( Vector3 a, Vector3 b );
	void		initialize( Vector3 a, Vector3 b, int level );
	void		update();
	bool		contains( Vector3 point );
	bool		isIntersectsRay( Vector3 point, Vector3 dir );
	void		updateByRay( Vector3 point, Vector3 dir );
	float		half( float fa, float fb ) { return fa + ( fb - fa ) * 0.5f; }

	void		createBindingList();
	void		clearBindingList();
	bool		addObjectToBindingListIfContains( void *obj, Vector3 ta, Vector3 tb, Vector3 tc );
	void		updatePassedNodeList();	
};

class a3dOcTreeManager { 
	int			calcId;
public:
	int			levelCount;
	a3dOcTree	*root;
	camera		*currCamera;

	a3dOcTree	**endNodeList;			// list of endnodes
	int			endNodeListCount;		
	int			passedNodeCount;		// update passedNodeCount;

	a3dOcTreeManager() {
		this->calcId = 0;
		this->levelCount = 0;
		this->currCamera = null;
		this->root = new a3dOcTree( this );
		this->endNodeList = null;
		this->endNodeListCount = 0;
		this->passedNodeCount = 0;
	}
	~a3dOcTreeManager() { this->clear(); }

	int			getCalcId() { return calcId; }
	void		clear();													// очистка ресурсов
	void		initialize( Vector3 a, Vector3 b, int levelCount );			// инициализация
	int			sizeToLevel( float size, float endSize );
	void		update( camera *cam );
	void		updateByRay( Vector3 point, Vector3 dir );					// проверяем на пересечение с лучом
	void		updatePassedNodeList();										// обновляет список нодов которые прошли проверку

	bool		addObjectToBindingListIfContains( void *obj, Vector3 a, Vector3 b, Vector3 c );
};

#endif