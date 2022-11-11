#ifndef A3D_MODEL_INC
#define A3D_MODEL_INC

#include "a3dMathObject.h"
#include "..\\delegate.h"
#include "..\\lightMapCreator.h"
#include "..\\Ncamera.h"

class a3dMaterialBase;
class a3dLightBase;
class a3dScene;
class a3dObject;
class a3dModelRef;
class a3dTextModelFile;

class a3dModel : public a3dMathObject, public a3dLightMapCreator {
	int			version;

public:
	Kernel		*kernel;

	a3dMaterialBase **material;
	int			materialCount;

	a3dObject	*object;
	int			objCount;

	a3dLightBase	**light;
	int			lightCount;

	a3dModel() { this->initializeDefault(); }
	~a3dModel() { this->clear(); }

	void			setKernel( Kernel *krn );
	void			initializeDefault();
	void			clear();

	bool			import( a3dTextModelFile &scFile );
	bool			loadFromFile( shString &fileName );
	bool			saveToFile( shString &fileName );

	int				calcSizeCore();
	int				getHeaderCode() { return a3dHeaders::a3dModelHeader; }
	bool			saveToMemoryCore( a3dMemoryFile &file );
	bool			loadFromMemoryCore( a3dMemoryFile &file );

	int				createVersion( int major, int minor ) { return ( ( major && 0x0000ffff ) << 16 ) | ( minor && 0x0000ffff ); }
	int				calcSaveObjCount();
	int				calcSaveMaterialCount();
	int				calcSaveLightCount();
	a3dMaterialBase* createMaterial( int header );
	a3dLightBase*	createLight( int header );

	a3dModelRef*	createReference();
	HRESULT			initializeData( a3dScene *scene );

	int				getObjectIndexByName( shString &objName );	

	HRESULT			render(camera *cam, a3dModelRef *ref);
	HRESULT 		render(camera *cam);
	HRESULT			save();
	HRESULT			restore();
	HRESULT			initialize() { return this->restore(); }

	// virtual
	void			correct();
	void			calcBVolume();

	int				calcFaceCount();
	a3dMaterialBase* getMaterialByName( shString &matName );

	// lightmap generation
	a3dLightMap**	getLightMaps( processLightMapPlaneDeletate fp, void *lpData );
	void			getLightMaps( a3dLightMap** llList, processLightMapPlaneDeletate fp, void *lpData );
	int				getLightMapCount();

	void			skipMaterialErrors(bool value);
};


#endif