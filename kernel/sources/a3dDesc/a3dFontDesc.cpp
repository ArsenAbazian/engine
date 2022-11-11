#include "a3dFontDesc.h"
#include "..\\font.h"

a3dFontDesc::~a3dFontDesc() { 
	
	if( this->font != null )
		delete this->font;
}

int a3dFontDesc::calcSizeCore() { 

	return a3dObjectDescBase::calcSizeCore() + this->fileName.calcSize() + this->posFileName.calcSize(); 
}

bool a3dFontDesc::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dObjectDescBase::saveToMemoryCore( file ) ) return false;
	if( !this->fileName.saveToMemory( file ) ) return false;
	if( !this->posFileName.saveToMemory( file ) ) return false;

	return true;
}

bool a3dFontDesc::loadFromMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dObjectDescBase::loadFromMemoryCore( file ) ) return false;
	if( !this->fileName.loadFromMemory( file ) ) return false;
	if( !this->posFileName.loadFromMemory( file ) ) return false;

	return true;
}