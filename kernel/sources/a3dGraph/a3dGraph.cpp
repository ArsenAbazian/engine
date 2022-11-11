#include "a3dGraph.h"
#include "memoryFile.h"

void a3dGraph::initializeDefault() { 
	
	this->type = GT_FLOAT;
	this->nodeCount = 0;
	ZeroMemory(this->nodeList, sizeof(a3dGraphNode*) * this->maxNodeCount);

	setVector3( this->color, 0.0f, 0.0f, 0.0f );
	this->lineType = 0;
	this->width = 1.0f;
}

void a3dGraph::clear() { 

	for(int i = 0; i < this->nodeCount; i++) { 
		if( this->nodeList[i] == null ) continue;
		delete this->nodeList[i];
		this->nodeList[i] = null;
	}

	this->type = GT_FLOAT;
	this->name.clear();
	this->nodeCount = 0;
}

int a3dGraph::calcSizeCore() { 
	
	int size =  sizeof(this->type) + this->name.calcSize() +  sizeof( nodeCount ) + sizeof( Vector3 ) + sizeof( int ) + sizeof( float );
	for( int i = 0; i < this->nodeCount; i++ ) { 
		size += this->nodeList[i]->calcSize();
	}

	return size;
}

bool a3dGraph::loadFromMemoryCore(a3dMemoryFile &file) { 

	int tp;
	if( !this->name.loadFromMemory(file) ) return false;
	if( !file.readInt(&tp) ) return false;
	this->type = (a3dGraphType)tp;
	if( !file.readFloatArray( this->color, 3 ) ) return false;
	if( !file.readInt( &this->lineType ) ) return false;
	if( !file.readFloat( &this->width ) ) return false;
	if( !file.readInt( &this->nodeCount ) ) return false;
	
	for( int i = 0; i < this->nodeCount; i++ ) { 
		this->nodeList[ i ] = new a3dGraphNode();
		file.savePos();
		if( !this->nodeList[ i ]->loadFromMemory(file) ) return false;
		if( file.delta() != this->nodeList[ i ]->calcSize() ) return false;
	}

	return true;
}

bool a3dGraph::saveToMemoryCore(a3dMemoryFile &file) {  

	if( !this->name.saveToMemory(file) ) return false;
	if( !file.writeInt((int)this->type) ) return false;
	if( !file.writeFloatArray(this->color, 3) ) return false;
	if( !file.writeInt( this->lineType ) ) return false;
	if( !file.writeFloat( this->width ) ) return false;
	if( !file.writeInt( this->nodeCount ) ) return false;

	for( int i = 0; i < this->nodeCount; i++ ) { 
		if( !this->nodeList[ i ]->saveToMemory(file) ) return false;
	}

	return true;
}

float a3dGraph::calcFloatValue(float t) { 
	
	if( this->nodeCount == 0 ) return 0.0f;
	if( t > this->getLength() ) return this->nodeList[this->nodeCount - 1]->p2[1];

	for( int i = 0; i < this->nodeCount; i++ ) { 
		if( t >= this->nodeList[i]->p1[0] && t <= this->nodeList[i]->p2[0] ) { 
			this->nodeList[i]->setTime(t);
			this->nodeList[i]->calcNode();
			return this->nodeList[i]->result;
		}
	} 
	
	return this->nodeList[0]->p1[1];
}

void a3dGraph::addNode() { 
	
	if( this->nodeCount >= maxNodeCount ) return;
	this->nodeList[this->nodeCount] = new a3dGraphNode();
	this->nodeCount++;
}

void a3dGraph::removeNode(int index) { 
	if( index >= this->nodeCount )
		index = this->nodeCount - 1;

	delete this->nodeList[index];
	for(int i = index; i < this->nodeCount - 1; i++) { 
		this->nodeList[i] = this->nodeList[i+1];
	}
}