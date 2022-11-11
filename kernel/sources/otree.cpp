#include "otree.h"

void a3dOcTreeManager::clear() { 

	if( this->root != null ) {
		this->root->clear(); 
		delete this->root;
		this->root = null;
	}

	if( this->endNodeList != null ) 
		delete[] this->endNodeList;
	this->endNodeList = null;
	this->endNodeListCount = 0;
	this->passedNodeCount = 0;
}

void a3dOcTree::deleteNode( a3dOcTree *node ) { 
	
	if( node == null ) return;
	node->clear();
	delete node;
}

void a3dOcTree::clear() { 
	
	if( this->bottomLeftNear != null ) this->deleteNode( this->bottomLeftNear );
	if( this->bottomLeftFar != null ) this->deleteNode( this->bottomLeftFar );
	if( this->bottomRightFar != null ) this->deleteNode( this->bottomRightFar );
	if( this->bottomRightNear != null ) this->deleteNode( this->bottomRightNear );
	if( this->topLeftNear != null ) this->deleteNode( this->topLeftNear );
	if( this->topLeftFar != null ) this->deleteNode( this->topLeftFar );
	if( this->topRightFar != null ) this->deleteNode( this->topRightFar );
	if( this->topRightNear != null ) this->deleteNode( this->topRightNear );
 	
	this->bottomLeftNear = null;
	this->bottomLeftFar = null;
	this->bottomRightFar = null;
	this->bottomRightNear = null;
	this->topLeftNear = null;
	this->topLeftFar = null;
	this->topRightFar = null;
	this->topRightNear = null;

	this->clearBindingList();
}

void a3dOcTree::createBindingList() { 

	this->clearBindingList();
	this->bindingList = new std::list<void*>();

	return ;
}

void a3dOcTree::clearBindingList() { 

	if( this->bindingList != null ) 
		delete this->bindingList;
	this->bindingList = null;

	return ;
}

bool a3dOcTree::addObjectToBindingListIfContains( void *obj, Vector3 ta, Vector3 tb, Vector3 tc ) { 

	if( !isTriangleIntersectsCube( ta, tb, tc, this->a, this->b ) ) return false;
	
	if( this->bottomLeftNear != null ) { 

		this->bottomLeftNear->addObjectToBindingListIfContains( obj, ta, tb, tc );
		this->bottomLeftFar->addObjectToBindingListIfContains( obj, ta, tb, tc );
		this->bottomRightFar->addObjectToBindingListIfContains( obj, ta, tb, tc );
		this->bottomRightNear->addObjectToBindingListIfContains( obj, ta, tb, tc );
		this->topLeftNear->addObjectToBindingListIfContains( obj, ta, tb, tc );
		this->topLeftFar->addObjectToBindingListIfContains( obj, ta, tb, tc );
		this->topRightFar->addObjectToBindingListIfContains( obj, ta, tb, tc );
		this->topRightNear->addObjectToBindingListIfContains( obj, ta, tb, tc );

		return true;
	}
	
	if( this->bindingList == null ) 
		this->createBindingList();
	this->bindingList->push_back( obj );
	
	return true;
}

bool a3dOcTree::isVisible() { 
	
	if( this->calcId == this->manager->getCalcId() ) return visible;
	if( this->manager->currCamera == null ) return true;
	
	a3dOcTree *node = this->parent;
	while( node != null ) { 
		if( node->calcId != this->manager->getCalcId() ) { // если не нашли подсчитанный блок
			node->visible = false;
			node->calcId = this->manager->getCalcId();
			node = node->parent;
			continue;
		}
		else break;
	}

	this->visible = false;
	this->calcId = this->manager->getCalcId();

	return false;
}

void a3dOcTree::swapVertices( Vector3 a, Vector3 b ) { 
	
	float tmp;
	
	if( a[2] < b[2] ) { tmp = a[2]; a[2] = b[2]; b[2] = tmp; }
	if( a[0] > b[0] ) { tmp = a[0]; a[0] = b[0]; b[0] = tmp; }
	if( a[1] < b[1] ) { tmp = a[1]; a[1] = b[1]; b[1] = tmp; }
}

void a3dOcTreeManager::initialize( Vector3 a, Vector3 b, int levelCount ) {

	if( this->root == null ) this->root = new a3dOcTree( this );
	this->root->initialize( a, b, levelCount );
	
	this->endNodeListCount = 1;
	int lc = levelCount;
	while( lc > 0 ) { 
		this->endNodeListCount *= 8;
		lc --;
	}

	this->endNodeList = new a3dOcTree*[ this->endNodeListCount ];
	this->passedNodeCount = 0;

	return;
}

