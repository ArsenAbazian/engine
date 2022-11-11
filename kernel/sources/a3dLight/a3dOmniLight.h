#ifndef A3D_OMNI_LIGHT_INC
#define A3D_OMNI_LIGHT_INC

#include "a3dLightBase.h"

class a3dOmniLight : public a3dLightBase { 
public:
	Vector3		pos;
	float		farAttenStart;
	float		farAttenEnd;

	a3dOmniLight() : a3dLightBase() { 
		this->initializeDefault();
	}
	~a3dOmniLight() { this->clear(); }

	void		initializeDefault();
	void		clear();
	bool		import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );

	int			getHeaderCode() { return a3dHeaders::a3dOmniLightHeader; }
	int			calcSizeCore();
	bool		saveToMemoryCore( a3dMemoryFile &file );
	bool		loadFromMemoryCore( a3dMemoryFile &file );

	void copyTo( a3dLightBase* light );
	a3dLightBase* clone();
};

#endif