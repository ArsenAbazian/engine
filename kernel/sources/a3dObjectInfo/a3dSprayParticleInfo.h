#ifndef A3D_SPRAY_PARTICLE_INFO_INC
#define A3D_SPRAY_PARTICLE_INFO_INC

#include "a3dParticleInfo.h"

class a3dSprayParticleSystemInfo : public a3dParticleSystemInfo { 
public:
	int				viewportCount;
	int				renderCount;
	float			dropSize;
	float			speed;
	float			lifeTime;
	int				birthRate;
	float			emitterHeight;
	float			emitterWidth;

	a3dSprayParticleSystemInfo() { this->initializeDefault(); }
	~a3dSprayParticleSystemInfo() { this->clear(); }

	void			initializeDefault();
	void			clear();

	bool			readFromLexemeList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &lli );

	void copyTo( a3dParticleSystemInfo *info );
	a3dParticleSystemInfo* clone();

	int				getHeaderCode() { return a3dHeaders::a3dSprayParticleSystemInfoHeader; }
	int				calcSizeCore();
	bool			saveToMemoryCore(a3dMemoryFile &file);
	bool			loadFromMemoryCore(a3dMemoryFile &file);

	HRESULT			loadParticle( a3dScene *scene );
};


#endif