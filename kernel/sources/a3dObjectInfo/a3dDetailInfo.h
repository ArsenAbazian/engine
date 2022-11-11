#ifndef A3D_MODEL_DETAIL_INFO_INC
#define A3D_MODEL_DETAIL_INFO_INC

#include "a3dVisualObjectInfoBase.h"
#include "..\\lightMapInfo.h"
#include "..\\a3dMaterial\\a3dShaderMaterial.h"

class a3dDetailRef;
class a3dDetail;
class a3dDetailInfo : public a3dObjectInfoBase {
public:
	a3dLightMapInfo		lightMapInfo;
	a3dShaderMaterial	material;

	a3dDetailInfo() { this->initializeDefault(); }
	~a3dDetailInfo() { this->clear(); }

	void	initializeDefault();
	void	clear();

	int		getHeaderCode() { return a3dHeaders::a3dModelDetailInfoHeader; }
	int		calcSizeCore();
	bool	saveToMemoryCore(a3dMemoryFile &file);
	bool	loadFromMemoryCore(a3dMemoryFile &file);

	void	initializeReference( a3dDetailRef *detailRef );
	void	assign(a3dDetailRef *detailRef);
	void	initialize( a3dDetail *det );
};

#endif