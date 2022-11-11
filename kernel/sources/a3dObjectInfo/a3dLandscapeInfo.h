#ifndef A3D_LANDSCAPE_INFO
#define A3D_LANDSCAPE_INFO

#include "3dtypes.h"
#include "a3dVisualObjectInfoBase.h"
#include "a3dLandPassInfo.h"

class a3dScene;
class a3dLandscape;
class a3dLandscapeInfo : public a3dVisualObjectInfoBase  { 
public:
	a3dLandscape	*land;

	shString		heightMapFileName;
	shString		effectFileName;
	shString		lightMapFileName;
	float			width;
	float			height;
	float			pixelError;
	int				patchPerSide;
	int				lightMapWidth;
	int				lightMapHeight;

	a3dLandPassInfo	pass[16];

	a3dLandscapeInfo() { this->initializeDefault(); }
	~a3dLandscapeInfo();

	void			initializeDefault();
	bool			readFromLexemeList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &lli );
	a3dLandscapeInfo* clone();

	int				getHeaderCode() { return a3dHeaders::a3dLandscapeInfoHeader; }
	int				calcSizeCore();
	bool			saveToMemoryCore(a3dMemoryFile &file);
	bool			loadFromMemoryCore(a3dMemoryFile &file);

	HRESULT			loadLand( a3dScene *scene );
	void			initialize( a3dLandscape *land );
};


#endif