#ifndef A3D_TEXTURE_INFO
#define A3D_TEXTURE_INFO

#include "..\\texture.h"
#include "a3dObjectInfoBase.h"

class a3dTextureInfo : public a3dObjectInfoBase { 
public:
	
	a3dTextureInfo() { this->texture = null; } 
	~a3dTextureInfo();
	Texture			*texture;
	int				getHeaderCode() { return a3dHeaders::a3dTextureInfoHeader; }
}; 

#endif