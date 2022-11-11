#ifndef A3D_MODEL_DESC_INC
#define A3D_MODEL_DESC_INC

#include "a3dDescBase.h"
#include "3dtypes.h"

class a3dModel;
class a3dScene;
class a3dModelRef;
class a3dModelDesc : public a3dObjectDescBase { 
public:
	shString		fileName;
	shString		fullPathName;
	a3dModel		*model;

	a3dModelDesc() { 
		this->model = null;
	}
	~a3dModelDesc();

	int		getHeaderCount() { return a3dHeaders::a3dModelDescHeader; }
	int		calcSizeCore();
	bool	loadFromMemoryCore( a3dMemoryFile &file );
	bool	saveToMemoryCore( a3dMemoryFile &file );

	HRESULT loadModel( a3dScene *scene );
	a3dModelRef* createReference();

	void	updateModel( a3dScene *scene );
};

#endif