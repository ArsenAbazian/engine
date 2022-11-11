#ifndef A3D_PARTICLE_DESC_INC
#define A3D_PARTICLE_DESC_INC

#include "a3dDescBase.h"
#include "3dtypes.h"
#include "..\\a3dParticleSystem\\particleTypes.h"

class a3dParticleSystemInfo;
class particleSystemBase;
class a3dScene;
class a3dParticleSystemDesc : public a3dObjectDescBase {
public:
	//a3dParticleSystemInfo	*psInfo;
	particleSystemBase	*pSystem;

	a3dParticleSystemDesc() { 
		//this->psInfo = null;
		this->pSystem = null;
	}
	~a3dParticleSystemDesc();

	int						getHeaderCount() { return a3dHeaders::a3dParticleSystemDescHeader; }
	int						calcSizeCore();
	bool					loadFromMemoryCore( a3dMemoryFile &file );
	bool					saveToMemoryCore( a3dMemoryFile &file );

	particleSystemBase*		createParticle( a3dParticleType type );	
	HRESULT					loadParticle( a3dScene *scene );
};


#endif
