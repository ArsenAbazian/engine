#ifndef A3D_TEXTURE_DESC_INC
#define A3D_TEXTURE_DESC_INC

#include "a3dDescBase.h"
#include "3dtypes.h"

class Texture;
class a3dScene;

class a3dTextureDesc : public a3dObjectDescBase { 
public:
	shString		fullPathName;
	shString		fileName;
	Texture			*texture;

	a3dTextureDesc() { 
		this->texture = null;
	}
	~a3dTextureDesc();

	int		getHeaderCount() { return a3dHeaders::a3dTextureDescHeader; }
	int		calcSizeCore();
	bool	loadFromMemoryCore( a3dMemoryFile &file );
	bool	saveToMemoryCore( a3dMemoryFile &file );

	HRESULT	loadTexture( a3dScene *scene );
};

#endif