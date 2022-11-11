#ifndef A3D_MODEL_OBJECT_INFO_INC
#define A3D_MODEL_OBJECT_INFO_INC

#include "a3dVisualObjectInfoBase.h"

class a3dDetailInfo;
class a3dObjectRef;
class a3dObject;
class a3dObjectInfo : public a3dVisualObjectInfoBase {
public:
	int				detailInfoCount;
	a3dDetailInfo	*detailInfo;

	a3dObjectInfo() { this->initializeDefault(); }
	~a3dObjectInfo() { this->clear(); }

	void initializeDefault();
	void clear();

	int		getHeaderCode() { return a3dHeaders::a3dModelObjectInfoHeader; }
	int		calcSizeCore();
	bool	saveToMemoryCore(a3dMemoryFile &file);
	bool	loadFromMemoryCore(a3dMemoryFile &file);

	void	initializeReference(a3dObjectRef *objectRef);
	void	initialize( a3dObject *obj );

	void	assign(a3dObjectRef *objRef);
};

#endif
