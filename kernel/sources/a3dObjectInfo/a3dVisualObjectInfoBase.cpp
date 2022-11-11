#include "a3dVisualObjectInfoBase.h"
#include "memoryFile.h"
#include "..\\a3dTextFileTokens.h"

bool a3dVisualObjectInfoBase::readFromLexemeList(std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 

	// skip type
	if( !safeNextLex( lexList, li ) ) return false;
	// skip class info
	if( !safeNextLex( lexList, li ) ) return false;
	// name
	if( !readStringValue( li, this->name ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readMatrixParam( lexList, li, a3dTextFileTokens::a3dMeshTransformString, this->basis ) ) return false;

	return true;
}

void a3dVisualObjectInfoBase::copyTo( a3dVisualObjectInfoBase *obj ) { 

	a3dObjectInfoBase::copyTo( (a3dObjectInfoBase*)obj );
	copyMatrix4( this->basis, obj->basis );
	obj->scaleX = this->scaleX;
	obj->scaleY = this->scaleY;
	obj->scaleZ = this->scaleZ;
}

int a3dVisualObjectInfoBase::calcSizeCore() { 

	return a3dObjectInfoBase::calcSizeCore() + sizeof( Matrix4 ) + sizeof(float) * 3;
}

bool a3dVisualObjectInfoBase::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dObjectInfoBase::saveToMemoryCore( file ) ) return false;
	if( !file.writeMatrix4( this->basis ) ) return false;
	if( !file.writeFloat( this->scaleX ) ) return false;
	if( !file.writeFloat( this->scaleY ) ) return false;
	if( !file.writeFloat( this->scaleZ ) ) return false;

	return true;
}

bool a3dVisualObjectInfoBase::loadFromMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dObjectInfoBase::loadFromMemoryCore( file ) ) return false;
	if( !file.readMatrix4( this->basis ) ) return false;
	if( !file.readFloat( &this->scaleX ) ) return false;
	if( !file.readFloat( &this->scaleY ) ) return false;
	if( !file.readFloat( &this->scaleZ ) ) return false;

	return true;
}