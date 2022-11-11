#ifndef A3D_MODEL_INFO
#define A3D_MODEL_INFO

#include "3dtypes.h"
#include "a3dVisualObjectInfoBase.h"

class a3dModelDesc;
class a3dModel;
class a3dModelRef;
class a3dObjectInfo;

class a3dModelInfo : public a3dVisualObjectInfoBase { 

public:
	a3dModelDesc *desc;
	a3dModel	*model;
	shString	fileName;
	shString	fullPathName;

	int			objectInfoCount;
	a3dObjectInfo *objectInfo;

	a3dModelInfo() { this->initializeDefault(); }
	~a3dModelInfo() { this->clear(); }

	void			initializeDefault();
	void			clear();
	bool			readFromLexemeList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &lli );
	a3dModelInfo*	clone();

	int		getHeaderCode() { return a3dHeaders::a3dModelInfoHeader; }
	int		calcSizeCore();
	bool	saveToMemoryCore(a3dMemoryFile &file);
	bool	loadFromMemoryCore(a3dMemoryFile &file);
	void	initialize( a3dModelDesc *desc );

	a3dModelRef* createReference();
	void	     initializeReference( a3dModelRef *modelRef );

	void		assign(a3dModelRef *modelRef);
};

#endif