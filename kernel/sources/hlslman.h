#ifndef HLSLMAN_INC
#define HLSLMAN_INC

#include "hstring.h"
#include "kernel.h"
#include <d3dx9effect.h>

class HLSLManager 
{ 
public:
	shString		result;
	LPD3DXEFFECT	lpEffect;

	HLSLManager() 
	{ 
		lpEffect = NULL;	
	}
	~HLSLManager() { }

	HRESULT createEffectFromFile(LPDIRECT3DDEVICE9 lpDevice, shString &fileName);
	HRESULT	createEffectFromString(LPDIRECT3DDEVICE9 lpDevice, shString &effect);
	HRESULT createEffectFromString(LPDIRECT3DDEVICE9 lpDevice, char *effect);
};

#endif