void a3dOcTree::initialize( Vector3 av, Vector3 bv, int levelCount ) { 
	
	// обязательно упорядочим вершины
	this->swapVertices( av, bv );
	
	copyVector3( av, this->a );
	copyVector3( bv, this->b );

	if( levelCount == 0 ) return;

	Vector3 na, nb;
	
	setVector3( na, this->a[0], this->half( this->b[1], this->a[1] ), this->half( this->b[2], this->a[2] ) );
	setVector3( nb, this->half( this->a[0], this->b[0] ), this->b[1], this->b[2] );
	this->bottomLeftNear = new a3dOcTree( this->manager );
	this->bottomLeftNear->initialize( na, nb, levelCount - 1 );

	setVector3( na, this->a[0], this->half( this->b[1], this->a[1] ), this->a[2] );
	setVector3( nb, this->half( this->a[0], this->b[0] ), this->b[1], this->half( this->b[2], this->a[2] ) );
	this->bottomLeftFar = new a3dOcTree( this->manager );
	this->bottomLeftFar->initialize( na, nb, levelCount - 1 );

	setVector3( na, this->half( this->a[0], this->b[0] ), this->half( this->b[1], this->a[1] ), this->a[2] );
	setVector3( nb, this->b[0], this->b[1], this->half( this->b[2], this->a[2] ) );
	this->bottomRightFar = new a3dOcTree( this->manager );
	this->bottomRightFar->initialize( na, nb, levelCount - 1 );

	setVector3( na, this->half( this->a[0], this->b[0] ), this->half( this->b[1], this->a[1] ), this->half( this->b[2], this->a[2] ) );
	setVector3( nb, this->b[0], this->b[1], this->b[2] );	
	this->bottomRightNear = new a3dOcTree( this->manager );
	this->bottomRightNear->initialize( na, nb, levelCount - 1 );
	
	setVector3( na, this->a[0], this->a[1], this->half( this->b[2], this->a[2] ) );
	setVector3( nb, this->half( this->a[0], this->b[0] ), this->half( this->b[1], this->a[1] ), this->b[2] );
	this->topLeftNear = new a3dOcTree( this->manager );
	this->topLeftNear->initialize( na, nb, levelCount - 1 );
	
	setVector3( na, this->a[0], this->a[1], this->a[2] );
	setVector3( nb, this->half( this->a[0], this->b[0] ), this->half( this->b[1], this->a[1] ), this->half( this->b[2], this->a[2] ) );
	this->topLeftFar = new a3dOcTree( this->manager );
	this->topLeftFar->initialize( na, nb, levelCount - 1 );

	setVector3( na, this->half( this->a[0], this->b[0] ), this->a[1], this->a[2] );
	setVector3( nb, this->b[0], this->half( this->b[1], this->a[1] ), this->half( this->b[2], this->a[2] ) );
	this->topRightFar = new a3dOcTree( this->manager );
	this->topRightFar->initialize( na, nb, levelCount - 1 );

	setVector3( na, this->half( this->a[0], this->b[0] ), this->a[1], this->half( this->b[2], this->a[2] ) );
	setVector3( nb, this->b[0], this->half( this->b[1], this->a[1] ), this->b[2] );	
	this->topRightNear = new a3dOcTree( this->manager );
	this->topRightNear->initialize( na, nb, levelCount - 1 );

	return;
}

void a3dOcTreeManager::update(camera *cam) { 
	
	this->calcId ++;
	this->currCamera = cam;
	this->root->update();
}

void a3dOcTreeManager::updateByRay(Vector3 point, Vector3 dir) { 
	this->calcId ++;
	this->root->updateByRay( point, dir );
}

bool a3dOcTree::contains( Vector3 point ) { 

	if( point[0] >= a[0] && point[0] <= b[0] && 
		point[1] >= b[1] && point[1] <= a[1] && 
		point[2] >= b[2] && point[2] <= a[2] ) return true;
	return false;
}

