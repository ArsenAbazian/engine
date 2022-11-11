#ifndef A3D_RAIL_SYSTEM_INFO_INC
#define A3D_RAIL_SYSTEM_INFO_INC

#include "a3dVisualObjectInfoBase.h"
#include "3dtypes.h"

class a3dRailManager;
class a3dScene;
class a3dRailSystemInfo : public a3dVisualObjectInfoBase  { 
public:
	a3dRailManager	*rSystem;

	shString		railFileName;
	shString		lineModelFileName;
	shString		roundModelFileName;
	shString		lineEffectFileName;
	shString		roundEffectFileName;
	shString		round2EffectFileName;
	shString		landName;
	int				landHeightPixelX;
	int				landHeightPixelY;

	a3dRailSystemInfo() { 
		this->rSystem = null;
		this->landHeightPixelX = 0;
		this->landHeightPixelY = 0;
	}

	~a3dRailSystemInfo();

	bool					readFromLexemeList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &lli );
	a3dRailSystemInfo*		clone();
	void					copyTo(a3dVisualObjectInfoBase *obj);

	int						getHeaderCode() { return a3dHeaders::a3dRailSystemInfoHeader; }
	int						calcSizeCore();
	bool					saveToMemoryCore(a3dMemoryFile &file);
	bool					loadFromMemoryCore(a3dMemoryFile &file);

	HRESULT					loadRail( a3dScene *scene );
};


#endif