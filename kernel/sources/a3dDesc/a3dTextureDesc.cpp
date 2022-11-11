#include "a3dTextureDesc.h"
#include "..\\texture.h"
#include "..\\a3dScene.h"

a3dTextureDesc::~a3dTextureDesc() { 
	if( this->texture != null )
		delete this->texture;
	this->texture = null;
}

int a3dTextureDesc::calcSizeCore() { 

	return a3dObjectDescBase::calcSizeCore() + this->fileName.calcSize(); 
}

bool a3dTextureDesc::saveToMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dObjectDescBase::saveToMemoryCore( file ) ) return false;
	if( !this->fileName.saveToMemory( file ) ) return false;

	return true;
}

bool a3dTextureDesc::loadFromMemoryCore( a3dMemoryFile &file ) { 

	if( !a3dObjectDescBase::loadFromMemoryCore( file ) ) return false;
	if( !this->fileName.loadFromMemory( file ) ) return false;

	return true;
}

HRESULT a3dTextureDesc::loadTexture( a3dScene *scene ) { 

	HRESULT rv;

	this->kernel = scene->kernel;
	this->fullPathName = this->fileName;
	this->fullPathName.appendPath( scene->textureDirectory );

	this->texture = new Texture();
	this->texture->kernel = scene->kernel;

	rv = this->texture->loadFromFile( this->fullPathName, true );
	if( FAILED( rv ) )
		return rv;

	return D3D_OK;
}