bool a3dOcTree::isIntersectsRay( Vector3 point, Vector3 dir ) { 

	if( this->contains( point ) ) 
		return true;

	Vector3 planeNormal, intPoint;
	
	// left plane planePoint = a
	setVector3( planeNormal, -1.0f, 0.0f, 0.0f );
	if( calcRay3IntersectionPoint( point, dir, a, planeNormal, intPoint) ) { // intersected
		if( intPoint[1] >= b[1] && intPoint[1] <= a[1] && 
			intPoint[2] >= b[2] && intPoint[2] <= a[2] ) return true;
	}

	// rightPlane planePoint = b
	setVector3( planeNormal, 1.0f, 0.0f, 0.0f );
	if( calcRay3IntersectionPoint( point, dir, b, planeNormal, intPoint) ) { // intersected
		if( intPoint[1] >= b[1] && intPoint[1] <= a[1] && 
			intPoint[2] >= b[2] && intPoint[2] <= a[2] ) return true;
	}

	// back plane planePoint = a
	setVector3( planeNormal, 0.0f, 0.0f, 1.0f );
	if( calcRay3IntersectionPoint( point, dir, a, planeNormal, intPoint) ) { // intersected
		if( intPoint[0] >= a[0] && intPoint[0] <= b[0] && 
			intPoint[1] >= b[1] && intPoint[1] <= a[1] ) return true;
	}

	// front plane planePoint = b
	setVector3( planeNormal, 0.0f, 0.0f, -1.0f );
	if( calcRay3IntersectionPoint( point, dir, b, planeNormal, intPoint) ) { // intersected
		if( intPoint[0] >= a[0] && intPoint[0] <= b[0] && 
			intPoint[1] >= b[1] && intPoint[1] <= a[1] ) return true;
	}

	// top plane planePoint = a
	setVector3( planeNormal, 0.0f, 1.0f, 0.0f );
	if( calcRay3IntersectionPoint( point, dir, a, planeNormal, intPoint) ) { // intersected
		if( intPoint[0] >= a[0] && intPoint[0] <= b[0] && 
			intPoint[2] >= b[2] && intPoint[2] <= a[2] ) return true;
	}

	// bottom plane planePoint = b
	setVector3( planeNormal, 0.0f, -1.0f, 0.0f );
	if( calcRay3IntersectionPoint( point, dir, b, planeNormal, intPoint) ) { // intersected
		if( intPoint[0] >= a[0] && intPoint[0] <= b[0] && 
			intPoint[2] >= b[2] && intPoint[2] <= a[2] ) return true;
	}

	return false;
}

void a3dOcTree::updateByRay( Vector3 point, Vector3 dir ) { 
	
	if( this->isIntersectsRay( point, dir ) )
		this->visible = true;
	else {
		this->visible = false;
		return;
	}
	
	// если еще есть куда опускаться
	if( this->bottomLeftNear != null ) { 
	
		this->bottomLeftNear->updateByRay( point, dir );
		this->bottomLeftFar->updateByRay( point, dir );
		this->bottomRightFar->updateByRay( point, dir );
		this->bottomRightNear->updateByRay( point, dir );
		this->topLeftNear->updateByRay( point, dir );
		this->topLeftFar->updateByRay( point, dir );
		this->topRightFar->updateByRay( point, dir );
		this->topRightNear->updateByRay( point, dir );
	}
	this->calcId = this->manager->getCalcId();
	
	return ;
}

void a3dOcTree::update() { 
	
	if( this->manager->currCamera == null ) return;
		
	int res = this->manager->currCamera->isBoxInFrustum( this->a, this->b );
	if( res == OUT_FRUSTUM ) { 
		this->visible = false;
		return;
	}
	
	// если еще есть куда опускаться
	if( this->bottomLeftNear != null ) { 
	
		this->bottomLeftNear->update();
		this->bottomLeftFar->update();
		this->bottomRightFar->update();
		this->bottomRightNear->update();
		this->topLeftNear->update();
		this->topLeftFar->update();
		this->topRightFar->update();
		this->topRightNear->update();
	}

	this->calcId = this->manager->getCalcId();

	return ;
}

void a3dOcTree::updatePassedNodeList() { 

	if( !this->visible ) return ;
	
	if( this->bottomLeftNear != null ) { 
		this->bottomLeftNear->updatePassedNodeList();
		this->bottomLeftFar->updatePassedNodeList();
		this->bottomRightFar->updatePassedNodeList();
		this->bottomRightNear->updatePassedNodeList();
		this->topLeftNear->updatePassedNodeList();
		this->topLeftFar->updatePassedNodeList();
		this->topRightFar->updatePassedNodeList();
		this->topRightNear->updatePassedNodeList();
		
		return ;
	}
	
	this->manager->endNodeList[ this->manager->passedNodeCount ] = this;
	this->manager->passedNodeCount ++;

	return ;
}

void a3dOcTreeManager::updatePassedNodeList() { 
	
	this->passedNodeCount = 0;
	this->root->updatePassedNodeList();	

	return ;
}

bool a3dOcTreeManager::addObjectToBindingListIfContains( void *obj, Vector3 a, Vector3 b, Vector3 c ) {

	if( this->root == null ) return false;

	return this->root->addObjectToBindingListIfContains( obj, a, b, c );
}