#ifndef A3D_VISUAL_OBJECT_INFO_BASE_INC
#define A3D_VISUAL_OBJECT_INFO_BASE_INC

#include "a3dObjectInfoBase.h"
#include "..\\vecmat.h"
#include "lexeme.h"
#include <list>

class a3dVisualObjectInfoBase : public a3dObjectInfoBase { 

public:
	Matrix4		basis;
	float		scaleX;
	float		scaleY;
	float		scaleZ;

	virtual bool readFromLexemeList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &lli );
	void	copyTo( a3dVisualObjectInfoBase *obj );	

	int		getHeaderCode() { return a3dHeaders::a3dVisualObjectInfoBaseHeader; }
	int		calcSizeCore();
	bool	saveToMemoryCore(a3dMemoryFile &file);
	bool	loadFromMemoryCore(a3dMemoryFile &file);
};

#endif