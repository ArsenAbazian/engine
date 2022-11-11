#ifndef A3D_EFFECT_INFO_INC
#define A3D_EFFECT_INFO_INC

#include "..\\kernel.h"
#include "a3dObjectInfoBase.h"
#include <d3dx9effect.h>

class a3dEffectInfo : public a3dObjectInfoBase { 
public:
	Kernel			*kernel;
	LPD3DXEFFECT	lpEffect;

	a3dEffectInfo() { this->initializeDefault(); }
	~a3dEffectInfo() { this->clear(); }

	void	initializeDefault();
	void	clear();

	void	setKernel( Kernel *krn ) { this->kernel = krn; }
	HRESULT loadEffect( shString fullPathName );

	int		getHeaderCode() { return a3dHeaders::a3dEffectInfoHeader; }
	int		calcSizeCore();
	bool	saveToMemoryCore(a3dMemoryFile &file);
	bool	loadFromMemoryCore(a3dMemoryFile &file);
};

#endif