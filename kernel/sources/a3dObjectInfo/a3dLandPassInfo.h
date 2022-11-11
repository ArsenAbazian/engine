#ifndef A3D_LAND_PASS_INFO
#define A3D_LAND_PASS_INFO

#include "..\\headerCodes.h"
#include "lexeme.h"
#include "hstring.h"
#include <list>

class a3dLandscapeInfo;
class a3dLandPassInfo : public a3dSerializableObject { 
public:
	a3dLandscapeInfo *landInfo;
	int				passIndex;

	bool		enabled;
	bool		animated;
	shString	name;
	shString	mapFileName;
	shString	maskFileName;
	shString	detailFileName;

	float		mapUSize;
	float		mapVSize;
	float		detUSize;
	float		detVSize;

	float		minDetailDist;
	float		maxDetailDist;
	float		maxDetailAlphaValue;

	a3dLandPassInfo() { this->initializeDefault(); }
	~a3dLandPassInfo() { }

	void		initializeDefault();
	bool		readFromLexemeList( std::list< scriptLexeme* > &lexList, std::list< scriptLexeme* >::iterator &lli, int index );
	void		copyTo( a3dLandPassInfo* info );

	int			getHeaderCode() { return a3dHeaders::a3dLandPassInfoHeader; }
	int			calcSizeCore();
	bool		saveToMemoryCore(a3dMemoryFile &file);
	bool		loadFromMemoryCore(a3dMemoryFile &file);
};


#endif