#ifndef A3D_LIGHTMAP_INFO
#define A3D_LIGHTMAP_INFO

#include "hstring.h"
#include "headerCodes.h"
#include "memoryFile.h"

class a3dLightMapInfo : public a3dSerializableObject {
public:
	bool		enabled;
	shString	lightMapTextureParamName;
	shString	lightMapFileName;
	int			lightMapWidth;
	int			lightMapHeight;
	int			textureCoorIndex;
	bool		isAtlasTextureCoords;
	float		u0;
	float		v0;
	float		uSize;
	float		vSize;

	a3dLightMapInfo() { this->initializeDefault(); }

	void			initializeDefault();
	
	int				getHeaderCode() { return a3dHeaders::a3dLightMapInfoHeader; }
	int				calcSizeCore();
	bool			saveToMemoryCore( a3dMemoryFile &file );
	bool			loadFromMemoryCore( a3dMemoryFile &file );
	
	void			assign( a3dLightMapInfo &lm );
};

#endif