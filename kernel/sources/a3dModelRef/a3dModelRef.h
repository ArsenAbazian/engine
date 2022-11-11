#ifndef A3D_MODEL_REF_INC
#define A3D_MODEL_REF_INC

#include "hstring.h"
#include "..\\kernel.h"
#include "..\\lightMapCreator.h"
#include "..\\a3dModel\\a3dMathObject.h"
#include "..\\delegate.h"
#include "..\\Ncamera.h"

class a3dModel;
class a3dModelDesc;
class a3dModelInfo;
class a3dObjectRef;
class a3dLightMap;

class a3dModelRef : public a3dLightMapCreator, public a3dMathObject { 
public:
	shString		name;
	a3dModel		*model;
	a3dModelDesc	*desc;
	a3dModelInfo	*info;

	int				objCount;
	a3dObjectRef	**objectRef;

	a3dModelRef() { this->initializeDefault(); }

	a3dModelRef( a3dModel *model ) {
		this->initializeDefault();
		this->initialize( model );
	}

	a3dModelRef( a3dModelDesc *desc ) { 
		this->initializeDefault();
		this->initialize( desc );
	}

	a3dModelRef( a3dModelInfo *info ) { 
		this->initializeDefault();	
		this->initialize( info );
	}

	~a3dModelRef() { this->clear(); }

	void	initializeDefault();
	void	initialize( a3dModel *model );
	void	initialize( a3dModelDesc *desc );
	void	initialize( a3dModelInfo *info );
	void	clear();

	int				getLightMapCount();
	a3dLightMap**	getLightMaps();
	a3dLightMap**	getLightMaps( processLightMapPlaneDeletate fp, void *lpData );

	// should apply basises before rendering
	void			applyBasises();
	HRESULT			render();
	HRESULT			render( camera *cam );
};

#endif