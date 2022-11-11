#include "a3dGraphNode.h"
#include "memoryFile.h"


void a3dGraphNode::initializeDefault() { 

	clearVector2(p1);
	clearVector2(p2);
	clearVector2(p3);
	clearVector2(p4);

	this->a = 0.0f;
	this->b = 0.0f;
	this->c = 0.0f;

	this->t = 0.0f;
	this->t2 = 0.0f;
	this->t3 = 0.0f;

	this->k = 0.0f;
}

void a3dGraphNode::clear() {  
	
	this->initializeDefault();
}

void a3dGraphNode::setType( a3dGraphNodeType nodeType ) { 

	this->type = nodeType;
	this->calcParams();
}

void a3dGraphNode::calcBezieParams() { 
	
	this->a = (-p1[1] + 3.0f * (p3[1] - p4[1]) + p2[1]);
	this->b = (-6.0f * p3[1] + 3.0f * (p1[1] + p4[1]));
	this->c = 3.0f * (p3[1] - p1[1]);
}

void a3dGraphNode::calcLineParams() { 
	
	this->k = ( this->p2[1] - this->p1[1] ) / ( this->p2[0] - this->p1[0] );	
}

void a3dGraphNode::calcParams() { 
	
	if( this->type == GNT_LINE )
		this->calcLineParams();
	else if( this->type == GNT_BEZIE )
		this->calcBezieParams();
}

void a3dGraphNode::setPoints(Vector2 pp1, Vector2 pp2) { 
	
	copyVector2(pp1, p1);
	copyVector2(pp2, p2);

	this->updateLocalTime();
	
	this->calcParams();
} 

void a3dGraphNode::setPoints(Vector2 pp1, Vector2 pp2, Vector2 pp3, Vector2 pp4) { 

	this->setPoints(pp1, pp2);

	copyVector2(pp3, p3);
	copyVector2(pp4, p4);

	this->calcParams();
}

void a3dGraphNode::calcNode() { 
	
	if(this->type == GNT_LINE) 
		this->calcLine();
	else if(this->type == GNT_BEZIE)
		this->calcBezie();
}

int a3dGraphNode::calcSizeCore() { 

	return 	sizeof( int ) + 
			sizeof( k ) + 
			sizeof( p1 ) + 
			sizeof( p2 ) + 
			sizeof( p3 ) + 
			sizeof( p4 ) + 
			sizeof( result ) + 
			sizeof( bResult) +
			sizeof( splitNext );
}

bool a3dGraphNode::loadFromMemoryCore(a3dMemoryFile &file ) {  

	int tp;

	if( !file.readInt( &tp ) ) return false;
	this->type = (a3dGraphNodeType)tp;
	if( !file.readFloat( &this->k ) ) return false;
	if( !file.readFloatArray( this->p1, 2 ) ) return false;
	if( !file.readFloatArray( this->p2, 2 ) ) return false;
	if( !file.readFloatArray( this->p3, 2 ) ) return false;
	if( !file.readFloatArray( this->p4, 2 ) ) return false;
	if( !file.readFloat( &this->result ) ) return false;
	if( !file.readBool( &this->bResult ) ) return false;
	if( !file.readBool( &this->splitNext ) ) return false;

	this->setPoints(p1, p2, p3, p4);
	this->calcParams();

	return true;
}

bool a3dGraphNode::saveToMemoryCore(a3dMemoryFile &file) { 

	int tp = (int) this->type;
	if( !file.writeInt( tp ) ) return false;
	if( !file.writeFloat( this->k ) ) return false;
	if( !file.writeFloatArray( this->p1, 2 ) ) return false;
	if( !file.writeFloatArray( this->p2, 2 ) ) return false;
	if( !file.writeFloatArray( this->p3, 2 ) ) return false;
	if( !file.writeFloatArray( this->p4, 2 ) ) return false;
	if( !file.writeFloat( this->result ) ) return false;
	if( !file.writeBool( this->bResult ) ) return false;
	if( !file.writeBool( this->splitNext ) ) return false;
	
	return true;
}