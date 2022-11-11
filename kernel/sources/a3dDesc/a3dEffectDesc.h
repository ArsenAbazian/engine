#ifndef A3D_EFFECT_DESC_INC
#define A3D_EFFECT_DESC_INC

#include "a3dDescBase.h"
#include <d3dx9effect.h>
#include "3dtypes.h"

class a3dScene;
class a3dEffectDesc : public a3dObjectDescBase { 
public:
	shString		fullPathName;
	shString		fileName;
	LPD3DXEFFECT	lpEffect;

	a3dEffectDesc() { 
		this->lpEffect = null;
	}
	~a3dEffectDesc() { 
		if( this->lpEffect != null ) {
			lpEffect->Release();
		}
	}

	int		getHeaderCount() { return a3dHeaders::a3dEffectDescHeader; }
	int		calcSizeCore();
	bool	loadFromMemoryCore( a3dMemoryFile &file );
	bool	saveToMemoryCore( a3dMemoryFile &file );

	HRESULT loadEffect( a3dScene *scene );
};

#endif