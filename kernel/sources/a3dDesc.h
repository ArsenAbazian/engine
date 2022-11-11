#ifndef A3D_DESC_INC
#define A3D_DESC_INC

#include "serialization.h"
#include "a3dTextSceneFile.h"
#include "railManager.h"
#include "font.h"
#include "wave.h"

class a3dScene;

class a3dObjectDescBase : public a3dSerializableObject { 
public:
	Kernel		*kernel;
	bool		used;	// cashing
	shString	name;

	a3dObjectDescBase() { }
	~a3dObjectDescBase() { }

	int		getHeaderCode() { return a3dHeaders::a3dObjectDescBaseHeader; }
	int		calcSizeCore();
	bool	loadFromMemoryCore( a3dMemoryFile &file );
	bool	saveToMemoryCore( a3dMemoryFile &file );
};

class a3dModelDesc : public a3dObjectDescBase { 
public:
	shString		fileName;
	shString		fullPathName;
	a3dModel		*model;

	a3dModelDesc() { 
		this->model = null;
	}
	~a3dModelDesc() { 
		if( this->model != null ) 
			delete this->model;
	}

	int		getHeaderCount() { return a3dHeaders::a3dModelDescHeader; }
	int		calcSizeCore();
	bool	loadFromMemoryCore( a3dMemoryFile &file );
	bool	saveToMemoryCore( a3dMemoryFile &file );

	HRESULT loadModel( a3dScene *scene );
};

class a3dLandscape;
class a3dLandscapeInfo;
class a3dLandscapeDesc : public a3dObjectDescBase { 
public:
	a3dLandscapeInfo		*landInfo;
	a3dLandscape			*land;

	a3dLandscapeDesc() { 
		this->land = null;
		this->landInfo = null;
	}
	~a3dLandscapeDesc() { 
		if( this->landInfo != null ) 
			delete landInfo;
	}

	int		getHeaderCount() { return a3dHeaders::a3dLandDescHeader; }
	int		calcSizeCore();
	bool	loadFromMemoryCore( a3dMemoryFile &file );
	bool	saveToMemoryCore( a3dMemoryFile &file );

	HRESULT loadLand( a3dScene *scene );
};

class a3dParticleSystemInfo;
class particleSystemBase;
class a3dParticleSystemDesc : public a3dObjectDescBase {
public:
	a3dParticleSystemInfo	*psInfo;
	particleSystemBase	*pSystem;

	a3dParticleSystemDesc() { 
		this->psInfo = null;
		this->pSystem = null;
	}
	~a3dParticleSystemDesc() { 
		if( this->psInfo != null ) 
			delete this->psInfo;
	}

	int		getHeaderCount() { return a3dHeaders::a3dParticleSystemDescHeader; }
	int		calcSizeCore();
	bool	loadFromMemoryCore( a3dMemoryFile &file );
	bool	saveToMemoryCore( a3dMemoryFile &file );

	HRESULT loadParticle( a3dScene *scene );
};

class a3dRailSystemInfo;
class a3dRailManager;
class a3dRailSystemDesc : public a3dObjectDescBase { 
public:
	a3dRailSystemInfo		*rsInfo;
	a3dRailManager			*rSystem;

	a3dRailSystemDesc() { 
		this->rsInfo = null;
		this->rSystem = null;
	}
	~a3dRailSystemDesc() { 
		if( this->rsInfo != null )
			delete this->rsInfo;
		if( this->rSystem != null )
			delete this->rSystem;
	}

	int		getHeaderCount() { return a3dHeaders::a3dRailSystemDescHeader; }
	int		calcSizeCore();
	bool	loadFromMemoryCore( a3dMemoryFile &file );
	bool	saveToMemoryCore( a3dMemoryFile &file );

	HRESULT loadRail( a3dScene *scene );
};

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

class a3dTextureDesc : public a3dObjectDescBase { 
public:
	shString		fullPathName;
	shString		fileName;
	Texture			*texture;
	
	a3dTextureDesc() { 
		this->texture = null;
	}
	~a3dTextureDesc() {
		if( this->texture != null ) 
			delete this->texture;
	}

	int		getHeaderCount() { return a3dHeaders::a3dTextureDescHeader; }
	int		calcSizeCore();
	bool	loadFromMemoryCore( a3dMemoryFile &file );
	bool	saveToMemoryCore( a3dMemoryFile &file );

	HRESULT	loadTexture( a3dScene *scene );
};

class a3dFontDesc : public a3dObjectDescBase { 
public:
	shString		fullPathName;
	shString		fileName;
	shString		posFullPathName;
	shString		posFileName;
	
	Font			*font;

	a3dFontDesc() { 
		this->font = null;
	}

	~a3dFontDesc() { 
		if( this->font != null )
			delete this->font;
	}

	int		getHeaderCount() { return a3dHeaders::a3dFontDescHeader; }
	int		calcSizeCore();
	bool	loadFromMemoryCore( a3dMemoryFile &file );
	bool	saveToMemoryCore( a3dMemoryFile &file );

	HRESULT	loadEffect( a3dScene *scene );
};

class a3dSoundDesc : public a3dObjectDescBase {
public:
	shString		fullPathName;
	shString		fileName;
	GameSound		*sound;
	
	a3dSoundDesc() { 
		this->sound = null;
	}
	~a3dSoundDesc() { 
		if( this->sound != null )
			delete this->sound;
	}

	int		getHeaderCount() { return a3dHeaders::a3dSoundDescHeader; }
	int		calcSizeCore();
	bool	loadFromMemoryCore( a3dMemoryFile &file );
	bool	saveToMemoryCore( a3dMemoryFile &file );

	HRESULT loadSound( a3dScene *scene );
};

#endif