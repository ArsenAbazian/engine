#ifndef A3D_PARTICLE_INFO_INC
#define A3D_PARTICLE_INFO_INC

#include "a3dVisualObjectInfoBase.h"
#include "3dtypes.h"
#include "..\\a3dParticleSystem\\particleTypes.h"

class particleSystemBase;
class a3dScene;
class a3dParticleSystemInfo : public a3dVisualObjectInfoBase  { 
public:
	a3dParticleType		type;
	shString			effectFileName;
	particleSystemBase	*pSystem;

	a3dParticleSystemInfo() {
		this->pSystem	= null;
		this->type		= ptNone;
	}
	~a3dParticleSystemInfo();

	bool			readFromLexemeList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &lli );
	bool			readEnd(std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &lli );

	virtual void copyTo( a3dParticleSystemInfo *info );
	virtual	a3dParticleSystemInfo* clone();

	int				getHeaderCode() { return a3dHeaders::a3dParticleSystemInfo; }
	int				calcSizeCore();
	bool			saveToMemoryCore(a3dMemoryFile &file);
	bool			loadFromMemoryCore(a3dMemoryFile &file);

	virtual HRESULT	loadParticle( a3dScene *scene );
	shString&		getName();

	particleSystemBase* createParticle();
};


#endif