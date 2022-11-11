#ifndef A3D_MULTI_MATERIAL_INC
#define A3D_MULTI_MATERIAL_INC

#include "a3dMaterialBase.h"

class a3dMultiMaterial : public a3dMaterialBase { 

	bool importSubMaterialEnablesList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	bool importSubMaterialIdList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	bool importSubMaterialNameList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
public:
	int						subMaterialCount;
	shString				*subMaterialNameList;
	bool					*subMaterialEnablesList;
	int						*subMaterialIdList;

	a3dMultiMaterial() { 
		this->type = MT_MULTI;
		this->subMaterialCount = 0;
		this->subMaterialNameList = null;
		this->subMaterialEnablesList = null;
		this->subMaterialIdList = null;
	}

	bool import( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &li );
	void clear();
	a3dMaterialBase* clone();
};


#endif