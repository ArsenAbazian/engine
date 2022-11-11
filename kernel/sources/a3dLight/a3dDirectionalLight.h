#ifndef A3D_DIRECTIONAL_LIGHT_INC
#define A3D_DIRECTIONAL_LIGHT_INC

#include "a3dLightBase.h"

class a3dDirectionalLight : public a3dLightBase { 
public:
	Vector3		dir;

	a3dDirectionalLight() : a3dLightBase() { 
		this->initializeDefault();
	}
	~a3dDirectionalLight() { this->clear(); }

	void		initializeDefault();
	void		clear();
	bool		import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );

	int			getHeaderCode() { return a3dHeaders::a3dDirectionalLightHeader; }	
	int			calcSizeCore();
	bool		saveToMemoryCore( a3dMemoryFile &file );
	bool		loadFromMemoryCore( a3dMemoryFile &file );

	void copyTo( a3dLightBase* light );
	a3dLightBase* clone();
};

#endif