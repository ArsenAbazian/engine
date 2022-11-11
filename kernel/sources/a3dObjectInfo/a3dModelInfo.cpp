#include "a3dModelInfo.h"
#include "a3dModelObjectInfo.h"
#include "..\\a3dTextFileTokens.h"
#include "..\\a3dDesc\\a3dModelDesc.h"
#include "..\\a3dModelRef\\a3dModelRef.h"
#include "..\\a3dModel\\a3dModel.h"
#include "..\\a3dModel\\a3dObject.h"
#include "memoryFile.h"

void a3dModelInfo::initializeDefault() {
	
	this->model = null;
	this->desc = null;
	this->objectInfo = null;
	this->objectInfoCount = 0;
}

void a3dModelInfo::clear() { 
	if( this->objectInfo != null )
		delete[] this->objectInfo;
	this->objectInfoCount = 0;
	this->desc = null;
	this->model = null;
}

bool a3dModelInfo::readFromLexemeList(std::list<scriptLexeme*> &lexList, std::list<scriptLexeme*>::iterator &li) { 

	if( !a3dVisualObjectInfoBase::readFromLexemeList( lexList, li ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !readStringParam( lexList, li, a3dTextFileTokens::a3dfileNameString, this->fileName ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, a3dTextFileTokens::a3dEndString ) ) return false;

	if( !safeNextLex( lexList, li ) ) return false;
	if( !isToken( li, a3dTextFileTokens::a3dModelString ) ) return false;

	return true;
}

a3dModelInfo* a3dModelInfo::clone() { 

	a3dModelInfo *res = new a3dModelInfo();

	a3dVisualObjectInfoBase::copyTo( (a3dVisualObjectInfoBase*) res );
	res->fileName = this->fileName;
	res->fullPathName = this->fullPathName;
	res->model = this->model;

	return res;
}

void a3dModelInfo::initialize( a3dModelDesc *desc ) { 

	this->desc = desc;
	this->model = this->desc->model;
	this->fullPathName = desc->fullPathName;
	this->fileName = desc->fileName;

	if(this->objectInfo == null) {
		this->objectInfoCount = this->desc->model->objCount;
		if(this->objectInfoCount != 0)
			this->objectInfo = new a3dObjectInfo[this->objectInfoCount];
	}
	for( int i = 0; i < this->objectInfoCount; i++ ) { 
		this->objectInfo[i].initialize(&desc->model->object[i]);
	}
}

int a3dModelInfo::calcSizeCore() { 

	int objSize = 0;
	for(int i = 0; i < this->objectInfoCount; i++) 
		objSize += this->objectInfo[i].calcSize();
	
	return a3dVisualObjectInfoBase::calcSizeCore() + this->fileName.calcSize() + sizeof(int) + objSize;
}

bool a3dModelInfo::saveToMemoryCore(a3dMemoryFile &file) { 

	if( !a3dVisualObjectInfoBase::saveToMemoryCore( file ) ) return false;
	if( !this->fileName.saveToMemory( file ) ) return false;
	if( !file.writeInt(this->objectInfoCount) ) return false;
	for(int i = 0;i < this->objectInfoCount; i++) {
		if(!this->objectInfo[i].saveToMemory(file)) return false;
	}

	return true;
}

bool a3dModelInfo::loadFromMemoryCore(a3dMemoryFile &file) { 

	if( !a3dVisualObjectInfoBase::loadFromMemoryCore( file ) ) return false;
	if( !this->fileName.loadFromMemory( file ) ) return false;
	if( !file.readInt( & this->objectInfoCount ) ) return false;

	if(this->objectInfoCount == null) return true;
	this->objectInfo = new a3dObjectInfo[this->objectInfoCount];
	for( int i = 0; i < this->objectInfoCount; i++ ) {
		if( !this->objectInfo[i].loadFromMemory(file) ) return false;
	}

	return true;
}

a3dModelRef* a3dModelInfo::createReference() { 
	
	a3dModelRef *mref = new a3dModelRef( this ); 
	this->initializeReference( mref );
	return mref;
}

void a3dModelInfo::initializeReference(a3dModelRef *modelRef) {
	
	modelRef->info = this;
	modelRef->desc = this->desc;
	
	copyMatrix4(this->basis, modelRef->basis);
	modelRef->scaleX = this->scaleX;
	modelRef->scaleY = this->scaleY;
	modelRef->scaleZ = this->scaleZ;
	modelRef->name = this->name;

	for(int i = 0; i < this->objectInfoCount; i++) { 
		this->objectInfo[i].initializeReference( modelRef->objectRef[i] );
	}
}

void a3dModelInfo::assign(a3dModelRef *modelRef) { 
	
	copyMatrix4( modelRef->basis, this->basis );
	this->name = modelRef->name;
	this->scaleX = modelRef->scaleX;
	this->scaleY = modelRef->scaleY;
	this->scaleZ = modelRef->scaleZ;
	
	for( int i = 0; i < modelRef->objCount; i++ ) { 
		this->objectInfo[i].assign(modelRef->objectRef[i]);
	}
}