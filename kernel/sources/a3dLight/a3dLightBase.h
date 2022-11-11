#ifndef A3D_LIGHT_BASE_INC
#define A3D_LIGHT_BASE_INC

#include "hstring.h"
#include "vecmat.h"
#include "lexeme.h"
#include "..\\headerCodes.h"
#include "a3dLightType.h"
#include <list>

class a3dMemoryFile;

class a3dLightBase : public a3dSerializableObject { 
public:
	a3dLightType	type;
	shString		name;
	Vector3			color;

	a3dLightBase() { this->initializeDefault(); }
	~a3dLightBase() { this->clear(); }

	virtual void initializeDefault();
	virtual void clear();
	virtual bool import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );

	int		getHeaderCode() { return a3dHeaders::a3dLightBaseHeader; }
	int		calcSizeCore();
	bool	saveToMemoryCore( a3dMemoryFile &file );
	bool	loadFromMemoryCore( a3dMemoryFile &file );
	bool	supportSave() { return true; }

	virtual void copyTo( a3dLightBase* light );
	virtual a3dLightBase* clone();
};


#endif