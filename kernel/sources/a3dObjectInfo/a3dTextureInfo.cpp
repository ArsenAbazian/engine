#include "a3dTextureInfo.h"

a3dTextureInfo::~a3dTextureInfo() { 
	if( this->texture != null ) 
		delete this->texture;
}