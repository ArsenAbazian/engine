#ifndef A3D_FONT_DESC_INC
#define A3D_FONT_DESC_INC

#include "a3dDescBase.h"
#include "3dtypes.h"
#include "..\\font.h"

class a3dFont;
class a3dScene;
class a3dFontDesc : public a3dObjectDescBase { 
public:
	shString		fullPathName;
	shString		fileName;
	shString		posFullPathName;
	shString		posFileName;

	a3dFont			*font;

	a3dFontDesc() { 
		this->font = null;
	}

	~a3dFontDesc();

	int		getHeaderCount() { return a3dHeaders::a3dFontDescHeader; }
	int		calcSizeCore();
	bool	loadFromMemoryCore( a3dMemoryFile &file );
	bool	saveToMemoryCore( a3dMemoryFile &file );

	HRESULT	loadEffect( a3dScene *scene );
};


#endif