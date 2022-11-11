#ifndef A3d_MATERIAL_BASE_INC
#define A3d_MATERIAL_BASE_INC

#include "serialization.h"
#include "a3dMaterialType.h"
#include "lexeme.h"
#include "..\\headerCodes.h"
#include "3dtypes.h"

class a3dMemoryFile;
class a3dEffectDesc;
class a3dTextureDesc;
class a3dScene;

class a3dMaterialBase : public a3dSerializableObject { 
public:
	a3dMaterialType	type;
	shString				name;

	a3dMaterialBase() { 
		this->type = MT_NONE;
	}
	~a3dMaterialBase() { this->clear(); }

	virtual bool supportSave() { return false; }
	virtual bool import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	virtual void clear();
	virtual	a3dMaterialBase* clone();

	int			getHeaderCode() { return a3dHeaders::a3dMaterialBaseHeader; }
	int			calcSizeCore();
	bool		saveToMemoryCore( a3dMemoryFile &file );
	bool		loadFromMemoryCore( a3dMemoryFile &file );

	virtual bool initializeTextures( a3dTextureDesc* textures, int count );
	virtual bool initializeEffects( a3dEffectDesc* effects, int count );

	virtual HRESULT initializeData( a3dScene *scene );
};


#endif
