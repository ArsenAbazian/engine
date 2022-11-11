#ifndef A3D_SOUND_DESC_INC
#define A3D_SOUND_DESC_INC

#include "a3dDescBase.h"
#include "3dtypes.h"

class GameSound;
class a3dScene;
class a3dSoundDesc : public a3dObjectDescBase {
public:
	shString		fullPathName;
	shString		fileName;
	GameSound		*sound;

	a3dSoundDesc() { 
		this->sound = null;
	}
	~a3dSoundDesc();

	int		getHeaderCount() { return a3dHeaders::a3dSoundDescHeader; }
	int		calcSizeCore();
	bool	loadFromMemoryCore( a3dMemoryFile &file );
	bool	saveToMemoryCore( a3dMemoryFile &file );

	HRESULT loadSound( a3dScene *scene );
};


#endif